#ifndef MYSQLDATABASECONNECTION_H
#define MYSQLDATABASECONNECTION_H
#include "databaseConnection.h"
#include <mysql/mysql.h>
#include <stdexcept>

namespace database 
{
    class MySQLQueryResult: public QueryResult
    {
    public:
        MySQLQueryResult(MYSQL_RES* result)
        {
            this->result = result;
            fields.clear();
            unsigned int numFields = mysql_num_fields(result);
            MYSQL_FIELD* field = mysql_fetch_fields(result);
            for (unsigned int i = 0; i < numFields; i++)
            {
                fields.push_back(field[i].name);
            }
        }

        ~MySQLQueryResult()
        {
            mysql_free_result(result);
        }

        bool nextRow() override;
        std::vector<std::string> getRow() override;
        std::vector<std::string> getFields() override;
    private:
        MYSQL_RES* result;
        MYSQL_ROW row;
    };

    class MySQLDBConnection: public DBConnection
    {
    public:
        MySQLDBConnection(const std::string& host, const std::string& user, const std::string& password, const std::string& database, unsigned int port = 3306)
        {
            connect(host, user, password, database, port);
        }

        ~MySQLDBConnection()
        {
            disconnect();
        }

        void connect(const std::string& host, const std::string& user, const std::string& password, const std::string& database, unsigned int port = 3306) override;
        void disconnect() override;
        std::shared_ptr<QueryResult> query(const std::string& queryString) override;
        uint64_t update(const std::string& queryString) override;
    private:
        MYSQL* connection;
    };
    
    class MySQLDBConnectionFactory: public DBConnectionFactory
    {
    public:
        MySQLDBConnectionFactory(const std::string& host, const std::string& user, const std::string& password, const std::string& database, unsigned int port = 3306)
        {
            this->host = host;
            this->user = user;
            this->password = password;
            this->database = database;
            this->port = port;
        }    

        std::shared_ptr<DBConnection> createConnection() override
        {
            return std::make_shared<MySQLDBConnection>(host, user, password, database, port);
        }

        MySQLDBConnectionFactory() = delete;
    private:
        std::string host;
        std::string user;
        std::string password;
        std::string database;
        unsigned int port;
    };
}


#endif