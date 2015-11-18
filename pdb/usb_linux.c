/*
 * Copyright (C) 2015 Wiky L
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.";
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>
#include <linux/usbdevice_fs.h>
#include <linux/version.h>
#include <pthread.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 20)
#include <linux/usb/ch9.h>
#else
#include <linux/usb_ch9.h>
#endif
#include <asm/byteorder.h>

typedef struct _usb_handle usb_handle;

static pthread_mutex_t usb_lock = PTHREAD_MUTEX_INITIALIZER;

struct _usb_handle{
    usb_handle *prev;
    usb_handle *next;

    char fname[64];
    int desc;
    unsigned char ep_in;
    unsigned char ep_out;

    unsigned zero_mask;
    unsigned writable;

    struct usbdevfs_urb urb_in;
    struct usbdevfs_urb urb_out;

    int urb_in_busy;
    int urb_out_busy;
    int dead;

    int mark;

    pthread_cond_t notify;
    pthread_mutex_t lock;

    pthread_t reaper_thread;
};

static usb_handle handle_list = {
    .prev = &handle_list,
    .next = &handle_list,
};


static void register_device(const char *dev_name, const char *devpath,
            unsigned char ep_in, unsigned char ep_out,
            int ifc, int serial_index, unsigned zero_mask);

static inline int badname(const char *name){
    while(*name){
        if(!isdigit(*name++)){
            return 1;
        }
    }
    return 0;
}

#define ADB_CLASS              0xff
#define ADB_SUBCLASS           0x42
#define ADB_PROTOCOL           0x1

static inline int is_adb_interface(int vid, int pid, int usb_class,
            int usb_subclass, int usb_protocol){
    return usb_class == ADB_CLASS && usb_subclass == ADB_SUBCLASS
        && usb_protocol == ADB_PROTOCOL;
}

static void find_usb_device(const char *base,
            void (*register_device_callback)
            (const char *, const char *, unsigned char, unsigned char, int, int, unsigned)){
    char busname[32], devname[32];
    unsigned char local_ep_in, local_ep_out;
    DIR *busdir, *devdir;
    struct dirent *de;
    int fd;

    if((busdir=opendir(base))==NULL){
        perror("openidr");
        return;
    }

    while((de=readdir(busdir))!=NULL){
        if(badname(de->d_name)){
            continue;
        }

        snprintf(busname, sizeof(busname), "%s/%s", base, de->d_name);
        if((devdir=opendir(busname))==NULL){
            continue;
        }

        while((de=readdir(devdir))){
            unsigned char devdesc[4096];
            unsigned char *bufptr = devdesc;
            unsigned char *bufend;
            struct usb_device_descriptor *device;
            struct usb_config_descriptor *config;
            struct usb_interface_descriptor *interface;
            struct usb_endpoint_descriptor *ep1, *ep2;
            unsigned zero_mask = 0;
            unsigned vid, pid;
            size_t desclength;

            if(badname(de->d_name)){
                continue;
            }
            snprintf(devname, sizeof(devname), "%s/%s", busname, de->d_name);

            if((fd=open(devname, O_RDONLY|O_CLOEXEC))<0){
                continue;
            }

            desclength = read(fd, devdesc, sizeof(devdesc));
            bufend = bufptr + desclength;


            if(desclength < USB_DT_DEVICE_SIZE + USB_DT_CONFIG_SIZE){
                close(fd);
                continue;
            }

            device = (struct usb_device_descriptor*)bufptr;
            bufptr += USB_DT_DEVICE_SIZE;

            if((device->bLength != USB_DT_DEVICE_SIZE)||
                        (device->bDescriptorType!=USB_DT_DEVICE)){
                close(fd);
                continue;
            }

            vid = device->idVendor;
            pid = device->idProduct;

            config = (struct usb_config_descriptor *)bufptr;
            bufptr += USB_DT_CONFIG_SIZE;
            if(config->bLength!=USB_DT_CONFIG_SIZE||
                        config->bDescriptorType!=USB_DT_CONFIG){
                close(fd);
                continue;
            }

            /* loop through all the descriptors and look for the ADB interface */
            while(bufptr<bufend){
                unsigned char length = bufptr[0];
                unsigned char type = bufptr[1];

                if(type==USB_DT_INTERFACE){
                    interface = (struct usb_interface_descriptor*)bufptr;
                    bufptr += length;

                    if(length!=USB_DT_INTERFACE_SIZE){
                        printf("interface descriptor has wrong size\n");
                        break;
                    }

                    if(interface->bNumEndpoints == 2 &&
                                is_adb_interface(vid, pid,
                                    interface->bInterfaceClass,
                                    interface->bInterfaceSubClass,
                                    interface->bInterfaceProtocol)){
                        struct stat st;
                        char pathbuf[128];
                        char link[256];
                        char *devpath = NULL;

                        ep1 = (struct usb_endpoint_descriptor *)bufptr;
                        bufptr += USB_DT_ENDPOINT_SIZE;
                        ep2 = (struct usb_endpoint_descriptor *)bufptr;
                        bufptr += USB_DT_ENDPOINT_SIZE;

                        if(bufptr>devdesc+desclength||
                                    ep1->bLength!=USB_DT_ENDPOINT_SIZE||
                                    ep1->bDescriptorType!=USB_DT_ENDPOINT||
                                    ep2->bLength!=USB_DT_ENDPOINT_SIZE||
                                    ep2->bDescriptorType!=USB_DT_ENDPOINT){
                            printf("endpoints not found!\n");
                            break;
                        }

                        /* both endpoints should be bulk */
                        if(ep1->bmAttributes != USB_ENDPOINT_XFER_BULK||
                                    ep2->bmAttributes !=USB_ENDPOINT_XFER_BULK){
                            printf("bulk endpoints not found\n");
                            continue;
                        }

                        /* aproto 01 needs 0 termination */
                        if(interface->bInterfaceProtocol==0x01){
                            zero_mask = ep1->wMaxPacketSize - 1;
                        }

                        /* we have a match. now we
                         * just need to figure out which is in and
                         * which is out
                         */
                        if(ep1->bEndpointAddress & USB_ENDPOINT_DIR_MASK){
                            local_ep_in = ep1->bEndpointAddress;
                            local_ep_out = ep2->bEndpointAddress;
                        }else{
                            local_ep_in = ep2->bEndpointAddress;
                            local_ep_out = ep1->bEndpointAddress;
                        }

                        /* Determine the device path */
                        if(!fstat(fd, &st) && S_ISCHR(st.st_mode)){
                            char *slash;
                            ssize_t link_len;
                            snprintf(pathbuf, sizeof(pathbuf),
                                        "/sys/dev/char/%d:%d",
                                        major(st.st_rdev),
                                        minor(st.st_rdev));
                            link_len = readlink(pathbuf, link,
                                        sizeof(link)-1);
                            if(link_len>0){
                                link[link_len] = '\0';
                                slash = strrchr(link, '/');
                                if(slash){
                                    snprintf(pathbuf, sizeof(pathbuf),
                                                "usb:%s", slash+1);
                                    devpath = pathbuf;
                                }
                            }
                        }

                        printf("devname:%s, devpath:%s\n",devname, devpath);

                        register_device_callback(devname, devpath,
                                    local_ep_in, local_ep_out,
                                    interface->bInterfaceNumber,
                                    device->iSerialNumber, zero_mask);
                        break;
                    }
                }else{
                    bufptr+=length;
                }
            }
            close(fd);
        }
        closedir(devdir);
    }
    closedir(busdir);
}

