#include <iostream>
#include "connectionPool.h"
#include "mysqlDatabaseConnection.h"

int main(int, char**){
    std::shared_ptr<database::DBConnectionFactory> factory = std::make_shared<database::MySQLDBConnectionFactory>(
        "", "", "", ""
    );

    std::shared_ptr<database::ConnectionPool> pool = database::ConnectionPool::getInstance(factory, 10, 20);
}
