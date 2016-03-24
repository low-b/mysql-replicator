#ifndef  __PACKET_HEADER_H_
#define  __PACKET_HEADER_H_

#include <iostream>
#include "packet.h"

namespace mysql_replicator {
struct PacketHeader : public Packet {
public:
    uint32_t get_packet_len() { return packet_len_; }
    uint8_t get_seq_num() { return seq_num_; }
    void set_packet_len(uint32_t len) { packet_len_ = len; }
    void set_seq_num(uint8_t seq_num) { seq_num_ = seq_num; }
    int size() {
        return 4;
    }
    void fromStream(std::istream &is);
    void toStream(std::ostream &os);
    void printPacket();
private:
    uint32_t packet_len_;
    uint8_t seq_num_;
};
}

#endif  //__PACKET_HEADER_H_
