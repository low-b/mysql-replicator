#ifndef __MYSQL_REPLICATOR_STATEMENT__
#define __MYSQL_REPLICATOR_STATEMENT__

#include <string>
#include <memory>
#include <boost/asio.hpp>
#include "mysql_replicator_resultset.h"

namespace mysql_replicator {
class MySQLReplicatorStatement {
public:
    MySQLReplicatorStatement(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
        :socket_(socket) {}
    std::shared_ptr<MySQLReplicatorResultSet> executeQuery(const std::string& sql);
    int executeUpdate(const std::string& sql);
private:
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
};
}
#endif //__MYSQL_REPLICATOR_STATEMENT__
