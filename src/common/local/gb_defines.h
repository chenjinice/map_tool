/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#ifndef __GB_DEFINES_H__
#define __GB_DEFINES_H__



// ltev 发送的aid
const int kBsmAid               =  111;
const int kMapAid               =  3618;
const int kSpatAid              =  3619;
const int kRsiAid               =  3620;
const int kRsmAid               =  3623;


// ----------- 国标rsi标牌索引，根据 GB 5768.2-2009 道路交通标志和标线 第2部分 -------------------
enum GbSignType{
    GB_SIGN_2           = 2,          // 急弯路
    GB_SIGN_5           = 5,          // 陡坡
    GB_SIGN_17          = 17,         // 易滑
    GB_SIGN_26          = 26,         // 路面低洼
    GB_SIGN_27          = 27,         // 过水路面
    GB_SIGN_34          = 34,         // 事故易发路段
    GB_SIGN_38          = 38,         // 道路施工
    GB_SIGN_53          = 53,         // 禁止机动车驶入
    GB_SIGN_76          = 76,         // 禁止超车
    GB_SIGN_78          = 78,         // 禁止停车
    GB_SIGN_81          = 81,         // 限宽
    GB_SIGN_82          = 82,         // 限高
    GB_SIGN_85          = 85,         // 限制速度
    GB_SIGN_114         = 114,        // 人行横道
    GB_SIGN_163         = 163,        // 车道变窄
    GB_SIGN_242         = 242,        // 前方学校
};



// ----------- GB/T 29100-2012 --------------------------------
enum GbEventType{
    GB_EVENT_407        = 407,        // 道路积水
    GB_EVENT_408        = 408,        // 道路湿滑
    GB_EVENT_707        = 707,        // 交通拥堵
};

#endif


