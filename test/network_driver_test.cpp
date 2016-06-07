#include "network_driver_test.h"
#include "network_driver.h"
#include "binlog_position.h"

using namespace mysql_replicator;
using namespace std;

namespace mysql_replicationer_test {

TEST(TransformerTest,  test_aaaa)
{
    NetworkDriver driver("10.99.68.64", 6969, "root", "root123");
    //NetworkDriver driver("10.94.45.24", 33061, "root", "123456");
    //NetworkDriver<> driver("10.195.157.39", 8306, "work", "213456");
    try {
    driver.init();
    std::shared_ptr<BinlogPosition> bin_pos = std::make_shared<BinlogPosition>();
    bin_pos->set_binlog_filename("mysql-bin.000018");
    bin_pos->set_binlog_pos(7931985);
    bin_pos->set_slave_id(12345);
    driver.sendBinlogDump(bin_pos);
    driver.takeBinlogEvent();
    } catch (MyException &e) {
        printf("error:%s", e.what());
    }
}
}
