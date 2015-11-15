#!/usr/bin/env python3
# encoding=utf8


from distutils.core import setup, Extension

pyadb = Extension('pyadb',
                  ['pyadb/pyadb.c', 'pyadb/wrapper.c', 'pyadb/util.c',
                   'pyadb/adb.c', 'pyadb/adb_client.c',
                   'pyadb/adb_auth_host.c', 'pyadb/commandline.c',
                   'pyadb/console.c', 'pyadb/file_sync_client.c',
                   'pyadb/fdevent.c', 'pyadb/get_my_path_linux.c',
                   'pyadb/services.c', 'pyadb/sockets.c', 'pyadb/transport.c',
                   'pyadb/transport_local.c', 'pyadb/transport_usb.c',
                   'pyadb/usb_linux.c', 'pyadb/usb_vendors.c',
                   'pyadb/libcutils/socket_inaddr_any_server.c',
                   'pyadb/libcutils/socket_local_client.c',
                   'pyadb/libcutils/socket_local_server.c',
                   'pyadb/libcutils/socket_loopback_client.c',
                   'pyadb/libcutils/socket_loopback_server.c',
                   'pyadb/libcutils/socket_network_client.c',
                   'pyadb/libcutils/load_file.c',
                   'pyadb/libzipfile/centraldir.c',
                   'pyadb/libzipfile/zipfile.c',
                   'pyadb/libmincrypt/rsa.c', 'pyadb/libmincrypt/sha256.c',
                   'pyadb/libmincrypt/sha.c', 'pyadb/libmincrypt/p256.c',
                   'pyadb/libmincrypt/p256_ec.c',
                   'pyadb/libmincrypt/p256_ecdsa.c'],
                  include_dirs=['pyadb/include', 'pyadb'],
                  define_macros=[('HAVE_SYS_PRCTL_H', 1),
                                 ('ADB_HOST', 1),
                                 ('HAVE_FORKEXEC', 1),
                                 ('HAVE_SYMLINKS', 1),
                                 ('HAVE_TERMIO_H', 1),
                                 ('_GNU_SOURCE', 1),
                                 ('_FILE_OFFSET_BITS', 64),
                                 ('HAVE_OFF64_T', 1)],
                  extra_link_args=['-lrt', '-pthread', '-lz', '-lcrypto']
                  )

setup(name='lucy',
      version='0.1',
      description='Android Device Manager based on adb',
      author='Wiky L',
      author_email='wiiiky@outlook.com',
      url='',
      scripts=['bin/lucy.py'],
      packages=['pydroid'],
      ext_modules=[pyadb],
      classifiers=[
          'Development Status :: 0.1',
          'Environment :: GNOME/GTK',
          'Intended Audience :: End Users/Desktop',
          'License :: OSI Approved :: GPLv3',
          'Operating System :: Linux',
          'Programming Language :: Python3',
          'Topic :: Tool',
          ],
      )
