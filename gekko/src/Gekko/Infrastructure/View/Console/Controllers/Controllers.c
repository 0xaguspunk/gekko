#include "Controllers.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

// Prototypes
void Controllers_logOut(DozerConnection dozerConnection);
void Controllers_buy(
        DozerConnection dozerConnection,
        char* request,
        StockList stockList,
        void (*stockLock)(),
        void (*stockUnlock)(),
        SellerList sellerList,
        void (*sellerLock)(),
        void (*sellerUnlock)()
);

void Controllers_ibex35(DozerConnection dozerConnection, StockList stockList, void (*lock)(), void (*unlock)());
void Controllers_sell(DozerConnection dozerConnection,
                      char * request,
                      StockList stockList,
                      void (*stockLock)(),
                      void (*stockUnlock)(),
                      SellerList sellerList,
                      void (*sellerLock)(),
                      void (*sellerUnlock)()
);

void informSeller(char * ticker, long long quantity, double price, SellerList sellerList, void (*sellerLock)(), void (*sellerUnlock)());
void findSellDone(DozerConnection dozerConnection, SellerList sellerList, void (*sellerLock)(), void (*sellerUnlock)());

/***********************************
*
* @Name: Controllers_mainController
* @Def: Main controller of the session
* @Arg: dozerConnection: DozerConnection, stockList: StockList, lock: void*, unlock: void*
*
***********************************/
void Controllers_mainController(DozerConnection dozerConnection,
                                StockList stockList,
                                void (*stockLock)(),
                                void (*stockUnlock)(),
                                SellerList sellerList,
                                void (*sellerLock)(),
                                void (*sellerUnlock)()
) {

    char stream[STREAM_LENGTH];
    int error;
    char type;

    Console_print("Dozer user ");
    Console_printNext(dozerConnection.name);
    Console_printNext(" connected\n");

    findSellDone(dozerConnection, sellerList, (void*) sellerLock, (void*) sellerUnlock);

    while (1) {

        // Bucle read
        error = read(dozerConnection.connection, stream, STREAM_LENGTH);

        if (error < STREAM_LENGTH) {
            Console_error("Lost connection with dozer");
            pthread_exit(0);
        }

        type = Stream_parseType(stream);

        // Select Option
        switch (type) {

            case 'X':
                Controllers_ibex35(dozerConnection, stockList, (void*) stockLock, (void*) stockUnlock);
                break;

            case 'B':
                Controllers_buy(dozerConnection, stream, stockList, (void*) stockLock, (void*) stockUnlock, sellerList, (void*) sellerLock, (void*) sellerUnlock);
                break;

            case 'S':
                Controllers_sell(dozerConnection, stream, stockList, (void*) stockLock, (void*) stockUnlock, sellerList, (void*) sellerLock, (void*) sellerUnlock);
                break;

            case 'Q':
                Controllers_logOut(dozerConnection);
                break;

            default:
                Console_printn("Unknow option");
                break;
        }
    }
}

void findSellDone(DozerConnection dozerConnection, SellerList sellerList, void (*sellerLock)(), void (*sellerUnlock)()) {

    Seller seller;
    char message[STREAM_LENGTH];
    char * stream;

    Console_printn("Checking stand by sales");

    (*sellerLock)();
    SellerList_goToFirstNode(sellerList);

    while(!SellerList_endOfList(sellerList)) {

        seller = SellerList_seller(sellerList);

        if (strcmp(Seller_name(seller), dozerConnection.name) == 0 && Seller_connection(seller) == -1) {
            Seller_setConnection(seller, -2);
            sprintf(message, "Sold ticker: %s, quantity: %lld, value: %.2f", Seller_ticker(seller), Seller_quantity(seller), Seller_quantity(seller)*Seller_price(seller));
            stream = Stream_create("Gekko", 'M', message);
            write(dozerConnection.connection, stream, STREAM_LENGTH);
        }

        SellerList_next(sellerList);
    }

    (*sellerUnlock)();
}

/***********************************
*
* @Name: Controllers_sell
* @Def: Sells stock
* @Arg: dozerConnection: DozerConnection, stockList: StockList, lock: void*, unlock: void*
*
***********************************/
void Controllers_sell(DozerConnection dozerConnection,
                                char * request,
                                StockList stockList,
                                void (*stockLock)(),
                                void (*stockUnlock)(),
                                SellerList sellerList,
                                void (*sellerLock)(),
                                void (*sellerUnlock)()
) {

    SellRequest sellRequest;
    Seller seller;
    Stock stock;
    char* stream;

    Console_printn("Dozer wants to sell ticker");

    sellRequest = Stream_parseSellRequest(request);

    // We add the stock
    (*stockLock)();
    stock = StockList_search(stockList, sellRequest.ticker);
    (*stockUnlock)();

    if (!stock) {
        stream = Stream_create("Gekko", 'B', "Unable to find ticker");
        write(dozerConnection.connection, stream, STREAM_LENGTH);
        return;
    }

    (*stockLock)();
    Stock_setQuantity(stock, Stock_quantity(stock) + sellRequest.quantity);
    (*stockUnlock)();


    seller = Seller_create(dozerConnection.name, sellRequest.ticker, sellRequest.quantity, dozerConnection.connection);

    (*sellerLock)();
    SellerList_addSeller(sellerList, seller);
    (*sellerUnlock)();

    Console_printn("Ticker on market");

    stream = Stream_create("Gekko", 'B', "Ticker on market");
    write(dozerConnection.connection, stream, STREAM_LENGTH);
}

