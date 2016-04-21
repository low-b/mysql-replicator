#include "mysql_replicator_connector.h"
#include "packet_header.h"
#include "handshake_init_packet.h"
#include "handshake_response_packet.h"
#include "ok_packet.h"
#include "err_packet.h"
#include "eof_packet.h"
#include "network_exception.h"
#include "bytes_helper.h"
#include "binlog_dump_exception.h"

namespace mysql_replicator {
void MySQLReplicatorConnector::connect() {
    using boost::asio::ip::tcp;
    //get ip and connect
    tcp::resolver resolver(io_service_);
    tcp::resolver::query query(host_.c_str(), "0");

    boost::system::error_code error = boost::asio::error::host_not_found;

    socket_.reset(new tcp::socket(io_service_));
    try {
        tcp::resolver::iterator endpoint_iterator=resolver.resolve(query);
        tcp::resolver::iterator end;

        while (error && endpoint_iterator != end) {
            tcp::endpoint endpoint=endpoint_iterator->endpoint();
            endpoint.port(port_);

            socket_->close();
            socket_->connect(endpoint, error);
            endpoint_iterator++;
        }
    } catch(...) {
        throw NetworkException("unexpexcted error occured when reslove host");
    }

    if (error) {
        throw NetworkException(std::string("cannot find usable host,error:") + 
                boost::system::system_error(error).what());
    }


    //authenticate

    //get init pacakge
    std::shared_ptr<PacketHeader> handshake_init_header(new PacketHeader);
    BytesHelper::readHeader(socket_, handshake_init_header);
    boost::asio::streambuf init_mesasge;
    std::istream init_stream(&init_mesasge);
    boost::asio::read(*socket_, init_mesasge,
            boost::asio::transfer_exactly(handshake_init_header->get_packet_len()));
    HandshakeInitPacket handshake_init_packet;
    handshake_init_packet.fromStream(init_stream);
    //handshake_init_packet.printPacket();
    
    //send response packet
    std::shared_ptr<HandshakeResponsePacket> handshake_response_packet(new HandshakeResponsePacket);
    handshake_response_packet->set_character_set(handshake_init_packet.get_character_set());
    handshake_response_packet->set_username(username_);
    handshake_response_packet->set_password(password_);
    handshake_response_packet->set_auth_reponse_data(handshake_init_packet.get_auth_data());
    BytesHelper::write(socket_, handshake_response_packet, handshake_init_header->get_seq_num() + 1);

    //get auth result packet
    std::shared_ptr<PacketHeader> handshake_result_header(new PacketHeader);
    BytesHelper::readHeader(socket_, handshake_result_header);
    boost::asio::streambuf result_message;
    std::istream result_stream(&result_message);
    boost::asio::read(*socket_, result_message,
            boost::asio::transfer_exactly(handshake_result_header->get_packet_len()));
    uint8_t auth_error_code = result_stream.peek();
    if (auth_error_code == 0) {
        OkPacket ok_packet;
        ok_packet.fromStream(result_stream);
        //ok_packet.printPacket();
    } else if (auth_error_code == 254) {
        EofPacket eof_packet;
        eof_packet.fromStream(result_stream);
        //eof_packet.printPacket();
    } else if (auth_error_code == 255) {
        ErrPacket err_packet;
        err_packet.fromStream(result_stream);
        //err_packet.printPacket();
    } else {
    }
}
}
