#include "handshake_response_packet.h"
#include "bytes_helper.h"
#include <openssl/sha.h>

namespace mysql_replicator {

const int DIGEST_LENTH = 20;

void HandshakeResponsePacket::toStream(std::ostream &os) {
    BytesHelper::writeFixUint32(os, (uint32_t)(1 | 4 | 512 | 32768));
    BytesHelper::writeFixUint32(os, (uint32_t)0x01000000);
    BytesHelper::writeFixUint8(os, character_set_);
    std::string reserved(23, 0);
    BytesHelper::writeFixString(os, reserved, 23);
    BytesHelper::writeNullString(os, username_);
    if (password_.empty()) {
        BytesHelper::writeFixUint8(os, (uint8_t)0);
    } else {
        std::string passwd_digest  = getPasswordDigest();
        BytesHelper::writeLenencString(os, passwd_digest);
    }
    //BytesHelper::writeNullString(os, "");
}
std::string HandshakeResponsePacket::getPasswordDigest() {
    SHA_CTX c;
    unsigned char md1[DIGEST_LENTH];
    unsigned char md2[DIGEST_LENTH];
    unsigned char md3[DIGEST_LENTH];

    SHA1_Init(&c);
    SHA1_Update(&c, password_.c_str(), password_.size());
    SHA1_Final(md1, &c);

    SHA1_Init(&c);
    SHA1_Update(&c, md1, DIGEST_LENTH);
    SHA1_Final(md2, &c);

    SHA1_Init(&c);
    SHA1_Update(&c, auth_reponse_data_.c_str(), auth_reponse_data_.size());
    SHA1_Update(&c, md2, DIGEST_LENTH);
    SHA1_Final(md3, &c);
    for (int i = 0; i < DIGEST_LENTH; i++) {
        md3[i] = md3[i] ^ md1[i];
    }
    return std::string((char*)md3, DIGEST_LENTH);
}
}
