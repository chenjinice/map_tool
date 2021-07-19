/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#ifndef __MSG_H__
#define __MSG_H__


#include <iostream>
#include <list>
#include <vector>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

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
void msgDecode(uint8_t *buffer, int len, QString ip, const timeval &tv, QJsonObject &json ,bool flag = false);


#endif

