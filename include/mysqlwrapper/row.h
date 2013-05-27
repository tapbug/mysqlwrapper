
#ifndef MYSQLWRAPPER_ROW_H
#define MYSQLWRAPPER_ROW_H

#include <mysql/mysql.h>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

namespace MySQLWrapper {

class Row_t {
public:
    Row_t(MYSQL_RES *result);

    ~Row_t();

    int size();

    Row_t& operator>>(std::string &s);

    Row_t& operator>>(int &i);

    Row_t& operator>>(double &d);

    Row_t& operator>>(bool &b);

    template<typename TO>
    Row_t& operator>> (TO &x)
    {
        boost::optional<std::string> result(fetchNextOpt());
        if (result) {
            x = boost::lexical_cast<TO>(*result);
        }
        return *this;
    }

    Row_t& operator>>(boost::optional<std::string> &s);

    Row_t& operator>>(boost::optional<int> &i);

    Row_t& operator>>(boost::optional<double> &d);

    Row_t& operator>>(boost::optional<bool> &b);

    template<typename TO>
    Row_t& operator>> (boost::optional<TO> &x)
    {
        boost::optional<std::string> result(fetchNextOpt());
        if (result) {
            x.reset(boost::lexical_cast<TO>(*result));
        }
        return *this;
    }

private:
    std::string fetchNext();

    boost::optional<std::string> fetchNextOpt();

    unsigned long long index;
    unsigned long long rowSize;
    MYSQL_ROW row;
    unsigned long *lengths;
};

} // namespace MySQLWrapper

#endif // MYSQLWRAPPER_ROW_H
