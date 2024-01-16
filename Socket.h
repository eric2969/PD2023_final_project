#define PORT 9487
//declare server address and client address
SOCKADDR_IN Server_addr, Ser_addr;;
//declare socket for listen and connection
SOCKET sListen, sConnection, sConnect;
//for initializing win-socket api
void socket_init(){
    WSADATA wsaData;
    WORD    DLLVersion;
    DLLVersion = MAKEWORD(2,1);//winsocket-dll version
    WSAStartup(DLLVersion,&wsaData);
}

//let this server connect to input ip and input port
int server_connect(const char ip[], const int& port = 9487){
    //set server address (localhost:port)
    Server_addr.sin_addr.s_addr    = inet_addr(ip);
    Server_addr.sin_family         = AF_INET;
    Server_addr.sin_port           = htons(port);
    //create listening socket and check return value
    sListen = socket(AF_INET,SOCK_STREAM,0);
    //if fail, return -1 and clean up connection data
    if (sListen == INVALID_SOCKET){
        return -1;
    }
    // Bind the socket and check return value
    int iResult = bind(sListen, (SOCKADDR*)&Server_addr, sizeof(Server_addr));
    //if fail, return -2, terminate socket and clean up connection data
    if (iResult == SOCKET_ERROR){
        closesocket(sListen);
        return -2;
    }
    //if error occur when listening, return -3, terminate socket and clean up connection data
    if(listen(sListen, 5) == SOCKET_ERROR){
        closesocket(sListen);
        return -3;
    }
    //try to connect to destination ip
    if(sConnection = accept(sListen, NULL, NULL))
        return 0; //if success, return 0
    else{
        //if fail, return 1, terminate socket and clean up connection data
        closesocket(sListen);
        return 1;
    }
}

void server_quit(){
	conn = 0;
	closesocket(sListen);
    closesocket(sConnection);
    WSACleanup();
}

void server_disconn(){
	conn = 0;
	closesocket(sListen);
    closesocket(sConnection);
}

//sending cstring via socket
int server_send(const char mes[]){
    //trying to send data
    if(send(sConnection, mes, sizeof(char) * DataSize, 0) == SOCKET_ERROR){
        //if fail, return -1, terminate socket and clean up connection data
        conn = 0;
        closesocket(sListen);
        closesocket(sConnection);
        return -1;
    }
    else
        return 0; //if success, return 0
}

//receive cstring from socket
int server_recv(char mes[]){
    //try to receiving data
    if(recv(sConnection, mes, sizeof(char) * DataSize, 0) == SOCKET_ERROR){
        //if fail, return -1, terminate socket and clean up connection data
        conn = 0;
        closesocket(sListen);
        closesocket(sConnection);
        return -1;
    }
    else
        return 0; //if success, return 0
}

//client
//let the client connect to server by ip and port
int client_connect(const char ip[], const int& port = 9487){
    //set server address
    Ser_addr.sin_addr.s_addr = inet_addr(ip);
    Ser_addr.sin_family      = AF_INET;
    Ser_addr.sin_port        = htons(port);
    //set up connecting socket
    sListen = socket(AF_INET, SOCK_STREAM, 0);
    //trying connect to server
    if(connect(sConnect, (SOCKADDR*)&Ser_addr, sizeof(Ser_addr)) == SOCKET_ERROR)
        return -1; //if fail, return -1
    else
        return 0; //if success return 0
}

void client_quit(){
    conn = 0;
    closesocket(sConnect);
    WSACleanup();
}

void client_disconn(){
    conn = 0;
    closesocket(sConnect);
}

//sending cstring via socket
int client_send(const char mes[]){
    //trying to send data
    if(send(sConnect, mes, sizeof(char) * DataSize, 0) == SOCKET_ERROR){
        conn = 0;
        closesocket(sConnect);
        return -1; //if fail, return -1
    } 
    else
        return 0; //if success return 0
}

//receive cstring from socket
int client_recv(char mes[]){
    //try to receiving data
    if(recv(sConnect, mes, sizeof(char) * DataSize, 0) == SOCKET_ERROR){
        conn = 0;
        closesocket(sConnect);
        return -1; //if fail, return -1
    }
    else
        return 0; //if success return 0
}

//get user's IP address
string getIP()
{
	char hostName[256];
	if(!gethostname(hostName,sizeof(hostName)))
	{
		hostent *host=gethostbyname(hostName);
		if(host!=NULL)
		{
			return inet_ntoa(*(struct in_addr*)*host->h_addr_list);
		}
	
	}  	
	return "Get IP failed.";  
}

