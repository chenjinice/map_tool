/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#include "convert.h"


using namespace std;


// asn的rsi转本地
bool rsiToLocal(const RoadSideInformation &rsi, LocalRsi &l, const timeval &tv) {
    l.msg_count                         = rsi.msgCnt;
    l.tv                                = tv;
    octetToLocal(&rsi.id,l.id);
    posToLocal(rsi.refPos,l.pos);
    // rtes
    if(rsi.rtes){
        for(int i=0;i<rsi.rtes->list.count;i++){
            RTEData_t * rtes            = rsi.rtes->list.array[i];
            LocalRte l_rtes;
            if(rtes->eventPos){
                posOffsetToLocal(*rtes->eventPos,l_rtes.event_pos,l.pos);
            }
            if(rtes->referencePaths){
                for(int m=0;m<rtes->referencePaths->list.count;m++){
                    ReferencePath *path  = rtes->referencePaths->list.array[m];
                    LocalRefPath l_path;
                    l_path.radius        = path->pathRadius * kResRadius;
                    for(int k=0;k<path->activePath.list.count;k++){
                        PositionOffsetLLV *pos = path->activePath.list.array[k];
                        PosWGS84 l_pos;
                        posOffsetToLocal(*pos,l_pos,l.pos);
                        l_path.points.push_back(l_pos);
                    }
                    l_rtes.paths.push_back(l_path);
                }
                l_rtes.rte_id               = rtes->rteId;
                l_rtes.type                 = rtes->eventType;
                l_rtes.source               = rtes->eventSource;
                descriptionToLocal(rtes->description,l_rtes.des);
                l.rtes.push_back(l_rtes);
            }
        }
    }
    // rtss
    if(rsi.rtss){
        for(int i=0;i<rsi.rtss->list.count;i++){
            RTSData_t * rtss            = rsi.rtss->list.array[i];
            LocalRts l_rtss;
            if(rtss->signPos){
                posOffsetToLocal(*rtss->signPos,l_rtss.sign_pos,l.pos);
            }
            if(rtss->referencePaths){
                for(int m=0;m<rtss->referencePaths->list.count;m++){
                    ReferencePath *path  = rtss->referencePaths->list.array[m];
                    LocalRefPath l_path;
                    l_path.radius        = path->pathRadius * kResRadius;
                    for(int k=0;k<path->activePath.list.count;k++){
                        PositionOffsetLLV *pos = path->activePath.list.array[k];
                        PosWGS84 l_pos;
                        posOffsetToLocal(*pos,l_pos,l.pos);
                        l_path.points.push_back(l_pos);
                    }
                    l_rtss.paths.push_back(l_path);
                }
                l_rtss.rts_id               = rtss->rtsId;
                l_rtss.type                 = rtss->signType;
                descriptionToLocal(rtss->description,l_rtss.des);
                l.rtss.push_back(l_rtss);
            }
        }
    }
    return true;
}








// ------------------------------------------------------------------------------------------------------------

static bool refPathsToAsn(const vector<LocalRefPath> &v,ReferencePathList *paths) {
    int count = v.size();
    for(int i=0;i<count;i++){
        const LocalRefPath &l   = v[i];
        ReferencePath *path     = new ReferencePath();
        path->pathRadius        = l.radius/kResRadius;

        int pt_count = l.points.size();
        for(int k=0;k<pt_count;k++){
            const PosWGS84 &  l_pos = l.points[k];
            PositionOffsetLLV * pos = new PositionOffsetLLV();
            posOffsetToAsnLatLon(l_pos,*pos);
            ASN_SET_ADD(&path->activePath,pos);
        }
        ASN_SET_ADD(&paths->list,path);
    }
    return true;
}

static bool rtssToAsn(const vector<LocalRts> &v,RTSList *rtss) {
    int count = v.size();
    for(int i=0;i<count;i++){
        const LocalRts &l   = v[i];
        RTSData *rts        = new RTSData();
        rts->rtsId          = l.rts_id;
        rts->signType       = l.type;
        rts->signPos        = new PositionOffsetLLV();

        posOffsetToAsnLatLon(l.sign_pos,*(rts->signPos));
        descriptionToAsn(l.des,rts->description,kDesStringMinSize,kDesStringMaxSize);

        if(l.paths.size() > 0){
            rts->referencePaths = new ReferencePathList();
            refPathsToAsn(l.paths,rts->referencePaths);
        }
        ASN_SET_ADD(&rtss->list,rts);
    }
    return true;
}

static bool rtesToAsn(const vector<LocalRte> &v,RTEList *rtes) {
    int count = v.size();
    for(int i=0;i<count;i++){
        const LocalRte &l   = v[i];
        RTEData *rte        = new RTEData();
        rte->rteId          = l.rte_id;
        rte->eventType      = l.type;
        rte->eventSource    = l.source;
        rte->eventPos       = new PositionOffsetLLV();

        posOffsetToAsnLatLon(l.event_pos,*(rte->eventPos));
        descriptionToAsn(l.des,rte->description,kDesStringMinSize,kDesStringMaxSize);

        if(l.paths.size() > 0){
            rte->referencePaths = new ReferencePathList();
            refPathsToAsn(l.paths,rte->referencePaths);
        }
        ASN_SET_ADD(&rtes->list,rte);
    }
    return true;
}

// 本地的rsi转asn
MessageFrame *rsiToAsn(const LocalRsi &l) {
    static uint s_count                 = 0;

    MessageFrame  *msg                  = new MessageFrame();
    msg->present                        = MessageFrame_PR_rsiFrame;
    RoadSideInformation *rsi            = &( msg->choice.rsiFrame );
    rsi->msgCnt                         = s_count++%(kMsgCountMax+1);

    octetToAsn(l.id,rsi->id,kIdSize,kIdSize);
    posToAsn(l.pos,rsi->refPos);

    if(l.rtss.size() > 0) {
        rsi->rtss               = new RTSList();
        rtssToAsn(l.rtss,rsi->rtss);
    }
    if(l.rtes.size() > 0) {
        rsi->rtes               = new RTEList();
        rtesToAsn(l.rtes,rsi->rtes);
    }
    return msg;
}



