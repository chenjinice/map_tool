/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#ifndef __LOCAL_H__
#define __LOCAL_H__


#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <sys/time.h>

#include <iostream>
#include <vector>
#include "gb_defines.h"

using namespace std;


/* 本地数据结构,把asn转化为这一种数据
 * 只加了必要的
 * 为方便修改，本地数据全部成员变量设为 public
 */


// ----------- 自定义场景id，ui显示需要 ------------------------------------------------------

enum UiEventType{
    UI_E_FCW        = 1,          // 前向碰撞预警
    UI_E_ICW        = 2,          // 交叉路口碰撞预警
    UI_E_LTA        = 3,          // 左转辅助
    UI_E_BSW        = 4,          // 盲区预警
    UI_E_DNPW       = 5,          // 逆向超车预警
    UI_E_EBW        = 6,          // 紧急制动预警
    UI_E_AVW        = 7,          // 异常车辆提醒
    UI_E_CLW        = 8,          // 车辆失控预警
    UI_E_SLW        = 9,          // 限速预警
    UI_E_RLVW       = 10,         // 闯红灯预警
    UI_E_VRUCW      = 11,         // 行人碰撞预警(弱势交通参与者)
    UI_E_GLOSA      = 12,         // 滤波车速引导
    UI_E_EVW        = 13,         // 紧急车辆提醒
    UI_E_RTS        = 14,         // rsi的标牌预警
    UI_E_RTE        = 15,         // rsi的事件预警
};


// ----------- 经纬度相关宏定义 -----------------------------
const double kEarthRadius          = 6371000.0;          // 地球半径, m
const double kSpeedMax             = 65535;              // 限速的默认最大速度, m/s
const double kDistMax              = 1e7;                // 默认距离,m
const double kLngMax               = 180.0;
const double kLatMax               = 90.0;




// 二维坐标点,单位:m
class Point2D{
public:
    double x            = 0;               // 单位:m
    double y            = 0;               // 单位:m
};

// 三维坐标点
class Point3D{
public:
    double x            = 0;               // 单位:m
    double y            = 0;               // 单位:m
    double z            = 0;               // 单位:m
};

// WGS84经纬度点
class PosWGS84{
public:
    double lng          = 0;               // 真实经度,度数
    double lat          = 0;               // 真实纬度,度数
    double elev         = 0;               // 高度，基本没用到,单位:m
};

// 相对位置
class RelativePos{
public:
    double h            = 0;               // horizontal, 左右距离 ,单位:m,右为正
    double v            = 0;               // Vertical,     前后距离 ,单位:m,前为正
    double dist         = 0;               // 总距离 , 单位:m
};

// 物体尺寸
class ObjSize{
public:
    float width         = 0;               // 宽度，单位:m
    float length        = 0;               // 长度，单位:m
    float height        = 0;               // 高度，单位:m
};

// gps天线位置
class AntennaPos{
public:
    float to_head       = 0;               // 与车头距离 ,m
    float to_tail       = 0;               // 与车尾距离 ,m
    float to_left       = 0;               // 与车最左边距离 ,m
    float to_right      = 0;               // 与车最后边距离 ,m
};

// -------------------bsm 相关 -----------------------------

// BSM
class LocalBsm
{
public:
    void  show() const{
        printf("bsm : id=");
        for(uint i=0;i<id.length();i++)printf("%02X ",id[i]);
        printf("(%s)\n",id.c_str());
        for(uint i=0;i<plate.length();i++)printf("%02X ",plate[i]);
        printf("\n--pos=(%.7lf,%.7lf,%.02lf) ,size=(w:%.02lf,l:%.02lf,h:%.02lf)\n",pos.lng,pos.lat,pos.elev,size.width,size.length,size.height);
        printf("--speed=%.02lf,acc=%.02lf,heading=%.02lf,accurancy=%d,event_flag=%d\n",speed,acc,heading,accuracy_h,event_flag);
    }
    
    int              msg_count          = 0;        // 0 ~ 127 , 可用于统计丢包情况
    timeval          tv                 = {0};      // 时间结构体

    string           id;                            // 车ID
    string           plate;                         // 车牌
    int              car_type           = 0;        // 车类型
    ObjSize          size;                          // 车尺寸
    AntennaPos       antenna;                       // gps天线位置
    PosWGS84         pos;                           // 车的经纬度和高度
    float            speed              = 0;        // 速度, m/s
    float            acc                = 0;        // 加速度, m/s^2
    float            heading            = 0;        // 航向角(度数)，北东地坐标系，正北为0,顺时针为正
    int              sec_mark           = 0;        // ?

