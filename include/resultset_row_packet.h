#ifndef  __RESULTSET_ROW_PACKET_H_
#define  __RESULTSET_ROW_PACKET_H_

#include <string>
#include <vector>
#include "packet.h"

namespace mysql_replicator {
class ResultsetRowPacket : public Packet {
public:
    void set_column_count(uint64_t column_count) {
        column_count_ = column_count;
    }
    void fromStream(std::istream &is);
    std::string get_data(uint32_t index) {
        return data_vec_[index - 1];
    }
    void printPacket();
private:
    uint64_t column_count_;
    std::vector<std::string> data_vec_;
};
}
#endif  //__RESULTSET_ROW_PACKET_H_
