#ifndef _Controllers_h_
#define _Controllers_h_

#include "../../../../Domain/Model/User/User.h"
#include "../../../../Domain/Model/Stock/StockList.h"
#include "../../../../Domain/Model/Stock/Stock.h"
#include "../Helpers/Console.h"
#include "../../../Socket/Stream.h"

void Controllers_showStock(User user);
void Controllers_showMoney(User user);
void Controllers_help();
void Controllers_exit();
void Controllers_error(char * error);
void Controllers_showIbex(int gekkoConnection, char* name);
void Controllers_buy(int gekkoConnection, User user, char* ticker, long long quantity);
void Controllers_showIbexGekkoResponse(char * data, int gekkoConnection);
void Controllers_sell(int gekkoConnection, User user, char* ticker, long long quantity);
void Controllers_bought(char * stream, User user);

#endif
