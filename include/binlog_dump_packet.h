#ifndef  __BINLOG_DUMP_PACKET_H_
#define  __BINLOG_DUMP_PACKET_H_

#include <string>
#include "packet.h"
#include "mysql_replicator_com.h"

namespace mysql_replicator {
class BinlogDumpPacket : public Packet {
public:
    BinlogDumpPacket() {
        command_ = COM_BINLOG_DUMP;
        flags_ = 0;
    }
    void toStream(std::ostream &os);
    void printPacket();
    void set_binlog_pos(uint32_t pos) {
        binlog_pos_ = pos;
    }
    void set_flags(uint16_t flags) {
        flags_ = flags;
    }
    void set_slave_id(uint32_t id) {
        slave_id_ = id;
    }
    void set_binlog_filename(std::string filename) {
        binlog_filename_ = filename;
    }
private:
    uint8_t command_;
    uint32_t binlog_pos_;
    uint16_t flags_;
    uint32_t slave_id_;
    std::string binlog_filename_;
};
}
#endif  //__BINLOG_DUMP_PACKET_H_
