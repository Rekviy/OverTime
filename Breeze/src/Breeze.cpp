#include "gameLayer.h"
#include "uiLayer.h"
#include "ui/themeManager.h"

#include <core/entryPoint.h>

//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

class Breeze : public overtime::application {
public:
	Breeze()
	{
		themeManager::init();
		pushLayer(new gameLayer());
		pushOverlay(new uiLayer());
	}
	~Breeze()
	{}

private:

};

overtime::application* overtime::createApplication()
{
	return new Breeze();
}
