#include "DozerConsole.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/select.h>
#include <unistd.h>
#include <signal.h>

// Defines
#define SHOW_STOCK "show stock"
#define SHOW_IBEX "show ibex"
#define SHOW_MONEY "show me the money"
#define BUY "buy"
#define SELL "sell"
#define EXIT "exit"
#define HELP "help"


// Prototypes
void readAndProcessOption(User user, int gekkoConnection);
void processOption(char * option, User user, int gekkoConnection);
void getTickerAndQuantity(char * string, char* ticker, long long *quantity);
void readAndProcessGekkoStream(User user, int gekkoConnection);

/***********************************
*
* @Name: DozerConsole_start
* @Def: Main loop of the console
* @Arg: user: User
*
***********************************/
void DozerConsole_start(User user, int gekkoConnection) {

    fd_set fds;
    int ready;

    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    FD_SET(gekkoConnection, &fds);

    while(1) {

        ready = select(gekkoConnection+1, &fds, NULL, NULL, NULL);

        if (ready) {
            if (FD_ISSET(0, &fds)) {
                readAndProcessOption(user, gekkoConnection);
            }

            if (FD_ISSET(gekkoConnection, &fds)) {
                readAndProcessGekkoStream(user, gekkoConnection);
            }
        }

        Console_promt(User_name(user));

        FD_SET(STDIN_FILENO, &fds);
        FD_SET(gekkoConnection, &fds);
    }
}

/***********************************
*
* @Name: readAndProcessOption
* @Def: Reads and processes the option selected
* @Arg: user: User, gekkoConnection: int
*
***********************************/
void readAndProcessOption(User user, int gekkoConnection) {

    char * option;

    option = Console_read();
    processOption(option, user, gekkoConnection);
}

void readAndProcessGekkoStream(User user, int gekkoConnection) {

    char type;
    char stream[STREAM_LENGTH];
    char *data;

    if (read(gekkoConnection, stream, STREAM_LENGTH) < STREAM_LENGTH) {
        Console_printNext("\n");
        Console_error("Lost connection with Gekko");
        raise(SIGINT);
    }

    type = Stream_parseType(stream);
    data = Stream_parseData(stream);

    switch(type) {
        case 'X':
            Controllers_showIbexGekkoResponse(data, gekkoConnection);
            break;
        case 'O':
            Console_printn("Connected to Gekko");
            break;
        case 'M':
            Console_printn("");
            Console_printn(data);
            Console_printn("");
            Controllers_bought(stream, user);
            break;
        default:
            Console_error("Unknown Gekko response");
            break;
    }
}

/***********************************
*
* @Name: processOption
* @Def: Processes the option selected
* @Ret: char*
*
***********************************/
void processOption(char * option, User user, int gekkoConnection) {

    char text[100];
    char *ticker = NULL;
    long long quantity;
    Stock stock;

    ticker = (char*)malloc(6*sizeof(char));
    
    if (strcmp(option, SHOW_STOCK) == 0) {
        Controllers_showStock(user);
        return;
    }

    if (strcmp(option, SHOW_MONEY) == 0) {
        Controllers_showMoney(user);
        return;
    }

    if (strcmp(option, SHOW_IBEX) == 0) {
        Controllers_showIbex(gekkoConnection, User_name(user));
        return;
    }

    if (strcmp(option, HELP) == 0) {
        Controllers_help();
        return;
    }

    if (strcmp(option, EXIT) == 0) {
        Controllers_exit();
        return;
    }

    strncpy(text, option, strlen(BUY));
    text[strlen(BUY)] = '\0';

    if (strcmp(text, BUY) == 0) {
        getTickerAndQuantity(option, ticker, &quantity);

        if (strlen(ticker) < 3 || quantity < 0) {
            Console_printn("");
            Console_error("Wrong arguments");
            Console_printn("");
            return;
        }

        Controllers_buy(gekkoConnection, user, ticker, quantity);
        return;
    }

    strncpy(text, option, strlen(SELL));
    text[strlen(SELL)] = '\0';

    if (strcmp(text, SELL) == 0) {
        getTickerAndQuantity(option, ticker, &quantity);

        if (strlen(ticker) < 3 || quantity < 0) {
            Console_printn("");
            Console_error("Wrong arguments");
            Console_printn("");
            return;
        }

        stock = StockList_search(User_stock(user), ticker);

        if (!stock) {
            Console_printn("");
            Console_error("You do not have that ticker!");
            Console_printn("");
            return;
        }

        if (Stock_quantity(stock) < quantity) {
            Console_printn("");
            Console_error("You do not have enough stock to sell that quantity");
            Console_printn("");
            return;
        }

        Controllers_sell(gekkoConnection, user, ticker, quantity);
        return;
    }

    if (strcmp(option, "\0") != 0) {
        Controllers_error(WRONG_OPTION);
        return;
    }
}

/***********************************
*
* @Name: getTickerAndQuantity
* @Def: Gets ticker and quantity
* @Arg: string: char*, ticker: char*, quantity: long long*
*
***********************************/
void getTickerAndQuantity(char * string, char * ticker, long long* quantity) {

    int i, k;
    char aux = '_';
    char number[20];

    i = 0;
    k = 0;

    while(aux != ' ') {

        if (aux == '\0') {
            *quantity = -1;
            ticker[0] = '\0';
            return;
        }

        aux = string[i];
        i++;
    }

    aux = string[i];

    while(aux != ' ') {

        if (aux == '\0') {
            *quantity = -1;
            ticker[0] = '\0';
            return;
        }

        aux = string[i];
        ticker[k] = aux;
        i++;
        k++;
    }

    ticker[k-1] = '\0';

    for (k = 0 ; ticker[k]; k++) {
        ticker[k] = toupper(ticker[k]);
    }

    k = 0;

    while(aux != '\0') {
        aux = string[i];
        number[k] = aux;
        i++;
        k++;
    }

    number[k] = '\0';

    *quantity = (double) atoll(number);
}
