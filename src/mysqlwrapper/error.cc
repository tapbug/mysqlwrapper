
#include <stdarg.h>
#include <stdio.h>

#include <mysqlwrapper/error.h>

namespace MySQLWrapper {

#define MYSQLWRAPPER_ERROR_MESSAGEBUFFERSIZE 65536

#define VARARGSTOSTRING() \
    ({ \
        va_list valist; \
        char messageBuffer[MYSQLWRAPPER_ERROR_MESSAGEBUFFERSIZE]; \
        va_start(valist, format); \
        vsnprintf(messageBuffer, MYSQLWRAPPER_ERROR_MESSAGEBUFFERSIZE, format, valist); \
        va_end(valist); \
        messageBuffer; \
    })

MySQLWrapperError_t::MySQLWrapperError_t(const char *format, ...)
  : std::runtime_error(VARARGSTOSTRING()),
    _code(0)
{
}

MySQLWrapperError_t::MySQLWrapperError_t(const int _code, const char *format, ...)
  : std::runtime_error(VARARGSTOSTRING()),
    _code(_code)
{
}

MySQLWrapperError_t::MySQLWrapperError_t(const std::string &message)
  : std::runtime_error(message.c_str()),
    _code(0)
{
}

MySQLWrapperError_t::MySQLWrapperError_t(const int _code, const std::string &message)
  : std::runtime_error(message.c_str()),
    _code(_code)
{
}

int MySQLWrapperError_t::code() const
{
    return _code;
}

#undef VARARGSTOSTRING
#undef MYSQLWRAPPER_ERROR_MESSAGEBUFFERSIZE

} // namespace MySQLWrapper
