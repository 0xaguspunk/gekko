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

typedef struct {
    char ticker[6];
    double money;
    long long quantity;
} BuyRequest;

typedef struct {
    char ticker[6];
    long long quantity;
} SellRequest;

char * Stream_create(char * origin, char type, char * data);
IbexResponse Stream_parseIbexResponse(char * stream);
char * Stream_parseName(char * stream);
char Stream_parseType(char * stream);
BuyRequest Stream_parseBuyRequest(char* stream);
SellRequest Stream_parseSellRequest(char * stream);

#endif
