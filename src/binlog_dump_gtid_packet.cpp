#include "binlog_dump_gtid_packet.h"
#include "bytes_helper.h"

namespace mysql_replicator {

void BinlogDumpGtidPacket::toStream(std::ostream &os) {
}

void BinlogDumpGtidPacket::printPacket() {
    std::cout << "--------binlog dump gtid packet begin---------" << std::endl;
    std::cout << "--------binlog dump gtid packet end---------" << std::endl;
}
}
