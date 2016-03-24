#ifndef  __HANDSHAKE_INIT_PACKET_H_
#define  __HANDSHAKE_INIT_PACKET_H_

#include <string>
#include "packet.h"

namespace mysql_replicator {
class HandshakeInitPacket : public Packet {
public:
    void fromStream(std::istream &is);
    uint32_t get_capability_flags() {
        return (uint32_t)capability_flags_lower_ + ((uint32_t)capability_flags_upper_ << 16);
    }
    std::string get_auth_data() {
        return auth_plugin_data_1_ + auth_plugin_data_2_.substr(0,12);
    }
    void printPacket();
    uint8_t get_character_set() { return character_set_; }
private:
    uint8_t protocol_version_;
    std::string server_version_;
    uint32_t connection_id_;
    std::string auth_plugin_data_1_;
    uint8_t filler_;
    uint16_t capability_flags_lower_;
    uint8_t character_set_;
    uint16_t status_flags_;
    uint16_t capability_flags_upper_;
    uint8_t auth_plugin_data_len_;
    std::string reserved_;
    std::string auth_plugin_data_2_;
    std::string auth_plugin_name_;

};
}
#endif  //__HANDSHAKE_INIT_PACKET_H_
