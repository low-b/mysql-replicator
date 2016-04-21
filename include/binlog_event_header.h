#ifndef  __BINLOG_EVENT_HEADER_H_
#define  __BINLOG_EVENT_HEADER_H_

#include "packet.h"

namespace mysql_replicator {
struct BinlogEventHeader : public Packet {
public:
    void fromStream(std::istream &is);
    void printPacket();
    uint8_t marker;
    uint32_t timestamp;
    uint8_t event_type;
    uint32_t server_id;
    uint32_t event_size;
    uint32_t log_pos;
    uint16_t flags;
};
}

#endif  //__BINLOG_EVENT_HEADER_H_
