
#ifndef MYSQLWRAPPER_CONNECTION_H
#define MYSQLWRAPPER_CONNECTION_H

#include <mysql/mysql.h>
#include <string>

namespace MySQLWrapper {

class Result_t;

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

    ~Connection_t();

    void connect();

    void query(const std::string &sentence);

    long long lastInsertId();

    long long lastMatchingRows();

    std::string escape(const std::string &input);

private:
    const std::string host;
    const bool usingSocket;
    const std::string socket;
    const int port;
    const std::string user;
    const std::string pass;
    const std::string name;
    MYSQL mysql;
};

} // namespace MySQLWrapper

#endif // MYSQLWRAPPER_CONNECTION_H
