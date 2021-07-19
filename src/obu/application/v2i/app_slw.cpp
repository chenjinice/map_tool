/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#include "application/application.h"


/*---- 限速预警 ----*/


// me        : 本车数据
// max_speed : 最大限速
// distance  : 与事件位置的距离
void appSlw(const LocalBsm &me, int max_speed, double distance)
{
    bool valid  = false;

    if(me.speed*3.6 >= max_speed){
        valid   =   true;
    }

    if(valid){
        sendEventToUi(UI_E_RTS,GB_SIGN_85,distance,max_speed);
    }

}

