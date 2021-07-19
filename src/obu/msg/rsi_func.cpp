/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#include "msg.h"

#include <unistd.h>

#include "device/this_device.h"



/*
 * 根据车的位置和航向角, 计算是否在影响区域内
 * pos : 本车位置
 * heading : 本车航向角
 * paths : 影响路径
 * 返回值 : true在影响区域内，false不在
 */
static bool inReferencePath(const PosWGS84 &pos, double heading, const vector<LocalRefPath> &paths)
{
    bool ret         = false;
    int  path_count  = paths.size();
    int  angle_range = 30.0;

    // 判断是否在影响半径以内
    for(int i=0;i<path_count;i++){
        const vector<PosWGS84> &points  = paths[i].points;
        double  radius                  = paths[i].radius;
        int     count                   = points.size();
        for(int m=0;m<count-1;m++){
            // 为了方便打印，全部计算完再判断
            double v_dist               = minVerticalDist(pos,points[m],points[m+1]);
            double road_heading         = getRoadAngle(points[m],points[m+1]);
            double angle                = getAngleDiff(road_heading,heading);
            myDebug("--[%.07lf,%.07lf]=>[%.07lf,%.07lf],radius=%.02f,road_heading=%.02f,v_dist=%.02f,angle=%.02f\n",
                    points[m].lng,points[m].lat,points[m+1].lng,points[m+1].lat,radius,road_heading,v_dist,angle);
            if( (v_dist <= radius) && (fabs(angle) <= angle_range) ){ ret = true;break; }
        }
    }
    return ret;
}


// rtss场景
static void rtsFunc(const LocalRts &rts)
{
    ThisDevice *me   = ThisDevice::instance();

    if(!me->scenes.rts)return;

    int des_value       = atoi(rts.des.c_str());
    double  dist        = getDistance(me->pos,rts.sign_pos);
    myDebug("rsi:rts=%d,pos=%.07lf,%.07lf,des=%d,dist=%.02f\n",rts.type,rts.sign_pos.lng,rts.sign_pos.lat,des_value,dist);

    if( !inReferencePath(me->pos,me->heading,rts.paths) )return;

    switch (rts.type) {
        case GB_SIGN_85:
            appSlw(*me,des_value,dist);
            break;
        default:
            sendEventToUi(UI_E_RTS,rts.type,dist,des_value);
            break;
    }
}


// rtes场景
static void rteFunc(const LocalRte &rte)
{
    ThisDevice *me      = ThisDevice::instance();

    if(!me->scenes.rte)return;

    int des_value       = atoi(rte.des.c_str());
    double  dist        = getDistance(me->pos,rte.event_pos);
    myDebug("rsi:rte=%d,pos=%.07lf,%.07lf,des=%d,dist=%.02f\n",rte.type,rte.event_pos.lng,rte.event_pos.lat,des_value,dist);

    if( !inReferencePath(me->pos,me->heading,rte.paths) )return;

    switch (rte.type) {
        default:
            sendEventToUi(UI_E_RTE,rte.type,dist,des_value);
            break;
    }
}

// ----------- rsi场景 -------------
void rsiFunc(const LocalRsi &rsi)
{
//    rsi.show();
    int rtes_count = rsi.rtes.size();
    int rtss_count = rsi.rtss.size();

    for(int i=0;i<rtes_count;i++){
        const LocalRte &rte = rsi.rtes[i];
        rteFunc(rte);
    }

    for(int i=0;i<rtss_count;i++){
        const LocalRts &rts = rsi.rtss[i];
        rtsFunc(rts);
    }
}
