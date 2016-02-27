#include "Controllers.h"

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

// Prototypes
void printStock(Stock stock);

/***********************************
*
* @Name: Controllers_showStock
* @Def: Show stock controller
* @Arg: user: User
*
***********************************/
void Controllers_showStock(User user) {

    StockList stockList;
    Stock stock;

    Console_printn("");

    stockList = User_stock(user);

    StockList_goToFirstNode(stockList);

    while(!StockList_endOfList(stockList)) {

        stock = StockList_stock(stockList);
        StockList_next(stockList);

        printStock(stock);
    }

    Console_printn("");
    Console_printn("");
}

/***********************************
*
* @Name: printStock
* @Def: Prints stock
* @Arg: stock: Stock
*
***********************************/
void printStock(Stock stock) {
    Console_print(Stock_ticker(stock));
    Console_printNext("-");
    Console_printDouble(Stock_quantity(stock));
    Console_printNext("\n");
}

/***********************************
*
* @Name: Controllers_showMoney
* @Def: Show money controller
* @Arg: user: User
*
***********************************/
void Controllers_showMoney(User user) {
    Console_printn("");
    Console_print("Your money: ");
    Console_printFloat(User_money(user));
    Console_printNext("\n");
    Console_printn("");
}

/***********************************
*
* @Name: Controllers_help
* @Def: Help controller
*
***********************************/
void Controllers_help() {
    Console_printn("");
    Console_printn("Dozer commands:");
    Console_printn("");
    Console_printn("show stock -> to see your acutal stock");
    Console_printn("show me the money -> to see your money");
    Console_printn("show ibex -> to check the IBEX");
    Console_printn("buy <ticker> <number> -> to buy stock");
    Console_printn("sell <ticker> <number> to sell stock");
    Console_printn("exit -> to exit Dozer");
    Console_printn("help -> to check the Dozer commands");
    Console_printn("");
}

/***********************************
*
* @Name: Controllers_exit
* @Def: Exit controller
*
***********************************/
void Controllers_exit() {
    Console_printn("");
    Console_printn("Sayonara!");
    Console_printn("");
    raise(SIGINT);
}

/***********************************
*
* @Name: Controllers_error
* @Def: Error controller
* @Arg: error: char*
*
***********************************/
void Controllers_error(char * error) {
    Console_printn("");
    Console_error(error);
    Console_printn("");
}

/***********************************
*
* @Name: Controllers_showIbex
* @Def: Shows ibex values from gekko
* @Arg: error: char*
*
***********************************/
void Controllers_showIbex(int gekkoConnection, char* name) {

    char *stream;

    stream = Stream_create(name, 'X', "Show Ibex");
    write(gekkoConnection, stream, STREAM_LENGTH);
}

void Controllers_showIbexGekkoResponse(char * data, int gekkoConnection) {

    int i;
    char stream[STREAM_LENGTH];

    Console_printn("");

    for(i = 0; i < 34; i++) {

        read(gekkoConnection, stream, STREAM_LENGTH);
        data = Stream_parseIbexResponse(stream);
        Console_printn(data);
    }

    Console_printn("");
}

/***********************************
*
* @Name: Controllers_buy
* @Def: Buys stock
* @Arg: gekkoConnection: int, user: User, ticker: char*, quantity: long long
*
***********************************/
void Controllers_buy(int gekkoConnection, User user, char* ticker, long long quantity) {

    char message[STREAM_DATA_LENGTH];
    char * stream;
    Stock stock;
    double cost;

    sprintf(message, "%s-%lld-%f", ticker, quantity, User_money(user));
    stream = Stream_create(User_name(user), 'B', message);
    write(gekkoConnection, stream, STREAM_LENGTH);

    read(gekkoConnection, stream, STREAM_LENGTH);

    Console_printn("");
    Console_printn(Stream_parseData(stream));
    Console_printn("");

    if (stream[STREAM_DATA_POSITION] == 'U') {
        return;
    }

    cost = Stream_getCost(stream);

    User_setMoney(user, User_money(user) - cost);
    stock = StockList_search(User_stock(user), ticker);

    if (stock) {
        Stock_setQuantity(stock, Stock_quantity(stock) + quantity);
        return;
    }

    stock = Stock_create(ticker, quantity);
    StockList_addStock(User_stock(user), stock);
}

void Controllers_sell(int gekkoConnection, User user, char* ticker, long long quantity) {

    char message[STREAM_DATA_LENGTH];
    char * stream;

    sprintf(message, "%s-%lld", ticker, quantity);
    stream = Stream_create(User_name(user), 'S', message);
    write(gekkoConnection, stream, STREAM_LENGTH);

    read(gekkoConnection, stream, STREAM_LENGTH);

    Console_printn("");
    Console_printn(Stream_parseData(stream));
    Console_printn("");
}

void Controllers_bought(char * stream, User user) {

    char* ticker;
    long long quantity;
    double won;
    Stock stock;

    ticker = Stream_parseTicker(stream);
    quantity = Stream_parseQuantity(stream);
    won = Stream_parseWon(stream);

    stock = StockList_search(User_stock(user), ticker);

    if (!stock) {
        Console_error("Ticker no longer exists");
        return;
    }

    Stock_setQuantity(stock, Stock_quantity(stock) - quantity);
    User_setMoney(user, User_money(user) + won);
}