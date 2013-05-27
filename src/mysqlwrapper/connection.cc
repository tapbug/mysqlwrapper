
#include <mysql/mysql.h>
#include <string>
#include <stdlib.h>

#include <mysqlwrapper/mysqlwrapper.h>
#include <mysqlwrapper/connection.h>
#include <mysqlwrapper/error.h>

namespace MySQLWrapper {

Connection_t::Connection_t(const std::string &host,
                           const std::string &socket,
                           const std::string &user,
                           const std::string &pass,
                           const std::string &name)
  : host(host),
    usingSocket(true),
    socket(socket),
    port(0),
    user(user),
    pass(pass),
    name(name)
{
    connect();
}

Connection_t::Connection_t(const std::string &host,
                           const int port,
                           const std::string &user,
                           const std::string &pass,
                           const std::string &name)
  : host(host),
    usingSocket(false),
    socket(),
    port(port),
    user(user),
    pass(pass),
    name(name)
{
    connect();
}

Connection_t::Connection_t(const std::string &host,
                           const std::string &socket,
                           const std::string &user,
                           const std::string &pass,
                           const std::string &name,
                           const std::vector<std::string> &connectQueries)
  : host(host),
    usingSocket(true),
    socket(socket),
    port(0),
    user(user),
    pass(pass),
    name(name),
    connectQueries(connectQueries)
{
    connect();
}

Connection_t::Connection_t(const std::string &host,
                           const int port,
                           const std::string &user,
                           const std::string &pass,
                           const std::string &name,
                           const std::vector<std::string> &connectQueries)
  : host(host),
    usingSocket(false),
    socket(),
    port(port),
    user(user),
    pass(pass),
    name(name),
    connectQueries(connectQueries)
{
    connect();
}

Connection_t::~Connection_t()
{
    mysql_close(&mysql);
}

void Connection_t::connect()
{
    MYSQL *result;
    mysql_init(&mysql);
    if (usingSocket) {
        result = mysql_real_connect(&mysql,
                                    host.c_str(),
                                    user.c_str(),
                                    pass.c_str(),
                                    name.c_str(),
                                    0,
                                    socket.c_str(),
                                    CLIENT_FOUND_ROWS);
    } else {
        result = mysql_real_connect(&mysql,
                                    host.c_str(),
                                    user.c_str(),
                                    pass.c_str(),
                                    name.c_str(),
                                    port,
                                    NULL,
                                    CLIENT_FOUND_ROWS);
    }

    if (!result) {
        throw MySQLWrapperError_t(mysql_errno(&mysql),
                                  "Cannot connect to MySQL server: %s",
                                  mysql_error(&mysql));
    }
    if (mysql_real_query(&mysql, "SET NAMES utf8", 14)) {
        throw MySQLWrapperError_t(mysql_errno(&mysql),
                                  "Cannot set encoding: %s",
                                  mysql_error(&mysql));
    }
    for (std::vector<std::string>::const_iterator iconnectQueries(connectQueries.begin()) ;
         iconnectQueries != connectQueries.end() ;
         ++iconnectQueries) {

        if (mysql_real_query(&mysql, iconnectQueries->data(), iconnectQueries->size())) {
            throw MySQLWrapperError_t(mysql_errno(&mysql),
                                      "Cannot execute connect query '%s': %s",
                                      iconnectQueries->c_str(),
                                      mysql_error(&mysql));
        }
    }
}

void Connection_t::query(MySQLWrapper_t *sql,
                         const std::string &sentence)
{
    int result = mysql_real_query(&mysql, sentence.data(), sentence.size());
    if (result) {
        if (mysql_errno(&mysql) == 2006) {
            // connection lost error
            sql->transaction = 0;
            connect();
            throw MySQLWrapperError_t(2006, "Lost connection to MySQL server.");
        } else {
            throw MySQLWrapperError_t(mysql_errno(&mysql),
                                      "Can't execute query: %s, query was: %s",
                                      mysql_error(&mysql),
                                      sentence.c_str());
        }
    }
}

uint64_t Connection_t::lastInsertId()
{
    return mysql_insert_id(&mysql);
}

size_t Connection_t::lastMatchingRows()
{
    return mysql_affected_rows(&mysql);
}

std::string Connection_t::escape(const std::string &input)
{
    char *out = 0;
    int size = input.length()*2+1;
    out = (char*) malloc(size+2);
    if (out) {
        out[0] = '\'';
        int size = mysql_real_escape_string(&mysql, out+1, input.c_str(), input.length());
        out[size+2] = 0;
        out[size+1] = '\'';
        std::string output(out, size+2);
        free((void*) out);
        return output;
    }
    throw std::bad_alloc();
}

} // namespace MySQLWrapper
