#ifndef CONNECTIONBASE_H
#define CONNECTIONBASE_H
#include <time.h>

namespace databaseConnection
{
    typedef enum
    {
        MYSQL,
        REDIS
    } ConnectionType;
    
    class ConnectionBase
    {
    public:
        ConnectionBase();
        virtual ~ConnectionBase();
        virtual void connect() = 0;
        virtual void disconnect() = 0;
        virtual void query() = 0;
        virtual void update() = 0;
        time_t getUnusedTime();
        void refreshLastUsedTime();
    private:
        time_t lastUsedTime;
    };
} // namespace databaseConnection

#endif // CONNECTIONBASE_H