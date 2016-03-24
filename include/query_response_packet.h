#ifndef  __QUERY_RESPONSE_PACKET_H_
#define  __QUERY_RESPONSE_PACKET_H_

#include <string>
#include "packet.h"

namespace mysql_replicator {
class QueryResponsePacket : public Packet {
public:
    void fromStream(std::istream &is);
    void printPacket();
private:
};
}
#endif  //__QUERY_RESPONSE_PACKET_H_
