#ifndef  __BYTES_HELPER_H_
#define  __BYTES_HELPER_H_

#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <boost/asio.hpp>
#include "packet.h"
#include "packet_header.h"
#include "binlog_event_header.h"
#include <boost/dynamic_bitset.hpp>

namespace mysql_replicator {
class BytesHelper {
public:
    static void readFixUint8(std::istream &is, uint8_t& value);
    static void readFixUint16(std::istream &is, uint16_t& value);
    static void readFixUint24(std::istream &is, uint32_t& value);
    static void readFixUint32(std::istream &is, uint32_t& value);
    static void readFixUint48(std::istream &is, uint64_t& value);
    static void readFixUint64(std::istream &is, uint64_t& value);
    static int readLenencUint(std::istream &is, uint64_t& value);
    static void readNullString(std::istream &is, std::string& value);
    static void readEofString(std::istream &is, std::string& value);
    static void readFixString(std::istream &is, std::string& value, size_t length);
    static void readLenencString(std::istream &is, std::string& value);
    static void writeFixUint8(std::ostream &os, const uint8_t& value);
    static void writeFixUint16(std::ostream &os, const uint16_t& value);
    static void writeFixUint24(std::ostream &os, const uint32_t& value);
    static void writeFixUint32(std::ostream &os, const uint32_t& value);
    static void writeFixUint48(std::ostream &os, const uint64_t& value);
    static void writeFixUint64(std::ostream &os, const uint64_t& value);
    static void writeNullString(std::ostream &os, const std::string& value);
    static void writeFixString(std::ostream &os, const std::string& value, size_t length);
    static void writeLenencString(std::ostream &os, const std::string& value);

    static void write(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
            std::shared_ptr<Packet> packet);
    static void write(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
            std::shared_ptr<Packet> packet,
            uint8_t seq);
    static void read(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
            boost::asio::streambuf& buff,
            uint8_t& seq);
    static void read(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
            boost::asio::streambuf& buff);
    static void readHeader(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
            std::shared_ptr<PacketHeader> packet_header);
    static void readEventHeader(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
            std::shared_ptr<BinlogEventHeader> event_header);
//    static void read(std::ostream &os, const std::string& value);
    static void fillBitmap(const std::string& byte_buffer, boost::dynamic_bitset<>& bitmap);
private:
    static void readValueByLength(std::istream &is, char* value, size_t length);
    static void writeValueByLength(std::ostream &os, const char* value, size_t length);
};

}

#endif  //__BYTES_HELPER_H_
