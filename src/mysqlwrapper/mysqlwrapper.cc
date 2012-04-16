
#include <string>

#include <mysqlwrapper/mysqlwrapper.h>
#include <mysqlwrapper/result.h>
#include <mysqlwrapper/row.h>
#include <mysqlwrapper/error.h>

namespace MySQLWrapper {

MySQLWrapper_t::MySQLWrapper_t(const std::string &masterHost,
                               const std::string &masterSocket,
                               const int masterPort,
                               const std::string &masterUser,
                               const std::string &masterPass,
                               const std::string &masterName,
                               const std::string &slaveHost,
                               const std::string &slaveSocket,
                               const int slavePort,
                               const std::string &slaveUser,
                               const std::string &slavePass,
                               const std::string &slaveName)
{
    if (masterHost == "localhost") {
        connections.first = new Connection_t(masterHost,
                                             masterSocket,
                                             masterUser,
                                             masterPass,
                                             masterName);
    } else {
        connections.first = new Connection_t(masterHost,
                                             masterPort,
                                             masterUser,
                                             masterPass,
                                             masterName);
    }
    if (!slaveHost.empty()) {
       if (slaveHost == "localhost") {
            connections.second = new Connection_t(slaveHost,
                                                  slaveSocket,
                                                  slaveUser,
                                                  slavePass,
                                                  slaveName);
        } else {
            connections.second = new Connection_t(slaveHost,
                                                  slavePort,
                                                  slaveUser,
                                                  slavePass,
                                                  slaveName);
        }
    }
    transaction = NULL;
}

MySQLWrapper_t::~MySQLWrapper_t()
{
    if (connections.second) {
        delete connections.second;
    }
    delete connections.first;
}

void MySQLWrapper_t::beginRW()
{
    if (transaction) {
        throw MySQLWrapperError_t("Already in transaction");
    }
    try {
        transaction = connections.first;
        transaction->query(this, "BEGIN WORK");
    } catch (const MySQLWrapperError_t &e) {
        if (e.code() != 2006) {
            throw;
        }
        transaction = connections.first;
        transaction->query(this, "BEGIN WORK");
    }
}

void MySQLWrapper_t::beginRO()
{
    if (transaction) {
        throw MySQLWrapperError_t("Already in transaction");
    }
    try {
        if (connections.second) {
            transaction = connections.second;
        } else {
            transaction = connections.first;
        }
        transaction->query(this, "BEGIN WORK");
    } catch (const MySQLWrapperError_t &e) {
        if (e.code() != 2006) {
            throw;
        }
        if (connections.second) {
            transaction = connections.second;
        } else {
            transaction = connections.first;
        }
        transaction->query(this, "BEGIN WORK");
    }
}

void MySQLWrapper_t::rollback()
{
    if (!transaction) {
        throw MySQLWrapperError_t("Not in transaction");
    }
    try {
        transaction->query(this, "ROLLBACK");
    } catch (const MySQLWrapperError_t &e) {
        if (e.code() != 2006) {
            throw;
        }
    }
    transaction = 0;
}

void MySQLWrapper_t::commit()
{
    if (!transaction) {
        throw MySQLWrapperError_t("Not in transaction");
    }
    transaction->query(this, "COMMIT");
    transaction = 0;
}

bool MySQLWrapper_t::isTransaction()
{
    return transaction != NULL;
}

void MySQLWrapper_t::query(const std::string &sentence)
{
    if (!transaction) {
        throw MySQLWrapperError_t("Not in transaction");
    }
    transaction->query(this, sentence);
}


int MySQLWrapper_t::lastInsertId()
{
    if (!transaction) {
        throw MySQLWrapperError_t("Not in transaction");
    }
    return transaction->lastInsertId();
}

int MySQLWrapper_t::lastMatchingRows() {
    if (!transaction) {
        throw MySQLWrapperError_t("Not in transaction");
    }
    return transaction->lastMatchingRows();
}

std::string MySQLWrapper_t::escape(const std::string &input)
{
    if (!transaction) {
        throw MySQLWrapperError_t("Not in transaction");
    }
    return transaction->escape(input);
}

} // namespace MySQLWrapper
