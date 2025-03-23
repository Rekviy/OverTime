#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

namespace overtime {
	class orthographCamera {
	public:
		orthographCamera(float left, float right, float bottom, float top, float near, float far);

		const glm::vec3& getPosition() const { return m_Pos; }
		void setPosition(const glm::vec3& position) { m_Pos = position; onUpdate(); }

		const float getRotation() const { return m_Rotation; }
		void setRotation(float rotation) { m_Rotation = rotation; onUpdate(); }

		const glm::mat4& getProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& getViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& getPVMatrix() const { return m_PVMatrix; }


	private:
		void onUpdate();

		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_PVMatrix;

		glm::vec3 m_Pos = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;

	};
}


#endif