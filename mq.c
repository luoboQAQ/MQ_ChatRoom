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

//储存客户端信息
void Store_Private_MQ_Name(void)
{
    Client_PID_Box[Client_Number] = Client_to_Server.client_pid;
    strcpy(Client_Name_Box[Client_Number], Client_to_Server.client_name);
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
    {
        Client_PID_Box[count] = Client_PID_Box[count + 1];
        strcpy(Client_Name_Box[count], Client_Name_Box[count + 1]);
    }
}

//服务端发送私有消息或共有消息
void Server_Send_Message(void)
{
    int count;
    int target_count;
    int source_count;

    printf("Client Target is: %s\n", Client_to_Server.target_name);
    Server_to_Client.mtype = DEFAULT_MSG;

    //私聊
    if (strcmp(Client_to_Server.target_name, BROADCAST_TO_ALL) != 0)
    {

        //找到接收者客户端
        for (target_count = 0; target_count < Client_Number; target_count++)
        {
            if (strcmp(Client_to_Server.target_name, Client_Name_Box[target_count]) == 0)
                break;
        }
        if (target_count >= Client_Number)
        {
            printf("[Warning]Target Client not found!\n");
            return;
        }
        //找到发送者客户端
        for (source_count = 0; source_count < Client_Number; source_count++)
        {
            if (Client_to_Server.client_pid == Client_PID_Box[source_count])
                break;
        }

        //发送私聊消息
        printf("Private MQ Pid : %d\n", Client_PID_Box[target_count]);
        PrivateMQ = Open_MQ(Client_PID_Box[target_count]);
        Server_to_Client.client_pid = Client_to_Server.client_pid;
        sprintf(Server_to_Client.mtext, "[%s] said to you : ", Client_Name_Box[source_count]);
        strcat(Server_to_Client.mtext, Client_to_Server.mtext);
        if ((msgsnd(PrivateMQ, &Server_to_Client, sizeof(Server_to_Client), IPC_NOWAIT)) != -1)
            printf("Write message to Client_%d Success!\n", Client_PID_Box[target_count]);
    }

    //群聊
    else
    {
        //筛选消息类型
        if (New_Client_Flag)
        {
            sprintf(Server_to_Client.mtext, "👦 [%s] 进入聊天室 \n", Client_to_Server.client_name);
            New_Client_Flag = 0;
        }
        else if (Quit_Flag)
        {
            sprintf(Server_to_Client.mtext, "💀 [%s] 退出了聊天室 \n", Client_to_Server.client_name);
            Quit_Flag = 0;
        }
        else
        {
            sprintf(Server_to_Client.mtext, "[%s] said: ", Client_to_Server.client_name);
            strcat(Server_to_Client.mtext, Client_to_Server.mtext);
        }

        //向客户端发送消息
        for (count = 0; count < Client_Number; count++)
        {
            PrivateMQ = Open_MQ(Client_PID_Box[count]);
            Server_to_Client.client_pid = Client_to_Server.client_pid;
            if ((msgsnd(PrivateMQ, &Server_to_Client, sizeof(Server_to_Client), IPC_NOWAIT)) != -1)
                printf("Write message to Client_%d Success!\n", Client_PID_Box[count]);

            usleep(100000);
        }
    }

    printf("\n");
}

//客户端发送数据
void Client_Write_Data(int Process, int Child_PID)
{

    PublicMQ = Open_MQ(PUBLIC_MQ);
    //捕获退出命令
    if (strcmp(Client_to_Server.mtext, CLIENT_QUIT_TEXT) == 0)
        Client_to_Server.mtype = CLIENT_QUIT;

    //发送消息
    if ((msgsnd(PublicMQ, &Client_to_Server, sizeof(Client_to_Server), IPC_NOWAIT)) == -1)
        printf("Fail to write client data\n");
    usleep(200000);

    //客户端退出
    if (Client_to_Server.mtype == CLIENT_QUIT)
    {
        printf("Client_%d exit\n", Client_to_Server.client_pid);

        //当是父进程唤起时，杀死子进程
        if (Process)
        {
            kill(Child_PID, SIGSTOP);
            usleep(1000);
            printf("Child process has been stoped\n");
        }
        exit(0);
    }
}

//客户端接收消息
void Client_Read_Data(void)
{
    PrivateMQ = Open_MQ(mypid);
    if (msgrcv(PrivateMQ, &Server_to_Client, sizeof(Server_to_Client), DEFAULT_MSG, MSG_NOERROR | IPC_NOWAIT) != -1)
    {
        Show_Local_Time();
        printf("%s\n", Server_to_Client.mtext);
    }
}

//打印当前时间
void Show_Local_Time(void)
{
    time_t tmpcal_ptr;
    struct tm *tmp_ptr = NULL;
    time(&tmpcal_ptr);
    tmp_ptr = localtime(&tmpcal_ptr);
    printf("%d-%02d-%02d ", (1900 + tmp_ptr->tm_year), (1 + tmp_ptr->tm_mon), tmp_ptr->tm_mday);
    printf("%02d:%02d:%02d 👇\n", tmp_ptr->tm_hour, tmp_ptr->tm_min, tmp_ptr->tm_sec);
}

//过滤私聊消息里的有效信息
void Private_Chat_Filter_By_Name(char *Client_Message)
{

    int offset;
    char buffer[2];
    char client_name_buffer[MAX_CLIENT_NAME_LEN] = {0};

    //通过PRIVATE_MSG_HEADER判断是否为私聊
    if (strncmp(Client_Message, PRIVATE_MSG_HEADER, strlen(PRIVATE_MSG_HEADER)) == 0)
    {
        printf("\n");
        offset = strlen(PRIVATE_MSG_HEADER);

        //过滤空格
        while (*(Client_Message + offset) == ' ')
            offset += 1;

        //获得有效的用户名
        while (offset < strlen(Client_Message) && (*(Client_Message + offset) != ' '))
        {
            sprintf(buffer, "%c", *(Client_Message + offset));
            strcat(client_name_buffer, buffer);
            offset++;
        }

        //过滤空格
        while (*(Client_Message + offset) == ' ')
            offset += 1;

        //保存有效的用户名和消息
        strcpy(Client_to_Server.mtext, (Client_Message + offset));
        strcpy(Client_to_Server.target_name, client_name_buffer);
    }
    //群聊
    else
    {
        strcpy(Client_to_Server.target_name, BROADCAST_TO_ALL);
        strcpy(Client_to_Server.mtext, Client_Message);
    }
}

//注册服务端的SIGINT
void Server_Sigcatch(int num)
{
    printf("\nServer is exiting...\n");
    msgctl(PublicMQ, IPC_RMID, NULL);
    printf("Removed PublicMQ:%d\nSee you again 😉\n\n", PublicMQ);
    exit(0);
}