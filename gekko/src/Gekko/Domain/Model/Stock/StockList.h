#ifndef _StockList_h_
#define _StockList_h_

#include "./Stock.h"

typedef struct StockListImplementation* StockList;

StockList StockList_create();
void StockList_destroy(StockList stockList);
int StockList_addStock(StockList stockList, Stock stock);
int StockList_removeStock(StockList stockList);
Stock StockList_search(StockList stockList, char* ticker);
int StockList_endOfList(StockList stockList);
int StockList_isEmpty(StockList stockList);
void StockList_goToFirstNode(StockList stockList);
void StockList_next(StockList stockList);
Stock StockList_stock(StockList stockList);

#endif
