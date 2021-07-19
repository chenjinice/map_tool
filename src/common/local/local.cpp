/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#include "local.h"


// 度数转弧度
double degreeToRadian(double degree)
{
    return degree*M_PI/180;
}

// 经纬度点转为地心坐标系XYZ
void WGS84To3D(double lng,double lat,Point3D &p)
{
    double r_lng = degreeToRadian(lng);
    double r_lat = degreeToRadian(lat);
    p.x = cos(r_lat)*cos(r_lng)*kEarthRadius;
    p.y = cos(r_lat)*sin(r_lng)*kEarthRadius;
    p.z = sin(r_lat)*kEarthRadius;
}

// 2维点p绕点reference(参考点)逆时针旋转degree度数后的坐标值
void rotate2D(Point2D &reference, Point2D &p, double degree)
{
    double radian = degreeToRadian(degree);
    double cos_value = cos(radian);
    double sin_value = sin(radian);
    double dx = p.x - reference.x;
    double dy = p.y - reference.y;
    double x = cos_value*dx - sin_value*dy + reference.x;
    double y = sin_value*dx + cos_value*dy + reference.y;
    p.x = x;
    p.y = y;
}

// 两个经纬度点之间求距离(Haversine公式)
double getDistance(const PosWGS84 &p1,const PosWGS84 &p2){
    double r_lng1 = degreeToRadian(p1.lng);
    double r_lat1 = degreeToRadian(p1.lat);
    double r_lng2 = degreeToRadian(p2.lng);
    double r_lat2 = degreeToRadian(p2.lat);

    double d_lng = r_lng2 - r_lng1;
    double d_lat = r_lat2 - r_lat1;

    double tmp = sin(d_lat/2)*sin(d_lat/2) + cos(r_lat1)*cos(r_lat2)*sin(d_lng/2)*sin(d_lng/2);
    double value = 2*asin(sqrt(tmp))*kEarthRadius;
    return value;
}

// 两个经纬度点求朝向角(东北天坐标系，正北为0度，顺时针0～360度)
// 方向 : p1为原点，p1指向p2的方向
double getRoadAngle(const PosWGS84 &p1,const PosWGS84 &p2)
{
    double lng_a = degreeToRadian(p1.lng);
    double lat_a = degreeToRadian(p1.lat);
    double lng_b = degreeToRadian(p2.lng);
    double lat_b = degreeToRadian(p2.lat);
    double y = sin(lng_b-lng_a) * cos(lat_b);
    double x = cos(lat_a)*sin(lat_b) - sin(lat_a)*cos(lat_b)*cos(lng_b-lng_a);
    double angle   = atan2(y,x)/M_PI*180.0;

    if(angle < 0)angle += 360.0;
    return angle;
}

// 计算两个(东北天坐标系,0~360)角度的差值 (angle2 - angle1)
// (这里保证输入数据为 0～360以内,函数内部没做转换)
// 结果为 -180 到 180 之间的值
double getAngleDiff(double angle1,double angle2)
{
    double tmp = angle2 - angle1;
    if(tmp > 180.0){
        tmp -= 360.0;
    }else if(tmp < -180.0){
        tmp += 360;
    }
    return tmp;
}

