/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */

#ifndef __MY_COMMON_H__
#define __MY_COMMON_H__


#include <stdio.h>
#include <sys/time.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>
#include <list>

#include <QDebug>

/*
#define myLogInit                  g_log.init
#define myError(format,...)        g_log.saveLog(L_ERROR,format,##__VA_ARGS__)
#define myWarn(format,...)         g_log.saveLog(L_WARN,format,##__VA_ARGS__)
#define myInfo(format,...)         g_log.saveLog(L_INFO,format,##__VA_ARGS__)
#define myDebug(format,...)        g_log.saveLog(L_DEBUG,format,##__VA_ARGS__)
#define myLogEnd                   g_log.end
*/


#define myLogInit                  printf
#define myError(format,...)        printf
#define myWarn(format,...)         printf
#define myInfo(format,...)         printf
#define myDebug(format,...)        printf
#define myLogEnd             


// fprintf缓存区大小
const int kIoBufSize             = 1024;
// 临时字符串大小
const int kLogBufSize            = 2048;


/*----------------保存日志的类----开始-------------------------*/
// 日志级别
typedef enum{
    L_NONE      = 0,
    L_ERROR     = 1,
    L_WARN      = 2,
    L_INFO      = 3,
    L_DEBUG     = 4,
}MyLogLevel;

class MyLog
{
public:
    MyLog();
    ~MyLog();
    void                    init(std::string f, int s=100, int l=L_INFO, int buff_size=kLogBufSize);
    void                    end();
    void                    openFile();
    FILE *                  fp();
    void                    getLogPre(char *buffer,std::string str);
    void                    saveLog(MyLogLevel level, const char *format,...);
    void                    saveAsn(uint8_t *buffer, int len);
    void                    checkFileSize();

private:
    std::string             m_file           = "cidi.log";
    std::mutex              m_mutex;
    long                    m_rotate_size    = 100*1024*1024;
    FILE *                  m_fp             = nullptr;
    uint32_t                m_count          = 0;
    char *                  m_buffer         = nullptr;
    MyLogLevel              m_level          = L_INFO;
    int                     m_buff_size      = kLogBufSize;
    bool                    m_print_flag     = false;
};

// 引用全局变量
extern MyLog g_log;
/*----------------保存日志的类----结束-------------------------*/




/*----------------测试两段代码之间运行时间的类----开始-------------------------*/
class MyTest
{
public:
    MyTest();

    void start();
    void end();
    void clear();

private:
    timeval     m_start   = {0};
    timeval     m_end     = {0};

    uint64_t    m_all     = 0;
    uint64_t    m_count   = 0;
};
/*----------------测试两段代码之间运行时间的类----结束-------------------------*/




/*------------------------ 其他 -------------------------*/
uint64_t    getMsNow();
int         getInterval(const timeval &tv1, const timeval &tv2);
int64_t     getIntervalToNow(const timeval &tv);
bool        checkInterval(timeval &tv,int ms);

double      eastNorthToLocal(double radius);
double      localToEastNorth(double degree);

bool        readFile(std::string path, std::string &str);
bool        writeToFile(std::string path,std::string &str);
bool        writeToFile(std::string path, uint8_t *str, int len);

// 获取编译的时间
std::string getCompileTime();



#endif



