
AM_CXXFLAGS = -Werror -Wall -O2 -D_FILE_OFFSET_BITS=64 -g ${CXXEXTRAFLAGS} -I../../include

lib_LTLIBRARIES = libmysqlwrapper.la

libmysqlwrapper_la_SOURCES = \
    connection.cc \
    error.cc \
    mysqlwrapper.cc \
    result.cc \
    row.cc \
    util.cc

libmysqlwrapper_la_LDFLAGS = -version-info 1:0:0

library_includedir = $(includedir)/mysqlwrapper

library_include_HEADERS = \
    ../../include/mysqlwrapper/connection.h \
    ../../include/mysqlwrapper/error.h \
    ../../include/mysqlwrapper/mysqlwrapper.h \
    ../../include/mysqlwrapper/result.h \
    ../../include/mysqlwrapper/row.h \
    ../../include/mysqlwrapper/util.h

