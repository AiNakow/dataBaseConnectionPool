#include "connectionPool.h"

namespace database
{
    std::shared_ptr<DBConnection> ConnectionPool::getConnection()
    {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, 
            [this]
            {
                return !connections.empty() || state.load() == ConnectionPoolState::SHUTDOWN;
            }
        );

        switch (state.load())
        {
        case ConnectionPoolState::RUNNING:
            break;
        case ConnectionPoolState::SHUTDOWN:
            throw std::runtime_error("connection pool is shutdown");
        default:
            break;
        }

        std::shared_ptr<DBConnection> connection = connections.front();
        connections.pop();
        return connection;
    }

    void ConnectionPool::releaseConnection(std::shared_ptr<DBConnection> connection)
    {
        std::unique_lock<std::mutex> lock(mutex);
        connection->refreshLastUsedTime();
        connections.push(connection);
        cv.notify_one();
    }

    void ConnectionPool::shutdown()
    {
        state = ConnectionPoolState::SHUTDOWN;
        cv.notify_all();
        monitorThread.join();
        producerThread.join();
    }

    void ConnectionPool::monitorConnections()
    {
        while (state.load() == ConnectionPoolState::RUNNING)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::unique_lock<std::mutex> lock(mutex);
            std::shared_ptr<DBConnection> old_connection = connections.front();
            if (old_connection->getUnusedTime() > idle_time)
            {
                connections.pop();
                current_size--;
            }
        }

        while (!connections.empty())
        {
            connections.pop();
            current_size--;
        }
    }

    void ConnectionPool::produceConnections()
    {
        while (state.load() == ConnectionPoolState::RUNNING)
        {
            std::unique_lock<std::mutex> lock(mutex);
            if (current_size < min_size)
            {
                connections.push(factory->createConnection());
                current_size++;
                cv.notify_one();
            }
            else
            {
                cv.wait(lock, 
                    [this]
                    {
                        return connections.empty();
                    }
                );

                if (current_size < max_size)
                {
                    connections.push(factory->createConnection());
                    current_size++;
                }
            }
            
        }
        
    }
}