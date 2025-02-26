#include "Breeze.h"
class testLayer : public overtime::layer {
public:
	testLayer() : layer("testLayer") {}
	void onUpdate() override
	{
		OT_INFO("testLayer - Update");
	}
	void onEvent(overtime::event& event) override
	{
		OT_TRACE("{0}", event.toString());
	}
};
class Breeze : public overtime::application {
public:
	Breeze()
	{
		pushLayer(new testLayer());
	}
	~Breeze()
	{}

private:

};

overtime::application* overtime::createApplication()
{
	return new Breeze();

}
