#ifndef  _BINLOG_EVENT_EXCEPTION_H_
#define  _BINLOG_EVENT_EXCEPTION_H_

#include "myexception.h"

namespace mysql_replicator {
class BinlogEventException : public MyException {
public:
    BinlogEventException(const std::string& what_str) 
    : MyException(what_str) {
        set_err_msg("BinlogEventException:" + get_err_msg());
    }
    ~BinlogEventException() throw() {}
    virtual const char* what() {
        return get_err_msg().c_str();
    }
};
}

#endif  //_BINLOG_EVENT_EXCEPTION_H_
