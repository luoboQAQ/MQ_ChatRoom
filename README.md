# MQ_ChatRoom

> Linux 使用消息缓冲队列实现聊天室功能

## 程序功能说明

- [x] 实现了多客户端的群聊功能
- [x] 实现了客户端双方的私聊功能
- [x] 私聊对象通过客户端的用户名进行标识
- [x] 完善了客户端和服务器的退出方式
- [ ] 优化代码，避免进程进入"忙等"

## 编译和测试说明

* 使用 make 实现客户端和服务器的编译
* server 是服务器程序
* client 是客户端程序
* 客户端默认为群发模式
* 客户端的私聊格式为 `@[Target_Client_Name] [Message_To_Be_Sent]`

## 程序代码

> 源代码遵循 [GNU General Public License v3.0](https://gitee.com/luoboQAQ/MQ_ChatRoom/blob/master/LICENSE) 开源许可协议

本代码基于[ZHJ0125](https://gitee.com/zhj0125)的[FIFO_ChatRoom](https://gitee.com/zhj0125/FIFO_ChatRoom)修改而成，在此感谢大佬的无私奉献。

