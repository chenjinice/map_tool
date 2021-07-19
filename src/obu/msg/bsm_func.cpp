/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */



#include "bsm_func.h"

#include <unistd.h>

#include <thread>

#include "map_func.h"
#include "device/this_device.h"
#include "application/application.h"
#include "my/my_common.h"



// ------------------------- bsm 缓存的类 --------------------------


BsmCache *BsmCache::instance()
{
    static BsmCache ins;
    return &ins;
}

BsmCache::~BsmCache()
{
    m_ready     = false;
}

BsmCache::BsmCache()
{
    myInfo("BsmCache init\n");
    thread tmp_thread(&BsmCache::bsmThreadFun,this);
    tmp_thread.detach();
}

void BsmCache::add(const LocalBsm &bsm)
{
    bool flag = false;
    list<LocalBsm>::iterator it;
    //存在遍历，要加锁
    m_mutex.lock();
    // 超过设定的长度的话，清掉第一个
    if(m_list.size() >= kBsmCacheSize){
        m_list.pop_front();
    }
    // 先查找列表是否已经存在相同的车id的数据
    for(it = m_list.begin();it != m_list.end();it++){
        if(it->id == bsm.id){
            flag = true;
            (*it) = bsm;
            break;
        }
    }
    if(!flag){
        m_list.push_back(bsm); // 不存在则push进去
    }
    m_mutex.unlock();
}

//  拷贝所有缓存一份
void BsmCache::getList(std::list<LocalBsm> &l)
{
    m_mutex.lock();
    l =  m_list;
    m_mutex.unlock();
}

// 清除 ms 毫秒之前的bsm数据
void BsmCache::clearOldBsm(int ms)
{
//    printf("bsm === %d\n",(int)m_list.size());
    if(m_list.size() == 0)return;
    timeval tv;
    list<LocalBsm>::iterator it;
    //存在遍历，要加锁
    m_mutex.lock();
    gettimeofday(&tv,nullptr);
    // 遍历，删除过时的数据
    for(it = m_list.begin();it != m_list.end();){
        int ret_ms = getInterval(tv,it->tv);
        if(abs(ret_ms) > ms){
            it = m_list.erase(it);
        }else{
            it++;
        }
    }
    m_mutex.unlock();
}

// 线程里定时清过时数据
void BsmCache::bsmThreadFun()
{
    m_ready = true;
    while (m_ready) {
        this->clearOldBsm(kBsmCacheTimeout);
        usleep(kBsmCacheInterval);
    }
}

// -----------------------------------------------------------







// -------------------------- bsm场景 -------------------------
void bsmFunc(const LocalBsm &bsm)
{
    //    bsm.show();

    ThisDevice * me          = ThisDevice::instance();
    SceneConfig &cfg         = me->scenes;
    BsmCache::instance()->add(bsm);

    CurrentMap him_map;
    RelativePos p,ant_p;
    getRelativePos(me->pos,bsm.pos,me->heading,p);
    getAntBasePos(me->antenna,p,ant_p);
    myDebug("bsm:%s,%.07lf,%.07lf,%.02f,%.02fkm/h,eflag=%d,h=%.02f(%.02f),v=%.02f(%.02f),dist=%.02f\n",
            bsm.id.c_str(),bsm.pos.lng,bsm.pos.lat,bsm.heading,bsm.speed*3.6,bsm.event_flag,p.h,ant_p.h,p.v,ant_p.v,p.dist);

    if(cfg.fcw){
        appFcw(*me,bsm,ant_p);
    }
    if(cfg.icw){
//        sceneIcw(*me,bsm,ant_p);
        MapCache::instance()->getMapOfCar(bsm,him_map);
        appIcw(*me,bsm,me->map_info,him_map); // 基于地图
    }
    if(cfg.avw && (bsm.event_flag > 0)){
        appAvw(*me,bsm,ant_p);
    }
}
