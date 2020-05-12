#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define SERVICE_PORT	5050

using namespace std;

class FtpServer
{
	int svc;        /* listening socket providing service */
	int rqst;       /* socket accepting the request */
	socklen_t alen;       /* length of address structure */
	struct sockaddr_in client_addr;  /* client's address */
	int port = SERVICE_PORT;

public:
	FtpServer(){
		struct sockaddr_in my_addr;    /* address of this service */
		int sockoptval = 1;
		char hostname[128];
		gethostname(hostname, 128);

		if ((svc = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			perror("cannot create socket");
			exit(1);
		}

		setsockopt(svc, SOL_SOCKET, SO_REUSEADDR, &sockoptval, sizeof(int));

		/* set up our address */

		memset((char*)&my_addr, 0, sizeof(my_addr));  /* 0 out the structure */
		my_addr.sin_family = AF_INET;   /* address family */
		my_addr.sin_port = htons(port);
		my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

		/* bind to the address to which the service will be offered */
		if (bind(svc, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
			perror("bind failed");
			exit(1);
		}

		/* set up the socket for listening with a queue length of 5 */
		if (listen(svc, 5) < 0) {
			perror("listen failed");
			exit(1);
		}

		printf("SERVER STARTED ON [%s], LISTENING ON PORT [%d].\n", hostname, port);

		alen = sizeof(client_addr);     /* length of address */
	}

	void start(){
		/* endless loop */
		for (;;) {
			rqst = accept(svc,(struct sockaddr *)&client_addr, &alen);		
			if(rqst == -1){
				perror("accept");
				continue;
			}

			printf("[%s][%d]: Connection Received.\n",
				inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
	        
			if(!fork()){	// child process
				close(svc);	// no need of listener
				dup2(rqst, 0);	// copies rqst fd to stdin
				execl("serve", "serve", 0);	// runs the executable serve
				exit(0);
			}
			close(rqst);
		}
		close(svc);
	}

	~FtpServer(){}
};

int main()
{
	FtpServer server;

	server.start();

	return 0;
}
