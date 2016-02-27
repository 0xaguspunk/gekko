#include "FileDozerLogin.h"

// Includes
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

// Prototypes
char * readName(int fileDescriptor);
double readMoney(int fileDescriptor);
void readAndAddStock(int fileDescriptor, User user);
char * readTicker(int fileDescriptor);
long long readQuantity(int fileDescriptor);

/***********************************
*
* @Name: FileDozerLogin_login
* @Def: Logs the user into the application
* @Ret: User
*
***********************************/
User FileDozerLogin_login() {

    int fileDescriptor;
    char * name;
    double money;
    User user;

    fileDescriptor = open(PATH_LOGIN_FILE, O_RDONLY);

    if (fileDescriptor < 0) {
        return 0;
    }

    name = readName(fileDescriptor);
    money = readMoney(fileDescriptor);

    user = User_create(name, money);

    readAndAddStock(fileDescriptor, user);

    close(fileDescriptor);

    return user;
}

/***********************************
*
* @Name: readName
* @Def: Reads the name of the user
* @Arg: fileDescriptor: int
* @Ret: char *
*
***********************************/
char * readName(int fileDescriptor) {

    char * name;
    char aux;
    int i;

    i = 0;
    read(fileDescriptor, &aux, sizeof(char));
    name = (char*)malloc(sizeof(char));

    while(aux != '\n') {

        name[i] = aux;
        i++;
        name = realloc(name, sizeof(char)*(i+1));
        read(fileDescriptor, &aux, sizeof(char));
    }

    name[i] = '\0';

    return name;
}

/***********************************
*
* @Name: readMoney
* @Def: Reads the money of the user
* @Arg: fileDescriptor: int
* @Ret: double
*
***********************************/
double readMoney(int fileDescriptor) {

    char aux;
    char text[50];
    int i;

    i = 0;

    read(fileDescriptor, &aux, sizeof(char));

    while(aux != '\n') {
        text[i] = aux;
        i++;
        read(fileDescriptor, &aux, sizeof(char));
    }

    text[i] = '\0';

    return atof(text);
}

/***********************************
*
* @Name: readAndAddStock
* @Def: Reads the stock of the user
* @Arg: fileDescriptor: int, user: User
*
***********************************/
void readAndAddStock(int fileDescriptor, User user) {

    Stock stock;
    char * ticker;
    long long quantity;

    while(1) {

        ticker = readTicker(fileDescriptor);

        if (!ticker) {
            break;
        }

        quantity = readQuantity(fileDescriptor);
        stock = Stock_create(ticker, quantity);
        User_addStock(user, stock);
    }
}

/***********************************
 *
 * @Name: readTicker
 * @Def: Reads the ticker of the user
 * @Arg: fileDescriptor: int
 * @Ret: char *
 *
 ***********************************/
char * readTicker(int fileDescriptor) {

    char * ticker;
    char aux;
    int i, bytesRead;

    i = 0;

    bytesRead = read(fileDescriptor, &aux, sizeof(char));

    if (aux == '\n' || bytesRead == 0) {
        return NULL;
    }

    ticker = (char*)malloc(sizeof(char));

    while(aux != '-') {

        ticker[i] = aux;
        i++;
        ticker = realloc(ticker, sizeof(char)*(i+1));
        read(fileDescriptor, &aux, sizeof(char));
    }

    ticker[i] = '\0';

    return ticker;
}

/***********************************
 *
 * @Name: readQuantity
 * @Def: Reads the quantity of stock the user
 * @Arg: fileDescriptor: int
 * @Ret: long long
 *
 ***********************************/
long long readQuantity(int fileDescriptor) {

    char aux;
    char text[50];
    int i;

    i = 0;

    read(fileDescriptor, &aux, sizeof(char));

    while(aux != '\n') {

        text[i] = aux;
        i++;
        read(fileDescriptor, &aux, sizeof(char));
    }

    text[i] = aux;

    return atoll(text);
}
