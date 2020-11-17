#include "echosc.h"

void usage(){
    cout << "syntax : echo-client <ip> <port>\n";
	cout << "sample : echo-client 192.168.0.5 1234\n";
}

void recvThread(int sd) {
    char recv[BUFFSIZE + 1];
    int size;
    while(1) {
        if(((size = read(sd, recv, BUFFSIZE)) < 0)){
            perror("read error");
			break;
        }
        recv[size] = '\0';
        cout << "&& Received message from server: " << recv << endl;
    }
    close(sd);
    exit(0);
}

int main(int argc, char* argv[]){
    int port, sd;
    struct sockaddr_in addr;

    if (argc != 3) {
        usage();
        return -1;
    }

    if(inet_pton(AF_INET, argv[1], &addr.sin_addr) <= 0) {
        perror("inet_pton error");
        return -1;
    }

    if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
		return -1;
    }
	
    port = atoi(argv[2]);
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

    if (connect(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
		return -1;
    }
	
    cout << "&& Connect Success.\n";

    thread t(recvThread, sd);
	t.detach();

    while(1) {
        string s;
		getline(cin, s);
		ssize_t res = send(sd, s.c_str(), s.size(), 0);
		if (res == 0 || res == -1) {
			cerr << "send return " << res << endl;
			perror("send");
			break;
		}
    }
    close(sd);

    printf("&& Connection terminated.\n");
}