all: echo_server echo_client

echo_server: echo_server.o
	g++ -o echo_server echo_server.o -lpthread

echo_client: echo_client.o
	g++ -o echo_client echo_client.o -lpthread

echo_server.o: echosc.h server.cpp
	g++ -c -o echo_server.o server.cpp

echo_client.o: echosc.h client.cpp
	g++ -c -o echo_client.o client.cpp

clean:
	rm -f echo_server
	rm -f echo_client
	rm -f *.o