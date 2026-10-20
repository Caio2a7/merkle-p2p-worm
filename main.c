#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 32

struct sockaddr_in addr;
int error;

int create_socket(port, host) {
    int sfd;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_aton(host, &addr.sin_addr);
    
    if((sfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar o socket");
        return 0;
    }
    return sfd
}

int main(){ 
    int sfd = create_socket(8080, "127.0.0.1");     

    int opt = 1;
    if((setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt)) < 0) {
        perror("Erro no setsockopt");
        close(sfd);
        return 0;
    }
    
    if(error = bind(sfd, (struct sockaddr*)&addr, sizeof addr) < 0) {
        if(errno == EADDRINUSE) {
            printf("Testando se porta ocupada é pelo amiguinho\n");
            if(connect(sfd, (struct sockaddr*)&addr, sizeof addr) < 0) {
                perror("Erro no connect do socket");
                close(sfd);
            }
            if(send(sfd, "PQP", 3, MSG_EOR) == (ssize_t)3) {
                printf("Enviado palavra mágica\n");
            }
            char wbuffer[256] = {0};
            int c;
            for(;;) {
                printf("Digite sua mensagem: ");
                scanf("%s", wbuffer);
                while((c = getchar()) != '\n') {}
                if(connect(sfd, (struct sockaddr*)&addr, sizeof addr) < 0) {
                    perror("Erro na conexão do socket");
                    close(sfd);
                }
                if(send(sfd, wbuffer, sizeof wbuffer, MSG_EOR)  != (ssize_t)3) {
                    perror("Mensagem não enviada");
                }
                memset(wbuffer, 0, sizeof wbuffer);
            }
        }
        perror("Erro no bind do socket");
        return 0;
    }
    if(listen(sfd, 10) < 0) {
        perror("Erro no listen do socket");
        close(sfd);
    }
    printf("Escutando na porta: %d\n", 8080);
    char cbuffer[BUFFER_SIZE] = {0};
    for(;;) {
        int cfd = accept(sfd, NULL, NULL);
        if(cfd < 0) {
            perror("Erro no accept do socket");
            close(cfd);
        }
        read(cfd, cbuffer, BUFFER_SIZE);

        char magic_message[2] = {0};
        int magic_count = 0;
        for(int i = 0; i < sizeof(cbuffer); i++) {
            switch(cbuffer[i]) {
                case 'P':
                    magic_message[magic_count] = 'P';
                    magic_count = 2;
                    break;
                case 'Q':
                    magic_message[1] = 'Q';
                    break; 
                case '\n':
                    cbuffer[i] = 's';
                    break;
                case '\t':
                    cbuffer[i] = '\0';
                    break;
                default:
                    break;
            }
        }
        if(strncmp(magic_message, "PQP", 3) == 0) {
            printf("WORM amigo detectado\n");
        } else {
            printf("Mensagem: %s\n", magic_message);
        }

        memset(cbuffer, 0, sizeof cbuffer);
        close(cfd);
    }
    close(sfd);
}
