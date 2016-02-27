#ifndef _Seller_h_
#define _Seller_h_

typedef struct SellerImplementation *Seller;

Seller Seller_create(char* name, char * ticker, long long quantity, int connection);
void Seller_destroy(Seller seller);
char* Seller_name(Seller seller);
char* Seller_ticker(Seller seller);
long long Seller_quantity(Seller seller);
int Seller_connection(Seller seller);
void Seller_setConnection(Seller seller, int connection);
double Seller_price(Seller seller);
void Seller_setPrice(Seller seller, double price);

#endif
