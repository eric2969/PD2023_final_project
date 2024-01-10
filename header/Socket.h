#pragma once
//declare litstener
TcpListener listener;
// accept a new connection
TcpSocket sock;

//let this server connect to input ip and input port
int server_connect(const int& port = 9487){
    // bind the listener to a port
    if (listener.listen(port) != Socket::Done)
        return -1;
    if (listener.accept(sock) != Socket::Done){
        return -2;
    }
    return 0; //suceed
}

//let the client connect to server by ip and port
int client_connect(const char ip[], const int& port = 9487){
    //trying connect to server
    if (sock.connect(ip, port) != Socket::Done)
        return -1;
    return 0;
}

void socket_disconnect(){
	listener.close();
    sock.disconnect();
}

//sending cstring via socket
int socket_send(const char mes[]){
    //trying to send data
    if (sock.send(mes, DataSize) != Socket::Done)
        return -1;
    return 0;
}

//receive cstring from socket
int socket_recv(char mes[]){
    //try to receiving data
    size_t received;
    if(sock.receive(mes, DataSize, received) != Socket::Done)
        return -1;
    return 0;
}

void chk_conn(int& ret, int& token, const int& chk_rate = 500){
    while(1){
        Thrd_lock.lock();
        if(token != 1){
            Thrd_lock.unlock();
            break;
        }
        if(socket_send("chk")){
            Thrd_lock.lock();
            ret = -1;
            Thrd_lock.unlock();
            break;
        }
        sleep(milliseconds(chk_rate));
    }
}

void Table_Trans(int& ret, int& token, char Snd[], char Rec[]){
    while(token != 1) {Sleep(flush_tick);}
    char tmp[DataSize];
    while(token > 0){
        Thrd_lock.lock();
        for(int i = 0;i < DataSize;i++) tmp[i] = Snd[i];
        Snd[110] = 0; //[110] for garbage
        Thrd_lock.unlock();
        if(socket_send(tmp)){
            ret = -1;
            break;
        }
        if(socket_recv(tmp)){
            ret = -1;
            break;
        }
        if(!strcmp(tmp, "chk"))
            continue;
        Thrd_lock.lock();
        for(int i = 0;i < DataSize;i++) Rec[i] = tmp[i];
        ret = 1;
        Thrd_lock.unlock();
    }
}