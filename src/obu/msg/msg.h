/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#ifndef __MSG_H__
#define __MSG_H__


#include <iostream>
#include <list>
#include <vector>

#include "local/local.h"
#include "convert.h"
#include "bsm_func.h"
#include "map_func.h"
#include "application/application.h"


// 场景处理
void    rsiFunc(const LocalRsi &rsi);
void    rsmFunc(const LocalRsm &rsm);
void    spatFunc(const LocalSpat &spat);

// 数据编解码
void    msgDecode(uint8_t *buffer, int len,const timeval &tv);


#endif

