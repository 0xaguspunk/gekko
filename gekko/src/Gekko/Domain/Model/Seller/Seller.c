#include "Seller.h"

// Includes
#include <stdlib.h>
#include <string.h>

// Stock definition
struct SellerImplementation {

    char name[20];
    char ticker[6];
    long long quantity;
    int connection;
    double price;
};

Seller Seller_create(char* name, char * ticker, long long quantity, int connection) {

    Seller seller = (Seller)malloc(sizeof * seller);

    if (seller) {
        strcpy(seller->name, name);
        strcpy(seller->ticker, ticker);
        seller->quantity = quantity;
        seller->connection = connection;
        seller->price = 0;
    }

    return seller;
}

void Seller_destroy(Seller seller) {
    free(seller);
}

char* Seller_name(Seller seller) {
    return seller->name;
}

char* Seller_ticker(Seller seller) {
    return seller->ticker;
}

long long Seller_quantity(Seller seller) {
    return seller->quantity;
}

int Seller_connection(Seller seller) {
    return seller->connection;
}

void Seller_setConnection(Seller seller, int connection) {
    seller->connection = connection;
}

double Seller_price(Seller seller) {
    return seller->price;
}

void Seller_setPrice(Seller seller, double price) {
    seller->price = price;
}
