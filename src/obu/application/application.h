/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "my/my_common.h"
#include "local/local.h"


// 发送事件给Ui
void sendEventToUi(int type,int sub_type=0,double distance=0, int des_value=0);


// -------------- V2V -----------------------------
// 前向碰撞预警
void appFcw(const LocalBsm &me,const LocalBsm &him,const RelativePos &p);
// 交叉路口碰撞预警,基于相对位置
void appIcw(const LocalBsm &me, LocalBsm &him,const RelativePos &p);
// 交叉路口碰撞预警,基于地图
void appIcw(const LocalBsm &me,const LocalBsm &him,const CurrentMap &me_map,const CurrentMap &him_map);
// 异常车辆预警
void appAvw(const LocalBsm &me,const LocalBsm &him,const RelativePos &p);



// -------------- V2I -----------------------------
// 限速预警
void appSlw(const LocalBsm &me, int max_speed, double distance);
// 弱势交通参与者(行人)碰撞预警
void appVrucw(const LocalBsm &me, const PosWGS84 &pos);













#endif





