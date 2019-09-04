#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <mysql/mysql.h>
#include <stdlib.h>
#include <string.h>

//server information
int clientfd[1024] = {0};
char accountClientfd[1024][20] = {0};
char serverIp[20] = "10.62.50.244";
int serverPort = 8888;

//functtions
void * handClient(void *arg);
int userRegister(char *buf, int confd );
int userLogin(char *buf, int confd);
int dragList(char *buf, int confd);
int dragInformation(char *buf, int confd);
int quitAsk(char *buf, int confd);
int dragTalkerInfo(char *buf, int confd);
int addFriend(char *buf, int confd);
int sendMsg(char *buf, int confd);
int delete(char *buf, int confd);
int block(char *buf, int confd);
int dragRecord(char *buf, int confd);

struct TalkerInfo{
    char talkerPhotoAddr[2];
    char talkeSig[50];
};





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
		printf("my confd, %d", confd);
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
			case '2':
				dragList(buf, confd);
				break;
			case '3':
				dragInformation(buf, confd);
				break;
			case '4':
				addFriend(buf, confd);
				break;
			case '5':
				quitAsk(buf, confd);
				break;
			case '6':
				dragTalkerInfo(buf, confd);
				break;
			case '7':
				sendMsg(buf, confd);
				break;
			case 'a':
				delete(buf, confd);
				break;
			case 'b':
				block(buf, confd);
				break;
			case '8':
				dragRecord(buf, confd);
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
			//search clientfd find confd and make accountconfd[1024] = account
			for(int i = 0; i < 1024; i++){
				if(clientfd[i] == confd){
					strcat(accountClientfd[i], uName);
					break;
				}
			}
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



