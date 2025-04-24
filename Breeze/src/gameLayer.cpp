#include "gameLayer.h"

using namespace overtime;

gameLayer::gameLayer() :layer("gameLayer")
{
	_frame = texture2D::create("assets/resources/frame.png");
}
void gameLayer::onAttach()
{}

void gameLayer::onDetach()
{

}

void gameLayer::onUpdate(overtime::timeStep ts)
{
	//render happens here
	rendererAPI::setClearColor({ 0, 0.6f, 0.6f, 1 });
	rendererAPI::clear();

	renderer2D::beginScene(cameraWrapper::getCamera());
	glm::vec2 size = { 30.0f,30.0f };
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			renderer2D::drawSquad({ -620.0f + j * size.x, 340.0f - i * size.y,0.0f }, size, _frame);
		}
	}
	renderer2D::endScene();
}

void gameLayer::onImGuiRender()
{


}

void gameLayer::onEvent(overtime::event& event)
{
	//eventDispatcher dispatcher(event);

}


