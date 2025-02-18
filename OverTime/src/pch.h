// precompiled header
// include here heavy headers which are included in many files
// this will speed-up build a lot
#pragma once
#ifndef PRECHEADER_H
#define PRECHEADER_H

//#include "src/core/application.h"
//#include "src/core/core.h"
//#include "src/core/entryPoint.h"
//#include "src/core/log.h"
//
//#include "src/events/applicationEvent.h"
//#include "src/events/event.h"
//#include "src/events/keyEvent.h"
//#include "src/events/mouseEvent.h"

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#ifdef OT_PLATFORM_WINDOWS
	#include <Windows.h>
#else

#endif
#endif 