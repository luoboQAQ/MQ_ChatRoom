#include "mq.h"

//打开指定的消息队列
int Open_MQ(int MQID)
{
    int TempMq;
    TempMq = msgget(MQID, IPC_CREAT | 0666);
    if (TempMq == -1)
    {
        perror("Open_MQ");
        exit(1);
    }
    return TempMq;
}

//储存客户端pid
void Store_Private_MQ_Pid(void)
{
    Client_PID_Box[Client_Number] = Client_to_Server.client_pid;
    printf("The %d has been recorded\n", Client_PID_Box[Client_Number]);
    Client_Number++;
    printf("Client Number is : %d\n", Client_Number);
}

//删除客户端信息
void Delete_Client_Data(void)
{
    int local;
    int count;

    for (local = 0; local < Client_Number; local++)
    {
        if (Client_to_Server.client_pid == Client_PID_Box[local])
            break;
    }
    for (count = local; count < Client_Number; count++)
        Client_PID_Box[count] = Client_PID_Box[count + 1];
}