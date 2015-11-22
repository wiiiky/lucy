#!/usr/bin/env python3
# encoding=utf8


from distutils.core import setup, Extension


pdb = Extension('pdb',
                  ['pdb/pdb.c', 'pdb/wrapper.c', 'pdb/util.c',
                   'pdb/adb.c', 'pdb/adb_client.c',
                   'pdb/adb_auth_host.c', 'pdb/commandline.c',
                   'pdb/console.c', 'pdb/file_sync_client.c',
                   'pdb/fdevent.c', 'pdb/get_my_path_linux.c',
                   'pdb/services.c', 'pdb/sockets.c', 'pdb/transport.c',
                   'pdb/transport_local.c', 'pdb/transport_usb.c',
                   'pdb/usb_linux.c', 'pdb/usb_vendors.c',
                   'pdb/libcutils/socket_inaddr_any_server.c',
                   'pdb/libcutils/socket_local_client.c',
                   'pdb/libcutils/socket_local_server.c',
                   'pdb/libcutils/socket_loopback_client.c',
                   'pdb/libcutils/socket_loopback_server.c',
                   'pdb/libcutils/socket_network_client.c',
                   'pdb/libcutils/load_file.c',
                   'pdb/libzipfile/centraldir.c',
                   'pdb/libzipfile/zipfile.c',
                   'pdb/libmincrypt/rsa.c', 'pdb/libmincrypt/sha256.c',
                   'pdb/libmincrypt/sha.c', 'pdb/libmincrypt/p256.c',
                   'pdb/libmincrypt/p256_ec.c',
                   'pdb/libmincrypt/p256_ecdsa.c'],
                  include_dirs=['pdb/include', 'pdb'],
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
      ext_modules=[pdb],
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
