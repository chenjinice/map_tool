#include "obuudp.h"

#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>
#include <thread>


ObuUdp::ObuUdp(QString ip)
{
    m_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_fd <= 0){
        printf("create udp failed , %s\n",strerror(errno));
        exit(1);
    }
    // 绑定地址,用于接收消息
    struct sockaddr_in this_addr;
    bzero(&this_addr, sizeof(struct sockaddr_in));
    this_addr.sin_family = AF_INET;
    this_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    this_addr.sin_port = htons(port);
    if(bind(m_fd,(struct sockaddr *)&(this_addr), sizeof(struct sockaddr_in)) == -1){
        perror("bind error...");
        exit(1);
    }
}
