#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>



#pragma pack(1) 
typedef struct file_fornt
{
	char name[51];
	int len;

}file_f;
#pragma pakc()

int main(int argc,char** argv)
{
	int sock_client;
	sock_client = socket(AF_INET,SOCK_STREAM,0);
	
	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = INADDR_ANY;
	myaddr.sin_port = htons(8888);	
	if(-1 == bind(sock_client,(struct sockaddr*)&myaddr,sizeof(myaddr)))
	{
		perror("bind fail:");
	}
	struct sockaddr_in seraddr;
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = inet_addr(argv[1]);
	seraddr.sin_port = htons(atoi(argv[2]));
	if(-1 == connect(sock_client,(struct sockaddr*)&seraddr,sizeof(seraddr)))
	{
		perror("connect fail:");
		exit(1);
	}
	char msg[1024];
	int ret; //查看发送接收成功数据个数

	
	file_f fi;
	ret = recv(sock_client,&fi,sizeof(fi),0);
//	fi.name[strlen(fi.name) + 1] = '\0';
//	printf("%d\n",fi.len);
	FILE* fp = fopen(fi.name,"wb");
	while(1)
	{
		ret = recv(sock_client,&msg,sizeof(msg),0);
		fwrite(msg,1,ret,fp);	
	
		if(ret == 0)
		break;
	}
	close(sock_client);
	return 0;
}	
