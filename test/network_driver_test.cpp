#include "network_driver_test.h"
#include "network_driver.h"
#include "binlog_position.h"

using namespace mysql_replicator;
using namespace std;

namespace mysql_replicationer_test {

TEST(TransformerTest,  test_aaaa)
{
    //NetworkDriver<BinlogPosition> driver("182.92.128.43", 33061, "qh", "123456");
    NetworkDriver<BinlogPosition> driver("10.94.45.24", 33061, "root", "123456");
    driver.init();
}
}
