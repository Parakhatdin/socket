#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define PORT 8080

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

typedef struct server
{
    int main_socket;
    struct sockaddr_in address;
} server;


server webSocket()
{
    server result;
    int main_socket;
    struct sockaddr_in address;

    printf("\n%s...tring create main socket\n", KYEL);
    if ((main_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In main socket create");
        exit(EXIT_FAILURE);
    }
    printf("%ssuccess! (main socket created)\n", KGRN);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    printf("\n%s...tring to bind\n", KYEL);
    if (bind(main_socket, (struct sockaddr *) &address, sizeof(address)) < 0)
    {
        perror("in bind");
        exit(EXIT_FAILURE);
    }
    printf("%ssuccess! (main socket bainded)\n", KGRN);
    
    printf("\n%s...tring to listen\n", KYEL);
    if (listen(main_socket, 10) < 0)
    {
        perror("in listen");
        exit(EXIT_FAILURE);
    }
    printf("%ssuccess! (main socket is listening)\n", KGRN);

    result.address = address;
    result.main_socket = main_socket;
    return result;
}

int main(int argc, char const *argv[])
{
    server main_socket = webSocket();

    int new_socket;
    int address_len = sizeof(main_socket.address);
    char buffer[30000] = {0};
    int valread;

    while (1)
    {
        strcpy(buffer, "");
        printf("\n%s...WAITING FOR CONNECTION...\n", KBLU);
        if ((new_socket = accept(main_socket.main_socket, (struct sockaddr *) &main_socket.address, (socklen_t*)&address_len)) < 0)
        {
            perror("in listen");
            exit(EXIT_FAILURE);
        }

        valread = read( new_socket , buffer, 30000);
        printf("%s\n",buffer );
        write(new_socket, buffer, strlen(buffer));
        close(new_socket);
    }
    close(main_socket.main_socket);
    return 0;
}
