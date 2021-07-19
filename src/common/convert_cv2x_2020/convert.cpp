/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#include "convert.h"


using namespace std;


// 把一个字节按位倒序
uint8_t byteReverse(uint8_t data) {
    uint8_t i;
    uint8_t value   = 0;
    for(i=0;i<8;i++){
        value       = value << 1;
        value      |= (data >> i)&0x01;
    }
    return value;
}

// bit string 转int,方便理解
bool bitStringToInt(const BIT_STRING_t *p, int &l) {
    if(!p)return true;
    uint8_t tmp[10]    = {0};

    int count = (p->size > 4) ? 4 : p->size ;
    for(int i=0;i<count;i++){
        l |= byteReverse(p->buf[i]) << (8*i);
    }
    return true;
}

// int转bit string
bool intToBitString(int value,BIT_STRING_t * &p,int len,int unused) {
    if(len > 4)return false;
    p = new BIT_STRING_t();
    p->buf  = new uint8_t[len]();
    p->size = len;
    p->bits_unused = unused;
    for(int i=0;i<len;i++){
        uint8_t tmp = ( value >> (i*8) )&0xFF;
        p->buf[i] = byteReverse(tmp);
    }
    return true;
}

// 根据asn某些数据的长度范围，和已有数据的长度，确定实际创建长度
int getCallocLength(int min, int max, int len) {
    // 如果min 比 max 大，调换一下
    if(min > max){
        int tmp = max;
        max     = min;
        min     = tmp;
    }
    if(len > max){
        return max;
    }else if(len < min){
        return min;
    }else{
        return len;
    }
}

// 检查经纬度数字是否合法
bool checkPosValid(const PosWGS84 &p) {
    bool flag = true;
    if( fabs(p.lng) > kLngMax) flag = false;
    if( fabs(p.lat) > kLatMax) flag = false;
    return flag;
}

// description转本地
bool descriptionToLocal(const Description *des, string &l) {
    if(des == nullptr)return false;
    switch (des->present) {
        case Description_PR_textString:
            l.append((char *)des->choice.textString.buf,des->choice.textString.size);
            break;
        case Description_PR_textGB2312:
            l.append((char *)des->choice.textGB2312.buf,des->choice.textGB2312.size);
            break;
        default:
            break;
    }
    return true;
}

// description转asn , optional 类型
bool descriptionToAsn(const string &l,Description * &des,int min,int max) {
    des                 = nullptr;
    int data_len        = l.length();

    if(data_len > 0){
        int calloc_len  = getCallocLength(min,max,data_len);
        int cpy_len     = data_len > calloc_len  ? calloc_len : data_len;

        des                           = new Description();
        des->present                  = Description_PR_textString;
        des->choice.textString.buf    = new uint8_t[calloc_len]();
        des->choice.textString.size   = calloc_len;
        memcpy(des->choice.textString.buf,l.data(),cpy_len);
    }
    return true;
}

// octet string 转本地
bool octetToLocal(const OCTET_STRING *str, string &l) {
    if(str == nullptr)return false;
    l.append((char *)str->buf,str->size);
    return true;
}

// 本地string转 asn 的octet string , 必选类型
bool octetToAsn(const string &l, OCTET_STRING &str, int min, int max ) {
    int data_len    = l.length();
    int calloc_len  = getCallocLength(min,max,data_len);
    int cpy_len     = data_len > calloc_len  ? calloc_len : data_len;

    str.buf        = new uint8_t[calloc_len]();
    str.size       = calloc_len;
    memcpy(str.buf,l.data(),cpy_len);
    return true;
}

// 本地string转 asn 的octet string , optional 类型
bool octetToAsn(const string &l, OCTET_STRING *&str, int min, int max) {
    str                 = nullptr;
    int data_len        = l.length();

    if(data_len > 0){
        int calloc_len  = getCallocLength(min,max,data_len);
        int cpy_len     = data_len > calloc_len  ? calloc_len : data_len;

        str             = new OCTET_STRING_t();
        str->buf        = new uint8_t[calloc_len]();
        str->size       = calloc_len;
        memcpy(str->buf,l.data(),cpy_len);
    }
    return true;
}

// asn的nodeId转为本地
bool nodeIdToLocal(const NodeReferenceID &id , LocalNodeId &l) {
    l.id            = id.id;
    if(id.region){
        l.region    = *id.region;
    }
    return true;
}

