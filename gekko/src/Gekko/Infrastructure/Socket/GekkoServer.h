#ifndef _GekkoServer_h_
#define _GekkoServer_h_

#include "./Stream.h"

#define SERVER_PORT 8100
#define SERVER_CONNECTIONS 5

int GekkoServer_startServer(int* serverConnection);
char* GekkoServer_acceptConnection(int serverConnection, int* dozerConnection);

#endif
