#ifndef  __NETWORK_DRIVER_H_
#define  __NETWORK_DRIVER_H_

#include <string>
#include <map>
#include <vector>
#include "binlog_event.pb.h"
#include "mysql_replicator_connector.h"
#include "mysql_replicator_dumper.h"
#include "db_meta.h"
#include "binlog_check_point.h"
#include "binlog_event_factory.h"

namespace mysql_replicator {

class NetworkDriver {
public: 
    NetworkDriver(const std::string& host, uint16_t port,
            const std::string& username,
            const std::string& password):
        host_(host), port_(port), username_(username),
        password_(password) {
        }
    void init();
    void sendBinlogDump(std::shared_ptr<BinlogCheckPoint> binlog_pos);
    void takeBinlogEvent(std::vector<std::shared_ptr<BinlogEvent> >& event_vec);
private:
    std::shared_ptr<DbMeta> db_meta_;
    std::shared_ptr<MySQLReplicatorConnector> conn_;
    std::shared_ptr<MySQLReplicatorDumper> dumper_;
    std::string host_;
    uint16_t port_;
    std::string username_;
    std::string password_;
    std::shared_ptr<BinlogEventFactory> event_factory_;
    std::shared_ptr<BinlogCheckPoint> check_point_;
};
}

#endif  //__NETWORK_DRIVER_H_
