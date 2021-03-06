#ifndef  __BINLOG_POSITION_H_
#define  __BINLOG_POSITION_H_

#include <memory>
#include <string>
#include "binlog_check_point.h"
#include "packet.h"

namespace mysql_replicator {
class BinlogPosition : public BinlogCheckPoint {
public:
    BinlogPosition() : flags_(0), slave_id_(0) {
    }
    void set_binlog_filename(std::string filename) {
        binlog_filename_ = filename;
    }
    std::string get_binlog_filename() {
        return binlog_filename_;
    }
    void set_binlog_pos(uint32_t pos) {
        binlog_pos_ = pos;
    }
    uint64_t get_binlog_pos() {
        return binlog_pos_;
    }
    void set_flags(uint16_t flags) {
        flags_ = flags;
    }
    void set_slave_id(uint32_t id) {
        slave_id_ = id;
    }
    virtual std::shared_ptr<Packet> get_dump_packet();
    virtual void process(std::shared_ptr<LogEvent> log_event);
private:
    std::string binlog_filename_;
    uint32_t binlog_pos_;
    uint16_t flags_;
    uint32_t slave_id_;
};
}
#endif  //__BINLOG_POSITION_H_
