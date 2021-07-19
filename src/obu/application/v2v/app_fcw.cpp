/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#include "application/application.h"


/*---- 前向碰撞预警 ----*/


static const double kFcwLeftMax     = 3.5;           // 左边最大距离,m
static const double KFcwRightMax    = 3.5;           // 右边最大距离,m
static const double kFcwVMax        = 60.0;          // 纵向最大距离,m
static const double kFcwVMin        = 0.0;           // 纵向最小距离,m
static const double kFcwSpeedMin    = 1.0;           // 本车速度最小值, m/s


// me  : 本车
// him : 从车
// p   : 从车在本车的相对位置
void appFcw(const LocalBsm &me, const LocalBsm &him, const RelativePos &p)
{
    // 从车在本车的规定范围以外，不触发
    if( p.h >  KFcwRightMax )return;
    if( p.h < -kFcwLeftMax  )return;
    if( p.v >  kFcwVMax     )return;
    if( p.v <  kFcwVMin     )return;

    if( me.speed < kFcwSpeedMin )return;


    double angle_diff     = getAngleDiff(me.heading,him.heading);
    // 计算两车纵向相对速度
//    double relative_speed = me.speed - cos(degreeToRadian(angle_diff))*him.speed;
//    if(relative_speed <= 0)return;

    // 计算速度和航向角差值
    double speed_diff     = me.speed - him.speed;
    if(fabs(angle_diff) > 30 )      return;
    if(speed_diff <= 0)             return;

    sendEventToUi(UI_E_FCW,0,p.dist);
}
