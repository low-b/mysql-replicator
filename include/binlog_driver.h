#ifndef  __BINLOG_DRIVER_H_
#define  __BINLOG_DRIVER_H_


namespace mysql_replicator {
template<class BinlogStartPoint, template <class> class DriverPolicy>
class BinlogDriver : public DriverPolicy<BinlogStartPoint> {
public: 
};

#endif  //__BINLOG_DRIVER_H_
