#pragma once
//declare litstener
TcpListener listener;
// accept a new connection
TcpSocket sock;
//selector
SocketSelector selector;

//let this server connect to input ip and input port
int server_connect(const int& port = 9487, const int& timeout = 10000){
    // bind the listener to a port
    if (listener.listen(port) != Socket::Done)
        return -1;
    //add listener to selector
    selector.add(listener);
    if (selector.wait(milliseconds(timeout))){
        // Test the listener
        if (selector.isReady(listener)){
            if (listener.accept(sock) == sf::Socket::Done){
                conn = 1, host = 1;
                return 1; //suceed
            }
            else
                return -3;
        }
    }
    return -2;
}

//let the client connect to server by ip and port
int client_connect(const char ip[], const int& port = 9487, const int& timeout = 10000){
    //trying connect to server
    if (sock.connect(ip, port, milliseconds(timeout)) != Socket::Done)
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
        if(!strcmp(tmp, "lose") || !strcmp(tmp, "win"))
            return;
    }
}

