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
	status = client_connect(ip, port);
	if(status == -1)
		cout << "connect fail\n";
	else
		cout << "connect ok\n";
	if(!status){
		while(1){
			if(client_recv(msg))
				cout << "recv fail\n";
			else
				cout << "recv ok : " << msg << endl;
				
			cin >> msg;
			if(client_send(msg))
				cout << "sending fail\n";
			else
				cout << "sending ok\n";
		}
	}
	return 0;
}
