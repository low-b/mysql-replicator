#ifndef  __BINLOG_CHECK_POINT_H_
#define  __BINLOG_CHECK_POINT_H_
#include <memory>
#include "packet.h"
#include "log_event.h"
namespace mysql_replicator {
class BinlogCheckPoint {
public:
    virtual std::shared_ptr<Packet> get_dump_packet() = 0;
    virtual void process(std::shared_ptr<LogEvent> log_event) = 0;
    virtual std::string get_binlog_filename() {
        return "";
    }
    virtual uint64_t get_binlog_pos() {
        return 4;
    }
    virtual uint64_t get_progress() {
        return 0;
    }
};
}
#endif  //__BINLOG_CHECK_POINT_H_
