#include "mq.h"

int main()
{
    Client_Number = 0;
    New_Client_Flag = 0;

    if (signal(SIGINT, &Server_Sigcatch) == SIG_ERR)
    {
        perror("signal");
        exit(1);
    }

    //打开公共消息队列
    PublicMQ = Open_MQ(PUBLIC_MQ);

    while (1)
    {
        if (msgrcv(PublicMQ, &Client_to_Server, sizeof(Client_to_Server), 0, MSG_NOERROR | IPC_NOWAIT) != -1)
        {
            printf("Client Pid is : %d\n", Client_to_Server.client_pid);
            printf("Client Message is : %s", Client_to_Server.mtext);

            //记录新加入的客户端
            if (Client_to_Server.mtype == IS_NEW_CLIENT)
            {
                printf("This is a new client!\n");
                New_Client_Flag = 1;
                Store_Private_MQ_Name();
            }

            //如果客户端退出，则删除消息队列
            if (Client_to_Server.mtype == CLIENT_QUIT)
            {
                msgctl(Client_to_Server.client_pid, IPC_RMID, NULL); //删除消息队列
                Delete_Client_Data();
                Client_Number--;
                Quit_Flag = 1;
                printf("Closed Client_%d Private MQ\n", Client_to_Server.client_pid);
                printf("Client Number is : %d\n\n", Client_Number);
            }

            //由服务端向客户端发送消息
            Server_Send_Message();
        }
        else
        {
            if (errno != ENOMSG)
            {
                perror("msgrcv");
                exit(1);
            }
        }
    }
    return 0;
}