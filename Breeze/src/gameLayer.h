#pragma once
#ifndef GAMELAYER_H
#define GAMELAYER_H
#include "cameraWrapper.h"

#include <overtime.h>

class gameLayer :public overtime::layer {
public:
	gameLayer();
	virtual void onAttach()override;
	virtual void onDetach() override;
	virtual void onUpdate(overtime::timeStep ts)override;
	virtual void onImGuiRender() override;
	virtual void onEvent(overtime::event& event) override;

private:
	overtime::ref<overtime::texture2D> _frame;
};

#endif