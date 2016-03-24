#ifndef  __BINLOG_GTID_H_
#define  __BINLOG_GTID_H_

#include <string>
#include <vector>
#include <utility>

class Sids {
private:
    std::string _sid;
    uint64_t _n_intervals;
    std::vector<std::pair<uint64_t, uint64_t> > _inverval_pairs;
};

class SidBlock {
private:
    uint64_t _n_sids;
    std::vector<Sids> _sid_vec;
};

class BinlogGtid {
public:
    void sendBinlogDump();
    const std::string toBuffer();
private:
    uint8_t _type;
    uint32_t _pos;
    uint16_t _flags;
    uint32_t _server_id;
    uint32_t _filename_len;
    std::string _filename;
    uint64_t _binlog_pos;
    uint32_t _date_size;
    uint64_t _n_size;
    SidBlock _sid_block;
};

#endif  //__BINLOG_GTID_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
