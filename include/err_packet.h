#ifndef  __ERR_PACKET_H_
#define  __ERR_PACKET_H_

#include <string>
#include "packet.h"

namespace mysql_replicator {
class ErrPacket : public Packet {
public:
    void fromStream(std::istream &is);
    void printPacket();
private:
    uint8_t header_;
    uint16_t error_code_;
    std::string sql_state_marker_;
    std::string sql_state_;
    std::string error_message_;
};
}
#endif  //__ERR_PACKET_H_
