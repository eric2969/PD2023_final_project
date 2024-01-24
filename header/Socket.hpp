#pragma once

using namespace sf;
//declare litstener
TcpListener listener;
// accept a new connection
TcpSocket sock;
//selector
SocketSelector selector;

//let this server connect to input ip and input port
int server_connect(const int& port = 9487, const int& timeout = 12000){
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
                selector.clear();
                selector.add(sock);
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
    selector.clear();
    selector.add(sock);
    conn = 1, host = 0;
    return 1;
}

void socket_disconnect(){
	listener.close();
    sock.disconnect();
    selector.clear();
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

int socket_recv(char mes[], const int& timeout = 3000){
	size_t RecSize;
    if(selector.wait(milliseconds(timeout))){
        if(selector.isReady(sock)){
            if(sock.receive(mes, DataSize, RecSize) != Socket::Done){
                socket_disconnect();
                return -1;
            }
            return 1;
        }
    }
    return 0;
}

string getIPAdr(){ return sock.getRemoteAddress().toString();}

void thrd_conn(const bool& isHost, const string& ip){
    Thrd_lock.lock();
    Thrd_ret = 0;
    Thrd_lock.unlock();
    int tmp;
    if(isHost)
        tmp = server_connect();
    else
        tmp = client_connect(ip.c_str());
    Thrd_lock.lock();
    Thrd_ret = tmp;
    Thrd_lock.unlock();
}

void wait_ready(){
    char tmp[DataSize]; int status = 0;
    while(1){
        status = socket_recv(tmp);
        if(status < 0){
            Thrd_lock.lock();
            Thrd_ret = -1;
            Thrd_lock.unlock();
            return;
        }
        Thrd_lock.lock();
        if(status > 0 && !strcmp("rdy", tmp)){ 
            Thrd_ret = 1;
            Thrd_lock.unlock();
            return;
        }
        if(Thrd_token < 0){
            Thrd_lock.unlock();
            return;
        }
        Thrd_lock.unlock();
    }
}

void Table_Trans(char Snd[], char Rec[]){
    while(Thrd_token != 1) {sleep(milliseconds(flush_tick));}
    char tmp[DataSize]; short status;
    while(1){
        Thrd_lock.lock();
        for(int i = 0;i < DataSize;i++) tmp[i] = Snd[i];
        Snd[110] = 0; //[110] for garbage
        if(Thrd_token < 0){
            Thrd_lock.unlock();
            return;
        }
        Thrd_lock.unlock();
        if(socket_send(tmp) < 0){
            Thrd_lock.lock();
            Thrd_ret = -1;
            Thrd_lock.unlock();
            return;
        }
        status = socket_recv(tmp);
        Thrd_lock.lock();
        if(status < 0){
            Thrd_ret = -1;
            Thrd_lock.unlock();
            return;
        }
        else if(!status){
            Thrd_lock.unlock();
            continue;
        }
        if(!strcmp(tmp, "chk") || !strcmp(tmp, "rdy")){
            Thrd_lock.unlock();
            continue;
        }
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

