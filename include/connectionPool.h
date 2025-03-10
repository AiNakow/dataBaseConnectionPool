#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H
#include "databaseConnection.h"
#include <vector>

namespace database
{
    class ConnectionPool
    {
    public:
        ConnectionPool(std::shared_ptr<DBConnectionFactory> factory, unsigned int size);
        ~ConnectionPool();
        std::shared_ptr<DBConnection> getConnection();
        void releaseConnection(std::shared_ptr<DBConnection> connection);
    };
}

#endif // CONNECTIONPOOL_H