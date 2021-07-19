/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#include "application/application.h"


/*---- 异常车辆预警 ----*/


static const double kAvwLeftMax     = 3.5;           // 左边最大距离,m
static const double KAvwRightMax    = 10.5;          // 右边最大距离,m
static const double kAvwVMax        = 150.0;         // 纵向最大距离,m
static const double kAvwVMin        = 0.0;           // 纵向最小距离,m
static const double kAvwSpeedMin    = 1.0;           // 本车速度最小值, m/s


// me  : 本车
// him : 从车
// p   : 从车在本车的相对位置
void appAvw(const LocalBsm &me, const LocalBsm &him, const RelativePos &p)
{
    // 从车在本车的规定范围以外，不触发
    if( p.h >  KAvwRightMax )return;
    if( p.h < -kAvwLeftMax  )return;
    if( p.v >  kAvwVMax     )return;
    if( p.v <  kAvwVMin     )return;

    if( me.speed < kAvwSpeedMin )return;

    // 计算两车纵向相对速度
    double angle_diff     = getAngleDiff(me.heading,him.heading);
    double relative_speed = me.speed - cos(degreeToRadian(angle_diff))*him.speed;
    if(relative_speed <= 0)return;

    sendEventToUi(UI_E_AVW,0,p.dist);
}
