#include "SocketGekkoConnection.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

// Implementations
struct GekkoConnectionConfigImplementation {
    char * IP;
    int port;
};

// Prototypes
GekkoConnectionConfig createGekkoConnectionConfig(char * ip, int port);
char * readIP(int fileDescriptor);
int readPort(int fileDescriptor);

/***********************************
*
* @Name: SocketGekkoConnection_configure
* @Def: Reads the configuration to connect to Gekko
* @Ret: GekkoConnectionConfig
*
***********************************/
GekkoConnectionConfig SocketGekkoConnection_configure() {

    int fileDescriptor, port;
    char * ip;
    GekkoConnectionConfig gekkoConnectionConfig;

    fileDescriptor = open(PATH_GEKKO_CONNECTION_CONFIGURATION, O_RDONLY);

    if (fileDescriptor < 0) {
        return 0;
    }

    ip = readIP(fileDescriptor);
    port = readPort(fileDescriptor);

    gekkoConnectionConfig = createGekkoConnectionConfig(ip, port);

    close(fileDescriptor);

    return gekkoConnectionConfig;
}

/***********************************
*
* @Name: SocketGekkoConnection_connect
* @Def: Connects to Gekko server
* @Arg: gekkoConnectionConfig: GekkoConnectionConfig, gekkoConnection: int*
* @Ret: int
*
***********************************/
int SocketGekkoConnection_connect(GekkoConnectionConfig gekkoConnectionConfig, int *gekkoConnection, char* name) {

    struct sockaddr_in gekkoAddress;
    char *stream;

    *gekkoConnection = socket(AF_INET, SOCK_STREAM, 0);

    if (*gekkoConnection == -1) {
        return -1;
    }

    gekkoAddress.sin_addr.s_addr = inet_addr(gekkoConnectionConfig->IP);
    gekkoAddress.sin_family = AF_INET;
    gekkoAddress.sin_port = htons(gekkoConnectionConfig->port);

    if(connect(*gekkoConnection, (struct sockaddr *)&gekkoAddress, sizeof(gekkoAddress)) < 0) {
        return -1;
    }

    stream = Stream_create(name, 'C', "CONNEXIO");

    write(*gekkoConnection, stream, STREAM_LENGTH);

    return 1;
}

/***********************************
*
* @Name: SocketGekkoConnection_disconnect
* @Def: Disconnects from Gekko server
* @Arg: gekkoConnection: int
*
***********************************/
void SocketGekkoConnection_disconnect(int gekkoConnection, char* name) {

    char * stream;

    stream = Stream_create(name, 'Q', "DESCONNEXIO");
    write(gekkoConnection, stream, STREAM_LENGTH);
    close(gekkoConnection);
}

/***********************************
*
* @Name: createGekkoConnectionConfig
* @Def: Creates an instance of GekkoConnectionConfig
* @Arg: ip: char*, port: int
* @Ret: GekkoConnectionConfig
*
***********************************/
GekkoConnectionConfig createGekkoConnectionConfig(char * ip, int port) {

    GekkoConnectionConfig gekkoConnectionConfig = (GekkoConnectionConfig)malloc(sizeof * gekkoConnectionConfig);

    if (gekkoConnectionConfig) {
        gekkoConnectionConfig->IP = ip;
        gekkoConnectionConfig->port = port;
    }

    return gekkoConnectionConfig;
}

/***********************************
*
* @Name: readIP
* @Def: Reads the Gekko IP
* @Arg: fileDescriptor: int
* @Ret: char*
*
***********************************/
char * readIP(int fileDescriptor) {

    char * ip;
    int i;
    char aux;

    i = 0;
    read(fileDescriptor, &aux, sizeof(char));
    ip = (char*)malloc(sizeof(char));

    while(aux != '\n') {

        ip[i] = aux;
        i++;
        ip = realloc(ip, sizeof(char)*(i+1));
        read(fileDescriptor, &aux, sizeof(char));
    }

    ip[i] = '\0';

    return ip;
}

/***********************************
*
* @Name: readPort
* @Def: Reads the Gekko port
* @Arg: fileDescriptor: int
* @Ret: int
*
***********************************/
int readPort(int fileDescriptor) {

    int i = 0;
    char aux;
    char text[100];

    read(fileDescriptor, &aux, sizeof(char));

    while(aux != '\n') {

        text[i] = aux;
        i++;
        read(fileDescriptor, &aux, sizeof(char));
    }

    text[i] = '\0';

    return atoi(text);
}