    float            gps_hdop           = 0;        // gps原始数据的水平精度因子(0.5~99.9)，越小质量越好
    int              accuracy_h         = 0;        // gps水平精度的置信水平
    int              accuracy_elev      = 0;        // gps高度精度的置信水平
    int              satellite_num      = 0;        // gps卫星数量
    int              gps_mode           = 0;        // gps定位模式 ,65(A)=自动，68(D)=差分，69(E)=估测，78(N)=数据无

    int              gear               = 0;        // 档位
    int              brake              = 0;        // 刹车
    int              light              = 0;        // 车灯
    int              door               = 0;        // 车门
    float            steer_angle        = 0;        // 方向盘角度

    int              event_flag         = 0;        // 车辆异常状态

    uint8_t          tsp_phase          = 0;
    uint16_t         tsp_node_id        = 0;
    int16_t          tsp_dist           = 0;
};

//---------------------------------------------------------- 




// -------------------map 相关 -----------------------------

// nodeId
class LocalNodeId{
public:
    int              id              = 0;
    int              region          = 0;
};

// 道路方向属性
enum LocalRoadDir{
    DIR_NONE                         = 0,
    DIR_STRAIGHT                     = 1,   // 可直行
    DIR_LEFT                         = 2,   // 可左转
    DIR_RIGHT                        = 4,   // 可右转
};

// 限速
class LocalSpeedLimit{
public:
    double                max        = kSpeedMax;            // 最大限速, m/s
    double                min        = 0;                    // 最小限速, m/s
};

// 车道的红绿灯id(左中右三个红绿灯相位id)
class LocalPhases
{
public:
    int                   left            = 0;               // 左转 phase id
    int                   straight        = 0;               // 直行 phase id
    int                   right           = 0;               // 右转 pahse id
};

// lane数据
class LocalLane
{
public:
    LocalNodeId           upstream_id;
    int                   link_id         = 0;                    // link id, asn里边没有，本地数据自行添加的，某些场景需要判断是不是在同一个link
    int                   lane_id         = 0;
    int                   dir             = DIR_NONE;             // 车道方向:直行、左转、右转,LocalRoadDir单个值或多个相加
    float                 lane_width      = 0;                    // lane宽度 ，m
    LocalSpeedLimit       speed;                                  // 限速
    LocalPhases           phases;                                 // 左中右三个红绿灯相位id
    vector<PosWGS84>      points;                                 // 经纬度点
};

// node
class LocalNode
{
public:
    LocalNodeId          id;                                      // node id
    PosWGS84             pos;                                     //
    vector<LocalLane>    lanes;                                   // 车道
};

// map
class LocalMap
{
public:
    void  show() const{
        printf("map : msg_count=%d\n",msg_count);
        int node_len = this->nodes.size();
        for(int i = 0; i < node_len; i++){
            const LocalNode &node = this->nodes[i];
            printf("-node : id=%d,region=%d\n",node.id.id,node.id.region);
            int lane_len = node.lanes.size();
            for(int k = 0 ; k < lane_len; k++){
                const LocalLane &lane = node.lanes[k];
                printf("--lane : link_id=%d,lane_id=%d,dir=%d,width=%.02lfm,speed=%.2lf~%.2lfm/s,phaseId=(%d,%d,%d)(←↑→)\n",
                       lane.link_id,lane.lane_id,lane.dir,lane.lane_width,lane.speed.min,lane.speed.max,lane.phases.left,lane.phases.straight,lane.phases.right);

                int point_len = lane.points.size();
                for (int n = 0; n < point_len;  n++){
                    const PosWGS84 & pt = lane.points[n];
                    printf("---point[%d] : lng=%.7lf,lat=%.7lf\n",n,pt.lng,pt.lat);
                }
            }
        }
        printf("map : end\n");
    }

    int                  msg_count          = 0;        // 0 ~ 127 , 可用于统计丢包情况
    timeval              tv                 = {0};      // 时间结构体
    vector<LocalNode>    nodes;
};

//---------------------------------------------------------- 





// -------------------rsi 相关 -----------------------------
// 影响路径
class LocalRefPath
{
public:
    double                  radius              = 0;        // 影响路径的半径
    vector<PosWGS84>        points;                         // 路径上的经纬度点，如果只有一个，则是影响一个圆圈
};

