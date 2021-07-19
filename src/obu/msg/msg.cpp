/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */


#include "msg.h"

#include <unistd.h>
#include <string.h>
#include <thread>
#include <mutex>
#include "device/this_device.h"



//-------------------------------数据编解码--------------------------------------------

static void asn_log(uint8_t *buffer,int len,MessageFrame_t *msg)
{

}

// 解析buffer ，并写场景代码
void msgDecode(uint8_t *buffer, int len, const timeval &tv)
{
    LocalBsm   bsm;
    LocalMap   map;
    LocalRsm   rsm;
    LocalRsi   rsi;
    LocalSpat  spat;

    asn_dec_rval_t   ret;
    MessageFrame_t * msg            = nullptr;
    ret = uper_decode_complete(nullptr,&asn_DEF_MessageFrame,(void **)&msg,buffer,len);
    if(ret.code != RC_OK){
        myDebug("uper_decode_complete failed\n");
        goto GO_END;
    }
    switch (msg->present) {
        case MessageFrame_PR_bsmFrame:
//            bsmToLocal(msg->choice.bsmFrame,bsm,tv);
            bsmFunc(bsm);
            break;
        case MessageFrame_PR_mapFrame:
            mapToLocal(msg->choice.mapFrame,map,tv);
            MapCache::instance()->add(map);
            break;
        case MessageFrame_PR_rsmFrame:
//            rsmToLocal(msg->choice.rsmFrame,rsm,tv);
            rsmFunc(rsm);
            break;
        case MessageFrame_PR_spatFrame:
//            spatToLocal(msg->choice.spatFrame,spat,tv);
            spatFunc(spat);
            break;
        case MessageFrame_PR_rsiFrame:
//            rsiToLocal(msg->choice.rsiFrame,rsi,tv);
            rsiFunc(rsi);
            break;
        case MessageFrame_PR_NOTHING:
        default:
            break;
    }
GO_END:
    if(msg){
        ASN_STRUCT_FREE(asn_DEF_MessageFrame,msg);
    }
}


