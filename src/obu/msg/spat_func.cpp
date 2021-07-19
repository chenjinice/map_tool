/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#include "msg.h"

#include <unistd.h>

#include "device/this_device.h"



// 把spat里的红绿灯数据 ， 设置到本车
static bool setLightData(CurrentLightInfo &info,const LocalLight &light)
{
    info.valid      = true;
    info.color      = light.color;
    info.time       = light.time;
    info.phase_id   = light.phase_id;
    return true;
}

// 获取当前车的灯信息
static void getCurrentLight(const LocalSpat &spat,const CurrentMap &me_map,CurrentLights &me_lights)
{
    int count               = spat.lights.size();
    LocalPhases me_phases   = me_map.phases; // 当前所在车道的红绿灯相位

    for(int i=0;i<count;i++){
        bool              flag  = false;
        const LocalLight &light = spat.lights[i];
        if(light.node_id.id != me_map.node_id.id)continue;
        if(light.node_id.region != me_map.node_id.region)continue;
        if(light.phase_id == me_phases.left){
            flag = setLightData(me_lights.left,light);
        }
        if(light.phase_id == me_phases.right){
            flag = setLightData(me_lights.right,light);
        }
        if(light.phase_id == me_phases.straight){
            flag = setLightData(me_lights.straight,light);
        }
        if(flag){
            me_lights.tv = spat.tv;
        }
    }
    //计算当前车道的灯,直行灯最后判断，所以优先级最高,注意这里的 if 没有 else
    if( me_lights.right.valid  && (me_map.dir & DIR_RIGHT) ){
        me_lights.current = me_lights.right;
    }
    if( me_lights.left.valid     && (me_map.dir & DIR_LEFT) ) {
        me_lights.current = me_lights.left;
    }
    if( me_lights.straight.valid && (me_map.dir & DIR_STRAIGHT) ){
        me_lights.current = me_lights.straight;
    }
}


//  spat 场景
void spatFunc(const LocalSpat &spat)
{
//    spat.show();
    int count               = spat.lights.size();
    if(count > 0){
        myDebug("spat received,id=%d,region=%d,phases=%d\n",spat.lights[0].node_id.id,spat.lights[0].node_id.region,count);
    }

    ThisDevice * me         = ThisDevice::instance();
    SceneConfig & cfg       = me->scenes;
    CurrentMap me_map       = me->map_info;         // 获取本车所在地图信息;

    if(me_map.valid == false)return;

    CurrentLights me_lights;
    getCurrentLight(spat,me_map,me_lights);
    me->lights = me_lights;
}


