/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#ifndef __MAP_FUNC_H__
#define __MAP_FUNC_H__

#include <iostream>
#include <list>
#include <mutex>
#include "local/local.h"


const int       kMapCacheSize        = 20;                  // 最多缓存多少个node
const int       kMapClearDist        = 5000;                // 清掉缓存中多少m以外的node
const int       kMapCacheInterval    = 1000000;             // 多久计算一次本车所在地图,微秒(1e-6秒)
const double    kMapAngleDiff        = 30.0;                // 车与道路最大允许航向角,度数


// map消息缓存和计算车道的类,单例模式
class MapCache{
public:
    ~MapCache();
    static MapCache *           instance();
    void                        add(const LocalMap &map);
    void                        getMapOfCar(const LocalBsm &him, CurrentMap &info);

private:
    MapCache();
    void                        mapThreadFun();
    void                        getCurrentLane();
    double                      scanLanes(const LocalNode &node,const PosWGS84 &pos,float heading,CurrentMap &map_info);

    bool                        m_ready         = false;
    std::mutex                  m_mutex;
    std::list<LocalNode>        m_list;


};


#endif

