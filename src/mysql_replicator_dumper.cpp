#include "mysql_replicator_dumper.h"
#include "packet_header.h"
#include "eof_packet.h"
#include "err_packet.h"
#include "binlog_event_header.h"
#include "bytes_helper.h"
#include "network_exception.h"
#include "binlog_dump_exception.h"
#include "binlog_event_factory.h"

namespace mysql_replicator {

void MySQLReplicatorDumper::sendBinlogDump(std::shared_ptr<BinlogCheckPoint> binlog_pos) {
    //send binlog dump packet
    BytesHelper::write(socket_, binlog_pos->get_dump_packet());
}

std::shared_ptr<LogEvent> MySQLReplicatorDumper::takeBinlogEvent() {
    std::shared_ptr<LogEvent> log_event;
    while (1) {
        boost::asio::streambuf response_message;
        std::istream response_stream(&response_message);
        BytesHelper::read(socket_, response_message);
        uint8_t stream_status = response_stream.peek();
        if (!stream_status == 0) {
            if (stream_status == 254) {
                EofPacket eof_packet;
                eof_packet.fromStream(response_stream);
                eof_packet.printPacket();
                return std::shared_ptr<LogEvent>();
            } else if (stream_status == 255) {
                ErrPacket err_packet;
                err_packet.fromStream(response_stream);
                err_packet.printPacket();
                throw BinlogDumpException("error occurred when take event from master");
            } else {
                throw BinlogDumpException("unknown return code");
            }
        }

        log_event = event_factory_->create(response_message);
        if (!log_event) {
            continue;
        }
        return log_event;
    }
}

}
