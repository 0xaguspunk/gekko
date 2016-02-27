#include "SellerList.h"

// Includes
#include <stdlib.h>
#include <string.h>

// SellerList definition
typedef struct n {

    Seller seller;
    struct n * next;

} Node;

struct SellerListImplementation {

    Node * first;
    Node * pointOfInterest;
};

/***********************************
*
* @Name: SellerList_create
* @Def: Creates a seller list with a ghost
* @Ret: SellerList
*
***********************************/
SellerList SellerList_create() {

    SellerList sellerList;
    Node * ghostNode;

    sellerList = (SellerList)malloc(sizeof * sellerList);

    if (!sellerList) {
        return NULL;
    }

    ghostNode = (Node*)malloc(sizeof(Node));

    if(!ghostNode) {
        free(sellerList);
        return NULL;
    }

    sellerList->pointOfInterest = sellerList->first = ghostNode;
    ghostNode->next = NULL;

    return sellerList;
}

/***********************************
*
* @Name: SellerList_destroy
* @Def: Destroys a list
* @Arg: sellerList: SellerList
*
***********************************/
void SellerList_destroy(SellerList sellerList) {

    SellerList_goToFirstNode(sellerList);

    while(!SellerList_endOfList(sellerList)) {
        SellerList_removeSeller(sellerList);
    }

    // Remove ghost
    free(sellerList->first);

    free(sellerList);
}

/***********************************
*
* @Name: SellerList_addSeller
* @Def: Adds seller to the list
* @Arg: sellerList: SellerList
*		seller: Seller
* @Ret: int
*
***********************************/
int SellerList_addSeller(SellerList sellerList, Seller seller) {

    Node * aux;

    aux = (Node*)malloc(sizeof(Node));

    if(!aux) {
        return 0;
    }

    SellerList_goToFirstNode(sellerList);

    aux->seller = seller;
    aux->next = sellerList->pointOfInterest->next;
    sellerList->pointOfInterest->next = aux;

    return 1;
}

/***********************************
*
* @Name: SellerList_removeSeller
* @Def: Deletes the seller pointed by the point of interest
* @Arg: sellerList: SellerList
* @Ret: int
*
***********************************/
int SellerList_removeSeller(SellerList sellerList) {

    Node *aux;

    if (SellerList_endOfList(sellerList) || SellerList_isEmpty(sellerList)) {
        return 0;
    }

    aux = sellerList->pointOfInterest->next;
    sellerList->pointOfInterest->next = sellerList->pointOfInterest->next->next;
    free(aux);

    return 1;
}

/***********************************
*
* @Name: SellerList_search
* @Def: Searches for the stock element and leaves the
*       point of interest pointing to the stock before that one
* @Arg: stockList: SellerList
*		ticker: char*
* @Ret: Stock
*
***********************************/
Seller SellerList_search(SellerList sellerList, char* ticker) {

    Seller seller;

    SellerList_goToFirstNode(sellerList);

    while(!SellerList_endOfList(sellerList)) {

        seller = SellerList_seller(sellerList);

        if (strcmp(Seller_ticker(seller), ticker) == 0) {
            return seller;
        }

        SellerList_next(sellerList);
    }

    return NULL;
}

/***********************************
*
* @Name: SellerList_endOfList
* @Def: Checks if the point of interest is the last element of the list
* @Arg: sellerList: SellerList
* @Ret: int
*
***********************************/
int SellerList_endOfList(SellerList sellerList) {
    return sellerList->pointOfInterest->next == NULL;
}

/***********************************
*
* @Name: SellerList_isEmpty
* @Def: Checks if the list is
* @Arg: sellerList: StockList Pointer
* @Ret: int
*
***********************************/
int SellerList_isEmpty(SellerList sellerList) {
    return sellerList->first->next == NULL;
}

/***********************************
*
* @Name: SellerList_goToFirstNode
* @Def: Places the point of interest in the first element
* @Arg: sellerList: SellerList
*
***********************************/
void SellerList_goToFirstNode(SellerList sellerList) {
    sellerList->pointOfInterest = sellerList->first;
}

/***********************************
*
* @Name: SellerList_next
* @Def: Places the point of interest in the next element
* @Arg: sellerList: SellerList
*
***********************************/
void SellerList_next(SellerList sellerList) {

    if (sellerList->pointOfInterest->next) {
        sellerList->pointOfInterest = sellerList->pointOfInterest->next;
    }
}

/***********************************
*
* @Name: SellerList_seller
* @Def: Returns the stock pointed by the point of interest
* @Arg: stockList: SellerList
*		stock: Stock
* @Ret: Stock
*
***********************************/
Seller SellerList_seller(SellerList sellerList) {

    if(SellerList_isEmpty(sellerList) || SellerList_endOfList(sellerList)) {
        return NULL;
    }

    return sellerList->pointOfInterest->next->seller;
}
