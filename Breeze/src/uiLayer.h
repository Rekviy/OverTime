#pragma once
#ifndef UILAYER_H
#define UILAYER_H

#include "ui/interactElement.h"
#include "cameraWrapper.h"

#include <overtime.h>

class uiLayer :public overtime::layer {
public:
	uiLayer();
	virtual void onAttach()override;
	virtual void onDetach() override;
	virtual void onUpdate(overtime::timeStep ts)override;
	virtual void onImGuiRender() override;
	virtual void onEvent(overtime::event& event) override;
	
	bool onWindowResize(overtime::windowResizeEvent& event);
private:
	std::vector<interactElement*> elems;
	

};

#endif