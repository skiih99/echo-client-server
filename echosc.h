#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <thread>
#include <vector>

using namespace std;

#define  BUFFSIZE   65536
#define  LISTENQ    10
