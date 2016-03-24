#ifndef  __HANDSHAKE_RESPONSE_PACKET_H_
#define  __HANDSHAKE_RESPONSE_PACKET_H_

#include <string>
#include "packet.h"

namespace mysql_replicator {
class HandshakeResponsePacket : public Packet {
public:
    void toStream(std::ostream &os);
    std::string getPasswordDigest();
    void set_character_set(uint8_t character_set) { character_set_ = character_set; }
    void set_username(const std::string& username) { username_ = username; }
    void set_password(const std::string& password) { password_ = password; }
    void set_auth_reponse_data(const std::string& auth_reponse_data) {
        auth_reponse_data_ = auth_reponse_data;
    }
private:
    uint8_t character_set_;
    std::string username_;
    std::string password_;
    std::string auth_reponse_data_;
};
}
#endif  //__HANDSHAKE_RESPONSE_PACKET_H_
