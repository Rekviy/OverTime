#include "renderer2D.h"
#include "vertexArray.h"
#include "shader.h"
#include "rendererAPI.h"

#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

namespace overtime {
	struct renderer2D::storage {
		ref<vertexArray> quadVA;
		ref<shader> colorShader;
	};

	static renderer2D::storage* s_Data;

	void renderer2D::init()
	{
		s_Data = new renderer2D::storage();
		float squareVertices[3 * 4] = {

			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};
		s_Data->quadVA = vertexArray::create();
		ref<vertexBuffer> squareVB;
		squareVB = vertexBuffer::create(squareVertices, sizeof(squareVertices));

		squareVB->setLayout({ { shaderDataType::Float3, "position" } });
		s_Data->quadVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		ref<indexBuffer> squareIB;
		squareIB = indexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->quadVA->setIndexBuffer(squareIB);
		s_Data->colorShader = shader::create({ {shader::type::vertex, "assets/shaders/color.vert"},
											   {shader::type::fragment, "assets/shaders/color.frag"} });

	}

	void renderer2D::shutdown()
	{
		delete s_Data;
	}

	void renderer2D::beginScene(const orthographCamera& camera)
	{
		s_Data->colorShader->bind();
		s_Data->colorShader->setMat4("u_ProjView", camera.getPVMatrix());
	}

	void renderer2D::endScene()
	{}

	void renderer2D::drawSquad(const glm::vec4& color, const glm::vec2& pos, const glm::vec2& size, float rotation)
	{
		drawSquad(color, { pos.x,pos.y, 0.0f }, size, rotation);
	}

	void renderer2D::drawSquad(const glm::vec4& color, const glm::vec3& pos, const glm::vec2& size, float rotation)
	{
		s_Data->colorShader->bind();
		s_Data->colorShader->setFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0, 0, 1));
		s_Data->colorShader->setMat4("u_Transform", transform);

		s_Data->quadVA->bind();
		rendererAPI::drawIndexed(s_Data->quadVA);
	}
}