int dragList(char *buf, int confd){


	MYSQL mysql;//句柄
	MYSQL_RES *result;//结果集指针
	MYSQL_ROW  row;//行结果
	char uName[20]={0};
	char sqlStr[1024]={0};
	sscanf(buf+2,"%s",uName);
	sprintf(sqlStr,"%s'%s'%s'%s'%s'%s'%s","select relation.friendaccount, information.phaddr, information.state, relation.groups from information, relation where relation.account = " ,uName," AND relation.friendaccount in (select relation.friendaccount from relation where relation.account =  ", uName,") AND information.account in (select relation.friendaccount from relation where relation.account = ",uName,") AND relation.friendaccount = information.account;");
	printf("%s\n",sqlStr);
	mysql_init(&mysql);
	if(mysql_real_connect(&mysql,"127.0.0.1","root","jiahua","linpop",0,NULL,0) == NULL)
	{
		printf("%s\n",mysql_error(&mysql));
		return -1;
	}

    //start quering
	mysql_query(&mysql,sqlStr);
    
	result = mysql_store_result(&mysql);
	if(result == NULL)
	{
        //没有收到数据库的任何数据
		printf("%s\n",mysql_error(&mysql));
		printf("hello\n");
		return -1;
	}
    //读取结果，返回结果集中的一行，数组，字符串数组
	//一共有4组数据，所以一共有4个ifelse语句


	for(long long i = 0; i < 100000000; i++){
				;
			}
	char bag[40];
	while(1){
		if(row = mysql_fetch_row(result))
		{
			
			memset(bag, 0, sizeof(bag));
			strcat(bag, "2|");
			strcat(bag, row[0]);
			strcat(bag, "|");
			strcat(bag, row[1]); 
			strcat(bag, "|");
			strcat(bag, row[2]); 
			strcat(bag, "|");
			strcat(bag, row[3]);
			send(confd, bag, sizeof(bag), 0);
			for(long long i = 0; i < 100000000; i++){
				;
			}
			

		}else{
			printf("读取行数据失败\n");
			break;
		}
	}
	printf("hello3");
	memset(bag, 0,sizeof(bag));
	
	send(confd, "x", sizeof("x"), 0);
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





int dragInformation(char *buf, int confd){
	MYSQL mysql;//句柄
	MYSQL_RES *result;//结果集指针
	MYSQL_ROW  row;//行结果
	char uName[20]={0};
	char sqlStr[1024]={0};
	sscanf(buf+2,"%s",uName);
	sprintf(sqlStr, "%s'%s';", "select * from information where account = ", uName);
	//sprintf(sqlStr,"%s'%s'%s'%s'%s'%s'%s","select relation.friendaccount, information.phaddr, information.state, relation.groups from information, relation where relation.account = " ,uName," AND relation.friendaccount in (select relation.friendaccount from relation where relation.account =  ", uName,") AND information.account in (select relation.friendaccount from relation where relation.account = ",uName,") AND relation.friendaccount = information.account;");
	printf("%s\n",sqlStr);
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
		printf("hello\n");
		return -1;
	}
    //读取结果，返回结果集中的一行，数组，字符串数组
	//一共有4组数据，所以一共有4个ifelse语句
	//printf("hello2\n");


	char myInformation[60];
	while(1){
		if(row = mysql_fetch_row(result)){

			memset(myInformation, 0, sizeof(myInformation));
			strcat(myInformation, "3|");
			strcat(myInformation, row[0]);
			strcat(myInformation, "|");
			strcat(myInformation, row[1]); 
			strcat(myInformation, "|");
			strcat(myInformation, row[2]); 
			strcat(myInformation, "|");
			strcat(myInformation, row[3]);
			
			send(confd, myInformation, sizeof(myInformation), 0);
			
			printf("%s\n",myInformation);
		}else{
			printf("读取行数据失败\n");
			break;
		}
		
	}

	memset(myInformation, 0,sizeof(myInformation));
	
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


	//set my state to be 1
	memset(sqlStr, 0, sizeof(sqlStr));
	sprintf(sqlStr, " %s'%s';", "update information set state = '1' where account = ", uName );	
	printf("%s\n", sqlStr);
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
	}else{
		printf("update successfully");
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


int quitAsk(char *buf, int confd){
	MYSQL mysql;//句柄
	MYSQL_RES *result;//结果集指针
	MYSQL_ROW  row;//行结果
	char uName[20]={0};
	char sqlStr[1024]={0};
	sscanf(buf+2,"%s",uName);
	sprintf(sqlStr,"%s'%s';","update information set state = '0' where account = ",uName);
	printf("%s\n",sqlStr);
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
	}else{
		//make accountfd to be ""
		for(int i = 0; i < 1024; i++){
			if(clientfd[i] == confd){
				memset(accountClientfd[i], 0, sizeof(accountClientfd[i]) );
				break;
			}
		}
		send(confd,"qqqqqqqq",strlen("qqqqqqqq"),0);
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




int dragTalkerInfo(char *buf, int confd){

	MYSQL mysql;//句柄
	MYSQL_RES *result;//结果集指针
	MYSQL_ROW  row;//行结果
	char uName[20]={0};
	char sqlStr[1024]={0};
	sscanf(buf+2,"%s",uName);
	sprintf(sqlStr,"%s'%s';","select phaddr, autograph from information where account = ",uName);
	printf("%s\n",sqlStr);
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
		printf("hello here");
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
	}else{
		struct TalkerInfo talkerInfo;
		memset(talkerInfo.talkeSig, 0, sizeof(talkerInfo.talkeSig));
		memset(talkerInfo.talkerPhotoAddr, 0, sizeof(talkerInfo.talkerPhotoAddr));
		result = mysql_store_result(&mysql);
		while(1){
			printf("nihao");
		if(row = mysql_fetch_row(result))
		{
			printf("hello112312");
			memset(talkerInfo.talkerPhotoAddr, 0,sizeof(talkerInfo.talkerPhotoAddr));
			memset(talkerInfo.talkeSig, 0,sizeof(talkerInfo.talkeSig));
			
			
			strcpy(talkerInfo.talkerPhotoAddr, row[0]);
			strcpy(talkerInfo.talkeSig, row[1]); 
			printf("%s %s \n", talkerInfo.talkerPhotoAddr,talkerInfo.talkeSig);
			send(confd, &talkerInfo, sizeof(talkerInfo), 0);
			
		}else{
			printf("读取行数据失败\n");
			break;
		}
	}

	
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



int addFriend(char *buf, int confd)
{
	MYSQL mysql;//句柄
	MYSQL_RES *result;//结果集指针
	MYSQL_ROW  row;//行结果
	char uName[20]={0};
	char fName[20] = {0};
	char code[10] = {0};
	char sqlStr[1024]={0};
	sscanf(buf+2,"%[^|]",uName);
	sscanf(buf+3+ strlen(uName),"%[^|]",fName);
	sscanf(buf+4+ strlen(uName) + strlen(fName),"%s",code);
	sprintf(sqlStr,"%s'%s'%s'%s';", "select * from relation where account = ", uName, "AND friendaccount = ", fName);
	printf("%s\n", sqlStr);
	mysql_init(&mysql);
	mysql_real_connect(&mysql,"127.0.0.1","root","jiahua","linpop",0,NULL,0);
	mysql_query(&mysql,sqlStr);
	result = mysql_store_result(&mysql);
	if(row = mysql_fetch_row(result)){
		//return 1
		send(confd, "1", strlen("1"), 0);
		
	}else{
		//not in my list
		if (result)
		{
			mysql_free_result(result);
			while (!mysql_next_result(&mysql))
			{
				result = mysql_store_result(&mysql);
				mysql_free_result(result);
			}
		}
		memset(sqlStr, 0, sizeof(sqlStr));
		sprintf(sqlStr,"%s'%s'%s'%s'%s;", "select * from relation where account = ", fName, "AND friendaccount = ", uName, "AND groups = '0'" );
		printf("%s\n", sqlStr);
		mysql_query(&mysql,sqlStr);
		result = mysql_store_result(&mysql);
		if(row = mysql_fetch_row(result)){
			//return 0
			send(confd, "0", strlen("0"), 0);
			
		}
		else{
			//im not blocked
			if (result)
			{
				mysql_free_result(result);
				while (!mysql_next_result(&mysql))
				{
					result = mysql_store_result(&mysql);
					mysql_free_result(result);
				}
			}
			memset(sqlStr, 0, sizeof(sqlStr));
			sprintf(sqlStr,"%s'%s';", "select * from userlist where account = ", fName );
			printf("%s\n", sqlStr);
			mysql_query(&mysql,sqlStr);
			result = mysql_store_result(&mysql);
			if(row = mysql_fetch_row(result)){
				//there is friendguy
				memset(sqlStr, 0, sizeof(sqlStr));
				sprintf(sqlStr,"%s'%s';", "select invcode from userlist where account = ", fName);
				printf("%s\n", sqlStr);
				mysql_query(&mysql,sqlStr);
				result = mysql_store_result(&mysql);
				if(row = mysql_fetch_row(result)){
					//return 0
					if(strcmp(code, row[0]) == 0){
						//return 2 并且加入到数据库
						send(confd, "2", strlen("2"), 0);
						//add to mysql
						memset(sqlStr, 0, sizeof(sqlStr));
						sprintf(sqlStr,"%s'%s','%s','1');", "insert into relation values(", uName, fName );
						printf("%s\n", sqlStr);
						mysql_query(&mysql,sqlStr);
						memset(sqlStr, 0, sizeof(sqlStr));
						sprintf(sqlStr,"%s'%s','%s','1');", "insert into relation values(", fName, uName );
						printf("%s\n", sqlStr);
						mysql_query(&mysql,sqlStr);
						
					}else{
						send(confd, "3", strlen("3"), 0);
						
					}
				}else{
					printf("搜索结果为空，怎么可能？");
					return 1;
				}
				
			}else{
				//return 4;
				send(confd, "4", strlen("4"), 0);
				
			}
		}
	}

}



int sendMsg(char *buf, int confd){
	MYSQL mysql;//句柄
	MYSQL_RES *result;//结果集指针
	MYSQL_ROW  row;//行结果
	
	char uName[20] = {0};
	char fName[20] = {0};
	char sendDate[30] = {0};
	char sendMessage[200] = {0};


	
	char sqlStr[1024]={0};
	sscanf(buf+2,"%[^|]|%[^|]|%[^|]|%[^|]",uName, fName, sendDate, sendMessage);
	sprintf(sqlStr,"%s'%s','%s','%s','%s');", "insert into record values( ", uName, fName, sendDate, sendMessage);
	printf("%s\n", sqlStr);
	mysql_init(&mysql);
	mysql_real_connect(&mysql,"127.0.0.1","root","jiahua","linpop",0,NULL,0);
	

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
				int fConfd;
				int flag = 0;
				for(int i = 0; i < 1024; i++){
					if(strcmp(accountClientfd[i], fName) == 0){
						//找到了fconfd
						
						fConfd = clientfd[i];
						printf("find friendConfd\n");
						printf("friend confd : %d\n",fConfd);
						printf("friend name: %s\n", accountClientfd[i]);
						flag = 1;
						break;
					}
				}
				if(flag ==0){
					printf("对方不zaixian\n");
					result = mysql_store_result(&mysql);
						if (result){
								mysql_free_result(result);
								while (!mysql_next_result(&mysql))
								{
									result = mysql_store_result(&mysql);
									mysql_free_result(result);
								}
							}
						return 1;
				}
				
				//
				//if he online, just send to him, if he offline, do nothing;

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
				memset(sqlStr, 0, sizeof(sqlStr));
				sprintf(sqlStr,"%s'%s';", "select state from information where account = ",fName);
				printf("%s\n", sqlStr);


				mysql_query(&mysql,sqlStr);
				result = mysql_store_result(&mysql);
				if(row = mysql_fetch_row(result)){
					if(strcmp(row[0], "0") == 0){
						//对方不在线 
						result = mysql_store_result(&mysql);
						if (result){
								mysql_free_result(result);
								while (!mysql_next_result(&mysql))
								{
									result = mysql_store_result(&mysql);
									mysql_free_result(result);
								}
							}
						return 1;
					}else if(strcmp(row[0], "1") == 0){
						//对方在线
						printf("!!!!!!!!!!!!!!!!!!!!!!!%s %s %s %s\n",uName, fName, sendDate, sendMessage);


						char bag1[300];
						memset(bag1, 0, sizeof(bag1));
						strcat(bag1,"7|");
						strcat(bag1, uName);
						strcat(bag1, "|");
						strcat(bag1, fName);
						strcat(bag1, "|");
						strcat(bag1, sendDate);
						strcat(bag1, "|");
						strcat(bag1, sendMessage);

						send(fConfd, bag1, strlen(bag1), 0);
						printf("bag1: %s\n", bag1);
						
						//return 1;
					}else{
						printf("其他结果，不在也在？？？no way\n");
						//return 1;
					}
				}else{
					printf("error, 怎么可能查无此人？？？\n");
					//return 1;
				}



				//memset all information
				//release your command
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
            }

}




int delete(char *buf, int confd){
	MYSQL mysql;//句柄
	MYSQL_RES *result;//结果集指针
	MYSQL_ROW  row;//行结果
	
	char uName[20] = {0};
	char fName[20] = {0};
	


	
	char sqlStr[1024]={0};
	sscanf(buf+2,"%[^|]|%[^|]",uName, fName);
	sprintf(sqlStr,"%s '%s' %s '%s';", "delete from relation where account = ", uName, "AND friendaccount = ", fName);
	printf("%s\n", sqlStr);
	mysql_init(&mysql);
	mysql_real_connect(&mysql,"127.0.0.1","root","jiahua","linpop",0,NULL,0);


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
		// char deleteReturn[10];
		// memset(deleteReturn, 0, sizeof(deleteReturn));
		// strcat(deleteReturn, "a|1");
		// send(confd, deleteReturn, sizeof(deleteReturn), 0);
		memset(sqlStr, 0, sizeof(sqlStr));
		sprintf(sqlStr,"%s '%s' %s '%s';", "delete from relation where account = ", fName, "AND friendaccount = ", uName);
		printf("%s\n", sqlStr);
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
			//change successfully
			char deleteReturn[10];
			memset(deleteReturn, 0, sizeof(deleteReturn));
			strcat(deleteReturn, "a|1");
			send(confd, deleteReturn, sizeof(deleteReturn), 0);
		}

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

}



int block(char *buf, int confd){
	MYSQL mysql;//句柄
	MYSQL_RES *result;//结果集指针
	MYSQL_ROW  row;//行结果
	
	char uName[20] = {0};
	char fName[20] = {0};

	char sqlStr[1024]={0};
	sscanf(buf+2,"%[^|]|%[^|]",uName, fName);
	sprintf(sqlStr,"%s '%s' %s '%s';", "update relation set groups = '0' where account = ", uName, "AND friendaccount = ", fName);
	printf("%s\n", sqlStr);
	mysql_init(&mysql);
	mysql_real_connect(&mysql,"127.0.0.1","root","jiahua","linpop",0,NULL,0);


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
		memset(sqlStr, 0, sizeof(sqlStr));
		sprintf(sqlStr,"%s '%s' %s '%s';", "delete from relation where account = ", fName, "AND friendaccount = ", uName);
		printf("%s\n", sqlStr);
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
			char blockReturn[10];
			memset(blockReturn, 0, sizeof(blockReturn));
			strcat(blockReturn, "b|1");
			send(confd, blockReturn, sizeof(blockReturn), 0);
		}
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
}



int dragRecord(char *buf, int confd){
	MYSQL mysql;//句柄
	MYSQL_RES *result;//结果集指针
	MYSQL_ROW  row;//行结果
	
	char uName[20] = {0};
	char fName[20] = {0};

	char sqlStr[1024]={0};
	sscanf(buf+2,"%[^|]|%[^|]",uName, fName);
	sprintf(sqlStr,"%s '%s' %s '%s';", "update relation set groups = '0' where account = ", uName, "AND friendaccount = ", fName);
	printf("%s\n", sqlStr);
	mysql_init(&mysql);
	mysql_real_connect(&mysql,"127.0.0.1","root","jiahua","linpop",0,NULL,0);


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
		memset(sqlStr, 0, sizeof(sqlStr));
		sprintf(sqlStr,"%s '%s' %s '%s';", "delete from relation where account = ", fName, "AND friendaccount = ", uName);
		printf("%s\n", sqlStr);
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
			char blockReturn[10];
			memset(blockReturn, 0, sizeof(blockReturn));
			strcat(blockReturn, "b|1");
			send(confd, blockReturn, sizeof(blockReturn), 0);
		}
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
}