// nodeId转为asn
bool nodeIdToAsn(const LocalNodeId &l , NodeReferenceID &id, bool has_region) {
    id.id               = l.id;
    if(has_region){
        id.region       = new RoadRegulatorID_t(l.region);
    }
    return true;
}

// asn的经纬度点转本地
bool posToLocal(const Position3D &pos, PosWGS84 &l) {
    l.lng           = pos.Long * kResPos;
    l.lat           = pos.lat  * kResPos;
    if(pos.elevation){
        l.elev      = *(pos.elevation) * kResElev;
    }
    return true;
}

// 本地经纬度点转asn
bool posToAsn(const PosWGS84 &l, Position3D &p, bool has_elev) {
    p.Long            = l.lng / kResPos;
    p.lat             = l.lat / kResPos;
    if(has_elev){
        p.elevation   = new Elevation_t(l.elev / kResElev);
    }
    return true;
}

// asn的物体尺寸转本地
bool sizeTolocal(const VehicleSize &size, ObjSize &l) {
    l.width         = size.width * kResSize;
    l.length        = size.length * kResSize;
    if(size.height){
        l.height    = *(size.height) * kResHeight;
    }
    return true;
}

// 本地物体尺寸转asn
bool sizeToAsn(const ObjSize &l, VehicleSize &size, bool has_height) {
    size.width              = l.width / kResSize;
    size.length             = l.length / kResSize;
    if(has_height){
        size.height  = new VehicleHeight_t(l.height / kResHeight);
    }
    return true;
}

// 红绿灯颜色转本地
bool lightColorToLocal(LightState_t light, LocalLightColor &l) {
    switch (light) {
        case LightState_flashing_red:
        case LightState_red:
            l = LIGHT_RED;
            break;
        case LightState_flashing_green:
        case LightState_permissive_green:
        case LightState_protected_green:
            l = LIGHT_GREEN;
            break;
        case LightState_yellow:
            l = LIGHT_YELLOW;
            break;
        case LightState_flashing_yellow:
            l = LIGHT_YELLOW_FLASH;
            break;
        default:
            l = LIGHT_NONE;
            break;
    }
    return  true;
}

// 红绿灯颜色转asn
bool lightColorToAsn(LocalLightColor l, LightState_t &light) {
    switch (l) {
        case LIGHT_RED :
            light = LightState_red;
            break;
        case LIGHT_GREEN :
            light = LightState_permissive_green;
            break;
        case LIGHT_YELLOW :
            light = LightState_yellow;
            break;
        case LIGHT_YELLOW_FLASH:
            light = LightState_flashing_yellow;
            break;
        default:
            light = LightState_unavailable;
            break;
    }
    return  true;
}

// 根据参考点和偏移值，转为本地经纬度点
bool posOffsetToLocal(const PositionOffsetLLV &offset, PosWGS84 &l, PosWGS84 &ref_pos) {
    bool full_latlon = false;
    long tmp_lng = 0, tmp_lat = 0;
    switch (offset.offsetLL.present) {
        case PositionOffsetLL_PR_position_LL1:
            tmp_lng = offset.offsetLL.choice.position_LL1.lon;
            tmp_lat = offset.offsetLL.choice.position_LL1.lat;
            break;
        case PositionOffsetLL_PR_position_LL2:
            tmp_lng = offset.offsetLL.choice.position_LL2.lon;
            tmp_lat = offset.offsetLL.choice.position_LL2.lat;
            break;
        case PositionOffsetLL_PR_position_LL3:
            tmp_lng = offset.offsetLL.choice.position_LL3.lon;
            tmp_lat = offset.offsetLL.choice.position_LL3.lat;
            break;
        case PositionOffsetLL_PR_position_LL4:
            tmp_lng = offset.offsetLL.choice.position_LL4.lon;
            tmp_lat = offset.offsetLL.choice.position_LL4.lat;
            break;
        case PositionOffsetLL_PR_position_LL5:
            tmp_lng = offset.offsetLL.choice.position_LL5.lon;
            tmp_lat = offset.offsetLL.choice.position_LL5.lat;
            break;
        case PositionOffsetLL_PR_position_LL6:
            tmp_lng = offset.offsetLL.choice.position_LL6.lon;
            tmp_lat = offset.offsetLL.choice.position_LL6.lat;
            break;
        case PositionOffsetLL_PR_position_LatLon:
            full_latlon = true;
            tmp_lng = offset.offsetLL.choice.position_LatLon.lon;
            tmp_lat = offset.offsetLL.choice.position_LatLon.lat;
            break;
        default:
            break;
    }
    if(full_latlon){
        l.lng = tmp_lng * kResPos;
        l.lat = tmp_lat * kResPos;
    }else{
        l.lng = tmp_lng * kResPos + ref_pos.lng;
        l.lat = tmp_lat * kResPos + ref_pos.lat;
    }
    return true;
}

