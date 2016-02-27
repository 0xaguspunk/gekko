#include "FileGekkoStarter.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

// Prototypes
void readStocks(int fileDescriptor, StockList stockList);
char * readTicker(int fileDescriptor);
double readPrice(int fileDescriptor);
long long readQuantity(int fileDescriptor);

/***********************************
 *
 * @Name: FileGekkoStarter_start
 * @Def: Upstarts with the first ibex information
 * @Ret: StockList
 *
 ***********************************/
StockList FileGekkoStarter_start() {

    int fileDescriptor;
    StockList stockList;

    fileDescriptor = open(PATH_STARTER_FILE, O_RDONLY);

    if (fileDescriptor < 0) {
        return 0;
    }

    stockList = StockList_create();
    readStocks(fileDescriptor, stockList);

    close(fileDescriptor);

    return stockList;
}

/***********************************
 *
 * @Name: readStocks
 * @Def: Loop that reads all stocks from a file
 * @Arg: fileDescriptor: int, stockList: StockList
 *
 ***********************************/
void readStocks(int fileDescriptor, StockList stockList) {

    char * ticker;
    double price;
    long long quantity;
    Stock stock;

    while(1) {

        ticker = readTicker(fileDescriptor);

        if (!ticker) {
            break;
        }

        price = readPrice(fileDescriptor);
        quantity = readQuantity(fileDescriptor);

        stock = Stock_create(ticker, price, quantity);

        StockList_addStock(stockList, stock);
    }
}

/***********************************
 *
 * @Name: readTicker
 * @Def: Reads ticker from file
 * @Arg: fileDescriptor: int
 * @Ret: char*
 *
 ***********************************/
char * readTicker(int fileDescriptor) {

    char * ticker;
    char aux;
    int i, bytesRead;

    i = 0;

    bytesRead = read(fileDescriptor, &aux, sizeof(char));

    if (aux == '\n' || bytesRead == 0) {
        return NULL;
    }

    ticker = (char*)malloc(sizeof(char));

    while(aux != '\t') {

        ticker[i] = aux;
        i++;
        ticker = realloc(ticker, sizeof(char)*(i+1));
        read(fileDescriptor, &aux, sizeof(char));
    }

    ticker[i] = '\0';

    return ticker;
}

/***********************************
 *
 * @Name: readPrice
 * @Def: Reads price of a ticker
 * @Arg: fileDescriptor: int
 * @Ret: double
 *
 ***********************************/
double readPrice(int fileDescriptor) {

    int i;
    char aux;
    char text[100];

    i = 0;

    read(fileDescriptor, &aux, sizeof(char));

    while(aux != '\t') {

        text[i] = aux;
        i++;
        read(fileDescriptor, &aux, sizeof(char));
    }

    text[i] = '\0';

    return atof(text);
}

/***********************************
 *
 * @Name: readQuantity
 * @Def: Reads quantity of a stock
 * @Arg: fileDescriptor: int
 * @Ret: long long
 *
 ***********************************/
long long readQuantity(int fileDescriptor) {

    char aux;
    char text[50];
    int i;

    i = 0;

    read(fileDescriptor, &aux, sizeof(char));

    while(aux != '\n') {

        text[i] = aux;
        i++;
        read(fileDescriptor, &aux, sizeof(char));
    }

    text[i] = aux;

    return atoll(text);
}
