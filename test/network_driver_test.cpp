#include "network_driver_test.h"
#include "network_driver.h"
#include "binlog_position.h"

using namespace mysql_replicator;
using namespace std;

namespace mysql_replicationer_test {

TEST(TransformerTest,  test_aaaa)
{
    //NetworkDriver<BinlogPosition> driver("182.92.128.43", 33061, "qh", "123456");
    NetworkDriver driver("10.94.45.24", 33061, "root", "123456");
    //NetworkDriver<> driver("10.195.157.39", 8306, "work", "213456");
    try {
    driver.init();
    std::shared_ptr<BinlogPosition> bin_pos = std::make_shared<BinlogPosition>();
    bin_pos->set_binlog_filename("mysql-bin.000012");
    bin_pos->set_binlog_pos(84453);
    bin_pos->set_slave_id(12345);
    driver.sendBinlogDump(bin_pos);
    driver.takeBinlogEvent();
    } catch (MyException &e) {
        printf("error:%s", e.what());
    }
}
}
