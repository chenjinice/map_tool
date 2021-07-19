/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */


#include "this_device.h"

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>


ThisDevice *ThisDevice::instance()
{
    static ThisDevice ins;
    return &ins;
}

ThisDevice::ThisDevice()
{
    this->size.width       = 2.5;
    this->size.height      = 3.0;
    this->size.length      = 10.0;

    this->antenna.to_head  = 10.0;
    this->antenna.to_left  = 1.1;

    this->antenna.to_tail  = this->size.length - this->antenna.to_head;
    this->antenna.to_right = this->size.width  - this->antenna.to_left;


    SceneConfig & c = this->scenes;
    c.fcw           = true;
    c.icw           = true;
    c.lta           = true;
    c.bsw           = true;
    c.dnpw          = true;
    c.ebw           = true;
    c.avw           = true;
    c.clw           = true;
    c.slw           = true;
    c.rlvw          = true;
    c.vrucw         = true;
    c.glosa         = true;
    c.tjw           = true;
    c.evw           = true;
    c.rts           = true;
    c.rte           = true;
}

ThisDevice::~ThisDevice()
{
}

