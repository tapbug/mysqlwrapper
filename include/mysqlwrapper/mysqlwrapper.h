
#ifndef MYSQLWRAPPER_MYSQLWRAPPER_H
#define MYSQLWRAPPER_MYSQLWRAPPER_H

#include <string>
#include <mysqlwrapper/connection.h>

namespace MySQLWrapper {

class Connection_t;

class MySQLWrapper_t {
friend class Result_t;
public:
    MySQLWrapper_t(const std::string &masterHost,
                   const std::string &masterSocket,
                   const int masterPort,
                   const std::string &masterUser,
                   const std::string &masterPass,
                   const std::string &masterName,
                   const std::string &slaveHost = "",
                   const std::string &slaveSocket = "",
                   const int slavePort = 0,
                   const std::string &slaveUser = "",
                   const std::string &slavePass = "",
                   const std::string &slaveName = "");

    ~MySQLWrapper_t();

    void beginRO();

    void beginRW();

    void rollback();

    void commit();

    bool isTransaction();

    void query(const std::string &sentence);

    int lastInsertId();

    int lastMatchingRows();

    std::string escape(const std::string &input);

private:
    std::pair<Connection_t*, Connection_t*> connections;

    Connection_t *transaction;
};

} // namespace MySQLWrapper

#endif // MYSQLWRAPPER_MYSQLWRAPPER_H
