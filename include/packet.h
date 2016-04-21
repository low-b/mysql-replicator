#ifndef  __MYSQL_REPLICATOR_PACKET_H_
#define  __MYSQL_REPLICATOR_PACKET_H_

#include <iostream>
#include "network_exception.h"

namespace mysql_replicator {
class Packet {
public:
    virtual void fromStream(std::istream&) {
        throw NetworkException("unsupported methods:fromStream");
    }
    virtual void toStream(std::ostream&) {
        throw NetworkException("unsupported methods:toStream");
    }
    virtual void printPacket() {
        throw NetworkException("unsupported methods:printPacket");
    }
};
}

#endif  //__MYSQL_REPLICATOR_PACKET_H_
