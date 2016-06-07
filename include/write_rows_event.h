#ifndef __WRITE_ROWS_EVENT_H__
#define __WRITE_ROWS_EVENT_H__

#include <vector>
#include "rows_event.h"

namespace mysql_replicator {
class WriteRowsEvent : public RowsEvent {
public:
    WriteRowsEvent(std::shared_ptr<BinlogEventHeader> header,
            uint8_t post_header_len) :
        RowsEvent(header, post_header_len) {}
};
}
#endif //__WRITE_ROWS_EVENT_H__
