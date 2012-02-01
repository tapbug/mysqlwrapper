
#ifndef MYSQLWRAPPER_ERROR_H
#define MYSQLWRAPPER_ERROR_H

#include <stdexcept>
#include <string>
#include <mysql/mysqld_error.h>

namespace MySQLWrapper {

class MySQLWrapperError_t : public std::runtime_error {
public:
    MySQLWrapperError_t(const char *format, ...);

    MySQLWrapperError_t(const int _code, const char *format, ...);

    MySQLWrapperError_t(const std::string &message);

    MySQLWrapperError_t(const int _code, const std::string &message);

    int code() const;

private:
    const int _code;
};

} // namespace MySQLWrapper

#endif // MYSQLWRAPPER_ERROR_H
