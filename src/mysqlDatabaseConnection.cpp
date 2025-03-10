#include "mysqlDatabaseConnection.h"

namespace database
{
    // MySQLQueryResult
    bool MySQLQueryResult::nextRow()
    {
        return (row = mysql_fetch_row(result)) != NULL;
    }

    std::vector<std::string> MySQLQueryResult::getRow()
    {
        return std::vector<std::string>(row, row + fields.size());
    }

    std::vector<std::string> MySQLQueryResult::getFields()
    {
        return fields;
    }

    
    // MySQLDBConnection
    void MySQLDBConnection::connect(const std::string& host, const std::string& user, const std::string& password, const std::string& database, unsigned int port)
    {
        connection = mysql_init(NULL);
        if (connection == NULL)
        {
            throw std::runtime_error("mysql_init() failed");
        }
        if (mysql_real_connect(connection, host.c_str(), user.c_str(), password.c_str(), database.c_str(), port, NULL, 0) == NULL)
        {
            throw std::runtime_error("mysql connection failed: " + std::string(mysql_error(connection)));
        }
    }

    void MySQLDBConnection::disconnect()
    {
        if (connection != NULL)
        {
            mysql_close(connection);
            connection = NULL;
        }
    }

    std::shared_ptr<QueryResult> MySQLDBConnection::query(const std::string& queryString)
    {
        if (connection == NULL)
        {
            throw std::runtime_error("connection is not established");
        }

        if (mysql_query(connection, queryString.c_str()) != 0)
        {
            throw std::runtime_error("query failed: " + std::string(mysql_error(connection)));
        }

        MYSQL_RES* result = mysql_store_result(connection);
        if (result == NULL)
        {
            throw std::runtime_error("mysql_store_result() failed: " + std::string(mysql_error(connection)));
        }
        
        return std::make_shared<MySQLQueryResult>(result);
    }

    uint64_t MySQLDBConnection::update(const std::string& queryString)
    {
        if (connection == NULL)
        {
            throw std::runtime_error("connection is not established");
        }

        if (mysql_query(connection, queryString.c_str()) != 0)
        {
            throw std::runtime_error("query failed: " + std::string(mysql_error(connection)));
        }

        return mysql_affected_rows(connection);
    }
}