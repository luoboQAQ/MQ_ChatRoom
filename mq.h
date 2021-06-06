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

#define IS_PARENT 1
#define PUBLIC_MQ 10
#define MAX_Client_Number 6
#define MAX_CLIENT_NAME_LEN 30
#define DEFAULT_MSG 1
#define IS_NEW_CLIENT 2
#define CLIENT_QUIT 3
#define BROADCAST_TO_ALL "Broadcast"
#define PRIVATE_MSG_HEADER "@"

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