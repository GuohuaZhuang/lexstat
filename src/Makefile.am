
# Some introduction here...
AUTOMAKE_OPTIONS				=	foreign

# Some introduction here...
ACLOCAL_AMFLAGS = -I m4

# Some introduction here...
lib_LTLIBRARIES					=	liblexstat.la

# Some introduction here...
liblexstat_la_SOURCES			=	lexstat.c				\
									stemming.c              \
									singleton_curl.cpp      \
									singleton_libxml2.cpp   \
									singleton_lexstat.cpp   \
									lexstat_api.cpp    
# 									gendict.c			     
# 									gendict_stopword.c  

# Some introduction here...
liblexstat_la_CXXFLAGS			=	$(CXXFLAGS)						\
									$(LIBXML2_CXXFLAGS)					\
									-DSYSCONFIG_DIR=\"$(sysconfig_dir)\"\
									-DDATA_DIR=\"$(datadir)/lexstat\"

# Some introduction here...
liblexstat_la_CPPFLAGS			=	$(CPPFLAGS)						\
									$(LIBXML2_CPPFLAGS)					\
									-DPREFIX=\"$(prefix)\"				\
									-Wno-deprecated						\
									-DSYSCONFIG_DIR=\"$(sysconfig_dir)\"\
									-DDATA_DIR=\"$(datadir)/lexstat\"
# Some introduction here...
liblexstat_la_CFLAGS			=	$(CFLAGS)						\
									$(LIBXML2_CFLAGS)					\
									-DPREFIX=\"$(prefix)\"				\
									-Wno-deprecated						\
									-DSYSCONFIG_DIR=\"$(sysconfig_dir)\"\
									-DDATA_DIR=\"$(datadir)/haha/lexstat\"

# Some introduction here...
liblexstat_la_LDFLAGS			=	$(LDFLAGS)	\
									$(LIBXML2_LIBS)	\
									-L$(prefix)/lib	

# Some introduction here...
liblexstat_la_LIBADD			=	-L$(prefix)/lib		\
									-ldl				\
									-lqdbm 				\
									-lxml2 				\
									-lm 				\
									-lcurl

# Some introduction here...
lexstatincludedir				=	$(includedir)/lexstat

# Some introduction here...
lexstatinclude_HEADERS			=	lexstat.h				\
									stemming.h				\
									singleton_curl.hpp      \
									singleton_libxml2.hpp   \
									singleton_lexstat.hpp   \
									lexstat_api.hpp

# Some introduction here...
# lexstatdatadir					=	$(datadir)/lexstat

