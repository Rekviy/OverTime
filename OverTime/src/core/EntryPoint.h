#pragma once
#ifndef ENTRYPOINT_H
#ifdef OT_PLATFORM_WINDOWS

extern overtime::Application* overtime::createApplication();

int main(int argc, char** argv) {
	auto app = overtime::createApplication();
	app->Run();
	delete app;
}


#else
	#error ONLY SUPPORT WINDOWS!!
#endif
#endif