// rsi的交通事件
class LocalRte
{
public:
    void show() const{
        printf("--rte:id=%d,type=%d,des=%s,%d paths,pos(%.07lf,%.07lf)\n",rte_id,type,des.c_str(),(int)paths.size(),event_pos.lng,event_pos.lat);
        for(size_t i=0;i<paths.size();i++){
            printf("----path[%d]:radius=%.02lf\n",(int)i,paths[i].radius);
            const LocalRefPath &p = paths[i];
            for(size_t m=0;m<p.points.size();m++){
                printf("------pos[%d]:lng=%.07lf,lat=%.07lf\n",(int)m,p.points[m].lng,p.points[m].lat);
            }
        }
    }
    int                     rte_id              = 0;
    int                     type                = 0;        // GB/T 29100-2012
    int                     source              = 0;        // 信息来源
    string                  des;                            // description
    PosWGS84                event_pos;
    vector<LocalRefPath>    paths;                          // ReferencePath,影响路径
};

// rsi的交通标志
class LocalRts
{
public:
    void show() const{
        printf("--rts:id=%d,type=%d,des=%s,%d paths,pos(%.07lf,%.07lf)\n",rts_id,type,des.c_str(),(int)paths.size(),sign_pos.lng,sign_pos.lat);
        for(size_t i=0;i<paths.size();i++){
            printf("----path[%d]:radius=%.02lf\n",(int)i,paths[i].radius);
            const LocalRefPath &p = paths[i];
            for(size_t m=0;m<p.points.size();m++){
                printf("------pos[%d]:lng=%.07lf,lat=%.07lf\n",(int)m,p.points[m].lng,p.points[m].lat);
            }
        }
    }
    int                     rts_id              = 0;
    int                     type                = 0;        // GB 5768.2-2009
    string                  des;                            // description
    PosWGS84                sign_pos;
    vector<LocalRefPath>    paths;                          // ReferencePath,影响路径
};

// rsi
class LocalRsi
{
public:
    void  show() const{
        printf("rsi:id=%s,lng=%.07lf,lat=%.07lf,%d rtes ,%d rtss\n",id.c_str(),pos.lng,pos.lat,(int)rtes.size(),(int)rtss.size());
        for (size_t i=0;i<rtes.size();i++){
            rtes[i].show();
        }
        for (size_t i=0;i<rtss.size();i++){
            rtss[i].show();
        }
    }

    int                  msg_count          = 0;        // 0 ~ 127 , 可用于统计丢包情况
    timeval              tv                 = {0};      // 时间结构体

    string               id;                            // rsu id
    PosWGS84             pos;                           // 参考点位置
    vector<LocalRte>    rtes;                          // 交通事件
    vector<LocalRts>    rtss;                          // 交通标志
};

//---------------------------------------------------------- 





// -------------------rsm 相关 -----------------------------

// 交通参与者
class LocalParticipant
{
public:
    void  show() const{
        printf("--rsm.ptc : id = ");
        for (uint i=0;i<id.size();i++) {
            printf("%02X ",id[i]);
        }
        printf("\n--type=%d,lng=%.07lf,lat=%.07lf,speed=%.2f,heading=%.2f\n",ptc_type,pos.lng,pos.lat,speed,heading);
    }

    string                      id;                         // id
    int                         ptc_type        = 0;        // 参与者类型
    uint16_t                    ptc_id          = 0;        // 参与者id
    int                         source          = 0;        // 来源
    PosWGS84                    pos;                        // 位置
    float                       speed           = 0;        // 速度 , m/s
    float                       heading         = 0;        // 航向角(度数)，北东地坐标系，正北为0,顺时针为正
    ObjSize                     size;                       // 尺寸

};

// rsm
class LocalRsm
{
public:
    void  show() const{
        int count = ptcs.size();
        printf("rsm : %d ptcs\n",count);
        for (int i=0;i<count;i++) {
            ptcs[i].show();
        }
    }

    int                         msg_count       = 0;        // 0 ~ 127 , 可用于统计丢包情况
    timeval                     tv              = {0};      // 时间结构体

    string                      id;                         // rsu id
    PosWGS84                    pos;                        // 位置
    vector<LocalParticipant>    ptcs;                       // 参与者列表
};

//---------------------------------------------------------- 





// -------------------spat 相关 -----------------------------

enum LocalLightColor{
    LIGHT_NONE                      = 0,
    LIGHT_RED                       = 'R',                        // 红
    LIGHT_GREEN                     = 'G',                        // 绿
    LIGHT_YELLOW                    = 'Y',                        // 黄
    LIGHT_YELLOW_FLASH              = 'F',                        // 黄闪
};

// 每个灯的数据
class LocalLight{
public:
    LocalNodeId                 node_id;                        // node_id
    int                         phase_id        = 0;            // 相位id
    LocalLightColor             color           = LIGHT_NONE;   // 灯颜色
    float                       time            = 0;            // 灯剩余时间, s
};

