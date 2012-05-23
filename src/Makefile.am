MNAME=uuid

QC_UUID.cpp: QC_UUID.qpp
	qpp $<

GENERATED_SOURCES = QC_UUID.cpp
CLEANFILES = $(GENERATED_SOURCES)

if COND_SINGLE_COMPILATION_UNIT
UUID_SOURCES = single-compilation-unit.cpp
single-compilation-unit.cpp: $(GENERATED_SOURCES)
else
UUID_SOURCES = uuid-module.cpp
nodist_uuid_la_SOURCES = $(GENERATED_SOURCES)
endif

lib_LTLIBRARIES = uuid.la
uuid_la_SOURCES = $(UUID_SOURCES)
uuid_la_LDFLAGS = -module -avoid-version ${UUID_LIBS} ${MODULE_LDFLAGS}

INCLUDES = -I$(top_srcdir)/include

AM_CPPFLAGS=$(QORE_CPPFLAGS) $(UUID_CPPFLAGS)

if COND_PROFILE
AM_CXXFLAGS=-g -pg
else
if COND_DEBUG
AM_CXXFLAGS=-g
endif
endif

install-exec-hook:
	mv $(DESTDIR)$(libdir)/$(MNAME).$(MODULE_SUFFIX) $(DESTDIR)$(libdir)/$(MNAME)-api-$(MODULE_API).qmod
	$(RM) $(DESTDIR)$(libdir)/$(MNAME).la $(DESTDIR)$(libdir)/$(MNAME).dll.a
