#include "Stock.h"

// Includes
#include <stdlib.h>

// Stock definition
struct StockImplementation {

    char * ticker;
    long long quantity;

};

/***********************************
*
* @Name: Stock_createStock
* @Def: Creates a stock
* @Arg: ticker: char[6], quantity: long long
* @Ret: Stock
*
***********************************/
Stock Stock_create(char * ticker, long long quantity) {

    Stock stock = (Stock)malloc(sizeof * stock);

    if (stock) {
        stock->ticker = ticker;
        stock->quantity = quantity;
    }

    return stock;
}

/***********************************
*
* @Name: Stock_setQuantity
* @Def: Modifies the quantity of a stock
* @Arg: stock: Stock, quantity: long long
*
***********************************/
void Stock_setQuantity(Stock stock, long long quantity) {
    stock->quantity = quantity;
}

/***********************************
*
* @Name: Stock_ticker
* @Def: Returns the ticker of a stock
* @Arg: stock: Stock
* @Ret: char *
*
***********************************/
char * Stock_ticker(Stock stock) {
    return stock->ticker;
}

/***********************************
*
* @Name: Stock_quantity
* @Def: Returns the quantity of a stock
* @Arg: stock: Stock
* @Ret: long long
*
***********************************/
long long Stock_quantity(Stock stock) {
    return stock->quantity;
}
