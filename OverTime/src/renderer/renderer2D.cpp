#include "renderer2D.h"
#include "vertexArray.h"
#include "shader.h"
#include "rendererAPI.h"

#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

namespace overtime {
	struct renderer2D::storage {
		ref<vertexArray> quadVA;
		ref<shader> textureShader;
		ref<texture2D> whiteTexture;
	};

	static renderer2D::storage* s_Data;

	void renderer2D::init()
	{
		s_Data = new renderer2D::storage();
		float squareVertices[5 * 4] = {

			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};
		s_Data->quadVA = vertexArray::create();
		ref<vertexBuffer> squareVB;
		squareVB = vertexBuffer::create(squareVertices, sizeof(squareVertices));

		squareVB->setLayout({ { shaderDataType::Float3, "position" },
							  { shaderDataType::Float2, "texCoord" }
			});
		s_Data->quadVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		ref<indexBuffer> squareIB;
		squareIB = indexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->quadVA->setIndexBuffer(squareIB);

		s_Data->whiteTexture = texture2D::create(1, 1);

		uint32_t whiteData = 0xffffffff;
		s_Data->whiteTexture->setData(&whiteData, sizeof(uint32_t));

		s_Data->textureShader = shader::create({ {shader::type::vertex, "assets/shaders/texture.vert"},
												 {shader::type::fragment, "assets/shaders/texture.frag"} });
		s_Data->textureShader->bind();
		s_Data->textureShader->setInt("u_Texture", 0);

	}

	void renderer2D::shutdown()
	{
		delete s_Data;
	}

	void renderer2D::beginScene(const orthographCamera& camera)
	{
		s_Data->textureShader->bind();
		s_Data->textureShader->setMat4("u_ProjView", camera.getPVMatrix());
	}

	void renderer2D::endScene()
	{}

	void renderer2D::drawSquad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
	{
		drawSquad({ pos.x,pos.y, 0.0f }, size, color, s_Data->whiteTexture, 1.0f);
	}
	void renderer2D::drawSquad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color)
	{
		drawSquad(pos, size, color, s_Data->whiteTexture, 1.0f);
	}
	void renderer2D::drawSquad(const glm::vec2& pos, const glm::vec2& size, const ref<texture2D>& texture, float textureScale)
	{
		drawSquad({ pos.x,pos.y, 0.0f }, size, glm::vec4(1.0f), texture, textureScale);
	}
	void renderer2D::drawSquad(const glm::vec3& pos, const glm::vec2& size, const ref<texture2D>& texture, float textureScale)
	{
		drawSquad(pos, size, glm::vec4(1.0f), texture, textureScale);
	}

	void renderer2D::drawSquad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, const ref<texture2D>& texture, float textureScale)
	{
		s_Data->textureShader->setFloat4("u_Color", color);
		s_Data->textureShader->setFloat("u_TexScale", textureScale);
		texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		s_Data->textureShader->setMat4("u_Transform", transform);

		s_Data->quadVA->bind();
		rendererAPI::drawIndexed(s_Data->quadVA);
	}

	void renderer2D::drawRotatedSquad(const glm::vec2& pos, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		drawRotatedSquad({ pos.x,pos.y, 0.0f }, size, rotation, color, s_Data->whiteTexture, 1.0f);
	}
	void renderer2D::drawRotatedSquad(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		drawRotatedSquad(pos, size, rotation, color, s_Data->whiteTexture, 1.0f);
	}
	void renderer2D::drawRotatedSquad(const glm::vec2& pos, const glm::vec2& size, float rotation, const ref<texture2D>& texture, float textureScale)
	{
		drawRotatedSquad({ pos.x,pos.y,0.0f }, size, rotation, glm::vec4(1.0f), texture, textureScale);
	}
	void renderer2D::drawRotatedSquad(const glm::vec3& pos, const glm::vec2& size, float rotation, const ref<texture2D>& texture, float textureScale)
	{
		drawRotatedSquad(pos, size, rotation, glm::vec4(1.0f), texture, textureScale);
	}

	void renderer2D::drawRotatedSquad(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& color, const ref<texture2D>& texture, float textureScale)
	{
		s_Data->textureShader->setFloat4("u_Color", color);
		s_Data->textureShader->setFloat("u_TexScale", textureScale);
		texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		transform = glm::rotate(transform, rotation, glm::vec3(0, 0, 1));
		s_Data->textureShader->setMat4("u_Transform", transform);

		s_Data->quadVA->bind();
		rendererAPI::drawIndexed(s_Data->quadVA);
	}
}
