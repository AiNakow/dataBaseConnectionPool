#ifndef CONNECTIONBASE_H
#define CONNECTIONBASE_H

namespace databaseConnection
{
    typedef enum
    {
        MYSQL,
        ORACLE,
        POSTGRESQL,
        SQLITE
    } ConnectionType;
    
    class ConnectionBase
    {
    public:
        virtual void connect() = 0;
        virtual void disconnect() = 0;
        virtual void query() = 0;
        virtual void update() = 0;
        virtual bool isConnected() = 0;
        void getUnusedTime();
        void refreshUnusedTime();
    private:
        long unusedTime;
    };
} // namespace databaseConnection

#endif // CONNECTIONBASE_H