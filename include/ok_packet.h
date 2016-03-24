#ifndef  __OK_PACKET_H_
#define  __OK_PACKET_H_

#include <string>
#include "packet.h"

namespace mysql_replicator {
class OkPacket : public Packet {
public:
    void fromStream(std::istream &is);
    void printPacket();
private:
    uint8_t header_;
    uint64_t affected_rows_;
    uint64_t last_insert_id_;
    uint16_t status_flags_;
    uint16_t warnings_num_;
    std::string status_info_;
};
}
#endif  //__ERR_PACKET_H_
