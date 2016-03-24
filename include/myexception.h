#ifndef  _MYEXCEPTION_H_
#define  _MYEXCEPTION_H_

#include <exception>
#include <string>

namespace mysql_replicator {

class MyException : public std::exception {
public:
    MyException() {}
    MyException(const std::string& what_str)
    : _err_msg(what_str) {}
    virtual ~MyException() throw() {}
    virtual const char* what() {
        return _err_msg.c_str();
    }
protected:
    void set_err_msg(const std::string& err_msg) {
        _err_msg = err_msg;
    }
    std::string get_err_msg() {
        return _err_msg;
    }
private:
    std::string _err_msg;

};
}

#endif  //_MYEXCEPTION_H_
