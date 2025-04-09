#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace overtime {

	orthographCamera::orthographCamera(float left, float right, float bottom, float top, float near, float far)
		:m_ProjectionMatrix(glm::ortho(left, right, bottom, top, near, far)), m_ViewMatrix(1.0f)
	{
		m_PVMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	void orthographCamera::setProjectionMatrix(float left, float right, float bottom, float top, float near, float far)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, near, far);
		m_PVMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	void orthographCamera::onUpdate()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Pos);
		transform = glm::rotate(transform, glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		
		m_ViewMatrix = glm::inverse(transform);
		m_PVMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
