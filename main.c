#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>

#define LOCALHOST "127.0.0.1"
#define PORT 8080

int main(){
    struct sockaddr_in addr;
    int sfd;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_aton(LOCALHOST, &addr.sin_addr);
    
    if((sfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar o socket");
        return 0;
    }
    if(bind(sfd, (struct sockaddr*)&addr, sizeof addr) < 0) {
        perror("Erro no bind do socket");
        return 0;
    }
    if(listen(sfd, 10) < 0) {
        perror("Erro no listen do socket");
        close(sfd);
    }
    printf("Escutando na porta: %d\n", PORT);
    for(;;) {
        int cfd = accept(sfd, NULL, NULL);
    }
}
