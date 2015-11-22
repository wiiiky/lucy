SRCS+= pyadb.c
SRCS+= wrapper.c
SRCS+= util.c

SRCS+= adb.c
SRCS+= adb_client.c
SRCS+= adb_auth_host.c
SRCS+= commandline.c
SRCS+= console.c
SRCS+= file_sync_client.c
SRCS+= fdevent.c
SRCS+= get_my_path_linux.c
SRCS+= services.c
SRCS+= sockets.c
SRCS+= transport.c
SRCS+= transport_local.c
SRCS+= transport_usb.c
SRCS+= usb_linux.c
SRCS+= usb_vendors.c

#VPATH+= libcutils
SRCS+= libcutils/socket_inaddr_any_server.c
SRCS+= libcutils/socket_local_client.c
SRCS+= libcutils/socket_local_server.c
SRCS+= libcutils/socket_loopback_client.c
SRCS+= libcutils/socket_loopback_server.c
SRCS+= libcutils/socket_network_client.c
SRCS+= libcutils/load_file.c

#VPATH+= libzipfile
SRCS+= libzipfile/centraldir.c
SRCS+= libzipfile/zipfile.c

#VPATH+= libmincrypt
SRCS+= libmincrypt/rsa.c
SRCS+= libmincrypt/sha256.c
SRCS+= libmincrypt/sha.c
SRCS+= libmincrypt/p256.c
SRCS+= libmincrypt/p256_ec.c
SRCS+= libmincrypt/p256_ecdsa.c

CPPFLAGS+= -DHAVE_SYS_PRCTL_H
CPPFLAGS+= -DADB_HOST=1
CPPFLAGS+= -DHAVE_FORKEXEC=1
CPPFLAGS+= -DHAVE_SYMLINKS
CPPFLAGS+= -DHAVE_TERMIO_H
CPPFLAGS+= -D_GNU_SOURCE
CPPFLAGS+= -D_FILE_OFFSET_BITS=64
CPPFLAGS+= -DHAVE_OFF64_T
CPPFLAGS+= -I.
CPPFLAGS+= -Iinclude
CPPFLAGS+= `pkg-config --cflags python3`

LIBS= -lrt -lpthread -lz -lcrypto 

CC= $(TOOLCHAIN)gcc
LD= $(TOOLCHAIN)gcc


all: pyadb.so

pyadb.so: $(SRCS)
	$(LD) -o $@ $(CPPFLAGS) $(SRCS) -shared -fPIC $(LIBS)

clean:
	-astyle --style=google -n *.c *.h 2>&1 1>/dev/null
	-rm -rf pyadb.so
