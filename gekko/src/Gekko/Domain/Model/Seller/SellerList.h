#ifndef _SellerList_h_
#define _SellerList_h_

#include "./Seller.h"

typedef struct SellerListImplementation* SellerList;

SellerList SellerList_create();
void SellerList_destroy(SellerList sellerList);
int SellerList_addSeller(SellerList sellerList, Seller seller);
int SellerList_removeSeller(SellerList sellerList);
Seller SellerList_search(SellerList sellerList, char* ticker);
int SellerList_endOfList(SellerList sellerList);
int SellerList_isEmpty(SellerList sellerList);
void SellerList_goToFirstNode(SellerList sellerList);
void SellerList_next(SellerList sellerList);
Seller SellerList_seller(SellerList sellerList);

#endif
