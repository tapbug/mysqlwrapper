
#ifndef MYSQLWRAPPER_UTIL_H
#define MYSQLWRAPPER_UTIL_H

#include <mysqlwrapper/mysqlwrapper.h>

namespace MySQLWrapper {

class Transaction_t {
public:
    Transaction_t(MySQLWrapper_t *db);

    template<class T_t>
    Transaction_t(T_t &db)
      : db(db.get())
    {
    }

    virtual ~Transaction_t();

    virtual void commit() = 0;

    virtual void rollback() = 0;

protected:
    MySQLWrapper_t *db;
};

class ROTransaction_t : public Transaction_t {
public:
    ROTransaction_t(MySQLWrapper_t *db);

    template<class T_t>
    ROTransaction_t(T_t &db)
      : Transaction_t(db.get())
    {
        db->beginRO();
    }

    virtual ~ROTransaction_t();

    virtual void commit();

    virtual void rollback();
};

class RWTransaction_t : public Transaction_t {
public:
    RWTransaction_t(MySQLWrapper_t *db);

    template<class T_t>
    RWTransaction_t(T_t &db)
      : Transaction_t(db.get())
    {
        db->beginRW();
    }

    virtual ~RWTransaction_t();

    virtual void commit();

    virtual void rollback();
};

} // namespace MySQLWrapper

#endif // MYSQLWRAPPER_UTIL_H
