#ifndef  __BINLOG_POSITION_H_
#define  __BINLOG_POSITION_H_
namespace mysql_replicator {
class BinlogPosition {
public:
    BinlogPosition() = default;
    BinlogPosition(const std::string& filename, uint32_t pos)
        : _pos(pos), _filename(filename) { }
    void sendBinlogDump() {}
private:
    uint32_t _pos;
    std::string _filename;
};
}
#endif  //__BINLOG_POSITION_H_
