#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H
#include "databaseConnection.h"
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>

namespace database
{
    enum class ConnectionPoolState : int8_t
    {
        RUNNING = 1,
        SHUTDOWN = 2
    };

    class ConnectionPool
    {
    public:
        ~ConnectionPool()
        {
            shutdown();
        }
        static std::shared_ptr<ConnectionPool> getInstance(std::shared_ptr<DBConnectionFactory> factory, unsigned int size)
        {
            if (instance == nullptr)
            {
                instance = std::make_shared<ConnectionPool>(factory, size);
            }
            return instance;
        }

        std::shared_ptr<DBConnection> getConnection();
        void releaseConnection(std::shared_ptr<DBConnection> connection);
        void monitorConnections();
        void shutdown();

        ConnectionPool(const ConnectionPool&) = delete;
        ConnectionPool& operator=(const ConnectionPool&) = delete;
        ConnectionPool(ConnectionPool&&) = delete;
        ConnectionPool& operator=(ConnectionPool&&) = delete;
    private:
        static std::shared_ptr<ConnectionPool> instance;
        std::atomic<ConnectionPoolState> state;
        std::atomic<size_t> min_size;
        std::atomic<size_t> max_size;
        std::queue<std::shared_ptr<DBConnection>> connections;
        std::mutex mutex;
        std::condition_variable cv;
        std::shared_ptr<DBConnectionFactory> factory;

        ConnectionPool(std::shared_ptr<DBConnectionFactory> factory, unsigned int size)
        {
            
        }
        
    };
}

#endif // CONNECTIONPOOL_H