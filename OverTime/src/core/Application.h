#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H
#include "core.h"
namespace overtime {
	class OVERTIME_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();

	};
Application* createApplication();


}
#endif