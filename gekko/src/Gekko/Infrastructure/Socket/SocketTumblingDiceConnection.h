#ifndef _SocketTumblingDiceConnection_h_
#define _SocketTumblingDiceConnection_h_

#include "./Stream.h"
#include "../../Domain/Model/Stock/Stock.h"
#include "../../Domain/Model/Stock/StockList.h"

#define PATH_TD_CONNECTION_CONFIGURATION "config/config.dat"

typedef struct TdConnectionImplementation * TdConnectionConfig;

TdConnectionConfig SocketTumblingDiceConnection_configure();
float SocketTumblingDiceConnection_configRefresh(TdConnectionConfig tdConnectionConfig);
int SocketTumblingDiceConnection_connect(TdConnectionConfig tdConnectionConfig, int* tumblingDiceConnection);
void SocketTumblingDiceConnection_disconnect(int tumblingDiceConnection);
int SocketTumblingDiceConnection_refreshIbexStock(int tumblingDiceConnection, StockList stockList, void (*lock)(), void (*unlock)());

#endif
