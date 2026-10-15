#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOCALHOST "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 512

int main(){ 
    struct sockaddr_in addr;
    int sfd, error;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_aton(LOCALHOST, &addr.sin_addr);
    
    if((sfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar o socket");
        return 0;
    }

    int opt = 1;
    if((setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt)) < 0) {
        perror("Erro no setsockopt");
        close(sfd);
        return 0;
    }
    
    if(error = bind(sfd, (struct sockaddr*)&addr, sizeof addr) < 0) {
        if(error == EADDRINUSE) {

        }
        perror("Erro no bind do socket");
        return 0;
    }
    if(listen(sfd, 10) < 0) {
        perror("Erro no listen do socket");
        close(sfd);
    }
    printf("Escutando na porta: %d\n", PORT);
    char cbuffer[BUFFER_SIZE] = {0};
    for(;;) {
        int cfd = accept(sfd, NULL, NULL);
        if(cfd < 0) {
            perror("Erro no accept do socket");
            close(cfd);
        }
        read(cfd, cbuffer, BUFFER_SIZE);
        if(strncmp(cbuffer, "PQP", 3) == 0) {
            printf("Worm detectado");
        } else {
            printf("%s", cbuffer);
        }
        memset(cbuffer, 0, sizeof cbuffer);
        close(cfd);
    }
    close(sfd);
}
