/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#include "convert.h"


using namespace std;


// 获取当前灯的颜色和剩余时间
static void getLightInfo(const PhaseStateList_t &state_list,LocalLight &l)
{
    int count = state_list.list.count;
    for(int i=0;i<count;i++){
        PhaseState *state = state_list.list.array[i];
        if(state->timing == nullptr)continue;
        switch (state->timing->present) {
            case TimeChangeDetails_PR_counting:
            {
                TimeCountingDown counting = state->timing->choice.counting;
                if(counting.startTime    != 0)continue;
                lightColorToLocal(state->light,l.color);
                l.time                    = counting.likelyEndTime * kResTiming;
            }
                break;
            case TimeChangeDetails_PR_utcTiming:
                break;
            default:
                continue;
                break;
        }
    }
}

//
static void phasesToLocal(const IntersectionState_t *it,LocalSpat &l)
{
    int count = it->phases.list.count;
    for(int i=0;i<count;i++){
        Phase_t *phase = it->phases.list.array[i];
        LocalLight l_light;
        nodeIdToLocal(it->intersectionId,l_light.node_id);
        l_light.phase_id = phase->id;
        getLightInfo(phase->phaseStates,l_light);
        l.lights.push_back(l_light);
    }
}

// spat转本地
bool spatToLocal(const SPAT &spat, LocalSpat &l, const timeval &tv)
{
    l.msg_count                         = spat.msgCnt;
    l.tv                                = tv;
    octetToLocal(spat.name,l.des_name);

    int count                           = spat.intersections.list.count;
    for(int i=0;i<count;i++){
        IntersectionState   *it         = spat.intersections.list.array[i];
        phasesToLocal(it,l);
    }
    return true;
}



//---------------------------------------------------------------------

// spat转asn
MessageFrame * spatToAsn(const LocalSpat &l)
{
    if(l.lights.size() == 0)return nullptr;

    static uint s_count                 = 0;
    MessageFrame  *msg                  = new MessageFrame();
    msg->present                        = MessageFrame_PR_spatFrame;
    SPAT *spat                          = &( msg->choice.spatFrame );
    spat->msgCnt                        = s_count++%(kMsgCountMax+1);

    octetToAsn(l.des_name,spat->name,kSpatDesMinSize,kSpatDesMaxSize);

    LocalNodeId node_id = l.lights[0].node_id;
    // 只能放同一个node_id的搞法
    IntersectionState   *inter          = new IntersectionState();
    ASN_SET_ADD(&spat->intersections.list,inter);
    nodeIdToAsn(node_id,inter->intersectionId,true);

    int count = l.lights.size();
    for(int i=0;i<count;i++){
        Phase *phase                    = new Phase();
        const LocalLight &l_light       = l.lights[i];
        ASN_SET_ADD(&inter->phases,phase);
        phase->id                       = l_light.phase_id;

        PhaseState *state               = new PhaseState();
        ASN_SET_ADD(&phase->phaseStates.list,state);
        state->timing = new TimeChangeDetails();
        state->timing->present          = TimeChangeDetails_PR_counting;
        state->timing->choice.counting.startTime = 0;
        state->timing->choice.counting.likelyEndTime = l_light.time/kResTiming;
        lightColorToAsn(l_light.color,state->light);
    }
    return msg;
}
