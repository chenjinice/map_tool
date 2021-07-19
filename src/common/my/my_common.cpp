/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */


#include "my_common.h"

#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>


using namespace std;


/*----------------保存日志的类----开始-------------------------*/
// 定义一个全局变量
MyLog g_log;

MyLog::MyLog()
{
    if(tcgetpgrp(0) == getpgid(0)){
        m_print_flag = true;    // 程序是否在前台运行
    }
}

MyLog::~MyLog()
{
    this->end();
}

/* 初始化
 * f  : log保存路径
 * s  : log保存大小,M，超过这个数，会轮换，最多两个文件
 * l  : log保存级别
 */
void MyLog::init(string f, int s, int l,int buff_size)
{
    m_file        = f;
    m_rotate_size = s * 1024 * 1024;
    m_level       = (MyLogLevel)l;
    m_buff_size     = buff_size;
    printf("MyLog : file=%s,size=%d M,level=%d,buff_size=%d\n",f.c_str(),s,m_level,buff_size);

    if(m_fp == nullptr){
        openFile();
    }
}

void MyLog::end()
{
    if(m_fp){
        fflush(m_fp);
        fclose(m_fp);
        m_fp = nullptr;
    }
    if(m_buffer){
        delete m_buffer;
    }
}

void MyLog::openFile()
{
    m_buffer    = new char[kIoBufSize];
    m_fp        = fopen(m_file.c_str(),"a");
    if(m_fp == nullptr){
        printf("MyLog init error : open %s failed\n",m_file.c_str());
    }else{
        int fd = fileno(m_fp);
        fchmod(fd, 0666);
        setvbuf (m_fp,m_buffer,_IOFBF,kIoBufSize); // 设置缓存区大小
    }
}

FILE *MyLog::fp()
{
    return m_fp;
}

// 设置log前缀
void MyLog::getLogPre(char *buffer, string str)
{
    time_t timep;
    struct tm *p;
    time (&timep);
    p=localtime(&timep);
    sprintf(buffer,"[%d-%02d-%02d %02d:%02d:%02d %s]<%d> ",
            p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec,str.c_str(),m_count++);
}

// 保存log
void MyLog::saveLog(MyLogLevel level, const char *format,...)
{
    static string s_array[5]      = {"","Error","Warning","Info","Debug"};

    if(level > m_level)return;
    if(m_fp == nullptr)return;

    string str = s_array[level];
    char *buffer = new char[m_buff_size];
    getLogPre(buffer,str);
    va_list ap;
    va_start(ap,format);
    vsprintf(buffer+strlen(buffer),format,ap);
    va_end(ap);

    if(m_print_flag){
        printf("%s",buffer);
    }

    m_mutex.lock();
    fprintf(m_fp,"%s", buffer);
//    fflush(m_fp);
    checkFileSize();
    m_mutex.unlock();
    delete[] buffer;
}

void MyLog::saveAsn(uint8_t *buffer,int len)
{
    char *str = new char[m_buff_size];
    getLogPre(str,"ASN");
    int str_len = strlen(str);
    sprintf(str+str_len,"↓ len=%d,data={",len);
    str_len = strlen(str);
    for(int i=0;i<len;i++){
        if(i == len-1)sprintf(str+str_len,"0x%02x}",buffer[i]);
        else          sprintf(str+str_len,"0x%02x,",buffer[i]);
        str_len += 5;
    }
    sprintf(str+str_len,"\n");

    m_mutex.lock();
    fprintf(m_fp,"%s", str);
    checkFileSize();
    m_mutex.unlock();
    delete[] str;
}

// 检查日志文件大小
void MyLog::checkFileSize()
{
    long file_size = ftell(m_fp);
    if(file_size > m_rotate_size){
        fflush(m_fp);
        fclose(m_fp);
        m_fp = nullptr;

        string log_file = m_file + "__old";
        remove(log_file.c_str());
        rename(m_file.c_str(),log_file.c_str());

        openFile();
    }
}
/*----------------保存日志的类----结束-------------------------*/






/*----------------测试两段代码之间运行时间的类----开始-------------------------*/
MyTest::MyTest()
{
}

// 开始计时
void MyTest::start()
{
    gettimeofday(&m_start,nullptr);
}

