/************************************************
* 												*
* GEKKO APP, PROJECT GEKKO SO 2014/2015	        *
* BY: AGUSTIN ARMELLINI FISCHER					*
*											   	*
************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#include "../../File/FileGekkoStarter.h"
#include "../../Socket/SocketTumblingDiceConnection.h"
#include "../../../Domain/Model/Stock/StockList.h"
#include "./Helpers/Console.h"
#include "./GekkoMessages.h"
#include "../../Socket/GekkoServer.h"
#include "./Controllers/Controllers.h"
#include "../../../Domain/Model/Seller/SellerList.h"

// Prototypes
void rsiHandler(int numsig);
void initSignals();
void configureTumblingDiceRefresh();
void rsiTumblingDice();
static void * tumbligDiceConnexionThread(void *arg);
void startServer();
void stockListMutexLock();
void stockListMutexUnlock();
void sellerListMutexLock();
void sellerListMutexUnlock();
static void * manageSession(void *arg);

//Globals
StockList stockList;
SellerList sellerList;
TdConnectionConfig tdConnectionConfig;
int tumblingDiceConnection;
int tumblingDiceConnectionError;
int serverConnection;

// Mutex
static pthread_mutex_t stockListMutex  = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t sellerListMutex  = PTHREAD_MUTEX_INITIALIZER;

/***********************************
*
* @Name: main
* @Def: Entry point of the application
*
***********************************/
int main() {
    
    // Init signals
    initSignals();

    // App configuration
    tdConnectionConfig = SocketTumblingDiceConnection_configure();

    if (!tdConnectionConfig) {
        Console_error(CONFIG_ERROR);
        exit(-1);
    }

    // Login
    stockList = FileGekkoStarter_start();

    if (!stockList) {
        Console_error(IBEX_READ_ERROR);
        exit(-1);
    }

    // Create seller list
    sellerList = SellerList_create();

    // Configure tumbling dice refresh
    configureTumblingDiceRefresh();

    // Application start
    Console_printn("Gekko Started ...");

    startServer();

    while(1) {

    }

    return 0;
}

/***********************************
*
* @Name: rsiHandler
* @Def: Routine in case of interruption
* @Arg: numsig: int
*
***********************************/
void rsiHandler(int numsig) {
    StockList_destroy(stockList);
    SellerList_destroy(sellerList);
    pthread_mutex_destroy(&stockListMutex);
    pthread_mutex_destroy(&sellerListMutex);
    SocketTumblingDiceConnection_disconnect(tumblingDiceConnection);
    Console_printn("Sayonara");
    exit(numsig);
}

/***********************************
*
* @Name: initSignals
* @Def: Signals initializer
*
***********************************/
void initSignals() {
    signal(SIGINT, (void*)rsiHandler);
    signal(SIGTERM, (void*)rsiHandler);
    signal(SIGQUIT, (void*)rsiHandler);
    signal(SIGHUP, (void*)rsiHandler);
}

/***********************************
*
* @Name: configureTumblingDiceRefresh
* @Def: Sets up alarm and registers rsiTumblingDice
*
***********************************/
void configureTumblingDiceRefresh() {
    tumblingDiceConnectionError = -1;
    signal(SIGALRM, (void*)rsiTumblingDice);
    alarm(1);
}

/***********************************
*
* @Name: rsiTumblingDice
* @Def: Routine for refresh alarm
*
***********************************/
void rsiTumblingDice() {
    pthread_t thread;
    pthread_create(&thread, NULL, tumbligDiceConnexionThread, NULL);
}

/***********************************
*
* @Name: tumbligDiceConnexionThread
* @Def: Thread that updates the ibex values
*
***********************************/
static void * tumbligDiceConnexionThread(void *arg) {

    if (tumblingDiceConnectionError < 0) {

        Console_printn("Connecting to Tumbling Dice");
        tumblingDiceConnectionError = SocketTumblingDiceConnection_connect(tdConnectionConfig, &tumblingDiceConnection);

        if (tumblingDiceConnectionError < 0) {
            Console_error("Unable to connect to Tumbling Dice");
        } else {
            Console_printn("Connected succesfully to Tumbling Dice");
        }

        alarm(1);

        return NULL;
    }

    alarm(SocketTumblingDiceConnection_configRefresh(tdConnectionConfig));

    tumblingDiceConnectionError = SocketTumblingDiceConnection_refreshIbexStock(
            tumblingDiceConnection,
            stockList,
            (void*) stockListMutexLock,
            (void*) stockListMutexUnlock
    );

    if (tumblingDiceConnectionError < -1) {
        Console_error("Disconnected from Tumbling Dice");
        Console_printn("Trying to reconnect ...");
        alarm(1);
    }

    Console_printn("Updated IBEX values");

    return arg;
}

/***********************************
*
* @Name: startServer
* @Def: Starts Gekko Server
*
***********************************/
void startServer() {

    int error;
    pthread_t thread;
    DozerConnection dozerConnection;

    error = GekkoServer_startServer(&serverConnection);

    if (error < 0) {
        Console_error("Unable to start server");
        exit(-1);
    }

    Console_printn("Waiting for Dozer connections ...");

    while(1) {
        dozerConnection.name = GekkoServer_acceptConnection(serverConnection, &dozerConnection.connection);

        if (dozerConnection.connection != -1) {
            pthread_create(&thread, NULL, manageSession, &dozerConnection);
        }
    }
}

static void * manageSession(void *arg) {

    DozerConnection* dozerConnection = (DozerConnection*) arg;

    Controllers_mainController(
            *dozerConnection,
            stockList,
            (void*) stockListMutexLock,
            (void*) stockListMutexUnlock,
            sellerList,
            (void*) sellerListMutexLock,
            (void*) sellerListMutexUnlock
    );

    return NULL;
}

/***********************************
*
* @Name: stockListMutexLock
* @Def: Locks when stockList is accessed
*
***********************************/
void stockListMutexLock() {

    int error;

    error = pthread_mutex_lock(&stockListMutex);

    if (error != 0) {
        Console_error("Mutex lock error");
        exit(-1);
    }
}

/***********************************
*
* @Name: stockListMutexUnlock
* @Def: Unlocks stockList
*
***********************************/
void stockListMutexUnlock() {

    int error;

    error = pthread_mutex_unlock(&stockListMutex);

    if (error != 0) {
        Console_error("Mutex unlock error");
        exit(-1);
    }
}

/***********************************
*
* @Name: sellerListMutexLock
* @Def: Locks when sellerList is accessed
*
***********************************/
void sellerListMutexLock() {

    int error;

    error = pthread_mutex_lock(&sellerListMutex);

    if (error != 0) {
        Console_error("Mutex lock error");
        exit(-1);
    }
}

/***********************************
*
* @Name: sellerListMutexUnlock
* @Def: Unlocks sellerList
*
***********************************/
void sellerListMutexUnlock() {

    int error;

    error = pthread_mutex_unlock(&sellerListMutex);

    if (error != 0) {
        Console_error("Mutex unlock error");
        exit(-1);
    }
}
