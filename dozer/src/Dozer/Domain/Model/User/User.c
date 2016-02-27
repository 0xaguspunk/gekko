#include "User.h"

// Includes
#include <stdlib.h>

// User definition
struct UserImplementation {

    char * name;
    double money;
    StockList stock;

};

/***********************************
*
* @Name: User_createUser
* @Def: Creates a user
* @Arg: name: char*, money: double
* @Ret: User
*
***********************************/
User User_create(char * name, double money) {

    User user = (User)malloc(sizeof * user);

    if (user) {
        user->name = name;
        user->money = money;
        user->stock = StockList_create();

        if (!user->stock) {
            free(user);
            user = NULL;
        }
    }

    return user;
}

/***********************************
*
* @Name: User_createUser
* @Def: Creates a user
* @Arg: user: User
*
***********************************/
void User_destroy(User user) {
    StockList_destroy(user->stock);
    free(user->name);
    free(user);
}

/***********************************
*
* @Name: User_money
* @Def: Creates a user
* @Arg: user: User, stock: Stock
* @Ret: double
*
***********************************/
void User_addStock(User user, Stock stock) {
    StockList_addStock(user->stock, stock);
}

/***********************************
*
* @Name: User_money
* @Def: Creates a user
* @Arg: user: User
* @Ret: double
*
***********************************/
double User_money(User user) {
    return user->money;
}

/***********************************
*
* @Name: User_setMoney
* @Def: Creates a user
* @Arg: user: User, money: double
*
***********************************/
void User_setMoney(User user, double money) {
    user->money = money;
}

/***********************************
*
* @Name: User_name
* @Def: Returns the money of the user
* @Arg: user: User
* @Ret: char *
*
***********************************/
char * User_name(User user) {
    return user->name;
}

/***********************************
*
* @Name: User_stock
* @Def: Returns the stock of the user
* @Arg: user: User
* @Ret: StockList
*
***********************************/
StockList User_stock(User user) {
    return user->stock;
}
