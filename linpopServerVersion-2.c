#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <mysql/mysql.h>

//server information
int clientfd[1024] = {0};
char serverIp[20] = "10.195.15.196";
int serverPort = 8888;

//functtions
void * handClient(void *arg);
int userRegister(char *buf, int confd );
int userLogin(char *buf, int confd);




//main program
int main(){

    pthread_t tid;
    struct sockaddr_in myaddr;
    struct sockaddr_in cliaddr;
    int socklen = sizeof(cliaddr);
    int lisfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&myaddr, 0, sizeof(myaddr));
    memset(&cliaddr, 0,sizeof(cliaddr)) ;
    myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = inet_addr(serverIp);
	myaddr.sin_port = htons(serverPort);


    //reuse the port if close forcely.建立socket
    int reuse = 1;
    if (setsockopt(lisfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        perror("setsockopet error\n");
        return -1;
    }

    //bind
    if(bind(lisfd, (struct sockaddr*)&myaddr,sizeof(myaddr)) != 0)
	{
		printf("bind error\n");
		return -1;
	}

    //listen
    int i = 1;
    while(1)
	{
        printf("Start listening!\n");
        listen(lisfd, 10);
		int confd = accept(lisfd, (struct sockaddr*)&cliaddr,&socklen);
		if(confd == -1)
		{
			printf("error accpet\n");
			return -1;
		}
        printf("accepted a new client\n");
		clientfd[i] = confd;//need change ,if there are more than 2048 people online at one time
		i++;

		//printf("newclient connected: ip=%s\t,port=%d\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));

		pthread_create(&tid,NULL,handClient,&clientfd[i-1]);
	}
    return 1;
}




void * handClient(void *arg)
{
	char buf[1024]={0};
	int *p = arg;
	int confd = *p;
	while(1)
	{
		if(recv(confd,buf,sizeof(buf),0) == 0)
		{
			pthread_exit(NULL);
		}
		printf("recv=%s\n",buf);
		switch(buf[0])
		{
			case '0':
				userRegister(buf, confd);
				break;
			case '1':
				userLogin(buf, confd);
				break;
			default:
				printf("error!");
		}
	}
}




int userRegister(char *buf, int confd ){

    //initialize
    MYSQL mysql;
    MYSQL_RES *result;
    MYSQL_ROW  row;
    char uName[20] = {0};
    char pWord[20] = {0};
    char sqlStr[1024] = {0};
    sscanf(buf+2, "%[^|]|%s",uName, pWord);
    sprintf(sqlStr, "%s'%s'", "select account from userlist where account=",uName);
    
    //be ready to query
    mysql_init(&mysql);
    if(mysql_real_connect(&mysql,"127.0.0.1","root","jiahua","linpop",0,NULL,0) == NULL)
	{
        //check if cannot connect
		printf("%s\n",mysql_error(&mysql));
		return -1;
	}

    //start query, return 0 it's ok;  others error!
    if(mysql_query(&mysql,sqlStr) != 0)
	{
		 //release your command
		 result = mysql_store_result(&mysql);
		if (result)
		{
			mysql_free_result(result);
			while (!mysql_next_result(&mysql))
			{
				result = mysql_store_result(&mysql);
				mysql_free_result(result);
			}
		}
	}else{
        //release your command
		result = mysql_store_result(&mysql);
        int rowCount = mysql_num_rows(result);

        if(rowCount != 0){
            //已经有人注册过了
            send(confd,"2",strlen("2"),0);
        }else{
            //没有人注册，开始插入数据库
            sprintf(sqlStr, "%s'%s','%s','%c')", "insert into userlist(account,password,status) values (",uName,pWord, '0');
            if(mysql_query(&mysql, sqlStr) != 0){
                //插入失败，未知原因
                send(confd,"0",strlen("0"),0);
                //release your command
                result = mysql_store_result(&mysql);
                if (result){
                        mysql_free_result(result);
                        while (!mysql_next_result(&mysql))
                        {
                            result = mysql_store_result(&mysql);
                            mysql_free_result(result);
                        }
                    }

            }else{
                //插入成功
                send(confd,"1",strlen("1"),0);
            }
        }
    }
	return 1;
}




int userLogin(char *buf, int confd)
{
	MYSQL mysql;//句柄
	MYSQL_RES *result;//结果集指针
	MYSQL_ROW  row;//行结果
	char uName[20]={0};
	char pWord[20]={0};
	char sqlStr[1024]={0};
	sscanf(buf+2,"%[^|]|%s",uName,pWord);
	sprintf(sqlStr,"%s'%s'", "select password from userlist where account=",uName);

	mysql_init(&mysql);
	if(mysql_real_connect(&mysql,"127.0.0.1","root","jiahua","linpop",0,NULL,0) == NULL)
	{
		printf("%s\n",mysql_error(&mysql));
		return -1;
	}

    //start quering
	if(mysql_query(&mysql,sqlStr) != 0)
	{
		printf("%s\n",mysql_error(&mysql));
		//release your command
		 result = mysql_store_result(&mysql);
		if (result)
		{
			mysql_free_result(result);
			while (!mysql_next_result(&mysql))
			{
				result = mysql_store_result(&mysql);
				mysql_free_result(result);
			}
		}
		return -1;
	}
    
	result = mysql_store_result(&mysql);
	if(result == NULL)
	{
        //没有收到数据库的任何数据
		printf("%s\n",mysql_error(&mysql));
		return -1;
	}
    //读取结果，返回结果集中的一行，数组，字符串数组
	if(row = mysql_fetch_row(result))
	{
		if(strcmp(row[0], pWord) == 0)
		{
			printf("登录成功\n");
            send(confd,"1",strlen("1"),0);
		}
		else
		{
			printf("密码错误\n");
            send(confd,"2",strlen("0"),0);
		}
	}else
	{
		printf("用户未注册\n");
        send(confd,"3",strlen("0"),0);
	}
    //release your command
		 result = mysql_store_result(&mysql);
		if (result)
		{
			mysql_free_result(result);
			while (!mysql_next_result(&mysql))
			{
				result = mysql_store_result(&mysql);
				mysql_free_result(result);
			}
		}
    return 1;
}