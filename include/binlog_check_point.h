#ifndef  __BINLOG_CHECK_POINT_H_
#define  __BINLOG_CHECK_POINT_H_
#include <memory>
#include "packet.h"
namespace mysql_replicator {
class BinlogCheckPoint {
public:
    virtual std::shared_ptr<Packet> get_dump_packet() = 0;

};
}
#endif  //__BINLOG_CHECK_POINT_H_
