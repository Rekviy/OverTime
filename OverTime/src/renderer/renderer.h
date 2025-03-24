#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "rendererAPI.h"
#include "shader.h"
#include "camera.h"

namespace overtime {
	class renderer {
	public:
		static void beginScene(orthographCamera& camera);
		static void endScene();
		static void submit(const std::shared_ptr<vertexArray>& vertexArray, const std::shared_ptr<shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));

	private:
		struct sceneData {
			glm::mat4 PVMatrix;
		};
		static sceneData* s_SceneData;
	};
}
#endif