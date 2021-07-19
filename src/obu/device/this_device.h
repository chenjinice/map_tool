/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#ifndef __THIS_DEVICE_H__
#define __THIS_DEVICE_H__


#include <iostream>
#include <mutex>
#include <vector>
#include "my/my_common.h"
#include "local/local.h"




// 场景开关
class SceneConfig
{
public:
    bool fcw             = false;       // 前向碰撞预警
    bool icw             = false;       // 交叉路口碰撞预警
    bool lta             = false;       // 左转辅助
    bool bsw             = false;       // 盲区预警
    bool dnpw            = false;       // 逆向超车预警
    bool ebw             = false;       // 紧急制动预警
    bool avw             = false;       // 异常车辆提醒
    bool clw             = false;       // 车辆失控预警
    bool slw             = false;       // 限速预警
    bool rlvw            = false;       // 闯红灯预警
    bool vrucw           = false;       // 行人碰撞预警(弱势交通参与者)
    bool glosa           = false;       // 滤波车速引导
    bool tjw             = false;       // 前方拥堵
    bool evw             = false;       // 紧急车辆提醒
    bool rts             = false;       // rsi的标牌预警
    bool rte             = false;       // rsi的事件预警
};



// 本设备，单例模式
class ThisDevice : public LocalBsm
{
public:
    ~ThisDevice();
    static ThisDevice *         instance();

    SceneConfig                 scenes;
    CurrentMap                  map_info;
    CurrentLights               lights;

private:
    ThisDevice();
};


#endif

