#include "mysql_replicator_statement.h"
#include "bytes_helper.h"
#include "query_packet.h"
#include "ok_packet.h"
#include "err_packet.h"
#include "eof_packet.h"
#include "packet_header.h"
#include "column_definition_packet.h"
#include "resultset_row_packet.h"

using std::shared_ptr;

namespace mysql_replicator {
std::shared_ptr<MySQLReplicatorResultset>
MySQLReplicatorStatement::executeQuery(const std::string& sql) {
    //send query packet
    std::shared_ptr<QueryPacket> query_packet(new QueryPacket);
    query_packet->set_query_string(sql);
    BytesHelper::write(socket_, query_packet);
    //get response packet
    std::shared_ptr<PacketHeader> response_header(new PacketHeader);
    BytesHelper::readHeader(socket_, response_header);
    boost::asio::streambuf response_message;
    std::istream response_stream(&response_message);
    boost::asio::read(*socket_, response_message,
            boost::asio::transfer_exactly(response_header->get_packet_len()));
    uint8_t result_num = response_stream.peek();;
    if (result_num == 255) {
        ErrPacket err_packet;
        err_packet.fromStream(response_stream);
        //err_packet.printPacket();
        return std::shared_ptr<MySQLReplicatorResultset>();
    } else if (result_num == 251) {
        //LOCAL INFILE
        return std::shared_ptr<MySQLReplicatorResultset>();
    } else if (result_num == 0) {
        OkPacket ok_packet;
        ok_packet.fromStream(response_stream);
        //ok_packet.printPacket();
        return std::shared_ptr<MySQLReplicatorResultset>();
    } else {
        shared_ptr<MySQLReplicatorResultset> resultset(new MySQLReplicatorResultset);
        //get result column count
        uint64_t column_count = 0;
        BytesHelper::readLenencUint(response_stream, column_count);
        resultset->set_column_count(column_count);

        //get column definition
        for (uint64_t i = 0; i < column_count; ++i) {
            std::shared_ptr<PacketHeader> column_header(new PacketHeader);
            BytesHelper::readHeader(socket_, column_header);
            boost::asio::streambuf column_mesasge;
            std::istream column_stream(&column_mesasge);
            boost::asio::read(*socket_, column_mesasge,
                    boost::asio::transfer_exactly(column_header->get_packet_len()));
            shared_ptr<ColumnDefinitionPacket> column(new ColumnDefinitionPacket);
            column->fromStream(column_stream);
            //column->printPacket();
            resultset->addColumn(column);
        }

        //get eof
        std::shared_ptr<PacketHeader> eof_header(new PacketHeader);
        BytesHelper::readHeader(socket_, eof_header);
        boost::asio::streambuf eof_message;
        std::istream eof_stream(&eof_message);
        boost::asio::read(*socket_, eof_message,
                boost::asio::transfer_exactly(eof_header->get_packet_len()));
        EofPacket eof_packet;
        eof_packet.fromStream(eof_stream);
        //eof_packet.printPacket();

        //get resultset row
        while (true) {
            std::shared_ptr<PacketHeader> row_header(new PacketHeader);
            BytesHelper::readHeader(socket_, row_header);
            boost::asio::streambuf row_message;
            std::istream row_stream(&row_message);
            boost::asio::read(*socket_, row_message,
                    boost::asio::transfer_exactly(row_header->get_packet_len()));
            uint8_t rowdata_eof_code = row_stream.peek();
            if (rowdata_eof_code == 254) {
                break;
            }
            shared_ptr<ResultsetRowPacket> row(new ResultsetRowPacket);
            row->set_column_count(column_count);
            row->fromStream(row_stream);
            resultset->addRow(row);
            //row->printPacket();
        }
        return resultset;

    }
    return std::shared_ptr<MySQLReplicatorResultset>();
}
int MySQLReplicatorStatement::executeUpdate(const std::string& sql) {
}
}