// 把经纬度转化为asn偏移值，需要参考点经纬度
// full  , true  代表不与参考点计算偏移值，填绝对经纬度值
//         false 代表填相对参考点的偏移值
bool posOffsetToAsn(const PosWGS84 &l, PositionOffsetLLV &offset, const PosWGS84 &ref_pos, bool full) {
    static long s_max_diff  = kLngMax * kResPos;
    static long s_max[OFFSET_COUNT] = {2047,8191,32767,131071,2097151,8388607,s_max_diff};

    if( !checkPosValid(l) || !checkPosValid(ref_pos) ){
        conLog("posOffsetToAsn  error, pos out of range : pos(%lf,%lf),ref_pos(%lf,%lf)\n", l.lng,l.lat,ref_pos.lng,ref_pos.lat);
        offset.offsetLL.present = PositionOffsetLL_PR_position_LL1;
        return false;
    }

    long  offset_lng = 0 , offset_lat = 0;
    PositionOffsetLL_PR present = PositionOffsetLL_PR_position_LatLon;
    if(full){
        offset_lng = l.lng / kResPos;
        offset_lat = l.lat / kResPos;
    }else{
        // 和参考点经纬度差值，限定在 -180 到 180
        double tmp_lng = l.lng - ref_pos.lng;
        double tmp_lat = l.lat - ref_pos.lat;
        if(tmp_lng > kLngMax )tmp_lng -= 2*kLngMax;
        if(tmp_lng < -kLngMax)tmp_lng += 2*kLngMax;
        offset_lng = tmp_lng / kResPos;
        offset_lat = tmp_lat / kResPos;
        long abs_lng = abs(offset_lng);
        long abs_lat = abs(offset_lat);
        long abs_max = abs_lng > abs_lat ? abs_lng : abs_lat;
        for(int i=0;i<OFFSET_COUNT;i++){
            if(abs_max <= s_max[i]){ present = (PositionOffsetLL_PR)(i+1); break; }
        }
    }
    offset.offsetLL.present = present;
    switch (present) {
        case PositionOffsetLL_PR_position_LL1:
            offset.offsetLL.choice.position_LL1.lon = offset_lng;
            offset.offsetLL.choice.position_LL1.lat = offset_lat;
            break;
        case PositionOffsetLL_PR_position_LL2:
            offset.offsetLL.choice.position_LL2.lon = offset_lng;
            offset.offsetLL.choice.position_LL2.lat = offset_lat;
            break;
        case PositionOffsetLL_PR_position_LL3:
            offset.offsetLL.choice.position_LL3.lon = offset_lng;
            offset.offsetLL.choice.position_LL3.lat = offset_lat;
            break;
        case PositionOffsetLL_PR_position_LL4:
            offset.offsetLL.choice.position_LL4.lon = offset_lng;
            offset.offsetLL.choice.position_LL4.lat = offset_lat;
            break;
        case PositionOffsetLL_PR_position_LL5:
            offset.offsetLL.choice.position_LL5.lon = offset_lng;
            offset.offsetLL.choice.position_LL5.lat = offset_lat;
            break;
        case PositionOffsetLL_PR_position_LL6:
            offset.offsetLL.choice.position_LL6.lon = offset_lng;
            offset.offsetLL.choice.position_LL6.lat = offset_lat;
            break;
        case PositionOffsetLL_PR_position_LatLon:
            offset.offsetLL.choice.position_LatLon.lon = offset_lng;
            offset.offsetLL.choice.position_LatLon.lat = offset_lat;
            break;
        default:
            break;
    }
    return true;
}

// 把经纬度转化为asn偏移值，且转化为绝对经纬度值
bool posOffsetToAsnLatLon(const PosWGS84 &l, PositionOffsetLLV &offset) {
    offset.offsetLL.present = PositionOffsetLL_PR_position_LatLon;
    offset.offsetLL.choice.position_LatLon.lon = l.lng / kResPos;
    offset.offsetLL.choice.position_LatLon.lat = l.lat / kResPos;
    return true;
}
