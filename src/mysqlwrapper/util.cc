
#include <mysqlwrapper/util.h>

namespace MySQLWrapper {

Transaction_t::Transaction_t(MySQLWrapper_t *db)
  : db(db)
{
}

Transaction_t::~Transaction_t()
{
}

ROTransaction_t::ROTransaction_t(MySQLWrapper_t *db)
  : Transaction_t(db)
{
    db->beginRO();
}

ROTransaction_t::~ROTransaction_t()
{
    if (db->isTransaction()) {
        db->rollback();
    }
}

void ROTransaction_t::commit()
{
    db->commit();
}

void ROTransaction_t::rollback()
{
    db->rollback();
}

RWTransaction_t::RWTransaction_t(MySQLWrapper_t *db)
  : Transaction_t(db)
{
    db->beginRW();
}

RWTransaction_t::~RWTransaction_t()
{
    if (db->isTransaction()) {
        db->rollback();
    }
}

void RWTransaction_t::commit()
{
    db->commit();
}

void RWTransaction_t::rollback()
{
    db->rollback();
}

} // namespace MySQLWrapper
