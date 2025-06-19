#include "gameLayer.h"
#include "ui/themeManager.h"

#include <core/entryPoint.h>

class Breeze : public overtime::application {
public:
	Breeze()
	{
		srand(time(NULL)); 
		themeManager::init();
		pushLayer(new gameLayer());
	}
	~Breeze()
	{
		themeManager::shutdown();
	}

private:

};

overtime::application* overtime::createApplication()
{
	return new Breeze();
}