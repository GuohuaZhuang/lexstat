
AUTOMAKE_OPTIONS = foreign

ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = liblexstat.la

liblexstat_la_SOURCES = lexstat.c               \
                        stemming.c              \
                        singleton_curl.cpp      \
                        singleton_libxml2.cpp   \
                        singleton_lexstat.cpp   \
                        lexstat_api.cpp    

liblexstat_la_CXXFLAGS = $(CXXFLAGS)                            \
                         $(LIBXML2_CXXFLAGS)                    \
                         -DSYSCONFIG_DIR=\"$(sysconfig_dir)\"   \
                         -DDATA_DIR=\"$(datadir)/lexstat\"

liblexstat_la_CPPFLAGS = $(CPPFLAGS)                            \
                         $(LIBXML2_CPPFLAGS)                    \
                         -DPREFIX=\"$(prefix)\"                 \
                         -Wno-deprecated                        \
                         -DSYSCONFIG_DIR=\"$(sysconfig_dir)\"   \
                         -DDATA_DIR=\"$(datadir)/lexstat\"

liblexstat_la_CFLAGS = $(CFLAGS)                                \
                       $(LIBXML2_CFLAGS)                        \
                       -DPREFIX=\"$(prefix)\"                   \
                       -Wno-deprecated                          \
                       -DSYSCONFIG_DIR=\"$(sysconfig_dir)\"     \
                       -DDATA_DIR=\"$(datadir)/lexstat\"

liblexstat_la_LDFLAGS = $(LDFLAGS)        \
                        $(LIBXML2_LIBS)   \
                        -L$(prefix)/lib    

liblexstat_la_LIBADD = -L$(prefix)/lib     \
                       -ldl                \
                       -lqdbm              \
                       -lxml2              \
                       -lm                 \
                       -lcurl

lexstatincludedir = $(includedir)/lexstat

lexstatinclude_HEADERS = lexstat.h               \
                         stemming.h              \
                         singleton_curl.hpp      \
                         singleton_libxml2.hpp   \
                         singleton_lexstat.hpp   \
                         lexstat_api.hpp

# lexstatdatadir = $(datadir)/lexstat

bin_PROGRAMS = lexstat

lexstat_SOURCES = lexstat.c
lexstat_LDADD = -ldl liblexstat.la 
lexstat_CFLAGS = $(CFLAGS)                              \
                 $(LIBXML2_CFLAGS)                      \
                 -DPREFIX=\"$(prefix)\"                 \
                 -Wno-deprecated                        \
                 -DSYSCONFIG_DIR=\"$(sysconfig_dir)\"   \
                 -DDATA_DIR=\"$(datadir)/lexstat\"

lexstat_CXXFLAGS = $(CXXFLAGS)                          \
                   $(LIBXML2_CXXFLAGS)                  \
                   -DSYSCONFIG_DIR=\"$(sysconfig_dir)\" \
                   -DDATA_DIR=\"$(datadir)/lexstat\"

lexstat_CPPFLAGS = $(CPPFLAGS)                          \
                   $(LIBXML2_CPPFLAGS)                  \
                   -DPREFIX=\"$(prefix)\"               \
                   -Wno-deprecated                      \
                   -DSYSCONFIG_DIR=\"$(sysconfig_dir)\" \
                   -DDATA_DIR=\"$(datadir)/lexstat\"

