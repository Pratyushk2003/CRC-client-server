#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#define PORT 8080
char data[20],divisor[20],temp[20],total[100],c[10];
int i,j,datalen,divisorlen,len,flag=1;
int main(int argc, char const* argv[])
{
    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = { 0 };
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
   	 printf("\n Socket creation error \n");
   	 return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
   	 printf("\nInvalid address/ Address not supported \n");
   	 return -1;
    }
    if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
   	 printf("\nConnection Failed \n");
   	 return -1;
    }
    FILE *fptr;
    if((fptr = fopen("test.txt","r"))==NULL){
    printf("Error\n");
    exit(1);}	 
 	fscanf(fptr,"%d",&divisorlen);
 	divisorlen = divisorlen +1;
  	 
 	for(int i = 0;i<divisorlen;i++){
		 int b;
		 fscanf(fptr,"%d",&b);
		 if(b>0){
			 divisor[i] = '1';
		 }
		 else{
			 divisor[i] = '0';
		 }
 	}
    fscanf(fptr,"%s",c);
 	for(int i = 0;i<8*strlen(c);i++)
 	{
		 data[i] = (0 != (c[i/8] & 1 << (~i&7)));
		 if(data[i]>0){ data[i] = '1';}
		 else{data[i] = '0';}
 	}
 	printf("%s",&data);	 
 	datalen = strlen(data);  
 	printf("\n");
 	len=datalen+divisorlen-1;
	printf("%s",divisor);
  	printf("\n");   
	for(i=0;i<datalen;i++)
	{
    	total[i]=data[i];
    	temp[i]=data[i];    	 
	} 	 
	for(i=datalen;i<len;i++)        	//padded with zeroes corresponding to divlen
    	total[i]='0';  
	for(j=0;j<divisorlen;j++)
    	data[j]=total[j];
	while(j<=len)
	{
    	if(data[0]=='1')            	// in XOR ans remains as it is except in case of 1
        	for(i = 1;i <divisorlen ; i++)
               	data[i] = (( data[i] == divisor[i])?'0':'1');
    	for(i=0;i<divisorlen-1;i++)        	// left shift data word by 1 after div
        	data[i]=data[i+1];
    	data[i]=total[j++];            	// replace empty right by total
	}                            	// check for crc
 	for(i=0;i<divisorlen;i++)             	// append crc output (remainder) at end of temp
      	temp[i+datalen]=data[i];
      	printf("\ntransmitted Code Word:%s",temp);     	 
printf("\n");
    send(sock, temp, strlen(temp), 0);
    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);
    printf("Acknowledgement received");
    // closing the connected socket
    fclose(fptr);
    close(client_fd);
    return 0;
}

