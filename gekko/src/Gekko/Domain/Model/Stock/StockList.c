#include "StockList.h"

// Includes
#include <stdlib.h>
#include <string.h>

// StockList definition
typedef struct n {

    Stock stock;
    struct n * next;

} Node;

struct StockListImplementation {

    Node * first;
    Node * pointOfInterest;
};

/***********************************
*
* @Name: StockList_create
* @Def: Creates a stock list with a ghost
* @Ret: StockList
*
***********************************/
StockList StockList_create() {

    StockList stockList;
    Node * ghostNode;

    stockList = (StockList)malloc(sizeof * stockList);

    if (!stockList) {
        return NULL;
    }

    ghostNode = (Node*)malloc(sizeof(Node));

    if(!ghostNode) {
        free(stockList);
        return NULL;
    }

    stockList->pointOfInterest = stockList->first = ghostNode;
    ghostNode->next = NULL;

    return stockList;
}

/***********************************
*
* @Name: StockList_destroy
* @Def: Destroys a list
* @Arg: stockList: StockList
*
***********************************/
void StockList_destroy(StockList stockList) {

    StockList_goToFirstNode(stockList);

    while(!StockList_endOfList(stockList)) {
        StockList_removeStock(stockList);
    }

    // Remove ghost
    free(stockList->first);

    free(stockList);
}

/***********************************
*
* @Name: StockList_addStock
* @Def: Adds stock to the list
* @Arg: stockList: StockList
*		stock: Stock
* @Ret: int
*
***********************************/
int StockList_addStock(StockList stockList, Stock stock) {

    Node * aux;

    aux = (Node*)malloc(sizeof(Node));

    if(!aux) {
        return 0;
    }

    StockList_goToFirstNode(stockList);

    aux->stock = stock;
    aux->next = stockList->pointOfInterest->next;
    stockList->pointOfInterest->next = aux;

    return 1;
}

/***********************************
*
* @Name: StockList_removeStock
* @Def: Deletes the stock pointed by the point of interest
* @Arg: stockList: StockList
* @Ret: int
*
***********************************/
int StockList_removeStock(StockList stockList) {

    Node *aux;

    if (StockList_endOfList(stockList) || StockList_isEmpty(stockList)) {
        return 0;
    }

    aux = stockList->pointOfInterest->next;
    stockList->pointOfInterest->next = stockList->pointOfInterest->next->next;
    free(aux);

    return 1;
}

/***********************************
*
* @Name: StockList_search
* @Def: Searches for the stock element and leaves the
*       point of interest pointing to the stock before that one
* @Arg: stockList: StockList
*		ticker: char*
* @Ret: Stock
*
***********************************/
Stock StockList_search(StockList stockList, char* ticker) {

    Stock stock;

    StockList_goToFirstNode(stockList);

    while(!StockList_endOfList(stockList)) {

        stock = StockList_stock(stockList);

        if (strcmp(Stock_ticker(stock), ticker) == 0) {
            return stock;
        }

        StockList_next(stockList);
    }

    return NULL;
}

/***********************************
*
* @Name: StockList_endOfList
* @Def: Checks if the point of interest is the last element of the list
* @Arg: stockList: StockList
* @Ret: int
*
***********************************/
int StockList_endOfList(StockList stockList) {
    return stockList->pointOfInterest->next == NULL;
}

/***********************************
*
* @Name: StockList_isEmpty
* @Def: Checks if the list is
* @Arg: stockList: StockList Pointer
* @Ret: int
*
***********************************/
int StockList_isEmpty(StockList stockList) {
    return stockList->first->next == NULL;
}

/***********************************
*
* @Name: StockList_goToFirstNode
* @Def: Places the point of interest in the first element
* @Arg: stockList: StockList
*
***********************************/
void StockList_goToFirstNode(StockList stockList) {
    stockList->pointOfInterest = stockList->first;
}

/***********************************
*
* @Name: StockList_next
* @Def: Places the point of interest in the next element
* @Arg: stockList: StockList
*
***********************************/
void StockList_next(StockList stockList) {

    if (stockList->pointOfInterest->next) {
        stockList->pointOfInterest = stockList->pointOfInterest->next;
    }
}

/***********************************
*
* @Name: StockList_stock
* @Def: Returns the stock pointed by the point of interest
* @Arg: stockList: StockList
*		stock: Stock
* @Ret: Stock
*
***********************************/
Stock StockList_stock(StockList stockList) {

    if(StockList_isEmpty(stockList) || StockList_endOfList(stockList)) {
        return NULL;
    }

    return stockList->pointOfInterest->next->stock;
}
