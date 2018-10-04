#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


int main()
{
	//第一步：创建监听套接字
	//socket函数：创建一个新的套接字，返回值为套接字描述符，用来唯一标识该这个套接字
	//参数解读：
	//第一个参数表示地址家族，AF_INET表示使用Internet地址家族
	//第二个参数表示套接字类型，套接字有三种不同类型：
	//SOCK_STREAM，流套接字，用于基于TCP协议的通信
	//SOCK_DGRAM，数据报式套接字，用于基于UDP协议的通信
	//SOCK_RAW，原始套接字，用于底层通信
	//第三个参数表示协议编号，0表示使用默认协议
	int sock_listen;
	sock_listen = socket(AF_INET, SOCK_STREAM, 0);

	//setsockopt函数：设置套接字属性
	//将套接字的SO_REUSEADDR属性设置为1，即允许地址复用
	int optval = 1;
	setsockopt(sock_listen, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	//第二步：绑定地址
	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET; //指定地址家族为Internet地址家族
	myaddr.sin_addr.s_addr = INADDR_ANY; //指定IP地址为本机任意IP
	//myaddr.sin_addr.s_addr = inet_addr("118.126.115.151"); //指定IP地址为本机的某个具体IP地址
	//inet_addr：将字符串形式的IP地址转换为一个无符号32位整数（网络字节序）
	myaddr.sin_port = htons(8888); //指定端口号
	//htons函数：将主机字节序表示的short类型数据转换为网络字节序表示

	if(bind(sock_listen, (struct sockaddr*)&myaddr, sizeof(myaddr)) == -1)
	{
		perror("bind error");
		exit(1);
	}

	//第三步：将sock_listen设置为监听状态
	//listen函数：将指定套接字设置为监听状态，其第二个参数表示连接等待队列的长度
	listen(sock_listen, 5);

	struct sockaddr_in clnaddr;
	socklen_t len;

	while(1)
	{	
		//第四步：接收客户端连接请求
		int sock_conn; //连接套接字，用于和相应的客户端通信

		//accept函数：接收一个客户端连接请求，如果调用该函数时没有客户端连接请求到来，它将会阻塞，直到成功接收到一个连接请求或出错才返回
		//sock_conn = accept(sock_listen, NULL, NULL);
		len = sizeof(clnaddr);
		sock_conn = accept(sock_listen, (struct sockaddr*)&clnaddr, &len);

		if(sock_conn != -1)
		{
			//接收客户端连接请求成功
			//第五步：收发数据

			printf("\n客户端%s:%d已经连接！\n", inet_ntoa(clnaddr.sin_addr), ntohs(clnaddr.sin_port));

			char msg[101];
			int ret;

			/*
			char* dj = malloc(500);
			int ret;

			printf("我说：");
			scanf("%s", msg);

			//发送数据
			//send函数：将内存中的一块数据发送给对方
			//send函数返回值为成功发送的字节数
			//第三个参数表示最多发送的字节数
			send(sock_conn, msg, strlen(msg), 0); //最后一个参数为0表示使用默认的发送方式
			*/

			//接收数据
			//recv函数：接收对方发送过来的数据并存放到指定的内存空间，如果调用该函数时没有数据到来，recv函数会一直等待（即阻塞程序的执行），直到接收到数据或连接断开它才返回
			//recv函数返回值为成功接收的字节数，如果在接收时对方断开了连接，那么它返回0，如果在接收时连接异常断开，那么它返回-1
			//第三个参数表四最多接收的字节数
			
			//while(1)
			{
				sleep(3);
				ret = recv(sock_conn, msg, 100, 0);

				if(ret > 0)
				{
					msg[ret] = '\0';
					printf("他说：%s\n", msg);
				}
				//else break;
			}

			//free(dj);
		}

		//第六步：断开连接（关闭连接套接字）
		close(sock_conn);	
	}
	
	//第七步：关闭监听套接字
	close(sock_listen);	
	

	return 0;
}
