#include "mysql_replicator_statement.h"
#include "bytes_helper.h"
#include "query_packet.h"
#include "ok_packet.h"
#include "err_packet.h"
#include "query_response_packet.h"
#include "packet_header.h"

namespace mysql_replicator {
std::shared_ptr<MySQLReplicatorResultSet>
MySQLReplicatorStatement::executeQuery(const std::string& sql) {
    //send query packet
    std::shared_ptr<QueryPacket> query_packet(new QueryPacket);
    query_packet->set_query_string(sql);
    BytesHelper::write(socket_, query_packet);
    //get response packet
    boost::asio::streambuf response_message;
    std::istream response_stream(&response_message);
    boost::asio::read(*socket_, response_message, boost::asio::transfer_exactly(4));
    PacketHeader response_header;
    response_header.fromStream(response_stream);
    response_header.printPacket();
    boost::asio::read(*socket_, response_message,
            boost::asio::transfer_exactly(response_header.get_packet_len()));
    uint8_t result_num = boost::asio::buffer_cast<const char*>(response_message.data())[0];
    if (result_num == 255) {
        printf("in error\n");
        ErrPacket err_packet;
        err_packet.fromStream(response_stream);
        err_packet.printPacket();
        return std::shared_ptr<MySQLReplicatorResultSet>();
    } else if (result_num == 251) {
        //LOCAL INFILE
        return std::shared_ptr<MySQLReplicatorResultSet>();
    } else if (result_num == 0) {
        printf("in ok\n");
        OkPacket ok_packet;
        ok_packet.fromStream(response_stream);
        ok_packet.printPacket();
        return std::shared_ptr<MySQLReplicatorResultSet>();
    } else {
        printf("in query response\n");
        QueryResponsePacket query_resonpse_packet;
        query_resonpse_packet.fromStream(response_stream);
        query_resonpse_packet.printPacket();
    }
    return std::shared_ptr<MySQLReplicatorResultSet>();
}
int MySQLReplicatorStatement::executeUpdate(const std::string& sql) {
}
}
