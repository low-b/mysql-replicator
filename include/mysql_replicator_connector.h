#ifndef  __MYSQL_REPLICATOR_CONNECTOR_H__
#define  __MYSQL_REPLICATOR_CONNECTOR_H__

#include <string>
#include <boost/asio.hpp>
#include "mysql_replicator_statement.h"

namespace mysql_replicator {
class MySQLReplicatorConnector {
public:
    MySQLReplicatorConnector(std::string host,
            uint16_t port,
            std::string username,
            std::string password)
        : host_(host),
        port_(port),
        username_(username),
        password_(password) {}
    void connect();
    std::shared_ptr<MySQLReplicatorStatement> createStatement() {
        return std::make_shared<MySQLReplicatorStatement>(socket_);
    }
    std::shared_ptr<boost::asio::ip::tcp::socket> get_socket() {
        return socket_;
    }
private:
    boost::asio::io_service io_service_;
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    std::string host_;
    uint16_t port_;
    std::string username_;
    std::string password_;
};
}

#endif  //__MYSQL_REPLICATOR_CONNECTOR_H__
