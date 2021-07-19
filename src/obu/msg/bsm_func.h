/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#ifndef __BSM_FUNC_H__
#define __BSM_FUNC_H__

#include <iostream>
#include <list>
#include <mutex>
#include "local/local.h"


const int kBsmCacheSize        = 30;          // 最多存多少辆车
const int kBsmCacheTimeout     = 1000;        // 丢弃缓存里多少毫秒以前的数据,毫秒
const int kBsmCacheInterval    = 1000000;     // 多久清理一次缓存,微秒(1e-6秒)


// bsm 消息缓存的类,单例模式
class BsmCache{
public:
    ~BsmCache();
    static BsmCache *       instance();
    void                    add(const LocalBsm &bsm);
    void                    getList(std::list<LocalBsm> &l);

private:
    BsmCache();
    void                    clearOldBsm(int ms);
    void                    bsmThreadFun();

    bool                    m_ready             = false;
    std::mutex              m_mutex;
    std::list<LocalBsm>     m_list;

};


// bsm场景
void bsmFunc(const LocalBsm &bsm);


#endif

