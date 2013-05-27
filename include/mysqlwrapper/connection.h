
#ifndef MYSQLWRAPPER_CONNECTION_H
#define MYSQLWRAPPER_CONNECTION_H

#include <mysql/mysql.h>
#include <stdint.h>
#include <string>
#include <vector>

namespace MySQLWrapper {

class Result_t;
class MySQLWrapper_t;

class Connection_t {
friend class Result_t;
public:
    Connection_t(const std::string &host,
                 const std::string &socket,
                 const std::string &user,
                 const std::string &pass,
                 const std::string &name);

    Connection_t(const std::string &host,
                 const int port,
                 const std::string &user,
                 const std::string &pass,
                 const std::string &name);

    Connection_t(const std::string &host,
                 const std::string &socket,
                 const std::string &user,
                 const std::string &pass,
                 const std::string &name,
                 const std::vector<std::string> &connectQueries);

    Connection_t(const std::string &host,
                 const int port,
                 const std::string &user,
                 const std::string &pass,
                 const std::string &name,
                 const std::vector<std::string> &connectQueries);

    ~Connection_t();

    void connect();

    void query(MySQLWrapper_t *sql,
               const std::string &sentence);

    uint64_t lastInsertId();

    size_t lastMatchingRows();

    std::string escape(const std::string &input);

private:
    const std::string host;
    const bool usingSocket;
    const std::string socket;
    const int port;
    const std::string user;
    const std::string pass;
    const std::string name;
    const std::vector<std::string> connectQueries;
    MYSQL mysql;
};

} // namespace MySQLWrapper

#endif // MYSQLWRAPPER_CONNECTION_H
