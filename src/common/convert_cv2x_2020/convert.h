/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#ifndef __CONVERT_H__
#define __CONVERT_H__


#include "local/local.h"
#include "MessageFrame.h"


// 打印函数
#define  conLog                 printf


// 外部可能用到的宏定义
const int OFFSET_COUNT          = 7;           // 国标经纬度点 7 种描述方式
const int kMsgCountMax          = 127;         // msg count 最大值
const int kIdSize               = 8;           // bsm,rsm,rsi中id的数据长度

// bsm


// rsm
const int kPtcIdSize            = 8;           // ptc id size
const int kPtcMinCount          = 1;           // rsm 参与者最小个数
const int kPtcMaxCount          = 16;          // rsm 参与者最大个数

// rsi
const int kRisPointMinCount     = 2;           // rsi的alertPath经纬度点最小个数
const int kRisPointMaxCount     = 32;          // rsi的alertPath经纬度点最大个数

// spat
const int kSpatDesMinSize       = 1;
const int kSpatDesMaxSize       = 63;

// description
const int kDesStringMinSize     = 1;           // description的IA5String最小长度
const int kDesStringMaxSize     = 512;         // description的IA5String最大长度
const int kDesGb2312MinSize     = 2;           // description的GB2312最小长度
const int kDesGb2312MaxSize     = 512;         // description的GB2312最大长度

// 分辨率
const double kResPos            = 1e-7;        // 经纬度分辨率
const double kResElev           = 0.1;         // 高度分辨率 0.1 m
const double kResSize           = 0.01;        // 物体长宽分辨率 0.01 m
const double kResHeight         = 0.05;        // 物体高度分辨率 0.05 m
const double kResSpeed          = 0.02;        // 国标速度分辨率 0.02 m/s
const double kResHeading        = 0.0125;      // 国标航向角分辨率 0.0125 度
const double kResAcc            = 0.02;        // 国标加速度分辨率 0.02 m/s^2
const double kResRadius         = 0.1;         // rsi影响半径分辨率 0.1 m
const double kResTiming         = 0.1;         // 红绿灯时间分辨率 0.1 s



// common convert 
uint8_t          byteReverse(uint8_t data);
bool             bitStringToInt(const BIT_STRING_t *p, int &l);
bool             intToBitString(int value, BIT_STRING_t *&p, int len, int unused = 0);
int              getCallocLength(int min,int max,int len);
bool             checkPosValid(const PosWGS84 &p);

bool             descriptionToLocal(const Description *des,string &l);
bool             descriptionToAsn(const string &l,Description * &des,int min,int max);
bool             octetToLocal(const OCTET_STRING *str,string &l);
bool             octetToAsn(const string &l, OCTET_STRING &str, int min, int max);
bool             octetToAsn(const string &l, OCTET_STRING * &str,int min, int max);
bool             nodeIdToLocal(const NodeReferenceID &id ,LocalNodeId &l);
bool             nodeIdToAsn(const LocalNodeId &l ,NodeReferenceID &id,bool has_region = false);
bool             posToLocal(const Position3D &pos, PosWGS84 &l);
bool             posToAsn(const PosWGS84 &l, Position3D &p, bool has_elev = false);
bool             sizeTolocal(const VehicleSize &size, ObjSize &l);
bool             sizeToAsn(const ObjSize &l, VehicleSize &size, bool has_height = false);
bool             lightColorToLocal(LightState_t light, LocalLightColor &l);
bool             lightColorToAsn(LocalLightColor l, LightState_t &light);
bool             posOffsetToLocal(const PositionOffsetLLV &offset,PosWGS84 &l,PosWGS84 &ref_pos);
bool             posOffsetToAsn(const PosWGS84 &l, PositionOffsetLLV &offset, const PosWGS84 &ref_pos, bool full = false);
bool             posOffsetToAsnLatLon(const PosWGS84 &l,PositionOffsetLLV &offset);

// bsm
bool             bsmToLocal(const BasicSafetyMessage &bsm, LocalBsm &l,const timeval &tv);
MessageFrame *   bsmToAsn(const LocalBsm &l);

// rsi
bool             rsiToLocal(const RoadSideInformation &rsi, LocalRsi &l,const timeval &tv);
MessageFrame *   rsiToAsn(const LocalRsi &l);

// rsm
bool             rsmToLocal(const RoadsideSafetyMessage &rsm,LocalRsm &l,const timeval &tv);
MessageFrame *   rsmToAsn(const LocalRsm &l);

// spat
bool             spatToLocal(const SPAT &spat,LocalSpat &l,const timeval &tv);
MessageFrame *   spatToAsn(const LocalSpat &l);

// map
bool             mapToLocal(const MapData &map,LocalMap &l,const timeval &tv);

#endif


