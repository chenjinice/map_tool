/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#include "application/application.h"


/*---- 交叉路口碰撞预警 ----*/


static const double kIcwVMax      = 100.0;         // 纵向最大距离,m
static const double kIcwVMin      = 2.0;           // 纵向最小距离,m
static const double kIcwDistMax   = 100.0;         // 两车最大距离,m
static const double kIcwSpeedMin  = 10.0;          // 两车最小速度, km/h
static const double kIcwAngleMax  = 120.0;         // 两车航向角差值最大值
static const double kIcwAngleMin  = 60.0;          // 两车航向角差值最小值


/*
 * 交叉路口碰撞预警,基于相对位置的算法
 * me  : 本车
 * him : 从车
 * p   : 从车在本车的相对位置
 */
void appIcw(const LocalBsm &me, LocalBsm &him, const RelativePos &p)
{
    // 从车在本车的规定范围以外，不触发
    if(p.v > kIcwVMax)return;
    if(p.v < kIcwVMin)return;
    if(p.dist > kIcwDistMax)return;

    // 速度车小，不触发
    if(me.speed*3.6  < kIcwSpeedMin)return;
    if(him.speed*3.6 < kIcwSpeedMin)return;

    // 两车航向角不在规定范围以内，不触发(不是基于地图，只能靠航向角筛选)
    double angle_diff = fabs(getAngleDiff(me.heading,him.heading));
    if(p.h < 0 && angle_diff < 0)return;
    if(p.h > 0 && angle_diff > 0)return;
    if( angle_diff > kIcwAngleMax )return;
    if( angle_diff < kIcwAngleMin )return;

    sendEventToUi(UI_E_ICW,0,p.dist);
}


/*
 * 交叉路口碰撞预警,基于相对位置的算法
 * me  : 本车
 * him : 从车
 * me_map  : 本车所在地图信息
 * him_map : 从车所在地图信息
 */
void appIcw(const LocalBsm &me, const LocalBsm &him, const CurrentMap &me_map, const CurrentMap &him_map)
{
    if(me.speed  < kIcwSpeedMin)return;
    if(him.speed < kIcwSpeedMin)return;

    // 要在同一个地图的不同 link
    if(!me_map.valid)return;
    if(!him_map.valid)return;
    if(me_map.node_id.id     != him_map.node_id.id)return;
    if(me_map.node_id.region != him_map.node_id.region)return;
    // 不能在同一个link
    if(me_map.link_id == him_map.link_id)return;

    // 两车航向角不在规定范围以内，不触发(滤掉正对方向link的)
    double angle_diff = fabs(getAngleDiff(me.heading,him.heading));
    if( angle_diff > 150.0 )return;

    double dist = getDistance(me.pos,him.pos);
    sendEventToUi(UI_E_ICW,dist);
}
