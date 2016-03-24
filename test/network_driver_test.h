#ifndef  _NETWORK_DRIVER_TEST_H_
#define  _NETWORK_DRIVER_TEST_H_

#include "gtest/gtest.h"
#undef main

namespace mysql_replicationer_test{

class NetworkDriverTest : public testing::Test {
public:
    NetworkDriverTest(){}
    virtual ~NetworkDriverTest(){}
};
}

#endif  //_NETWORK_DRIVER_TEST_H_
