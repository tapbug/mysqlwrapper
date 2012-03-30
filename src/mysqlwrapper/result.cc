
#include <mysql/mysql.h>
#include <string>

#include <mysqlwrapper/result.h>

namespace MySQLWrapper {

Result_t::Result_t(MySQLWrapper_t *sql)
  : index(0)
{
    if (!sql->transaction) {
        throw MySQLWrapperError_t("Not in transaction");
    }
    result = mysql_store_result(&sql->transaction->mysql);
    if (!result) {
        int err(mysql_errno(&sql->transaction->mysql));
        throw MySQLWrapperError_t(err,
                                  "Can't fetch result: %s",
                                  mysql_error(&sql->transaction->mysql));
    }
}

Result_t::~Result_t()
{
    mysql_free_result(result);
}

Row_t Result_t::fetchRow()
{
    if (index >= result->row_count) {
        throw MySQLWrapperError_t("No more rows in result: size=%ld, index=%ld",
                                  index,
                                  result->row_count);
    }
    ++index;
    return Row_t(result);
}

unsigned long long Result_t::size() const
{
    return result->row_count;
}

bool Result_t::empty() const
{
    return result->row_count == 0;
}

Result_t::operator bool() const
{
    return index < result->row_count;
}

Row_t Result_t::operator>>(std::string &s)
{
    return fetchRow() >> s;
}

Row_t Result_t::operator>>(int &i)
{
    return fetchRow() >> i;
}

Row_t Result_t::operator>>(double &d)
{
    return fetchRow() >> d;
}

Row_t Result_t::operator>>(bool &b)
{
    return fetchRow() >> b;
}

} // namespace MySQLWrapper
