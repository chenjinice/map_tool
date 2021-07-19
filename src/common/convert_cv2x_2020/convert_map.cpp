/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#include "convert.h"

using namespace std;

// asn的speedlimits转为本地
static void speedLimitsToLocal(SpeedLimitList_t *limits,LocalSpeedLimit &l_limit)
{
    if(!limits)return;
    int count = limits->list.count;
    int max = kSpeedMax/kResSpeed, min = 0;
    for(int i = 0; i < count; i++){
        RegulatorySpeedLimit_t *p = limits->list.array[i];
        switch (p->type) {
            case SpeedLimitType_maxSpeedInSchoolZone:
            case SpeedLimitType_maxSpeedInSchoolZoneWhenChildrenArePresent:
            case SpeedLimitType_maxSpeedInConstructionZone:
            case SpeedLimitType_vehicleMaxSpeed:
            case SpeedLimitType_vehicleNightMaxSpeed:
            case SpeedLimitType_truckMaxSpeed:
            case SpeedLimitType_truckNightMaxSpeed:
            case SpeedLimitType_vehiclesWithTrailersMaxSpeed:
            case SpeedLimitType_vehiclesWithTrailersNightMaxSpeed:
                if(p->speed < max)max = p->speed;
                break;
            case SpeedLimitType_vehicleMinSpeed:
            case SpeedLimitType_truckMinSpeed:
            case SpeedLimitType_vehiclesWithTrailersMinSpeed:
                if(p->speed > min)min = p->speed;
                break;
            default:
                break;
        }
    }
    l_limit.max = max * kResSpeed;
    l_limit.min = min * kResSpeed;
}

// asn的movement转为本地
static void movementsToLocal(MovementList_t *movements,LocalPhases &l_phase)
{
    if(movements == nullptr)return;

    int count = movements->list.count;
    Movement ** array = movements->list.array;

    // 红绿灯的方向，依次为直行左转右转，超过3个，忽略
    if(count > 0){
        l_phase.straight  = *(array[0]->phaseId);
    }
    if(count > 1){
        l_phase.left      = *(array[1]->phaseId);
    }
    if(count > 2){
        l_phase.right     = *(array[2]->phaseId);
    }
}

// asn的connectsTo的phase id转为本地
static void connectsToLocal(ConnectsToList_t *connects,LocalPhases &l_phase,int road_dir)
{
    if(!connects)return;
    int count       = connects->list.count;
    Connection ** p = connects->list.array;

    if(road_dir & DIR_STRAIGHT){
        if(count > 0 && p[0]->phaseId)l_phase.straight      = *(p[0]->phaseId);
        if(road_dir & DIR_LEFT){
            if(count > 1 && p[1]->phaseId)l_phase.left      = *(p[1]->phaseId);
            if(road_dir & DIR_RIGHT){
                if(count > 2 && p[2]->phaseId)l_phase.right = *(p[2]->phaseId);
            }
        }else if(road_dir & DIR_RIGHT){
            if(count > 1 && p[1]->phaseId)l_phase.right     = *(p[1]->phaseId);
        }
    }else if(road_dir & DIR_LEFT){
        if(count > 0 && p[0]->phaseId)l_phase.left          = *(p[0]->phaseId);
    }else if(road_dir & DIR_RIGHT){
        if(count > 0 && p[0]->phaseId)l_phase.right         = *(p[0]->phaseId);
    }
}

// asn的point转为本地
static void pointsToLocal(PointList_t *points,vector<PosWGS84> &l_points,PosWGS84 &ref_pos)
{
    if(!points)return;
    int count = points->list.count;
    for(int i = 0; i < count; i++){
        RoadPoint_t *p = points->list.array[i];
        PosWGS84 l_p;
        posOffsetToLocal(p->posOffset,l_p,ref_pos);
        l_points.push_back(l_p);
    }
}

// asn的node转为本地
static void nodeToLocal(Node_t *node,LocalNode &l_node)
{
    if(!node)return;
    if(!node->inLinks)return;
    LinkList_t *inlinks = node->inLinks;
    PosWGS84 pos;
    posToLocal(node->refPos,pos);
    nodeIdToLocal(node->id,l_node.id);


    int count = inlinks->list.count;
    for(int i = 0; i < count; i++){
        Link_t *link = inlinks->list.array[i];
        int  link_id = (i+1)*100;    // 本地设置一个link id.不同link的link id设置为不同
        int lane_count = link->lanes.list.count;
        LocalPhases     l_phase;
        LocalSpeedLimit l_speed;
        float           lane_width = 0;
        movementsToLocal(link->movements,l_phase);
        speedLimitsToLocal(link->speedLimits,l_speed);
        lane_width     = link->linkWidth*kResSize;

        // 从lane里的connectsTo获取link的左中右三个灯
        for(int m = 0; m < lane_count; m++){
            Lane_t *lane = link->lanes.list.array[m];
            int road_dir = DIR_NONE;
            bitStringToInt(lane->maneuvers,road_dir);
            connectsToLocal(lane->connectsTo,l_phase,road_dir);
        }
        // lane 转本地
        bool has_valid_lane = false;
        for(int m = 0; m < lane_count; m++){
            Lane_t *lane = link->lanes.list.array[m];
            if(!lane->points)continue;
            if(lane->points->list.count == 0)continue;
            LocalLane l_lane;
            if(lane->laneWidth)lane_width = (*lane->laneWidth)*kResSize;
            nodeIdToLocal(link->upstreamNodeId,l_lane.upstream_id);
            bitStringToInt(lane->maneuvers,l_lane.dir);
            speedLimitsToLocal(lane->speedLimits,l_speed);

            l_lane.link_id      = link_id;
            l_lane.lane_id      = lane->laneID;
            l_lane.lane_width   = lane_width;
            l_lane.speed        = l_speed;
            l_lane.phases       = l_phase;
            pointsToLocal(lane->points,l_lane.points,pos);
            l_node.lanes.push_back(l_lane);
            has_valid_lane = true;
        }
        //如果没有lane，则把link points转本地
        if( (has_valid_lane == false) && (link->points) ){
            LocalLane l_lane;
            nodeIdToLocal(link->upstreamNodeId,l_lane.upstream_id);
            l_lane.link_id      = link_id;
            l_lane.lane_id      = link_id++;
            l_lane.lane_width   = lane_width;
            l_lane.dir          = DIR_STRAIGHT;
            l_lane.phases       = l_phase;
            l_lane.speed        = l_speed;
            pointsToLocal(link->points,l_lane.points,pos);
            l_node.lanes.push_back(l_lane);
        }
    }
}

// asn的map转为本地
bool mapToLocal(const MapData &map, LocalMap &l, const timeval &tv)
{
    l.msg_count                 = map.msgCnt;
    l.tv                        = tv;
    int count = map.nodes.list.count;
    for(int i = 0; i < count; i++){
        Node_t *node = map.nodes.list.array[i];
        LocalNode l_node;
        posToLocal(node->refPos,l_node.pos);
        nodeToLocal(node,l_node);
        l.nodes.push_back(l_node);
    }
    return true;
}


