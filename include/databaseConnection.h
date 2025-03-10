#ifndef CONNECTIONBASE_H
#define CONNECTIONBASE_H
#include <time.h>
#include <memory>
#include <string>
#include <vector>

namespace database
{
    class QueryResult
    {
    public:
        QueryResult();
        virtual ~QueryResult() = default;
        virtual bool nextRow() = 0;
        virtual std::vector<std::string> getRow() = 0;
        virtual std::vector<std::string> getFields() = 0;
    protected:
        std::vector<std::string> fields;
    };

    class DBConnection
    {
    public:
        DBConnection();
        virtual ~DBConnection() = default;
        virtual void connect(const std::string& host, const std::string& user, const std::string& password, const std::string& database, unsigned int port) = 0;
        virtual void disconnect() = 0;
        virtual std::shared_ptr<QueryResult> query(const std::string& queryString) = 0;
        virtual uint64_t update(const std::string& queryString) = 0;
        time_t getUnusedTime();
        void refreshLastUsedTime();
    private:
        time_t lastUsedTime;
    };

    class DBConnectionFactory
    {
    public:
        virtual ~DBConnectionFactory() = default;
        virtual std::shared_ptr<DBConnection> createConnection() = 0;
    };
} // namespace databaseConnection

#endif // CONNECTIONBASE_H