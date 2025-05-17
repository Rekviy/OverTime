#include "gameLayer.h"
#include "cameraWrapper.h"
#include "gameStateMachine.h"
using namespace overtime;

gameLayer::gameLayer()
	:layer("gameLayer")
{
	_state = new gsm();
	
}
void gameLayer::onAttach()
{}

void gameLayer::onDetach()
{}

void gameLayer::onUpdate(timeStep ts)
{
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
	cameraWrapper::updateCamera(event.getWidth(), event.getHeight());
	return false;
}
