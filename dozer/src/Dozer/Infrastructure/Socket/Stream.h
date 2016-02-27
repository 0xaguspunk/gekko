#ifndef _Stream_h_
#define _Stream_h_

#define STREAM_LENGTH 115
#define STREAM_ORIGIN_LENGTH 14
#define STREAM_ORIGIN_POSITION 0
#define STREAM_TYPE_LENGTH 1
#define STREAM_TYPE_POSITION 14
#define STREAM_DATA_LENGTH 100
#define STREAM_DATA_POSITION 15

typedef struct {
    char ticker[6];
    double increment;
} IbexResponse;

char * Stream_create(char * origin, char type, char * data);
char* Stream_parseIbexResponse(char * stream);
char* Stream_parseData(char *stream);
double Stream_getCost(char *stream);
char Stream_parseType(char * stream);
char* Stream_parseTicker(char *stream);
long long Stream_parseQuantity(char *stream);
double Stream_parseWon(char *stream);

#endif
