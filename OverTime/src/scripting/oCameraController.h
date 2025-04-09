#pragma once
#ifndef OCAMERACONTROLLER_H
#define OCAMERACONTROLLER_H
#include "core/core.h"
#include "renderer/camera.h"
#include "core/timeStep.h"
#include "events/applicationEvent.h"
#include "events/mouseEvent.h"


namespace overtime {
	class orthographCameraController {
	public:
		orthographCameraController(float aspectRatio, bool rotationEnable = false);
		void onUpdate(timeStep ts);
		void onEvent(event& e);
		orthographCamera& getCamera() { return m_Camera; }
		const orthographCamera& getCamera() const { return m_Camera; }

	private:
		bool onMouseScrolled(mouseScrolledEvent& e);
		bool onWindowResized(windowResizeEvent& e);

		bool m_RotationEnable;
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;

		glm::vec3 m_Position = { 0.0f,0.0f, 0.0f };
		float m_Rotation = 0.0f;
		float m_TranslationSpeed = 5.0f, m_RotationSpeed = 45.0f;

		orthographCamera m_Camera;
	};
}

#endif