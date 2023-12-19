#pragma comment(linker,/DEFAULTLIB:WS2_32)
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <string>

using namespace std;


#define        PORT_NUM        (9527)

int main()
{

    /*
        struct WSAData 
        {  
            WORD            wVersion;  
            WORD            wHighVersion;  
            char            szDescription[WSADESCRIPTION_LEN+1];  
            char            szSystemStatus[WSASYSSTATUS_LEN+1];  
            unsigned short    iMaxSockets;  
            unsigned short    iMaxUdpDg;  
            char FAR*        lpVendorInfo;  
        };  */
    WSADATA wsaData;
    WORD    DLLVersion;
    DLLVersion = MAKEWORD(2,1);//winsocket-dll version
	
//	WSAStartup()
    // ??WSAStartup ?? Winsocket-DLL
    int err = WSAStartup(DLLVersion,&wsaData);
    if (err!=0)
    {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n",err);
        return 1;
    }
    if (LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wVersion)!=1)
    {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        printf("Could not find a usable version of Winsock.dll\n");
        WSACleanup();
        return 1;
    }
    else
    {
        printf("The Winsock 2.1 dll was found okay\n");
    }
    // 摰?? socket雿?鞈? (銝???,????雿?鞈?,?隞交?????鞈?蝯?摮??雿?鞈?)
    /*
    /*
     *
     * Socket address, internet style. 
        struct sockaddr_in 
        {
                short   sin_family;
                u_short sin_port;
                struct  in_addr sin_addr;
                char    sin_zero[8];
        };
        #ifndef s_addr
    
     * Internet address (old style... should be updated)

        struct in_addr 
        {
            union 
            {
                    struct { u_char s_b1,s_b2,s_b3,s_b4; } S_un_b;
                    struct { u_short s_w1,s_w2; } S_un_w;
                    u_long S_addr;
            } S_un;
        }
    */
    SOCKADDR_IN    addr;
    int addrlen = sizeof(addr);

    // Create socket
    SOCKET sListen        ;        //listening for an incoming connection
    SOCKET sConnection    ;        //oerating if a connection was found

    // AF_INET        :  銵函內撱箇??ocket撅祆internet family
    // SOCK_STREAM    :  銵函內撱箇??ocket?畚onnection-oriented socket
    sConnection = socket(AF_INET,SOCK_STREAM,0);

    // 閮剖?雿?鞈?????
    addr.sin_addr.s_addr    = inet_addr("127.0.0.1");
    addr.sin_family            = AF_INET;
    addr.sin_port            = htons(PORT_NUM);


    // 閮剖?Listen
    sListen = socket(AF_INET,SOCK_STREAM,0);
    if (sListen == INVALID_SOCKET)
    {
        printf("socket function failed with error : %u \n",WSAGetLastError());
        WSACleanup();
        return 1;
    }
    // Bind the socket
    int iResult = bind (sListen,(SOCKADDR*)&addr,sizeof(addr));
    if (iResult == SOCKET_ERROR)
    {
        printf("Bind failed with error : %u \n", WSAGetLastError());
        closesocket(sListen);
        WSACleanup();
        return 1;
    }
    //SOMAXCONN: listening without any limit
    if(listen(sListen, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("listen function failed with error: %d \n", WSAGetLastError());
        closesocket(sListen);
        WSACleanup();
        return 1;

    }
    else
        printf("Listening on socket...\n");

 
    // 蝑????
    SOCKADDR_IN clientAddr;
    while (1)
    {
        cout << "Waitting for connect... "<<endl;
        if(sConnection = accept(sListen,(SOCKADDR*)&clientAddr,&addrlen))
        {
            cout << "a connection was found."<<endl;
            printf("Server : got a connection from : %s\n",inet_ntoa(addr.sin_addr));

            //Send message to client 
            char sendbuf[200] = "sending data test";
            printf("Send buf to client (0x%x) \n", &sendbuf);

            //----------------------
            // Send an initial buffer
            iResult = send(sConnection,sendbuf,(int)strlen(sendbuf),0);
            if (iResult == SOCKET_ERROR)
            {

                printf("send failed with error :%d \n", WSAGetLastError());
                closesocket(sConnection);
                WSACleanup();
                return 1;
            }            
        }
    }
 
    getchar();
    getchar();
}

