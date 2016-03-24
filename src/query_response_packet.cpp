#include "query_response_packet.h"
#include "bytes_helper.h"

namespace mysql_replicator {

void QueryResponsePacket::fromStream(std::istream &is) {
}

void QueryResponsePacket::printPacket() {
    std::cout << "--------query response packet begin---------" << std::endl;
    std::cout << "--------query response packet end---------" << std::endl;
}
}
