if COND_SINGLE_COMPILATION_UNIT
UUID_SOURCES = single-compilation-unit.cpp
else
UUID_SOURCES = uuid-module.cpp
endif

lib_LTLIBRARIES = uuid.la
uuid_la_SOURCES = $(UUID_SOURCES)
uuid_la_LDFLAGS = -module -avoid-version ${UUID_LIBS}

INCLUDES = -I$(top_srcdir)/include $(UUID_INCLUDES)

AM_CPPFLAGS=$(QORE_CPPFLAGS)

if COND_PROFILE
AM_CXXFLAGS=-g -pg
else
if COND_DEBUG
AM_CXXFLAGS=-g
endif
endif

install-exec-hook:
	mv $(DESTDIR)$(libdir)/uuid.$(MODULE_SUFFIX) $(DESTDIR)$(libdir)/uuid-api-$(MODULE_API).qmod
	rm -f $(DESTDIR)$(libdir)/uuid.la
