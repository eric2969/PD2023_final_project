SOCKADDR_IN Client_addr;
SOCKET sConnect;

int client_connect(const char ip[], const int& port){
	Client_addr.sin_addr.s_addr = inet_addr(ip);
	Client_addr.sin_family      = AF_INET;
	Client_addr.sin_port        = htons(port);
	sConnect = socket(AF_INET, SOCK_STREAM, 0);
	if(connect(sConnect, (SOCKADDR*)&Client_addr, sizeof(Client_addr)) == SOCKET_ERROR)
		return -1;
	else
		return 0;
}

int client_send(const char mes[]){
	if(send(sConnect, mes, sizeof(mes), 0) == SOCKET_ERROR)
        return -1;
    else
        return 0;
}

int client_recv(char mes[]){
	strcpy(mes, "");
	if(recv(sConnect, mes, sizeof(mes), 0) == SOCKET_ERROR)
		return -1;
	else
		return 0;
}
