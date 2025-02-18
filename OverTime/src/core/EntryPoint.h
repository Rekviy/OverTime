#pragma once
#ifndef ENTRYPOINT_H
#ifdef OT_PLATFORM_WINDOWS
extern overtime::application* overtime::createApplication();
#include "log.h"
int main(int argc, char** argv)
{
	auto app = overtime::createApplication();
	app->Run();
	delete app;
}


#else
#error ONLY SUPPORT WINDOWS!!
#endif
#endif