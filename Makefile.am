## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src \
	res

dist_doc_DATA = \
	README \
	COPYING \
	AUTHORS \
	ChangeLog \
	INSTALL \
	NEWS

EXTRA_DIST = res

# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(docdir)
