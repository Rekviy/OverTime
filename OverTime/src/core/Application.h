#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H
#include "core.h"
namespace overtime {
	class OVERTIME_API application {
	public:
		application();
		virtual ~application();

		void Run();

	};
	application* createApplication();


}
#endif