#include "GekkoServer.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/***********************************
*
* @Name: GekkoServer_startServer
* @Def: Starts Gekko Server
* @Arg: serverConnection: int*
* @Ret: int
*
***********************************/
int GekkoServer_startServer(int* serverConnection) {

    struct sockaddr_in serv_addr;

    *serverConnection = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (*serverConnection < 0) {
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(SERVER_PORT);

    if (bind(*serverConnection, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        return -1;
    }

    listen(*serverConnection, SERVER_CONNECTIONS);

    return 0;
}

/***********************************
*
* @Name: GekkoServer_acceptConnection
* @Def: Gets Gekko Server connection
* @Arg: serverConnection: int, name: char*
* @Ret: char*
*
***********************************/
char* GekkoServer_acceptConnection(int serverConnection, int* dozerConnection) {

    struct sockaddr_in cli_addr;
    socklen_t len;
    int error;
    char* stream;
    char * name;

    len = sizeof(cli_addr);

    *dozerConnection = accept(serverConnection, (void *) &cli_addr, &len);

    stream = (char*)malloc(sizeof(char)*STREAM_LENGTH);

    error = read(*dozerConnection, stream, STREAM_LENGTH);

    if (error < STREAM_LENGTH) {
        exit(-1);
    }

    name = Stream_parseName(stream);

    stream = Stream_create("Gekko", 'O', "CONNEXIO OK");
    write(*dozerConnection, stream, STREAM_LENGTH);

    free(stream);

    return name;
}