/***********************************
*
* @Name: Controllers_buy
* @Def: Buys stock
* @Arg: dozerConnection: DozerConnection, stockList: StockList, lock: void*, unlock: void*
*
***********************************/
void Controllers_buy(
        DozerConnection dozerConnection,
        char* request,
        StockList stockList,
        void (*stockLock)(),
        void (*stockUnlock)(),
        SellerList sellerList,
        void (*sellerLock)(),
        void (*sellerUnlock)()
) {

    BuyRequest buyRequest;
    Stock stock;
    char * stream;
    double price;
    char message[STREAM_DATA_LENGTH];

    buyRequest = Stream_parseBuyRequest(request);

    Console_printn("Dozer wants to buy stock");

    (*stockLock)();
    stock = StockList_search(stockList, buyRequest.ticker);

    if (!stock) {
        (*stockUnlock)();
        stream = Stream_create("Gekko", 'B', "Unable to find ticker");
        write(dozerConnection.connection, stream, STREAM_LENGTH);
        return;
    }

    if (Stock_quantity(stock) < buyRequest.quantity) {
        (*stockUnlock)();
        stream = Stream_create("Gekko", 'B', "Unable to buy that quantity");
        write(dozerConnection.connection, stream, STREAM_LENGTH);
        return;
    }

    price = Stock_price(stock) * buyRequest.quantity;

    if (price > buyRequest.money) {
        (*stockUnlock)();
        stream = Stream_create("Gekko", 'B', "Unable to buy that quantity, need more money");
        write(dozerConnection.connection, stream, STREAM_LENGTH);
        return;
    }

    Stock_setQuantity(stock, Stock_quantity(stock) - buyRequest.quantity);
    informSeller(buyRequest.ticker, buyRequest.quantity, Stock_price(stock), sellerList, (void*) sellerLock, (void*) sellerUnlock);
    (*stockUnlock)();

    sprintf(message, "Bought, cost: %.2f", price);
    stream = Stream_create("Gekko", 'B', message);
    write(dozerConnection.connection, stream, STREAM_LENGTH);
    return;
}

void informSeller(char * ticker, long long quantity, double price, SellerList sellerList, void (*sellerLock)(), void (*sellerUnlock)()) {

    Seller seller;
    char* stream;
    char* message;
    int error;

    message = (char*)malloc(sizeof(char)*STREAM_DATA_LENGTH);

    (*sellerLock)();
    SellerList_goToFirstNode(sellerList);

    Console_printn("Informing seller");

    while(!SellerList_endOfList(sellerList) && quantity > 0) {

        seller = SellerList_seller(sellerList);

        if (strcmp(Seller_ticker(seller), ticker) == 0  && Seller_quantity(seller) <= quantity && Seller_connection(seller) > 0) {

            Console_printn("Seller found!");

            sprintf(message, "Sold ticker: %s, quantity: %lld, value: %.2f", ticker, Seller_quantity(seller), Seller_quantity(seller)*price);
            stream = Stream_create("Gekko", 'M', message);
            error = write(Seller_connection(seller), stream, STREAM_LENGTH);

            if (error < STREAM_LENGTH) {
                Seller_setPrice(seller, price);
                Seller_setConnection(seller, -1);
            } else {
                Seller_setConnection(seller, -2);
            }
        }

        SellerList_next(sellerList);
    }

    (*sellerUnlock)();
}

/***********************************
*
* @Name: Controllers_ibex35
* @Def: Sends ibex update to the connected dozer
* @Arg: dozerConnection: DozerConnection, stockList: StockList, lock: void*, unlock: void*
*
***********************************/
void Controllers_ibex35(DozerConnection dozerConnection, StockList stockList, void (*lock)(), void (*unlock)()) {

    int i;
    Stock stock;
    char message[STREAM_DATA_LENGTH];
    char *stream;

    Console_printn("Dozer wants to check the IBEX 35");

    (*lock)();

    StockList_goToFirstNode(stockList);

    for(i = 0; i < 35; i++) {
        stock = StockList_stock(stockList);
        sprintf(message, "%s - %lld - %.2f", Stock_ticker(stock), Stock_quantity(stock), Stock_price(stock));
        stream = Stream_create("Gekko", 'X', message);
        write(dozerConnection.connection, stream, STREAM_LENGTH);
        StockList_next(stockList);
    }

    (*unlock)();

    Console_printn("IBEX 35 update sent to Dozer");
}

/***********************************
*
* @Name: Controllers_logOut
* @Def: Logs out the connected dozer
* @Arg: dozerConnection: DozerConnection
*
***********************************/
void Controllers_logOut(DozerConnection dozerConnection) {
    Console_print("Dozer user ");
    Console_printNext(dozerConnection.name);
    Console_printNext(" logged out\n");
    close(dozerConnection.connection);
    pthread_exit(0);
    return;
}
