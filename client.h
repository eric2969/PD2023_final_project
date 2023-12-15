
//return sockfd
//other things won't be used after connection
int client()
{
    //socket的建立
    int sockfd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0); //client_sockfd is a global variable in main

    if (sockfd == -1){
        printf("Fail to create a socket.");
    }

    //socket的連線
    struct sockaddr_in info;
    bzero(&info,sizeof(info));
    info.sin_family = PF_INET;

    //localhost test
    info.sin_addr.s_addr = inet_addr("127.0.0.1");
    info.sin_port = htons(8700);


    int err = connect(sockfd, (struct sockaddr *)&info, sizeof(info)); //send client info to the server
    if(err == -1){
        throw std::runtime_error("Connection error");
    }
    else
        printf("Client connected to server.");

    return sockfd;
}

//send packed message to client
void send_to_server(char message, int sockfd){
    send(sockfd, message, sizeof(message), 0); //send sockfd so that the server knows where to send back
}

char receive_from_server(int sockfd){
    char receiveMessage[100] = {};
    recv(sockfd, receiveMessage, sizeof(receiveMessage), 0); //ssize_t recv(int sockfd, void *buf, size_t len, int flags);
    return receiveMessage;
}

void client_disconnect(int sockfd){
    close(sockfd);
    printf("client disconnected");
    return 0;
}