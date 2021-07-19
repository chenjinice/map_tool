/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#include "application.h"


/*
 * 发送事件给Ui
 * type         : 事件类型 ,local.h有宏定义
 * sub_type     : 子类型
 * distance     : 距离, 米
 * des_value    : rsi 的description转整型
 */
void sendEventToUi(int type, int sub_type, double distance, int des_value)
{
    myDebug("event:type=%d,sub=%d,dist=%.02lf,des=%d\n",type,sub_type,distance,des_value);
}


