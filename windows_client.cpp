#pragma comment(lib, "WS2_32")

#include <WinSock2.h>
#include <iostream>
#include <string>

using namespace std;


#define        PORT_NUM        (9527)


int main()
{
    string confirm;
    char message[200];

    //?? Winsock-DLL
    int r;
    WSAData wsaData;
    WORD DLLVersion;
    DLLVersion = MAKEWORD(2,1);
    r = WSAStartup(DLLVersion, &wsaData);

    //摰??蝯?socket 雿輻??sockadder_in 蝯?
    SOCKADDR_IN addr;

    int addlen = sizeof(addr);

    //閮剖? socket
    SOCKET sConnect;

    //AF_INET: internet-family
    //SOCKET_STREAM: connection-oriented socket
    sConnect = socket(AF_INET, SOCK_STREAM, 0);

    //閮剖? addr 鞈?
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_NUM);

    cout << "connect to server?[Y] or [N]" << endl;
    cin >> confirm;

    if(confirm == "N")
    {
        exit(1);
    }else{
        if(confirm == "Y")
        {
            connect(sConnect, (SOCKADDR*)&addr, sizeof(addr));
            //?交 server 蝡舐?閮
            ZeroMemory(message, 200);
            r = recv(sConnect, message, sizeof(message), 0);
            cout << message << endl;

            //閮剖? closesocket ??銝???TIME-WAIT ??,?湔??socket
            //BOOL bDontLinger = FALSE;
            //setsockopt(sConnect,SOL_SOCKET,SO_DONTLINGER,(const char*)&bDontLinger,sizeof(BOOL));

            //?乩?敺??蝙?剁??舐 closesocket ?????
            closesocket(sConnect);

            getchar();
            getchar();
        }
    }
    return 0 ;
}

