#ifndef  __NETWORK_DRIVER_H_
#define  __NETWORK_DRIVER_H_

#include <string>
#include <map>
#include <vector>
#include "binlog_event.pb.h"
#include "normal_parser.h"
#include "mysql_replicator_connector.h"
#include "db_meta.h"

namespace mysql_replicator {
using namespace binlog_listener;

template<typename BinlogStartPoint,
    typename BinlogParser = NormalParser>
class NetworkDriver : public BinlogParser {
public: 
    NetworkDriver(const std::string& host, uint16_t port,
            const std::string& username,
            const std::string& password):
        host_(host), port_(port), username_(username),
        password_(password) {
            conn_ = std::make_shared<MySQLReplicatorConnector>(
                    host_, port_, username_, password_);
        }
    void set_start_point(const BinlogStartPoint& start_point) {
        start_point_ = start_point;
    }
    const BinlogStartPoint& get_start_point() {
        return start_point_;
    }
    void init();
    std::shared_ptr<BinlogEvent> takeBinlogEvent();
private:
    BinlogStartPoint start_point_;
    //std::vector<Handler> _handlers;
    std::shared_ptr<DbMeta> db_meta_;
    std::shared_ptr<MySQLReplicatorConnector> conn_;
    std::string host_;
    uint16_t port_;
    std::string username_;
    std::string password_;

};
}

#include "network_driver.hpp"
#endif  //__NETWORK_DRIVER_H_
