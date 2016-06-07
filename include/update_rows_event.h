#ifndef __UPDATE_ROWS_EVENT_H__
#define __UPDATE_ROWS_EVENT_H__

#include <vector>
#include "rows_event.h"

namespace mysql_replicator {
class UpdateRowsEvent : public RowsEvent {
public:
    UpdateRowsEvent(std::shared_ptr<BinlogEventHeader> header,
            uint8_t post_header_len) :
        RowsEvent(header, post_header_len) {}
};
}
#endif //__UPDATE_ROWS_EVENT_H__
