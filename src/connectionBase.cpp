#include "connectionBase.h"

namespace databaseConnection
{
    ConnectionBase::ConnectionBase()
    {
        lastUsedTime = time(NULL);
    }

    ConnectionBase::~ConnectionBase()
    {
    }

    time_t ConnectionBase::getUnusedTime()
    {
        return time(NULL) - lastUsedTime;
    }

    void ConnectionBase::refreshLastUsedTime()
    {
        lastUsedTime = time(NULL);
    }
} // namespace databaseConnection