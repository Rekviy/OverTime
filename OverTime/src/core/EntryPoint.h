#pragma once
#ifndef ENTRYPOINT_H
#ifdef OT_PLATFORM_WINDOWS
extern overtime::application* overtime::createApplication();
#include "log.h"
int main(int argc, char** argv)
{
	overtime::log::init();
	OT_CORE_CRIT("OSHIBKA STOP");
	OT_CRIT("OSHIBKA STOP2");
	auto app = overtime::createApplication();
	app->Run();
	delete app;
}


#else
#error ONLY SUPPORT WINDOWS!!
#endif
#endif