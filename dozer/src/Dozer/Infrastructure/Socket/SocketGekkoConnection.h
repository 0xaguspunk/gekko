#ifndef _SocketGekkoConnection_h_
#define _SocketGekkoConnection_h_

#include "./Stream.h"

#define PATH_GEKKO_CONNECTION_CONFIGURATION "config/config.dat"

typedef struct GekkoConnectionConfigImplementation * GekkoConnectionConfig;

GekkoConnectionConfig SocketGekkoConnection_configure();
int SocketGekkoConnection_connect(GekkoConnectionConfig gekkoConnectionConfig, int *gekkoConnection, char* name);
void SocketGekkoConnection_disconnect(int gekkoConnection, char* name);

#endif
