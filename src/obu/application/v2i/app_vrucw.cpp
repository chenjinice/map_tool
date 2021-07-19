/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#include "application/application.h"
#include "my/my_common.h"


/*---- 弱势交通参与者(行人)碰撞预警 ----*/


static const double kPcwLeftMax     = 5.0;           // 左边最大距离,m
static const double KPcwRightMax    = 5.0;           // 右边最大距离,m
static const double kPcwVMax        = 60.0;          // 纵向最大距离,m
static const double kPcwVMin        = 0.0;           // 纵向最小距离,m
static const double kPcwSpeedMin    = 1.0;           // 本车速度最小值, m/s


// me     : 本车
// pos    : 行人位置
void appVrucw(const LocalBsm &me,const PosWGS84 &pos)
{
    RelativePos p,ant_p;
    getRelativePos(me.pos,pos,me.heading,p);
    getAntBasePos(me.antenna,p,ant_p);
    myDebug("vrucw:%.07lf,%.07lf,h=%.02f(%.02f),v=%.02f(%.02f)\n",pos.lng,pos.lat,p.h,ant_p.h,p.v,ant_p.v);

    if( me.speed < kPcwSpeedMin )return;
    // 从车在本车的规定范围以外，不触发
    if( ant_p.h >  KPcwRightMax )return;
    if( ant_p.h < -kPcwLeftMax  )return;
    if( ant_p.v >  kPcwVMax     )return;
    if( ant_p.v <  kPcwVMin     )return;

    sendEventToUi(UI_E_VRUCW,0,p.dist);
}
