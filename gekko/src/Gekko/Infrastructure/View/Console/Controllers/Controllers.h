#ifndef _Controllers_h_
#define _Controllers_h_

#include "../../../Socket/Stream.h"
#include "../Helpers/Console.h"
#include "../../../../Domain/Model/Stock/Stock.h"
#include "../../../../Domain/Model/Stock/StockList.h"
#include "../../../../Domain/Model/Seller/Seller.h"
#include "../../../../Domain/Model/Seller/SellerList.h"

typedef struct {
    int connection;
    char * name;
} DozerConnection;

void Controllers_mainController(
        DozerConnection dozerConnection,
        StockList stockList,
        void (*stockLock)(),
        void (*stockUnlock)(),
        SellerList sellerList,
        void (*sellerLock)(),
        void (*sellerUnlock)()
);

#endif
