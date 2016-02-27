#include "SocketTumblingDiceConnection.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

// Implementations
struct TdConnectionImplementation {
    float refresh;
    char * IP;
    int port;
};

// Prototypes
TdConnectionConfig createTdConnectionConfig(float refresh, char* ip, int port);
float readRefresh(int fileDescriptor);
char * readIp(int fileDescriptor);
int readPort(int fileDescriptor);
int askTumblingDiceForIbexUpdate(int tumblingDiceConnection);
int processTumblingDiceIbexUpdate(int tumblingDiceConnection, StockList stockList, void (*lock)(), void (*unlock)());

/***********************************
*
* @Name: SocketTumblingDiceConnection_configure
* @Def: Reads the configuration to connect to TumblingDice
* @Ret: TdConnectionConfig
*
***********************************/
TdConnectionConfig SocketTumblingDiceConnection_configure() {

    int fileDescriptor, port;
    float refresh;
    char * ip;
    TdConnectionConfig tdConnectionConfig;

    fileDescriptor = open(PATH_TD_CONNECTION_CONFIGURATION, O_RDONLY);

    if (fileDescriptor < 0) {
        return 0;
    }

    refresh = readRefresh(fileDescriptor);
    ip = readIp(fileDescriptor);
    port = readPort(fileDescriptor);

    tdConnectionConfig = createTdConnectionConfig(refresh, ip, port);

    close(fileDescriptor);

    return tdConnectionConfig;
}

/***********************************
*
* @Name: SocketTumblingDiceConnection_configRefresh
* @Def: Returns the refresh from the configuration
* @Arg: tdConnectionConfig: TdConnectionConfig
* @Ret: float
*
***********************************/
float SocketTumblingDiceConnection_configRefresh(TdConnectionConfig tdConnectionConfig) {
    return tdConnectionConfig->refresh;
}

/***********************************
*
* @Name: SocketTumblingDiceConnection_connect
* @Def: Connects to Tumbling Dice
* @Arg: tdConnectionConfig: TdConnectionConfig, tumblingConnection: int
* @Ret: int
*
***********************************/
int SocketTumblingDiceConnection_connect(TdConnectionConfig tdConnectionConfig, int *tumblingDiceConnection) {

    struct sockaddr_in tumblingDiceAddress;
    char *stream;

    *tumblingDiceConnection = socket(AF_INET, SOCK_STREAM, 0);

    if (*tumblingDiceConnection == -1) {
        return -1;
    }

    tumblingDiceAddress.sin_addr.s_addr = inet_addr(tdConnectionConfig->IP);
    tumblingDiceAddress.sin_family = AF_INET;
    tumblingDiceAddress.sin_port = htons(tdConnectionConfig->port);

    if(connect(*tumblingDiceConnection, (struct sockaddr *)&tumblingDiceAddress, sizeof(tumblingDiceAddress)) < 0) {
        return -1;
    }

    stream = Stream_create("Gekko", 'C', "CONNEXIO");

    write(*tumblingDiceConnection, stream, STREAM_LENGTH);

    return read(*tumblingDiceConnection, stream, STREAM_LENGTH);
}

/***********************************
*
* @Name: SocketTumblingDiceConnection_disconnect
* @Def: Disconnects from Tumbling Dice
* @Arg: tumblingConnection: int
*
***********************************/
void SocketTumblingDiceConnection_disconnect(int tumblingDiceConnection) {

    char * stream;

    stream = Stream_create("Gekko", 'Q', "DESCONNEXIO");
    write(tumblingDiceConnection, stream, STREAM_LENGTH);
    close(tumblingDiceConnection);
}

/***********************************
*
* @Name: SocketTumblingDiceConnection_refreshIbexStock
* @Def: Updates ibex stock
* @Arg: tumblingDiceConnection: int, stockList: StockList, lock: void*, unlock: void*
* @Ret: int
*
***********************************/
int SocketTumblingDiceConnection_refreshIbexStock(int tumblingDiceConnection, StockList stockList, void (*lock)(), void (*unlock)()) {

    int error;

    error = askTumblingDiceForIbexUpdate(tumblingDiceConnection);

    if (error < -1) {
        return -1;
    }

    return processTumblingDiceIbexUpdate(tumblingDiceConnection, stockList, lock, unlock);
}

/***********************************
*
* @Name: createTdConnectionConfig
* @Def: Creates an instance of TdConnectionConfig
* @Arg: refresh: float, ip: char*, port: int
* @Ret: TdConnectionConfig
*
***********************************/
TdConnectionConfig createTdConnectionConfig(float refresh, char* ip, int port) {

    TdConnectionConfig tdConnectionConfig = (TdConnectionConfig)malloc(sizeof * tdConnectionConfig);

    if (tdConnectionConfig) {
        tdConnectionConfig->refresh = refresh;
        tdConnectionConfig->IP = ip;
        tdConnectionConfig->port = port;
    }

    return tdConnectionConfig;
}

/***********************************
*
* @Name: readRefresh
* @Def: Reads the Gekko port
* @Arg: fileDescriptor: int
*
***********************************/
float readRefresh(int fileDescriptor) {

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

    return atof(text);
}

/***********************************
*
* @Name: readIP
* @Def: Reads the Gekko IP
* @Arg: fileDescriptor: int
* @Ret: char*
*
***********************************/
char * readIp(int fileDescriptor) {

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

/***********************************
*
* @Name: askTumblingDiceForIbexUpdate
* @Def: Asks for ibex update
* @Arg: tumblingDiceConnection: int
* @Ret: int
*
***********************************/
int askTumblingDiceForIbexUpdate(int tumblingDiceConnection) {

    char *stream;
    int error;

    stream = Stream_create("Gekko", 'P', "PETICIO");

    error = write(tumblingDiceConnection, stream, sizeof(char)*STREAM_LENGTH);

    if (error != 115) {
        return -1;
    }

    return 0;
}

/***********************************
*
* @Name: processTumblingDiceIbexUpdate
* @Def: Updates the stock list with the new values of the ibex
* @Arg: tumblingDiceConnection: int, stockList: StockList, lock: void*, unlock: void*
* @Ret: int
*
***********************************/
int processTumblingDiceIbexUpdate(int tumblingDiceConnection, StockList stockList, void (*lock)(), void (*unlock)()) {

    int error, ibexCount;
    char * stream;
    IbexResponse ibexResponse;
    Stock stock;

    ibexCount = 0;
    stream = (char*)malloc(sizeof(char)*STREAM_LENGTH);

    while(ibexCount < 35) {

        error = read(tumblingDiceConnection, stream, sizeof(char)*STREAM_LENGTH);

        if (error != 115) {
            return -1;
        }

        ibexResponse = Stream_parseIbexResponse(stream);
        (*lock)();
        stock = StockList_search(stockList, ibexResponse.ticker);
        Stock_updatePrice(stock, ibexResponse.increment);
        (*unlock)();
        ibexCount++;
    }

    return 0;
}
