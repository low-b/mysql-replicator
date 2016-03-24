#ifndef  __QUERY_PACKET_H_
#define  __QUERY_PACKET_H_

#include <string>
#include "packet.h"
#include "mysql_replicator_com.h"

namespace mysql_replicator {
class QueryPacket : public Packet {
public:
    QueryPacket() {
        command_ = COM_QUERY;
    }
    void set_query_string(std::string query_string) {
        query_string_ = query_string;
    }
    void toStream(std::ostream &os);
    void printPacket();
private:
    uint8_t command_;
    std::string query_string_;
};
}
#endif  //__QUERY_PACKET_H_
