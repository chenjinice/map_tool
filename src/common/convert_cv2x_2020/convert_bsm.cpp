/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#include "convert.h"


using namespace std;



// asn的bsm转本地
bool bsmToLocal(const BasicSafetyMessage &bsm, LocalBsm &l, const timeval &tv)
{
    l.msg_count                         = bsm.msgCnt;
    l.tv                                = tv;
    octetToLocal(&bsm.id,l.id);
    posToLocal(bsm.pos,l.pos);
    sizeTolocal(bsm.size,l.size);
    l.car_type                          = bsm.vehicleClass.classification;
    l.speed                             = bsm.speed*kResSpeed;
    l.heading                           = bsm.heading*kResHeading;
    l.acc                               = bsm.accelSet.Long*kResAcc;
    if(bsm.safetyExt && bsm.safetyExt->events){
        bitStringToInt(bsm.safetyExt->events,l.event_flag);
    }
    return true;
}

// 本地bsm转asn
MessageFrame *bsmToAsn(const LocalBsm &l)
{
    static uint s_count                 = 0;

    MessageFrame  *msg                  = new MessageFrame();
    msg->present                        = MessageFrame_PR_bsmFrame;
    BasicSafetyMessage *bsm             = &( msg->choice.bsmFrame );
    bsm->msgCnt                         = s_count++%(kMsgCountMax+1);

    octetToAsn(l.id,bsm->id,kIdSize,kIdSize);
    posToAsn(l.pos,bsm->pos);
    sizeToAsn(l.size,bsm->size,true);
    bsm->vehicleClass.classification    = l.car_type;
    bsm->speed                          = l.speed / kResSpeed;
    bsm->heading                        = l.heading / kResHeading;
    bsm->accelSet.Long                  = l.acc / kResAcc;
    bsm->secMark                        = l.sec_mark;
    if(l.event_flag > 0){
        bsm->safetyExt = new VehicleSafetyExtensions();
        intToBitString(l.event_flag,bsm->safetyExt->events,2);
    }
    return msg;
}



