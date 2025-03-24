// precompiled header
// include here heavy headers which are included in many files
// this will speed-up build a lot
#pragma once
#ifndef PRECHEADER_H
#define PRECHEADER_H

#include "core/log.h"
#include "core/core.h"
#include "core/application.h"

#include "core/layers/layer.h"
#include "core/layers/layerStack.h"

#include "core/codes/keyCodes.h"
#include "core/codes/mouseButtonCodes.h"

#include "events/applicationEvent.h"
#include "events/keyEvent.h"
#include "events/mouseEvent.h"

#include "imGui/imguiLayer.h"

#include "renderer/shader.h"
#include "renderer/buffer.h"
#include "renderer/vertexArray.h"
#include "renderer/rendererAPI.h"
#include "renderer/renderer.h"
#include "renderer/camera.h"

#include "platform/window.h"
#include "platform/input.h"

#ifdef OT_PLATFORM_WINDOWS
	#include "platform/windows/winput.h"
	#include "platform/windows/wwindow.h"
#endif

#endif 