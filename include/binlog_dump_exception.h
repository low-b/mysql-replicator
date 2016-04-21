#ifndef  _BINLOG_DUMP_EXCEPTION_H_
#define  _BINLOG_DUMP_EXCEPTION_H_

#include "myexception.h"

namespace mysql_replicator {
class BinlogDumpException : public MyException {
public:
    BinlogDumpException(const std::string& what_str) 
    : MyException(what_str) {
        set_err_msg("BinlogDumpException:" + get_err_msg());
    }
    ~BinlogDumpException() throw() {}
    virtual const char* what() {
        return get_err_msg().c_str();
    }
};
}

#endif  //_BINLOG_DUMP_EXCEPTION_H_