// 经纬度点p到线段ab的最短距离计算-矢量法
// flag会被设置为： 0(p到ab有垂线),-1(p在向量ab后方)，-2(p在向量ab前方)
double minDistance(const PosWGS84 &p ,const PosWGS84 &a,const PosWGS84 &b, int *flag){
    if(flag)*flag = 0;
    Point3D op, oa , ob;
    WGS84To3D(p.lng,p.lat,op);
    WGS84To3D(a.lng,a.lat,oa);
    WGS84To3D(b.lng,b.lat,ob);

    // ap与ab向量乘积为负数，说明角pab为钝角，p在点a的外侧，最短距离为ap的长度
    double cross = (op.x-oa.x)*(ob.x-oa.x) + (op.y-oa.y)*(ob.y-oa.y) + (op.z-oa.z)*(ob.z-oa.z);
    if(cross <= 0){
        if( (cross < 0) && (flag) )*flag = -1;
        return sqrt( (op.x-oa.x)*(op.x-oa.x) +(op.y-oa.y)*(op.y-oa.y) + (op.z-oa.z)*(op.z-oa.z) );
    }

    // ap与ab向量乘积大于ab的平方，说明ap在ab上的投影超过ab长度，则p在b的外侧，最短距离为bp的长度
    double ab = (oa.x-ob.x)*(oa.x-ob.x) + (oa.y-ob.y)*(oa.y-ob.y) + (oa.z-ob.z)*(oa.z-ob.z);
    if(cross >= ab){
        if( (cross > ab) && (flag) )*flag = -2;
        return sqrt( (op.x-ob.x)*(op.x-ob.x) +(op.y-ob.y)*(op.y-ob.y) + (op.z-ob.z)*(op.z-ob.z) );
    }

    // 求ap到ab上垂线长度
    double r = cross/ab;
    double dx = oa.x + r*(ob.x-oa.x);
    double dy = oa.y + r*(ob.y-oa.y);
    double dz = oa.z + r*(ob.z-oa.z);
    return sqrt( (op.x-dx)*(op.x-dx) + (op.y-dy)*(op.y-dy) + (op.z-dz)*(op.z-dz) );
}

/*
 * 经纬度点p到线段ab的最短垂直距离,没有垂线的话，返回 DIST_MAX
 */
double minVerticalDist(const PosWGS84 &p, const PosWGS84 &a, const PosWGS84 &b)
{
    int flag;
    double ret  = kDistMax;
    double dist = minDistance(p,a,b,&flag);
    if(flag == 0){
        ret = dist;
    }
    return ret;
}

// 根据p1的位置和方位角，求p2在p1的哪个位置, pos.h为左右距离，pos.v为前后距离,dist为总距离
void getRelativePos(const PosWGS84 &p1,const PosWGS84 &p2, double p1_degree, RelativePos &p)
{
    double x1 = 110995.645 * p1.lat;
    double y1 = 88477.308  * p1.lng;

    double x2 = 110995.645 * p2.lat;
    double y2 = 88477.308  * p2.lng;

    double angle = p1_degree;
    double temp_angle=0.0;
    if((angle>=45.0)&&(angle<135.0))
    {
        temp_angle=(angle-45.0)/10.5;
        angle -= 4.5;
        angle += temp_angle;
    }
    else if((angle>=225.0)&&(angle<315.0))
    {
        temp_angle=(angle-225.0)/10.5;
        angle -= 4.5;
        angle += temp_angle;
    }
    else if((angle>=135.0)&&(angle<225.0))
    {
        temp_angle=(angle-135.0)/10.5;
        angle += 4.5;
        angle -= temp_angle;
    }
    else if(angle<45.0)
    {
        angle -= angle/10.5;
    }
    else
    {
        temp_angle=(angle-315.0)/10.5;
        angle += 4.5;
        angle -= temp_angle;
    }
    double tmp_ang = M_PI * angle / 180.0;
    p.v = (x2-x1)*cos(tmp_ang) + (y2-y1)*sin(tmp_ang);
    p.h = (y2-y1)*cos(tmp_ang) - (x2-x1)*sin(tmp_ang);
    p.dist = getDistance(p1,p2);

//    printf("%.02lf:h = %.02lf,v = %.02lf,dist=%lf\n",p1_degree,pos.h,pos.v,pos.dist);
}

// 根据本车gps天线的位置，转化相对位置,p.dist不会变,ant=antenna(天线)
void getAntBasePos(const AntennaPos &ant,const RelativePos &p, RelativePos &ant_p)
{
    if( (p.h >= -ant.to_left) && (p.h <= ant.to_right) ){
        ant_p.h = 0;
    }else if( p.h < -ant.to_left ){
        ant_p.h = p.h + ant.to_left;
    }else if(p.h > ant.to_right) {
        ant_p.h = p.h - ant.to_right;
    }

    if( (p.v >= -ant.to_tail) && (p.v <= ant.to_head) ){
        ant_p.v = 0;
    }else if( p.v < -ant.to_tail ){
        ant_p.v = p.v + ant.to_tail;
    }else if(p.v > ant.to_head) {
        ant_p.v = p.v - ant.to_head;
    }

    ant_p.dist = p.dist;
}




