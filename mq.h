#ifndef __MYMQ__
#define __MYMQ__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define PUBLIC_MQ 10
#define MAX_Client_Number 6
#define MAX_CLIENT_NAME_LEN 30
#define IS_NEW_CLIENT 1
#define CLIENT_QUIT 2

struct msgbuf
{
    long mtype;                            //消息编号
    int client_pid;                        //客户端pid
    char client_name[MAX_CLIENT_NAME_LEN]; //客户端姓名
    char target_name[MAX_CLIENT_NAME_LEN]; //客户端目标姓名
    char mtext[256];                       //消息内容
};

int Open_MQ(int MQID);
void Store_Private_MQ_Pid(void);
void Delete_Client_Data(void);

int New_Client_Flag;
int Quit_Flag;
int Client_Number;
int PublicMQ;
struct msgbuf Client_to_Server;
int Client_PID_Box[MAX_Client_Number];

#endif //__MYMQ__