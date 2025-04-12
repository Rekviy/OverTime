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
		inline float getZoomLevel() const { return m_ZoomLevel; }
		void setZoomLevel(float level) { m_ZoomLevel = level; }
		orthographCamera& getCamera() { return m_Camera; }
		const orthographCamera& getCamera() const { return m_Camera; }

	private:
		bool onMouseScrolled(mouseScrolledEvent& event);
		bool onWindowResized(windowResizeEvent& event);

		bool m_RotationEnable;
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;

		glm::vec3 m_Position = { 0.0f,0.0f, 0.0f };
		float m_Rotation = 0.0f;
		float m_TranslationSpeed = 5.0f, m_RotationSpeed = 180.0f;

		orthographCamera m_Camera;
	};
}

#endif