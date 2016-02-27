#ifndef _FileDozerLogin_h_
#define _FileDozerLogin_h_

#include "../../Domain/Model/User/User.h"
#include "../../Domain/Model/Stock/Stock.h"

#define PATH_LOGIN_FILE "config/stock.dat"

User FileDozerLogin_login();

#endif
