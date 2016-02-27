#ifndef _User_h_
#define _User_h_

#include "../Stock/StockList.h"

// Structure definition
typedef struct UserImplementation* User;

// Methods
User User_create(char * name, double money);
void User_destroy(User user);
void User_addStock(User user, Stock stock);
double User_money(User user);
char * User_name(User user);
void User_setMoney(User user, double money);
StockList User_stock(User user);

#endif
