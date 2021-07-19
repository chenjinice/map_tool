/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#include "map_func.h"

#include <unistd.h>

#include <thread>

#include "my/my_common.h"
#include "device/this_device.h"


MapCache *MapCache::instance()
{
    static MapCache ins;
    return &ins;
}

MapCache::~MapCache()
{
    m_ready = false;
}

MapCache::MapCache()
{
    myInfo("MapCache init\n");
    thread tmp_thread(&MapCache::mapThreadFun,this);
    tmp_thread.detach();
}

void MapCache::mapThreadFun()
{
    m_ready = true;
    while (m_ready) {
        // 循环计算本车所在车道的信息
        this->getCurrentLane();
        usleep(kMapCacheInterval);
    }
}

void MapCache::add(const LocalMap &map)
{
//    map.show();
    int count = map.nodes.size();

    if(count > 0) {
        myDebug("map received ,id=%d,region=%d,nodes=%d\n",map.nodes[0].id.id,map.nodes[0].id.region,count);
    }

    list<LocalNode>::iterator m;
    m_mutex.lock();       // 有遍历，加锁
    for(int i=0;i<count;i++){
        const LocalNode &node = map.nodes[i];
        bool flag = false;
        // 超过设定的长度的话，清掉第一个
        if(m_list.size() >= kMapCacheSize){
            m_list.pop_front();
        }
        //查找node是否已经存在，
        for(m=m_list.begin();m!=m_list.end();m++){
            if(node.id.id != m->id.id)continue;
            if(node.id.region != m->id.region)continue;
            flag = true;
            break;
        }
        // 不存在，则添加到cache
        if(!flag)m_list.push_back(node);
    }
    m_mutex.unlock();
}

// 根据bsm包的gps数据，计算所在车道信息
void MapCache::getMapOfCar(const LocalBsm &him, CurrentMap &info)
{
    list<LocalNode>::iterator node;
    // 有遍历，加锁
    m_mutex.lock();
    for(node=m_list.begin();node!=m_list.end();node++){
        this->scanLanes(*node,him.pos,him.heading,info);
    }
    m_mutex.unlock();

//    myDebug("him,node_id=%d,region=%d,link_id=%d,lane_id=%d,dist_to_line=%.02lf,speed=(%.02lf~%.02lf)\n",
//            info.node_id.id,info.node_id.region,info.link_id,info.lane_id,info.dist_to_line, info.speed.min, info.speed.max);
}


// 计算本车所在车道
void MapCache::getCurrentLane()
{
    ThisDevice * me = ThisDevice::instance();
    CurrentMap info;
    list<LocalNode>::iterator node;
    // 有遍历，加锁
    m_mutex.lock();
    for(node=m_list.begin();node!=m_list.end();){
        double min = this->scanLanes(*node,me->pos,me->heading,info);
        if(min > kMapClearDist){
            node = m_list.erase(node);  // 清除距离本车太远的node
        }else{
            node++;
        }
    }
    m_mutex.unlock();
    me->map_info = info;

//    myDebug("node_id=%d,region=%d,link_id=%d,lane_id=%d,dist_to_line=%.02lf,speed=(%.02lf~%.02lf)\n",
//            info.node_id.id,info.node_id.region,info.link_id,info.lane_id,info.dist_to_line, info.speed.min, info.speed.max);
}

// 遍历node的lanes，返回本车与node的所有点的最短距离
double MapCache::scanLanes(const LocalNode &node,const PosWGS84 &pos,float heading,CurrentMap &map_info)
{
    double min                  = kDistMax;
    for(uint i = 0 ;i < node.lanes.size(); i++){
        const LocalLane & lane = node.lanes[i];
        int count               = lane.points.size();
        if(count <= 0) continue;

        const PosWGS84 &end     = lane.points[count-1];
        double dist_to_line     = getDistance(end,pos);
        double vertical_min     = kDistMax;
        for(int i=0;i<count-1;i++){
            int flag;
            double dist = minDistance(pos,lane.points[i],lane.points[i+1],&flag);
            if(dist < min)min = dist;
            if( (flag == 0) && (dist < vertical_min) ){
                // 计算车的航向角跟地图车道的航向角的差值，看是不是在范围内
                double road_angle     = getRoadAngle(lane.points[i],lane.points[i+1]);
                double abs_angle      = fabs(getAngleDiff(heading,road_angle));
                if(abs_angle <= kMapAngleDiff)  vertical_min = dist;
            }
        }
        // 最小垂直距离小于车道宽度，则本车在此车道内
        if(vertical_min <= lane.lane_width){
            if(map_info.valid && (map_info.vertical_dist < vertical_min) )continue;
            map_info.node_id               = node.id;
            map_info.link_id               = lane.link_id;
            map_info.lane_id               = lane.lane_id;
            map_info.dir                   = lane.dir;
            map_info.lane_width            = lane.lane_width;
            map_info.vertical_dist         = vertical_min;
            map_info.dist_to_line          = dist_to_line;
            map_info.phases                = lane.phases;
            map_info.speed                 = lane.speed;
            map_info.valid                 = true;
        }
    }
    return min;
}

