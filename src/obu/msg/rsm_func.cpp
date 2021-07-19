/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */


#include "msg.h"

#include <unistd.h>

#include "device/this_device.h"



// rsm场景 
void rsmFunc(const LocalRsm &rsm)
{
//    rsm.show();
    ThisDevice *me = ThisDevice::instance();
    SceneConfig &cfg = me->scenes;

    int count = rsm.ptcs.size();
    for(int i=0;i<count;i++){
        const LocalParticipant &ptc = rsm.ptcs[i];
        if(cfg.vrucw && (ptc.ptc_type == ParticipantType_pedestrian) ) appVrucw(*me,ptc.pos);
    }
}