// spat
class LocalSpat
{
public:
    void show() const{
        printf("---- spat : msg_count=%d \n",this->msg_count);
        int len = this->lights.size();
        for(int i = 0; i < len ;i++){
            const LocalLight &light = this->lights[i];
            printf("id:%d,region:%d,phase_id:%d,color:%c,time:%lf\n",
                    light.node_id.id,light.node_id.region,light.phase_id,light.color,light.time);
        }
        printf("---- spat end\n");
    }

    int                         msg_count       = 0;        // 0 ~ 127 , 可用于统计丢包情况
    timeval                     tv              = {0};      // 时间结构体

    string                      des_name;
    vector<LocalLight>          lights;

};
//----------------------------------------------------------






// ---------------------本车当前的数据信息---------------------
// 当前所在地图信息
class CurrentMap
{
public:
    void show() const{
        printf("--currentMap-- (valid:%d)\n",valid);
        if(valid){
            printf("----id:%d,region:%d,dir:%d,lane_width:%.02lf,v_dist:%.02lf,dist_to_line:%.02lf\n",node_id.id,node_id.region,dir,lane_width,vertical_dist,dist_to_line);
            printf("----speed(min:%.02lf,max:%.02lf),phases(left:%d,straight:%d,right:%d)\n",speed.min,speed.max,phases.left,phases.straight,phases.right);
        }
    }
    bool                        valid           = false;            // 是否在地图内,决定地图数据是否有效
    LocalNodeId                 node_id;                            // 当前车道node_id
    int                         link_id         = 0;                // 当前link的id
    int                         lane_id         = 0;                // 当前车道id
    int                         dir             = DIR_NONE;         // 车道方向:直行、左转、右转,RoadDir单个值或多个相加
    double                      lane_width      = 0;                // 当前车道宽度 ，m
    double                      vertical_dist   = kDistMax;         // 与当前车道垂线距离, m
    double                      dist_to_line    = kDistMax;         // 与当前车道停止线距离, m

    LocalPhases                 phases;                             // 车道左中右红绿灯相位id
    LocalSpeedLimit             speed;                              // 当前车道限速
};

// 单个红绿灯信息
class CurrentLightInfo
{
public:
    bool                        valid           = false;            // 数据是否有效
    LocalLightColor             color           = LIGHT_NONE;       // 灯颜色
    float                       time            = 0;                // 灯剩余时间, s
    int                         phase_id        = 0;                // 地图里的phase id
};

// 当前红绿灯倒计时
class CurrentLights
{
public:
    timeval                     tv  = {0};                          // 得到灯数据的时间
    CurrentLightInfo            current;                            // 当前灯
    CurrentLightInfo            left;                               // 左转灯
    CurrentLightInfo            straight;                           // 直行灯
    CurrentLightInfo            right;                              // 右转灯
};

// --------------------------------------------------------------------






// ----------------------经纬度相关计算-----------------------------------

// 度数转弧度
double degreeToRadian(double degree);

// 经纬度点转为地心坐标系XYZ
void WGS84To3D(double lng, double lat,Point3D &p);

// 2维点p绕点reference(参考点)逆时针旋转degree度数后的坐标值
void rotate2D(Point2D &reference,Point2D &p,double degree);

// 两个经纬度点之间求距离(Haversine公式)
double getDistance(const PosWGS84 &p1, const PosWGS84 &p2 );

// 两个经纬度点求朝向角(东北天坐标系，正北为0度，顺时针0～360度)
// 方向 : p1为原点，p1指向p2的方向
double getRoadAngle(const PosWGS84 &p1, const PosWGS84 &p2);

// 计算两个(东北天坐标系,0~360)角度的差值
double getAngleDiff(double angle1,double angle2);

// 经纬度点p到线段ab的最短距离计算-矢量法
// flag会被设置为： 0(p到ab有垂线),-1(p在向量ab后方)，-2(p在向量ab前方)
double minDistance(const PosWGS84 &p , const PosWGS84 &a, const PosWGS84 &b, int *flag = nullptr);
// 经纬度点p到线段ab的最短垂直距离,没有垂线的话，返回 DIST_MAX
double minVerticalDist(const PosWGS84 &p , const PosWGS84 &a, const PosWGS84 &b);

// 根据p1的位置和方位角，求p2在p1的哪个位置, pos.h为左右距离，pos.v为前后距离,dist为总距离
void   getRelativePos(const PosWGS84 &p1 , const PosWGS84 &p2, double p1_degree, RelativePos &p);

// 根据本车gps天线的位置，转化相对位置,p.dist不会变,ant=antenna(天线)
void   getAntBasePos(const AntennaPos &ant, const RelativePos &p, RelativePos &ant_p);

#endif

