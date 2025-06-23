#include "gameLayer.h"
#include "cameraWrapper.h"
#include "gameStateMachine.h"
using namespace overtime;
namespace breeze {
	gameLayer::gameLayer()
		:layer("gameLayer")
	{
		_state = new gsm();

	}
	void gameLayer::onAttach()
	{
		_state->init();
	}

	void gameLayer::onDetach()
	{
		_state->shutdown();
	}

	void gameLayer::onUpdate(timeStep ts)
	{
		_state->onUpdate(ts);

		//render happens here
		rendererAPI::setClearColor({ 0, 0.6f, 0.6f, 1 });
		rendererAPI::clear();

		renderer2D::beginScene(cameraWrapper::getCamera());

		_state->onRender();

		renderer2D::endScene();
	}

	void gameLayer::onImGuiRender()
	{
		_state->onImGuiRender();
	}

	void gameLayer::onEvent(event& event)
	{
		eventDispatcher dispatcher(event);
		dispatcher.dispatch<windowResizeEvent>(OT_BIND_EVENT_FN(gameLayer::onWindowResize));

		_state->onEvent(event);
	}

	bool gameLayer::onWindowResize(windowResizeEvent& event)
	{
		cameraWrapper::updateCamera((float)event.getWidth(), (float)event.getHeight());
		return false;
	}
}