//declare server address and client address
SOCKADDR_IN Server_addr;
//declare socket for listen and connection
SOCKET sListen, sConnection;

//for initializing win-socket api
void socket_init(){
    WSADATA wsaData;
    WORD    DLLVersion;
    DLLVersion = MAKEWORD(2,1);//winsocket-dll version
    WSAStartup(DLLVersion,&wsaData);
    addrlen = sizeof(Server_addr);
}

//let this server connect to input ip and input port
int server_connect(const char ip[], const int& port){
    //set server address (localhost:port)
    Server_addr.sin_addr.s_addr    = inet_addr(ip);
    Server_addr.sin_family         = AF_INET;
    Server_addr.sin_port           = htons(port);
    //create listening socket and check return value
    sListen = socket(AF_INET,SOCK_STREAM,0);
    //if fail, return -1 and clean up connection data
    if (sListen == INVALID_SOCKET){
        WSACleanup();
        return -1;
    }
    // Bind the socket and check return value
    int iResult = bind(sListen, (SOCKADDR*)&Server_addr, sizeof(Server_addr));
    //if fail, return -2, terminate socket and clean up connection data
    if (iResult == SOCKET_ERROR){
        closesocket(sListen);
        WSACleanup();
        return -2;
    }
    //if error occur when listening, return -3, terminate socket and clean up connection data
    if(listen(sListen, 5) == SOCKET_ERROR){
        closesocket(sListen);
        WSACleanup();
        return -3;
    }
    //try to connect to destination ip
    if(sConnection = accept(sListen, NULL, NULL))
        return 0; //if success, return 0
    else{
        //if fail, return 1, terminate socket and clean up connection data
        closesocket(sListen);
        WSACleanup();
        return 1;
    }
}

void server_disconn(){
    closesocket(sConnection);
    WSACleanup();
}

//sending cstring via socket
int server_send(const char mes[]){
    //trying to send data
    if(send(sConnection, mes, sizeof(mes), 0) == SOCKET_ERROR){
        //if fail, return -1, terminate socket and clean up connection data
        closesocket(sConnection);
        WSACleanup();
        return -1;
    }
    else
        return 0; //if success, return 0
}

//receive cstring from socket
int server_recv(char mes[]){
    //reset container
    strcpy(mes, "");
    //try to receiving data
    if(recv(sConnection, mes, sizeof(mes), 0) == SOCKET_ERROR){
        //if fail, return -1, terminate socket and clean up connection data
        closesocket(sConnection);
        WSACleanup();
        return -1;
    }
    else
        return 0; //if success, return 0
}
