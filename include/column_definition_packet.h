#ifndef  __COLUMN_DEFINITION_PACKET_H_
#define  __COLUMN_DEFINITION_PACKET_H_

#include <string>
#include "packet.h"

namespace mysql_replicator {
class ColumnDefinitionPacket : public Packet {
public:
    void fromStream(std::istream &is);
    void printPacket();
private:
    std::string catalog_;
    std::string schema_;
    std::string table_;
    std::string org_table_;
    std::string name_;
    std::string org_name_;
    uint8_t next_length_;
    uint16_t charset_;
    uint32_t column_length_;
    uint8_t type_;
    uint16_t flags_;
    uint8_t decimals_;
    uint16_t filler_;
};
}
#endif  //__COLUMN_DEFINITION_PACKET_H_
