#include "test2d.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


test2dLayer::test2dLayer() : layer("testLayer"), _cameraControls(1280.0f / 720.0f, true)
{}

void test2dLayer::onAttach()
{
	_texture = overtime::texture2D::create("assets/resources/Screenshot.png");
	_cherryTexture = overtime::texture2D::create("assets/resources/cherry.png");
}

void test2dLayer::onDetach()
{}

void test2dLayer::onUpdate(overtime::timeStep ts)
{
	_cameraControls.onUpdate(ts);
	overtime::rendererAPI::setClearColor({ 0, 0.6f, 0.6f, 1 });
	overtime::rendererAPI::clear();

	overtime::renderer2D::beginScene(_cameraControls.getCamera());
	overtime::renderer2D::drawSquad(_squareColor, { -0.5f,0.5f }, { 1.0f,1.0f });
	overtime::renderer2D::drawSquad(_squareColor, { 0.5f,-0.5f }, { 0.75f,0.3f });
	overtime::renderer2D::drawSquad(_cherryTexture, { 0.0f,-0.75f }, { 1.0f,1.0f });


	overtime::renderer2D::endScene();
}

void test2dLayer::onImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Color", glm::value_ptr(_squareColor));
	ImGui::End();
}

void test2dLayer::onEvent(overtime::event& event)
{
	_cameraControls.onEvent(event);
}
