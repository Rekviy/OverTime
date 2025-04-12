#pragma once
#ifndef TEST2D_H
#define TEST2D_H
#include <overtime.h>

class test2dLayer : public overtime::layer {
public:
	test2dLayer();
	virtual ~test2dLayer() override = default;
	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate(overtime::timeStep ts) override;
	virtual void onImGuiRender() override;
	virtual void onEvent(overtime::event& event) override;
private:

	overtime::orthographCameraController _cameraControls;
	glm::vec4 _squareColor = { 0.4f, 0.8f, 0.2f, 1.0f };
};

#endif