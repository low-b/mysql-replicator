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
    boost::asio::streambuf response_message;
    std::istream response_stream(&response_message);
    BytesHelper::read(socket_, response_message);
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
            boost::asio::streambuf column_message;
            std::istream column_stream(&column_message);
            BytesHelper::read(socket_, column_message);
            shared_ptr<ColumnDefinitionPacket> column(new ColumnDefinitionPacket);
            column->fromStream(column_stream);
            //column->printPacket();
            resultset->addColumn(column);
        }

        //get eof
        boost::asio::streambuf eof_message;
        std::istream eof_stream(&eof_message);
        BytesHelper::read(socket_, eof_message);
        EofPacket eof_packet;
        eof_packet.fromStream(eof_stream);
        //eof_packet.printPacket();

        //get resultset row
        while (true) {
            boost::asio::streambuf row_message;
            std::istream row_stream(&row_message);
            BytesHelper::read(socket_, row_message);
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
