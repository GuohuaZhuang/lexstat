
AUTOMAKE_OPTIONS			=	foreign

ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS				=	lexstat_web.fcgi

lexstat_web_fcgi_SOURCES	=	lexstat_web.cpp
lexstat_web_fcgi_LDADD		=	-ldl -lfcgi ../src/liblexstat.la
lexstat_web_fcgi_CPPFLAGS	=	$(CPPFLAGS) -Wno-deprecated -DSYSCONFIG_DIR=\"$(sysconfig_dir)\" -DDATA_DIR=\"$(data_dir)\"
lexstat_web_fcgi_CXXFLAGS	=	$(CXXFLAGS) -Wno-deprecated -DSYSCONFIG_DIR=\"$(sysconfig_dir)\" -DDATA_DIR=\"$(data_dir)\"

