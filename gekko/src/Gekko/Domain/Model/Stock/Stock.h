#ifndef _Stock_h_
#define _Stock_h_

typedef struct StockImplementation* Stock;

Stock Stock_create(char * ticker, double price, long long quantity);
void Stock_destroy(Stock stock);
void Stock_setQuantity(Stock stock, long long quantity);
char * Stock_ticker(Stock stock);
long long Stock_quantity(Stock stock);
double Stock_price(Stock stock);
void Stock_updatePrice(Stock stock, double variation);

#endif
