#include "echosc.h"

bool broadcast = false;
bool echo = false;

vector<int> clientsd;

void usage(){
    cout << "syntax : echo-server <port> [-e [-b]]\n";
	cout << "sample : echo-server 1234 -e -b\n";
}

void recvThread(int sd) {
    char recv[BUFFSIZE + 1];
    int size;
    while(1) {
        if(((size = read(sd, recv, BUFFSIZE)) <= 0)){
            perror("read error");
			break;
        }
        recv[size] = '\0';
        cout << "&& Received message from client: " << recv << endl;

        if(broadcast) {
            for(int tmpsd : clientsd) {
                if(write(tmpsd, recv, strlen(recv)) != strlen(recv)) {
                    perror("send error");
                    break;
                }
            }
            cout << "%% Send message to all clients.\n";
        }
        else if(echo) {
            if(write(sd, recv, strlen(recv)) != strlen(recv)) {
                perror("send error");
                break;
            }
        }       
    }
    cout << "&& Client disconnected.\n";
    close(sd);
    
    vector<int>::iterator iter = clientsd.begin();
    for (int i = 0; i < clientsd.size(); i++) {
        if(clientsd[i] == sd) {
            clientsd.erase(iter + i);
        }
    }
        
    
}

int main(int argc, char *argv[]) {
    int port, listensd;
    struct sockaddr_in addr;

    if (argc > 4 || argc < 2) {
        usage();
        return -1;
    }

    port = atoi(argv[1]);

    if (argc > 2) {
        for (int i = 2; i < argc; i++) {
            if(!strcmp(argv[i], "-e")) {
                echo = true;
                continue;
            }
            if(!strcmp(argv[i], "-b")) {
                broadcast = true;
                continue;
            }
        }
    }

    if((listensd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
		return -1;
    }

    int optval = 1;
	int res = setsockopt(listensd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (res == -1) {
		perror("setsockopt error");
		return -1;
	}

    addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
    memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

    if(bind(listensd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind error");
		return -1;
    }

    if (listen(listensd, LISTENQ) < 0) {
        perror("listen error");
        return -1;
    }

    cout << "&& Server, ready for connected. Port number: " << port << endl;

    while(1) {
        int cli_sd;
        struct sockaddr_in cli_addr;
        socklen_t len = sizeof(cli_addr);

        if((cli_sd = accept(listensd, (struct sockaddr *)&cli_addr, &len)) < 0) {
            perror("accept error");
            break;
        }
        cout << "&& Client connect! sd: " << cli_sd << endl;
        clientsd.push_back(cli_sd);
        thread* t = new thread(recvThread, cli_sd);
        t->detach();
    }

    close(listensd);

    return 0;

}