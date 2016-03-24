#ifndef  __BINLOG_LISTENER_EVENT_H_
#define  __BINLOG_LISTENER_EVENT_H_

#include <istream>

namespace mysql_replicator {
class BinlogListenerEvent {
public:
    void init(const istream& buff) = 0;
    shared_ptr<BinlogEvent> toEvent();
};
}

#endif  //__BINLOG_LISTENER_EVENT_H_
