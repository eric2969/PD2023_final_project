
//create listening port
int server(){
    //socket的建立
    int sockfd = 0, forClientSockfd = 0;
    sockfd = socket(PF_INET , SOCK_STREAM , 0);

    if (sockfd == -1){
        throw std::runtime_error("Fail to create a socket.");
    }
    else
        printf("Server is activated.");

    //socket的連線
    struct sockaddr_in serverInfo;
    bzero(&serverInfo,sizeof(serverInfo)); //initialization

    serverInfo.sin_family = PF_INET; //Ipv4 structure
    serverInfo.sin_addr.s_addr = INADDR_ANY; //set server IP address?
    serverInfo.sin_port = htons(8700); //Host TO Network Short integer; unify endianess
    bind(sockfd, (struct sockaddr *)&serverInfo, sizeof(serverInfo)); //bind address on server side
    listen(sockfd, 5);

    return sockfd;
}

//send packed message to client
void send_to_client(char message, int sockfd){
    struct sockaddr_in clientInfo;
    int forClientSockfd = 0;
    socklen_t addrlen = sizeof(clientInfo);
    forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);
    send(forClientSockfd, message, sizeof(message), 0);
}

char receive_from_client(){
    struct sockaddr_in clientInfo;
    int forClientSockfd = 0;
    socklen_t addrlen = sizeof(clientInfo);
    char inputBuffer[256] = {};
    forClientSockfd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);
    recv(forClientSockfd, inputBuffer, sizeof(inputBuffer), 0);
    return inputBuffer;
}

void server_disconnect(int sockfd){
    close(sockfd);
    printf("client disconnected");
    return 0;
}