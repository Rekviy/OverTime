#pragma once
#ifndef ENTRYPOINT_H
#ifdef OT_PLATFORM_WINDOWS
extern overtime::application* overtime::createApplication();
#include "log.h"
int main(int argc, char** argv)
{
	overtime::log::init();
	OT_CORE_INFO("Log initialized.");
	OT_INFO("Log initialized.");
	auto app = overtime::createApplication();
	app->run();
	delete app;
}


#else
#error ONLY SUPPORT WINDOWS!!
#endif
#endif