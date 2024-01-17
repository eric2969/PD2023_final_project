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
    conn = 1, host = 1;
    return 1; //suceed
}

//let the client connect to server by ip and port
int client_connect(const char ip[], const int& port = 9487){
    //trying connect to server
    if (sock.connect(ip, port) != Socket::Done)
        return -1;
    conn = 1, host = 0;
    return 1;
}

void socket_disconnect(){
	listener.close();
    sock.disconnect();
    conn = 0;
}

//sending cstring via socket
int socket_send(const char mes[]){
    //trying to send data
    if (sock.send(mes, DataSize) != Socket::Done){
        socket_disconnect();
        conn = 0;
        return -1;
    }
    return 1;
}

//receive cstring from socket
int socket_recv(char mes[]){
    //try to receiving data
    size_t received;
    if(sock.receive(mes, DataSize, received) != Socket::Done){
        socket_disconnect();
        conn = 0;
        return -1;
    }
    return 1;
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

void Table_Trans(char Snd[], char Rec[]){
    while(Thrd_token != 1) {sleep(milliseconds(flush_tick));}
    char tmp[DataSize];
    while(Thrd_token > 0){
        Thrd_lock.lock();
        for(int i = 0;i < DataSize;i++) tmp[i] = Snd[i];
        Snd[110] = 0; //[110] for garbage
        Thrd_lock.unlock();
        if(socket_send(tmp) < 0){
            Thrd_ret = -1;
            return;
        }
        if(socket_recv(tmp) < 0){
            Thrd_ret = -1;
            return;
        }
        if(!strcmp(tmp, "chk"))
            continue;
        Thrd_lock.lock();
        for(int i = 0;i < DataSize;i++)
            if(i == 110)
                Rec[i] += tmp[i];
            else
                Rec[i] = tmp[i];
        Thrd_ret = 1;
        Thrd_lock.unlock();
    }
}

