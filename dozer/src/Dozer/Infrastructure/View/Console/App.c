/************************************************
* 												*
* DOZER APP, PROJECT GEKKO SO 2014/2015	        *
* BY: AGUSTIN ARMELLINI FISCHER					*
*											   	*
************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "../../../Application/Service/DozerLogin.h"
#include "../../File/FileDozerLogin.h"
#include "../../Socket/SocketGekkoConnection.h"
#include "../../../Domain/Model/User/User.h"
#include "./Helpers/Console.h"
#include "./DozerConsole.h"
#include "./DozerMessages.h"

// Prototypes
void rsiHandler(int numsig);
void initSignals();

//Globals
User user;
int gekkoConnection;
GekkoConnectionConfig gekkoConnectionConfig;

/***********************************
*
* @Name: main
* @Def: Entry point of the application
*
***********************************/
int main() {

    DozerLogin_login dozerLogin_login;
    int error;

    // Init signals
    initSignals();

    // Dependency Injection
    dozerLogin_login = &FileDozerLogin_login;

    // App configuration
    gekkoConnectionConfig = SocketGekkoConnection_configure();

    if (!gekkoConnectionConfig) {
        Console_error(CONFIG_ERROR);
        exit(-1);
    }

    // Login
    user = (*dozerLogin_login)();

    if (!user) {
        Console_error(LOGIN_ERROR);
        exit(-1);
    }

    // We connect to Gekko
    error = SocketGekkoConnection_connect(gekkoConnectionConfig, &gekkoConnection, User_name(user));

    if (error < 0) {
        Console_error("Unable to connect to Gekko");
        exit(-1);
    }

    // Application start
    DozerConsole_start(user, gekkoConnection);

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
    SocketGekkoConnection_disconnect(gekkoConnection, User_name(user));
    User_destroy(user);
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
