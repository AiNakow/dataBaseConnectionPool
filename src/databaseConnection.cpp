#include "databaseConnection.h"

namespace database
{
    DBConnection::DBConnection()
    {
        lastUsedTime = time(NULL);
    }

    DBConnection::~DBConnection()
    {
    }

    time_t DBConnection::getUnusedTime()
    {
        return time(NULL) - lastUsedTime;
    }

    void DBConnection::refreshLastUsedTime()
    {
        lastUsedTime = time(NULL);
    }
} // namespace databaseConnection