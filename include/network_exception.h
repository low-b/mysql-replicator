#ifndef  _NETWORK_EXCEPTION_H_
#define  _NETWORK_EXCEPTION_H_

#include "myexception.h"

namespace mysql_replicator {
class NetworkException : public MyException {
public:
    NetworkException(const std::string& what_str) 
    : MyException(what_str) {
        set_err_msg("NetworkException:" + get_err_msg());
    }
    ~NetworkException() throw() {}
    virtual const char* what() {
        return get_err_msg().c_str();
    }
};
}

#endif  //_NETWORK_EXCEPTION_H_
