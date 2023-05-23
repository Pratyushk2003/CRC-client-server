#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#define PORT 8080
char data[20],divisor[20],temp[20],total[100];
int i,j,datalen,divlen,len,flag=1;
int main(int argc, char const* argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    char* hello;
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))
   	 == 0) {
   	 perror("socket failed");
   	 exit(EXIT_FAILURE);
    }
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,  SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
   	 perror("setsockopt");
   	 exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
   	 perror("bind failed");
   	 exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
   	 perror("listen");
   	 exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address,(socklen_t*)&addrlen))
   	 < 0) {
   	 perror("accept");
   	 exit(EXIT_FAILURE);
    }
    valread = read(new_socket, total, 100);
    printf("%s\n", total);
 	size_t len = strlen(total); 
 	FILE *fptr;
 	fptr = fopen("test.txt", "r");
 	fscanf(fptr,"%d",&divlen);
 	divlen = divlen + 1;
 	for(int i = 0;i<divlen;i++){
		 int b;
		 fscanf(fptr,"%d",&b);
		 if(b>0){
			 divisor[i] = '1';
		 }
		 else{
			 divisor[i] = '0';
		 }
 	}
 	printf("%s",divisor);
 	for(j=0;j<divlen;j++)
    	data[j]=total[j];
	while(j<=len)
	{
    	if(data[0]=='1')            	// in XOR ans remains as it is except in case of 1
        	for(i = 1;i <divlen ; i++)
               	data[i] = (( data[i] == divisor[i])?'0':'1');
    	for(i=0;i<divlen-1;i++)        	// left shift data word by 1 after div
        	data[i]=data[i+1];
    	data[i]=total[j++];            	// replace empty right by total
	}
	for(i=0;i<divlen-1;i++)
    	if(data[i]=='1')
    	{
        	flag=0;
        	break;
    	}   	 
	if(flag==1)
   	 hello = "successful!!";
	else
	hello = "received data contains errors...";
	send(new_socket, hello, strlen(hello), 0);
    printf("\nAcknowledgement sent\n");
    close(new_socket);
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}

