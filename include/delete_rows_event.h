#ifndef __DELETE_ROWS_EVENT_H__
#define __DELETE_ROWS_EVENT_H__

#include <vector>
#include "rows_event.h"

namespace mysql_replicator {
class DeleteRowsEvent : public RowsEvent {
public:
    DeleteRowsEvent(std::shared_ptr<BinlogEventHeader> header,
            uint8_t post_header_len) :
        RowsEvent(header, post_header_len) {}
};
}
#endif //__DELETE_ROWS_EVENT_H__
