#ifndef  __BINLOG_GID_H_
#define  __BINLOG_GID_H_
namespace mysql_replicator {
class BinlogGID : public BinlogStartPoint {
    void sendBinlogDump();
};
}
#endif  //__BINLOG_GID_H_
