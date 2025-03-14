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
        static std::shared_ptr<ConnectionPool> getInstance(std::shared_ptr<DBConnectionFactory> factory, unsigned int min_size, unsigned int max_size, time_t idle_time = 60)
        {
            if (instance == nullptr)
            {
                instance = std::make_shared<ConnectionPool>(factory, min_size, max_size, idle_time);
            }
            return instance;
        }

        std::shared_ptr<DBConnection> getConnection();
        void releaseConnection(std::shared_ptr<DBConnection> connection);
        void shutdown();

        ConnectionPool() = delete;
        ConnectionPool(const ConnectionPool&) = delete;
        ConnectionPool& operator=(const ConnectionPool&) = delete;
        ConnectionPool(ConnectionPool&&) = delete;
        ConnectionPool& operator=(ConnectionPool&&) = delete;
    private:
        static std::shared_ptr<ConnectionPool> instance;
        std::atomic<ConnectionPoolState> state;
        std::atomic<size_t> min_size;
        std::atomic<size_t> max_size;
        std::atomic<size_t> current_size;
        std::atomic<time_t> idle_time;
        std::queue<std::shared_ptr<DBConnection>> connections;
        std::mutex mutex;
        std::condition_variable cv;
        std::shared_ptr<DBConnectionFactory> factory;
        std::thread monitorThread;
        std::thread producerThread;

        void monitorConnections();
        void produceConnections();

        ConnectionPool(std::shared_ptr<DBConnectionFactory> factory, unsigned int min_size, unsigned int max_size, time_t idle_time): 
                        factory(factory), min_size(min_size), max_size(max_size), current_size(0), idle_time(idle_time), state(ConnectionPoolState::RUNNING)
        {
            monitorThread = std::thread(&ConnectionPool::monitorConnections, this);
            producerThread = std::thread(&ConnectionPool::produceConnections, this);
        }
        
    };
}

#endif // CONNECTIONPOOL_H