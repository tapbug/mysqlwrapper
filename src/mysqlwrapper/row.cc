
#include <mysql/mysql.h>
#include <string>

#include <mysqlwrapper/row.h>
#include <mysqlwrapper/error.h>

namespace MySQLWrapper {

Row_t::Row_t(MYSQL_RES *result)
  : index(0),
    rowSize(result->field_count),
    row(mysql_fetch_row(result)),
    lengths(mysql_fetch_lengths(result))
{
}

Row_t::~Row_t()
{
}

int Row_t::size()
{
    return rowSize;
}

std::string Row_t::fetchNext()
{
    boost::optional<std::string> result(fetchNextOpt());
    if (result) {
        return *result;
    } else {
        return std::string();
    }
}

boost::optional<std::string> Row_t::fetchNextOpt()
{
    boost::optional<std::string> result;
    if (index >= rowSize) {
        throw MySQLWrapperError_t("No more values in row: size=%ld index=%ld",
                                  rowSize, index);
    }
    if (row[index]) {
        result.reset(std::string(row[index], lengths[index]));
    }
    ++index;
    return result;
}

Row_t& Row_t::operator>>(std::string &s)
{
    boost::optional<std::string> result(fetchNextOpt());
    if (result) {
        s = *result;
    }
    return *this;
}

Row_t& Row_t::operator>>(int &i)
{
    boost::optional<std::string> result(fetchNextOpt());
    if (result) {
        i = boost::lexical_cast<int>(*result);
    }
    return *this;
}

Row_t& Row_t::operator>> (double &d)
{
    boost::optional<std::string> result(fetchNextOpt());
    if (result) {
        d = boost::lexical_cast<double>(*result);
    }
    return *this;
}

Row_t& Row_t::operator>>(bool &b)
{
    boost::optional<std::string> result(fetchNextOpt());
    if (result) {
        if (*result == "1") {
            b = true;
        } else if (*result == "0") {
            b = false;
        } else {
            throw MySQLWrapperError_t("Invalid literal `%s' for boolean",
                result->c_str());
        }
    }
    return *this;
}

Row_t& Row_t::operator>>(boost::optional<std::string> &s)
{
    boost::optional<std::string> result(fetchNextOpt());
    if (result) {
        s.reset(*result);
    }
    return *this;
}

Row_t& Row_t::operator>>(boost::optional<int> &i)
{
    boost::optional<std::string> result(fetchNextOpt());
    if (result) {
        i.reset(boost::lexical_cast<int>(*result));
    }
    return *this;
}

Row_t& Row_t::operator>> (boost::optional<double> &d)
{
    boost::optional<std::string> result(fetchNextOpt());
    if (result) {
        d.reset(boost::lexical_cast<double>(*result));
    }
    return *this;
}

Row_t& Row_t::operator>>(boost::optional<bool> &b)
{
    boost::optional<std::string> result(fetchNextOpt());
    if (result) {
        if (*result == "1") {
            b.reset(true);
        } else if (*result == "0") {
            b.reset(false);
        } else {
            throw MySQLWrapperError_t("Invalid literal `%s' for boolean",
                result->c_str());
        }
    }
    return *this;
}

} // namespace MySQLWrapper
