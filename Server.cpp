#include <bits/stdc++.h>
#include <windows.h>

int addrlen;

#include "Server.h"
#include "Client.h"

using namespace std;

signed main(){
	char ip[16], msg[256];
	int port, status;
	socket_init();
	cin >> ip >> port;
	status = server_connect(ip, port);
	if(status == -1)
		cout << "invalid socket\n";
	else if(status == -2)
		cout << "socket error\n";
	else if(status == -3)
		cout << "listen error\n";
	else if(status == 1)
		cout << "connect error\n";
	else
		cout << "connect ok\n";
	if(!status){
		while(1){
			cin >> msg;
			if(server_send(msg))
				cout << "sending fail\n";
			else
				cout << "sending ok\n";
			if(server_recv(msg))
				cout << "recv fail\n";
			else
				cout << "recv ok : " << msg << endl;
		}
	}
	return 0;
}
