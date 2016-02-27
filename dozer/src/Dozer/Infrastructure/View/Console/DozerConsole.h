#ifndef _DozerConsole_h_
#define _DozerConsole_h_

#include "./Helpers/Console.h"
#include "./DozerMessages.h"
#include "./Controllers/Controllers.h"
#include "../../../Domain/Model/User/User.h"
#include "../../Socket/Stream.h"

void DozerConsole_start(User user, int gekkoConnection);

#endif
