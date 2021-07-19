/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#include "convert.h"


using namespace std;


// rsm转本地数据
bool rsmToLocal(const RoadsideSafetyMessage &rsm, LocalRsm &l, const timeval &tv)
{
    l.msg_count                         = rsm.msgCnt;
    l.tv                                = tv;
    octetToLocal(&rsm.id,l.id);
    posToLocal(rsm.refPos,l.pos);

    int count                           = rsm.participants.list.count;
    for(int i=0;i<count;i++){
        LocalParticipant    l_ptc;
        ParticipantData_t * ptc         = rsm.participants.list.array[i];
        posOffsetToLocal(ptc->pos,l_ptc.pos,l.pos);
        sizeTolocal(ptc->size,l_ptc.size);
        octetToLocal(ptc->id,l_ptc.id);
        l_ptc.ptc_id                    = ptc->ptcId;
        l_ptc.ptc_type                  = ptc->ptcType;
        l_ptc.source                    = ptc->source;
        l_ptc.speed                     = ptc->speed   * kResSpeed;
        l_ptc.heading                   = ptc->heading * kResHeading;
        l.ptcs.push_back(l_ptc);
    }
    return true;
}

// rsm转asn
MessageFrame * rsmToAsn(const LocalRsm &l)
{
    static uint s_count                 = 0;
    MessageFrame  *msg                  = new MessageFrame();
    msg->present                        = MessageFrame_PR_rsmFrame;
    RoadsideSafetyMessage   *rsm        = &( msg->choice.rsmFrame );
    rsm->msgCnt                         = s_count++%(kMsgCountMax+1);
    octetToAsn(l.id,rsm->id,kIdSize,kIdSize);
    posToAsn(l.pos,rsm->refPos);

    int ptc_count                       = l.ptcs.size();
    int count = getCallocLength(kPtcMinCount,kPtcMaxCount,ptc_count);
    for(int i=0;i<count;i++){
        ParticipantData  *      ptc     = new ParticipantData();
        const LocalParticipant &l_ptc   = l.ptcs[i];
        ASN_SET_ADD(&rsm->participants,ptc);
        octetToAsn(l_ptc.id,ptc->id,kPtcIdSize,kIdSize);
        ptc->ptcId                      = l_ptc.ptc_id;
        ptc->ptcType                    = l_ptc.ptc_type;
        ptc->source                     = l_ptc.source;
        ptc->speed                      = l_ptc.speed   / kResSpeed;
        ptc->heading                    = l_ptc.heading / kResHeading;
        posOffsetToAsn(l_ptc.pos,ptc->pos,l.pos);
        sizeToAsn(l_ptc.size,ptc->size);
    }
    return msg;
}


