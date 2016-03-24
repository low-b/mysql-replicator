#include "handshake_init_packet.h"
#include "bytes_helper.h"

namespace mysql_replicator {

void HandshakeInitPacket::fromStream(std::istream &is) {
    BytesHelper::readFixUint8(is, protocol_version_);
    BytesHelper::readNullString(is, server_version_);
    BytesHelper::readFixUint32(is, connection_id_);
    BytesHelper::readFixString(is, auth_plugin_data_1_, 8);
    BytesHelper::readFixUint8(is, filler_);
    BytesHelper::readFixUint16(is, capability_flags_lower_);
    BytesHelper::readFixUint8(is, character_set_);
    BytesHelper::readFixUint16(is, status_flags_);
    BytesHelper::readFixUint16(is, capability_flags_upper_);
    BytesHelper::readFixUint8(is, auth_plugin_data_len_);
    if (!(capability_flags_upper_ & 0x0008)) {
        auth_plugin_data_len_ = 0;
    }
    BytesHelper::readFixString(is, reserved_, 10);
    if (capability_flags_lower_ & 0x8000) {
        BytesHelper::readFixString(is, auth_plugin_data_2_, 13);
    }
    if (capability_flags_upper_ & 0x0008) {
        BytesHelper::readNullString(is, auth_plugin_name_);
    }
}

void HandshakeInitPacket::printPacket() {
    std::cout << "--------handshake init packet begin---------" << std::endl;
    std::cout << "protocol_version_:" << protocol_version_ << std::endl;
    std::cout << "server_version_:" << server_version_ << std::endl;
    std::cout << "connection_id_:" << connection_id_ << std::endl;
    std::cout << "auth_plugin_data_1_:" << auth_plugin_data_1_ << std::endl;
    std::cout << "filler_:" << filler_ << std::endl;
    std::cout << "capability_flags_lower_:" << capability_flags_lower_ << std::endl;
    std::cout << "character_set_:" << character_set_ << std::endl;
    std::cout << "status_flags_:" << status_flags_ << std::endl;
    std::cout << "capability_flags_upper_:" << capability_flags_upper_ << std::endl;
    std::cout << "auth_plugin_data_len_:" << (uint32_t)auth_plugin_data_len_ << std::endl;
    std::cout << "reserved_:" << reserved_ << std::endl;
    std::cout << "auth_plugin_data_2_:" << auth_plugin_data_2_ << std::endl;
    std::cout << "auth_plugin_name_:" << auth_plugin_name_ << std::endl;
    std::cout << "--------handshake init packet end---------" << std::endl;
}
}
