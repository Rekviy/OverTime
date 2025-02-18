#include "application.h"
//#include "events/"
#include "core/log.h"
namespace overtime {

	application::application()
	{

	}

	application::~application()
	{

	}

	void application::Run()
	{
		overtime::log::init();
		OT_CORE_ERROR("WAGHHH!!!");
		while (true);
	}

}

