#ifndef  __EOF_PACKET_H_
#define  __EOF_PACKET_H_

#include <string>
#include "packet.h"

namespace mysql_replicator {
class EofPacket : public Packet {
public:
    void fromStream(std::istream &is);
    void printPacket();
private:
    uint8_t header_;
    uint16_t warnings_num_;
    uint16_t status_flags_;
};
}
#endif  //__EOF_PACKET_H_
