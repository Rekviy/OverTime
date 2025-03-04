#include "Breeze.h"
class testLayer : public overtime::layer {
public:
	testLayer() : layer("testLayer") {}
	void onUpdate() override
	{
		//OT_INFO("testLayer - Update");
	}
	void onEvent(overtime::event& event) override
	{
		//OT_TRACE("{0}", event.toString());
		if (event.getEventType() == overtime::eventType::keyPressed) {
			overtime::keyPressedEvent& e = (overtime::keyPressedEvent&)event;
			OT_TRACE("Key pressed {0}, {1}", e.getKeyCode(), (char)e.getKeyCode());
		}

	}
};
class Breeze : public overtime::application {
public:
	Breeze()
	{
		pushLayer(new testLayer());
		pushOverlay(new overtime::imGuiLayer());
	}
	~Breeze()
	{}

private:

};

overtime::application* overtime::createApplication()
{
	return new Breeze();

}
