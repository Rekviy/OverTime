#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H
#include "core.h"
#include "events/event.h"
namespace overtime {
	class OVERTIME_API application {
	public:
		application();
		virtual ~application();

		void Run();

	};

	// To be defined in CLIENT
	application* createApplication();


}
#endif