SOCKADDR_IN Server_addr, cliAddr;
SOCKET sListen, sConnection;

void socket_init(){
    WSADATA wsaData;
    WORD    DLLVersion;
    DLLVersion = MAKEWORD(2,1);//winsocket-dll version
    WSAStartup(DLLVersion,&wsaData);
    addrlen = sizeof(Server_addr);
}

int server_connect(const char ip[], const int& port){
    Server_addr.sin_addr.s_addr    = inet_addr(ip);
    Server_addr.sin_family         = AF_INET;
    Server_addr.sin_port           = htons(port);
    sListen = socket(AF_INET,SOCK_STREAM,0);
    printf("ser %s\n", inet_ntoa(Server_addr.sin_addr));
    if (sListen == INVALID_SOCKET){
        WSACleanup();
        return -1;
    }
    // Bind the socket
    int iResult = bind(sListen, (SOCKADDR*)&Server_addr, sizeof(Server_addr));
    if (iResult == SOCKET_ERROR){
        closesocket(sListen);
        WSACleanup();
        return -2;
    }
    //SOMAXCONN: listening without any limit
    if(listen(sListen, 5) == SOCKET_ERROR){
        closesocket(sListen);
        WSACleanup();
        return -3;
    }
    if(sConnection = accept(sListen,(SOCKADDR*)&cliAddr,&addrlen)){
        return 0;
    }
    else
        return 1;
}

int server_send(const char mes[]){
    if(send(sConnection, mes, sizeof(mes), 0) == SOCKET_ERROR){
        closesocket(sConnection);
        WSACleanup();
        return -1;
    }
    else
        return 0;
}

int server_recv(char mes[]){
    strcpy(mes, "");
    if(recv(sConnection, mes, sizeof(mes), 0) == SOCKET_ERROR){
        closesocket(sConnection);
        WSACleanup();
        return -1;
    }
    else
        return 0;
}