// 结束计时,打印间隔
void MyTest::end()
{
    gettimeofday(&m_end,nullptr);
    int64_t t = (int64_t)(m_end.tv_sec-m_start.tv_sec)*1000000 + (int64_t)(m_end.tv_usec-m_start.tv_usec);
    m_all += t;
    m_count++;
    double avg = 1.0*m_all/m_count;
    printf("==%llu== avg:%.02f , us:%lld\n",m_count,avg,t);
}

void MyTest::clear()
{
    m_start = {0};
    m_end   = {0};
    m_all   = 0;
    m_count = 0;
}
/*----------------测试两段代码之间运行时间的类----结束-------------------------*/




// 获取当前时间戳，精确到毫秒
uint64_t getMsNow()
{
    timeval now;
    gettimeofday(&now,nullptr);
    uint64_t ret = (uint64_t)now.tv_sec*1000 + (uint64_t)now.tv_usec/1000;
    return   ret;
}

//获取两个时间戳之间差值 ,毫秒
int getInterval(const timeval &tv1, const timeval &tv2)
{
    int ret = (tv1.tv_sec-tv2.tv_sec)*1000 + (tv1.tv_usec-tv2.tv_usec)/1000;
    return ret;
}

// 获取于当前时间戳差值 ,毫秒
int64_t getIntervalToNow(const timeval &tv)
{
    timeval now;
    gettimeofday(&now,nullptr);
    int64_t ret = (int64_t)(now.tv_sec - tv.tv_sec)*1000 + (int64_t)(now.tv_usec-tv.tv_usec)/1000;
    return ret;
}


// 检查与当前时间差是否超过某个值,tv : 需要与当前时间做比较的时间,ms : 时间差是否超过ms（毫秒）
// 返回值 false : 时间差小于 ms,
//       true : 时间差大于等于 ms ,此时更新参数 tv 的时间为当前时间
bool checkInterval(timeval &tv,int ms)
{
    if((tv.tv_sec == 0) && (tv.tv_usec == 0)){
        gettimeofday(&tv,nullptr);
        return true;
    }

    timeval now = {0};
    gettimeofday(&now,nullptr);
    int64_t interval = ((int64_t)now.tv_sec - (int64_t)tv.tv_sec)*1000 + (now.tv_usec-tv.tv_usec)/1000;
    if( (interval >= ms ) || (interval < 0) ){
        gettimeofday(&tv,nullptr);
        return true;
    }else{
        return false;
    }
}


// 自驾那边的东北天航向角，转obu的北东地航向角0~360
double eastNorthToLocal(double radius)
{
    double r = -atan2(sin(radius),cos(radius)); // 东北天逆时针为正，北东地顺时针为正
    r += M_PI/2;       // 东北天正东为0，转为北东地的正北为0
    r = r*180/M_PI;    // 转度数
    if(r < 0)r+=360;   // 转为 0~360 这种
    return r;
}

// 将0~360的北东地航向角，转为东北天
double localToEastNorth(double degree)
{
    degree -= 90;
    if(degree >= 180)degree= degree-360;
    double r = -degree*M_PI/180;
    return r;
}

// 读取文件
bool readFile(std::string path, std::string &str)
{
    ifstream in(path);
    if(!in.is_open()){
        return false;
    }
    ostringstream tmp;
    tmp << in.rdbuf();
    str = tmp.str();
    return true;
}

// 写入文件
bool writeToFile(string path, string &str)
{
    ofstream outFile(path, ios::out | ios::binary);
    outFile.write(str.c_str(),str.length());
    outFile.close();
    return true;
}

bool writeToFile(string path, uint8_t *str, int len)
{
    ofstream outFile(path, ios::out | ios::binary);
    outFile.write((const char *)str,len);
    outFile.close();
    return true;
}

// 获取编译时间
string getCompileTime(){
    string str;
    int year = 0, month = 0 ,day = 0;
    const char *pMonth[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    const char date[12] = __DATE__; // 取编译时间
    for(int i = 0; i < 12; i++){
        if(memcmp(date, pMonth[i], 3) == 0){
            month = i + 1;
            break;
        }
    }
    year = atoi(date + 7);
    day  = atoi(date + 4);

    char buffer[100] ={0};
    sprintf(buffer,"%04d.%02d.%02d_%s",year,month,day,__TIME__);
    str = buffer;
    return str;
}