int server_broadcast(){
    char hostName[256] = "Unknown";
	if(gethostname(hostName,sizeof(hostName)))
	{
		std::cout << "Get Host Name Failed" << endl; 
	}
	char buf[1024] = "Tetris:";
	strcat(buf,hostName);
	//set server address
    Server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    Server_addr.sin_family      = AF_INET;
    Server_addr.sin_port        = PORT;
    char so_broadcast = 1;
    //set up connecting socket
    sListen = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    /*SO_BROADCAST: broadcast attribute*/
    if(setsockopt(sListen, SOL_SOCKET, SO_BROADCAST, &so_broadcast, sizeof(so_broadcast)) == SOCKET_ERROR){
        perror("setsockopt");
        cout << WSAGetLastError() << '\n';
        return -1;
    }
    int iResult = bind(sListen, (SOCKADDR*)&Server_addr, sizeof(Server_addr));
    //if fail, return -2, terminate socket and clean up connection data
    if (iResult == SOCKET_ERROR){
        closesocket(sListen);
        return -2;
    }
    
    
    while(1){
        Ser_addr.sin_family = AF_INET; /*IPv4*/
        Ser_addr.sin_port = htons(PORT);  /*Set port number*/
        Ser_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST); /*The broadcast address*/
        /*sendto() doesn't need to be connected*/
        std::cout << "hi "; 
        if((sendto(sListen, buf, strlen(buf), 0, (struct sockaddr*)&Ser_addr, sizeof(Ser_addr))) == SOCKET_ERROR){
            perror("sendto");
            std::cout << "Message Sent Failed\n";
            return -1;
        }
        int addr_len = sizeof(Ser_addr);
        std::cout << "IP sent: " << inet_ntoa(Ser_addr.sin_addr) << '\n';
        int sz = recvfrom(sListen, buf, 128, 0, (sockaddr *)&Ser_addr, &addr_len);
        if (sz > 0) {
          buf[sz] = 0;
          printf("Get Message:\n %s\n", buf);
          printf("get IP %s \n", inet_ntoa(Ser_addr.sin_addr));
          printf("get Port %d \n\n", ntohs(Ser_addr.sin_port));
        }
        Sleep(500);
    } 
    
}

int client_join(){
    char hostName[256] = "Unknown";
	if(gethostname(hostName,sizeof(hostName)))
	{
		std::cout << "Get Host Name Failed" << endl; 
	}
	char buf[1024];
	//set server address
    Server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    Server_addr.sin_family      = AF_INET;
    Server_addr.sin_port        = PORT;
    char so_broadcast = 1;
    //set up connecting socket
    sListen = socket(AF_INET, SOCK_DGRAM, 0);
    /*SO_BROADCAST: broadcast attribute*/
    if(setsockopt(sListen, SOL_SOCKET, SO_BROADCAST, &so_broadcast, sizeof(so_broadcast))<0){
        perror("setsockopt");
        return -1;
    }
    int iResult = bind(sListen, (SOCKADDR*)&Server_addr, sizeof(Server_addr));
    //if fail, return -2, terminate socket and clean up connection data
    if (iResult == SOCKET_ERROR){
        closesocket(sListen);
        return -2;
    }
    
    
    while(1){
        Ser_addr.sin_family = AF_INET; /*IPv4*/
        Ser_addr.sin_port = htons(PORT);  /*Set port number*/
        Ser_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST); /*The broadcast address*/
        /*sendto() doesn't need to be connected*/
        
        int addr_len = sizeof(Ser_addr);
        std::cout << "IP sent: " << inet_ntoa(Ser_addr.sin_addr);
        int sz = recvfrom(sListen, buf, 128, 0, (sockaddr *)&Ser_addr, &addr_len);
        if (sz > 0) {
          buf[sz] = 0;
          printf("Get Message:\n %s\n", buf);
          printf("get IP %s \n", inet_ntoa(Ser_addr.sin_addr));
          printf("get Port %d \n\n", ntohs(Ser_addr.sin_port));
          if((sendto(sListen, buf, strlen(buf), 0, (struct sockaddr*)&Ser_addr, sizeof(Ser_addr))) < 0){
              perror("sendto");
              return -1;
          }
        }
        Sleep(500);
    }
}
