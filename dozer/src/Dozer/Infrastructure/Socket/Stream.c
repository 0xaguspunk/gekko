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
* @Ret: char*
*
***********************************/
char* Stream_parseIbexResponse(char * stream) {

    int i, k;
    char *response;

    response = (char*) malloc(STREAM_DATA_LENGTH);

    i = STREAM_DATA_POSITION;
    k = 0;

    while(i < STREAM_LENGTH) {
        response[k] = stream[i];
        i++;
        k++;
    }

    return response;
}

/***********************************
*
* @Name: Stream_parseData
* @Def: Parses stream
* @Arg: stream: char*
* @Ret: char*
*
***********************************/
char* Stream_parseData(char *stream) {

    int i, k;
    char *response;

    response = (char*) malloc(STREAM_DATA_LENGTH);

    i = STREAM_DATA_POSITION;
    k = 0;

    while(i < STREAM_LENGTH) {
        response[k] = stream[i];
        i++;
        k++;
    }

    return response;
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

char* Stream_parseTicker(char *stream) {

    int i,k;
    char aux = ' ';
    char *ticker;

    i = STREAM_DATA_POSITION;
    ticker = (char*)malloc(sizeof(char)*6);

    while(aux != ':') {
        aux = stream[i];
        i++;
    }

    i++;
    k = 0;

    while(aux != ',') {
        aux = stream[i];
        ticker[k] = aux;
        k++;
        i++;
    }

    ticker[k-1] = '\0';

    return ticker;
}

long long Stream_parseQuantity(char *stream) {

    int i,k;
    char aux = ' ';
    char number[20];

    i = STREAM_DATA_POSITION;

    while(stream[i] != ',') {
        i++;
    }

    i++;

    while(stream[i] != ':') {
        i++;
    }

    i++;
    k = 0;

    while(aux != ',') {
        aux = stream[i];
        number[k] = aux;
        k++;
        i++;
    }

    number[k-1] = '\0';

    return atoll(number);
}

double Stream_parseWon(char *stream) {

    int i,k;
    char aux = ' ';
    char number[20];

    i = STREAM_DATA_POSITION;

    while(stream[i] != ',') {
        i++;
    }

    i++;

    while(stream[i] != ',') {
        i++;
    }

    i++;

    while(stream[i] != ':') {
        i++;
    }

    i++;
    k = 0;

    while(aux != '\0') {
        aux = stream[i];
        number[k] = aux;
        k++;
        i++;
    }

    number[k-1] = '\0';

    return atof(number);
}

/***********************************
*
* @Name: Stream_getCost
* @Def: Parses stream's cost
* @Arg: stream: char*
* @Ret: double
*
***********************************/
double Stream_getCost(char *stream) {

    int i, k;
    char number[20];
    char aux = ' ';

    i = STREAM_DATA_POSITION;
    k = 0;

    while(aux != ':') {
        aux = stream[i];
        i++;
    }

    i++;

    while(aux != '\0') {
        aux = stream[i];
        number[k] = aux;
        k++;
        i++;
    }

    number[k] = '\0';

    return atof(number);
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
