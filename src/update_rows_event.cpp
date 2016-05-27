#include "update_rows_event.h"
#include "bytes_helper.h"
#include "mysql_replicator_com.h"

namespace mysql_replicator {

void UpdateRowsEvent::fromStream(std::istream &is) {
}

void UpdateRowsEvent::printPacket() {
    std::cout << "--------table map event packet begin---------" << std::endl;
    std::cout << "--------table map event packet end---------" << std::endl;
}
}
