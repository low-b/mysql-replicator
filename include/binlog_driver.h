/***************************************************************************
 * 
 * Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file binlog_driver.h
 * @author qianhan(com@baidu.com)
 * @date 2015/11/27 17:59:24
 * @brief 
 *  
 **/

#ifndef  __BINLOG_DRIVER_H_
#define  __BINLOG_DRIVER_H_


namespace mysql_replicator {
template<class BinlogStartPoint, template <class> class DriverPolicy>
class BinlogDriver : public DriverPolicy<BinlogStartPoint> {
public: 
};

#endif  //__BINLOG_DRIVER_H_
