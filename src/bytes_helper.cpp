#include "bytes_helper.h"
#include "packet_header.h"

namespace mysql_replicator {
void BytesHelper::readValueByLength(std::istream &is, char* value, size_t length) {
    char ch;
    for (size_t ct = 0; ct < length; ++ct) {
        is.get(ch);
        value[ct] = ch;
    }
}
void BytesHelper::writeValueByLength(std::ostream &os, const char* value, size_t length) {
    for (size_t ct = 0; ct < length; ct++) {
        os.write(value + ct, 1);
    }
}
void BytesHelper::readFixUint8(std::istream &is, uint8_t& value) {
    value = 0;
    readValueByLength(is, (char*)&value, sizeof(value));
}
void BytesHelper::readFixUint16(std::istream &is, uint16_t& value) {
    value = 0;
    readValueByLength(is, (char*)&value, sizeof(value));
}
void BytesHelper::readFixUint24(std::istream &is, uint32_t& value) {
    value = 0;
    readValueByLength(is, (char*)&value, 3);
}
void BytesHelper::readFixUint32(std::istream &is, uint32_t& value) {
    value = 0;
    readValueByLength(is, (char*)&value, sizeof(value));
}
void BytesHelper::readFixUint48(std::istream &is, uint64_t& value) {
    value = 0;
    readValueByLength(is, (char*)&value, 6);
}
void BytesHelper::readFixUint64(std::istream &is, uint64_t& value) {
    value = 0;
    readValueByLength(is, (char*)&value, sizeof(value));
}
int BytesHelper::readLenencUint(std::istream &is, uint64_t& value) {
    value = 0;
    char ch;
    is.get(ch);
    uint8_t byte = (uint8_t)ch;
    if (byte < 251) {
        value = byte;
    }else if (byte == 252) {
        readValueByLength(is, (char*)&value, 2);
    } else if (byte == 253) {
        readValueByLength(is, (char*)&value, 3);
    } else if (byte == 254) {
        readValueByLength(is, (char*)&value, 8);
    } else if (byte == 251) {
        return 251;
    } else if (byte == 255) {
        return 255;
    }
    return 0;
}
void BytesHelper::readNullString(std::istream &is, std::string& value) {
    char ch;
    std::ostringstream out;
    is.get(ch);
    while(is.good() && ch != '\0'){
        out.put(ch);
        is.get(ch);
    }
    value = out.str();
}
void BytesHelper::readFixString(std::istream &is, std::string& value, size_t length) {
    char ch;
    size_t ct = 0;
    std::ostringstream out;
    while (ct < length) {
        is.get(ch);
        out.put(ch);
        ++ct;
    }
    value = out.str();
}
//void BytesHelper::readLenencString(std::istream &is, std::string& value) {
//}
void BytesHelper::writeFixUint8(std::ostream &os, const uint8_t& value) {
    const char *ptr= (const char*)(&value);
    writeValueByLength(os, ptr, sizeof(value));
}
void BytesHelper::writeFixUint16(std::ostream &os, const uint16_t& value) {
    const char *ptr= (const char*)(&value);
    writeValueByLength(os, ptr, sizeof(value));
}
void BytesHelper::writeFixUint24(std::ostream &os, const uint32_t& value) {
    const char *ptr= (const char*)(&value);
    writeValueByLength(os, ptr, 3);
}
void BytesHelper::writeFixUint32(std::ostream &os, const uint32_t& value) {
    const char *ptr= (const char*)(&value);
    writeValueByLength(os, ptr, sizeof(value));
}
void BytesHelper::writeFixUint48(std::ostream &os, const uint64_t& value) {
    const char *ptr= (const char*)(&value);
    writeValueByLength(os, ptr, 6);
}
void BytesHelper::writeFixUint64(std::ostream &os, const uint64_t& value) {
    const char *ptr= (const char*)(&value);
    writeValueByLength(os, ptr, sizeof(value));
}
void BytesHelper::writeNullString(std::ostream &os, const std::string& value) {
    os.write(value.data(), value.size());
    os << '\0';
}
void BytesHelper::writeFixString(std::ostream &os, const std::string& value, size_t length) {
    os.write(value.data(), length);
}
void BytesHelper::writeLenencString(std::ostream &os, const std::string& value) {
    size_t size = value.size();
    if (size < 252) {
        writeFixUint8(os, (uint8_t)size);
    } else if (size < (1 << 16L)) {
        writeFixUint8(os, (uint8_t)252);
        writeFixUint16(os, (uint16_t)size);
    } else if (size < (1 << 24L)) {
        writeFixUint8(os, (uint8_t)253);
        writeFixUint24(os, (uint32_t)size);
    } else {
        writeFixUint8(os, (uint8_t)254);
        writeFixUint32(os, (uint32_t)size);
    }
    os.write(value.data(), value.size());
}
void BytesHelper::write(
        std::shared_ptr<boost::asio::ip::tcp::socket> socket,
        std::shared_ptr<Packet> packet) {
    write(socket, packet, 0);
}
void BytesHelper::write(
        std::shared_ptr<boost::asio::ip::tcp::socket> socket,
        std::shared_ptr<Packet> packet, uint8_t seq) {
    boost::asio::streambuf packet_message;
    std::ostream packet_stream(&packet_message);
    packet->toStream(packet_stream);
    PacketHeader header;
    size_t packet_size = packet_message.size();
    header.set_packet_len(packet_size);
    header.set_seq_num(seq);
    boost::asio::streambuf header_message;
    std::ostream header_stream(&header_message);
    header.toStream(header_stream);
    std::vector<boost::asio::streambuf::const_buffers_type> packet_with_header;
    packet_with_header.push_back(header_message.data());
    packet_with_header.push_back(packet_message.data());
    boost::asio::write(*socket, packet_with_header,
            boost::asio::transfer_at_least(4 + packet_size));
}
}