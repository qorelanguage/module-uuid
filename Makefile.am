AUTOMAKE_OPTIONS = foreign

SUBDIRS = src

noinst_HEADERS = src/uuid-module.h

EXTRA_DIST = COPYING ChangeLog AUTHORS README \
	RELEASE-NOTES \
	test/uuid-test.q \
	docs/uuid-module-doc.html \
	qore-uuid-module.spec

ACLOCAL_AMFLAGS=-I m4

libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck
