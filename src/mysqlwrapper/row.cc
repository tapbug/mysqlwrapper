
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
    if (index >= rowSize) {
        throw MySQLWrapperError_t("No more values in row: size=%ld index=%ld",
                                  rowSize, index);
    }
    std::string value(row[index], lengths[index]);
    ++index;
    return value;
}

Row_t& Row_t::operator>>(std::string &s)
{
    s = fetchNext();
    return *this;
}

Row_t& Row_t::operator>>(int &i)
{
    i = lexical_cast<std::string, int>(fetchNext());
    return *this;
}

Row_t& Row_t::operator>> (double &d)
{
    d = lexical_cast<std::string, double>(fetchNext());
    return *this;
}

Row_t& Row_t::operator>>(bool &b)
{
    std::string val = fetchNext();
    if (val == "1") {
        b = true;
    } else if (val == "0") {
        b = false;
    } else {
        throw MySQLWrapperError_t("Invalid literal `%s' for boolean",
                                  val.c_str());
    }
    return *this;
}

} // namespace MySQLWrapper
