#ifndef  __BINLOG_DUMP_GTID_PACKET_H_
#define  __BINLOG_DUMP_GTID_PACKET_H_

#include <string>
#include "packet.h"

namespace mysql_replicator {
class BinlogDumpGtidPacket : public Packet {
public:
    void toStream(std::ostream &os);
    void printPacket();
private:
    uint8_t header_;
    uint16_t warnings_num_;
    uint16_t status_flags_;
};
}
#endif  //__BINLOG_DUMP_GTID_PACKET_H_