static void register_device(const char *dev_name, const char *devpath,
            unsigned char ep_in, unsigned char ep_out,
            int ifc, int serial_index, unsigned zero_mask){
    usb_handle *usb=NULL;
    int n = 0;
    char serial[256];

    pthread_mutex_lock(&usb_lock);
    for(usb=handle_list.next;usb!=&handle_list;usb=usb->next){
        if(!strcmp(usb->fname, dev_name)){
            pthread_mutex_unlock(&usb_lock);
            return;
        }
    }
    pthread_mutex_unlock(&usb_lock);

    printf("usb localed new device %s (%d/%d/%d)\n",
                dev_name, ep_in, ep_out, ifc);
    usb=calloc(1, sizeof(usb_handle));
    strcpy(usb->fname, dev_name);
    usb->ep_in = ep_in;
    usb->ep_out = ep_out;
    usb->zero_mask = zero_mask;
    usb->writable = 1;

    pthread_cond_init(&usb->notify, NULL);
    pthread_mutex_init(&usb->lock, NULL);

    usb->mark = 1;
    usb->reaper_thread = 0;

    usb->desc = open(usb->fname, O_RDWR|O_CLOEXEC);
    if(usb->desc<0){
        usb->desc = open(usb->fname, O_RDONLY|O_CLOEXEC);
        if(usb->desc<0){
            goto FAIL;
        }
        usb->writable = 0;
    }else{
        if(ioctl(usb->desc, USBDEVFS_CLAIMINTERFACE, &ifc)!=0){
            goto FAIL;
        }
    }

    serial[0] = '\0';
    memset(serial, 0 ,sizeof(serial));
    if(serial_index){
        struct usbdevfs_ctrltransfer ctrl;
        __u16 buffer[128];
        __u16 languages[128];
        int i, result;
        int languageCount = 0;

        memset(languages, 0 , sizeof(languages));
        memset(&ctrl, 0, sizeof(ctrl));

        ctrl.bRequestType=USB_DIR_IN|USB_TYPE_STANDARD|USB_RECIP_DEVICE;
        ctrl.bRequest = USB_REQ_GET_DESCRIPTOR;
        ctrl.wValue = (USB_DT_STRING<<8)|0;
        ctrl.wIndex=0;
        ctrl.wLength = sizeof(languages);
        ctrl.data = languages;
        ctrl.timeout = 1000;

        result = ioctl(usb->desc, USBDEVFS_CONTROL, &ctrl);
        if(result>0){
            languageCount = (result-2)/2;
        }

        for (i = 0; i < languageCount; i++) {
            memset(buffer, 0 ,sizeof(buffer));
            memset(&ctrl, 0 ,sizeof(ctrl));
            ctrl.bRequestType = USB_DIR_IN|USB_TYPE_STANDARD|USB_RECIP_DEVICE;
            ctrl.bRequest = USB_REQ_GET_DESCRIPTOR;
            ctrl.wValue = (USB_DT_STRING << 8) | serial_index;
            ctrl.wIndex = __le16_to_cpu(languages[i]);
            ctrl.wLength = sizeof(buffer);
            ctrl.data = buffer;
            ctrl.timeout = 1000;

            result = ioctl(usb->desc, USBDEVFS_CONTROL, &ctrl);
            if (result > 0) {
                int i;
                // skip first word, and copy the rest to the serial string, changing shorts to bytes.
                result /= 2;
                for (i = 1; i < result; i++){
                    serial[i - 1] = __le16_to_cpu(buffer[i]);
                }
                serial[i - 1] = 0;
                break;
            }
        }
    }

    pthread_mutex_lock(&usb_lock);
    usb->next = &handle_list;
    usb->prev = handle_list.prev;
    usb->prev->next = usb;
    usb->next->prev=usb;
    pthread_mutex_unlock(&usb_lock);

    /*register_usb_transport(usb, serial, devpath, usb->writable);*/
    return;

FAIL:
    if(usb->desc>=0){
        close(usb->desc);
    }
    free(usb);
}

int main(int argc, const char *argv[]){
    find_usb_device("/dev/bus/usb", register_device);

    usb_handle *ptr=NULL;
    for(ptr=handle_list.next;ptr!=&handle_list;ptr=ptr->next){
        printf("%s:%d\n",ptr->fname, ptr->desc);
    }
    return 0;
}
