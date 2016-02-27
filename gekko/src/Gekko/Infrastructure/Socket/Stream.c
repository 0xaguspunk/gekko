#include "Stream.h"

#include <stdlib.h>
#include <unistd.h>

// Prototypes
void setOrigin(char* stream, char* origin);
void setType(char* stream, char type);
void setData(char* stream, char* data);

/***********************************
*
* @Name: Stream_create
* @Def: Creates a stream
* @Arg: origin: char*, type: char, data: char*
* @Ret: char*
*
***********************************/
char * Stream_create(char * origin, char type, char * data) {

    char * stream;

    stream = (char*)malloc(sizeof(char) * STREAM_LENGTH);

    setOrigin(stream, origin);
    setType(stream, type);
    setData(stream, data);

    return stream;
}

/***********************************
*
* @Name: Stream_parseIbexResponse
* @Def: Parses stream
* @Arg: stream: char*
* @Ret: IbexResponse
*
***********************************/
IbexResponse Stream_parseIbexResponse(char * stream) {

    int i, k;
    char aux;
    char number[10];
    IbexResponse ibexResponse;

    i = STREAM_DATA_POSITION;
    k = 0;

    while(aux != '-' && aux != '+') {
        aux = stream[i];
        ibexResponse.ticker[k] = aux;
        i++;
        k++;
    }

    ibexResponse.ticker[k-1] = '\0';

    k = 0;
    number[k] = aux;
    k++;

    while(aux != '\0') {
        aux = stream[i];
        number[k] = aux;
        k++;
        i++;
    }

    number[k] = '\0';

    ibexResponse.increment = (double)atof(number);

    return ibexResponse;
}

/***********************************
*
* @Name: Stream_parseName
* @Def: Parses the name of a stream
* @Arg: stream: char*
* @Ret: char*
*
***********************************/
char * Stream_parseName(char * stream) {

    char * name;
    int i;

    name = (char*)malloc(sizeof(char) * STREAM_ORIGIN_LENGTH);

    for(i = 0; i < STREAM_ORIGIN_LENGTH; i++) {
        name[i] = stream[i];
    }

    return name;
}

SellRequest Stream_parseSellRequest(char * stream) {

    SellRequest sellRequest;
    int i, k;
    char aux;
    char number[20];

    i = STREAM_DATA_POSITION;
    k = 0;

    while(aux != '-') {
        aux = stream[i];
        sellRequest.ticker[k] = aux;
        i++;
        k++;
    }

    sellRequest.ticker[k-1] = '\0';

    k = 0;

    do {
        aux = stream[i];
        number[k] = aux;
        k++;
        i++;
    } while(aux != '\0');

    number[k] = '\0';

    sellRequest.quantity = (long long)atoll(number);

    return sellRequest;
}


/***********************************
*
* @Name: Stream_parseType
* @Def: Parses the type of a stream
* @Arg: stream: char*
* @Ret: char
*
***********************************/
char Stream_parseType(char * stream) {
    return stream[STREAM_TYPE_POSITION];
}

BuyRequest Stream_parseBuyRequest(char* stream) {

    BuyRequest buyRequest;
    int i, k;
    char aux;
    char number[20];

    i = STREAM_DATA_POSITION;
    k = 0;

    while(aux != '-') {
        aux = stream[i];
        buyRequest.ticker[k] = aux;
        i++;
        k++;
    }

    buyRequest.ticker[k-1] = '\0';

    k = 0;

    do {
        aux = stream[i];
        number[k] = aux;
        k++;
        i++;
    } while(aux != '-');

    number[k] = '\0';

    buyRequest.quantity = (long long)atoll(number);

    k = 0;

    while(aux != '\0') {
        aux = stream[i];
        number[k] = aux;
        i++;
        k++;
    }

    number[k] = '\0';

    buyRequest.money = (double) atof(number);

    return buyRequest;
}

/***********************************
*
* @Name: setOrigin
* @Def: Sets the origin of a stream
* @Arg: stream: char*, origin: char*
*
***********************************/
void setOrigin(char* stream, char* origin) {

    char aux;
    int i;

    i = STREAM_ORIGIN_POSITION;
    aux = origin[i];

    while(aux != '\0' && i < STREAM_ORIGIN_LENGTH) {
        stream[i] = aux;
        i++;
        aux = origin[i];
    }

    for(;i < STREAM_ORIGIN_LENGTH; i++) {
        stream[i] = '\0';
    }
}

/***********************************
*
* @Name: setType
* @Def: Sets the type of a stream
* @Arg: stream: char*, type: char
*
***********************************/
void setType(char* stream, char type) {
    stream[STREAM_TYPE_POSITION] = type;
}

/***********************************
*
* @Name: setData
* @Def: Sets the data of a stream
* @Arg: stream: char*, data: char*
*
***********************************/
void setData(char* stream, char* data) {

    char aux;
    int i, j;

    j = 0;
    i = STREAM_DATA_POSITION;
    aux = data[j];

    while(aux != '\0' && j < STREAM_DATA_LENGTH) {
        stream[i] = aux;
        i++;
        j++;
        aux = data[j];
    }

    for(i = i; i < STREAM_DATA_LENGTH; i++) {
        stream[i] = '\0';
    }
}
