#include "mq.h"

int main()
{
    pid_t pid;
    mypid = getpid();
    //打印欢迎信息
    printf("\n🎉 欢迎来到MQ聊天室 🎉\n\n请输入您的用户名: ");
    scanf("%s", Client_to_Server.client_name);
    getchar();
    Client_to_Server.client_pid = mypid;
    Client_to_Server.mtype = IS_NEW_CLIENT;
    strcpy(Client_to_Server.target_name, BROADCAST_TO_ALL);

    //客户端和服务器握手
    Client_Write_Data(!IS_PARENT, -1);
    Client_Read_Data();
    Client_to_Server.mtype = DEFAULT_MSG;

    //fork出两个进程来保持连接
    if ((pid = fork()) < 0)
    {
        perror("fork");
        exit(1);
    }
    //父进程用于发送信息
    else if (pid > 0)
    {
        while (1)
        {
            fgets(Client_to_Server.mtext, 60, stdin);
            Client_to_Server.client_pid = getpid();
            Private_Chat_Filter_By_Name(Client_to_Server.mtext);
            Client_Write_Data(IS_PARENT, pid);
        }
    }
    //子进程用于接收消息
    else
    {
        while (1)
        {
            Client_Read_Data();
        }
    }
    return 0;
}