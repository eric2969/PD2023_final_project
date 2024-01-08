#pragma once
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
    sConnect = socket(AF_INET, SOCK_STREAM, 0);
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
