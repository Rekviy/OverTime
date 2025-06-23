#include "gameLayer.h"
#include "ui/themeManager.h"

#include <core/entryPoint.h>
namespace breeze {
	class breeze : public overtime::application {
	public:
		breeze()
		{
			srand(time(NULL));
			themeManager::init();
			pushLayer(new gameLayer());
		}
		~breeze()
		{
			themeManager::shutdown();
		}

	private:

	};
}
	overtime::application* overtime::createApplication()
	{
		return new breeze::breeze();
	}
