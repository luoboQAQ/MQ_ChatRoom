#ifndef __MYMQ__
#define __MYMQ__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#define IS_PARENT 1                  //是否为父进程
#define PUBLIC_MQ 10                 //公共消息队列编号
#define MAX_Client_Number 6          //客户端数量的最大值
#define MAX_CLIENT_NAME_LEN 30       //客户端姓名的最大长度
#define DEFAULT_MSG 1                //默认消息
#define IS_NEW_CLIENT 2              //客户端上线消息
#define CLIENT_QUIT 3                //客户端下线消息
#define CLIENT_QUIT_TEXT "quit\n"    //用户退出命令
#define BROADCAST_TO_ALL "Broadcast" //群聊消息的标记
#define PRIVATE_MSG_HEADER "@"       //私聊消息的关键字

struct msgbuf
{
    long mtype;                            //消息编号
    int client_pid;                        //客户端pid
    char client_name[MAX_CLIENT_NAME_LEN]; //客户端姓名
    char target_name[MAX_CLIENT_NAME_LEN]; //客户端目标姓名
    char mtext[100];                       //消息内容
};

int Open_MQ(int MQID);
void Store_Private_MQ_Name(void);
void Delete_Client_Data(void);
void Server_Send_Message(void);
void Client_Read_Data(void);
void Show_Local_Time(void);
void Private_Chat_Filter_By_Name(char *Client_Message);
void Client_Write_Data(int Process, int Child_PID);
void Server_Sigcatch(int num);

int New_Client_Flag;
int Quit_Flag;
int Client_Number;
int PublicMQ, PrivateMQ;
int mypid;
struct msgbuf Client_to_Server, Server_to_Client;
int Client_PID_Box[MAX_Client_Number];
char Client_Name_Box[MAX_Client_Number][MAX_CLIENT_NAME_LEN];

#endif //__MYMQ__