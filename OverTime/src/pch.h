// precompiled header
// include here heavy headers which are included in many files
// this will speed-up build a lot
#pragma once
#ifndef PRECHEADER_H
#define PRECHEADER_H


#include "core/core.h"
#include "core/application.h"
#include "core/window.h"

#include "core/log.h"

#include "core/layer.h"
#include "core/layerStack.h"

#include "core/input.h"
#include "core/keyCodes.h"
#include "core/mouseButtonCodes.h"


#include "events/event.h"
#include "events/applicationEvent.h"
#include "events/keyEvent.h"
#include "events/mouseEvent.h"

#include "imGui/imguiLayer.h"

#include "platform/windows/winput.h"
#include "platform/windows/wwindow.h"

//#include <iostream>
//#include <memory>
//#include <utility>
//#include <algorithm>
//#include <functional>
//#include <string>
//#include <sstream>
//#include <vector>
//#include <unordered_map>
//#include <unordered_set>

#ifdef OT_PLATFORM_WINDOWS
	#include <Windows.h>
#else

#endif
#endif 