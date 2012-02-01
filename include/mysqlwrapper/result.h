
#ifndef MYSQLWRAPPER_RESULT_H
#define MYSQLWRAPPER_RESULT_H

#include <mysql/mysql.h>
#include <string>

#include <mysqlwrapper/row.h>
#include <mysqlwrapper/mysqlwrapper.h>
#include <mysqlwrapper/error.h>
#include <mysqlwrapper/connection.h>

namespace MySQLWrapper {

class Result_t {
public:
    Result_t(MySQLWrapper_t *sql);

    template<class T_t>
    Result_t(T_t &sql)
      : index(0)
    {
        if (!sql->transaction) {
            throw MySQLWrapperError_t("Not in transaction");
        }
        result = mysql_store_result(&sql->transaction->mysql);
    }

    ~Result_t();

    Row_t fetchRow();

    unsigned long long size() const;

    bool empty() const;

    operator bool() const;

    Row_t operator>>(std::string &s);

    Row_t operator>>(int &i);

    Row_t operator>>(double &d);

    Row_t operator>>(bool &b);

    template<typename TO>
    Row_t operator>>(TO &x)
    {
        return fetchRow() >> x;
    }

private:
    MYSQL_RES *result;
    unsigned long long index;
    Row_t *currentRow;
};

} // namespace MySQLWrapper

#endif // MYSQLWRAPPER_RESULT_H
