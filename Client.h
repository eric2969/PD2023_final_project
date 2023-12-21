//declare server address (destination)
SOCKADDR_IN Ser_addr;
//declare socket for connection
SOCKET sConnect;

//let the client connect to server by ip and port
int client_connect(const char ip[], const int& port){
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

//sending cstring via socket
int client_send(const char mes[]){
	//trying to send data
	if(send(sConnect, mes, sizeof(mes), 0) == SOCKET_ERROR)
        return -1; //if fail, return -1 
    else
        return 0; //if success return 0
}

//receive cstring from socket
int client_recv(char mes[]){
	//reset container
	strcpy(mes, "");
	//try to receiving data
	if(recv(sConnect, mes, sizeof(mes), 0) == SOCKET_ERROR)
		return -1; //if fail, return -1 
	else
		return 0; //if success return 0
}
