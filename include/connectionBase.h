#ifndef CONNECTIONBASE_H
#define CONNECTIONBASE_H

namespace databaseConnection
{
    class ConnectionBase
    {
    public:
        virtual void connect() = 0;
        virtual void disconnect() = 0;
        virtual bool isConnected() = 0;
    };
} // namespace databaseConnection

#endif // CONNECTIONBASE_H