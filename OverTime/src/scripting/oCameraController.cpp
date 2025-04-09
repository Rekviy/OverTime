#include "oCameraController.h"
#include "platform/input.h"
#include "core/codes/keyCodes.h"
#include "core/codes/mouseButtonCodes.h"

namespace overtime {
	orthographCameraController::orthographCameraController(float aspectRatio, bool rotationEnable)
		:m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_RotationEnable(rotationEnable)
	{

	}
	void orthographCameraController::onUpdate(timeStep ts)
	{
		OT_CORE_INFO("camera x:{0} y:{1} angle:{2} zoom:{3}", m_Position.x, m_Position.y, m_Rotation, m_ZoomLevel);
		if (input::isKeyPressed(OT_KEY_W))
			m_Position.y += m_TranslationSpeed * ts;
		else if (input::isKeyPressed(OT_KEY_S))
			m_Position.y -= m_TranslationSpeed * ts;
		if (input::isKeyPressed(OT_KEY_A))
			m_Position.x -= m_TranslationSpeed * ts;
		else if (input::isKeyPressed(OT_KEY_D))
			m_Position.x += m_TranslationSpeed * ts;
		if (m_RotationEnable) {
			if (input::isKeyPressed(OT_KEY_E))
				m_Rotation += m_RotationSpeed * ts;
			else if (input::isKeyPressed(OT_KEY_Q))
				m_Rotation -= m_RotationSpeed * ts;
			m_Camera.setRotation(m_Rotation);
		}
		m_Camera.setPosition(m_Position);
		m_TranslationSpeed = m_ZoomLevel;
	}
	void orthographCameraController::onEvent(event& event)
	{
		eventDispatcher dispatcher(event);
		dispatcher.dispatch<mouseScrolledEvent>(OT_BIND_EVENT_FN(orthographCameraController::onMouseScrolled));
		dispatcher.dispatch<windowResizeEvent>(OT_BIND_EVENT_FN(orthographCameraController::onWindowResized));


	}
	bool orthographCameraController::onMouseScrolled(mouseScrolledEvent& event)
	{
		m_ZoomLevel -= event.getYOffset() * 0.5f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.setProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
	bool orthographCameraController::onWindowResized(windowResizeEvent& event)
	{
		m_AspectRatio = (float)event.getWidth() /(float) event.getHeight();
		m_Camera.setProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}