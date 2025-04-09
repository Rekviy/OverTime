#include "Breeze.h"
#include "renderer/openGL/openGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class testLayer : public overtime::layer {
public:
	testLayer() : layer("testLayer"), _cameraControls(1280.0f/720.0f,true)
	{
		float triangleVertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			  0.0f, 0.5f, 0.0f,
		};

		_triangleVA = overtime::vertexArray::create();
		overtime::ref<overtime::vertexBuffer> triangleVB;
		triangleVB = overtime::vertexBuffer::create(triangleVertices, sizeof(triangleVertices));

		triangleVB->setLayout({ { overtime::shaderDataType::Float3, "position" } });
		_triangleVA->addVertexBuffer(triangleVB);

		unsigned triangleIndices[3] = { 0,1,2 };
		overtime::ref<overtime::indexBuffer> triangleIB;
		triangleIB = overtime::indexBuffer::create(triangleIndices, sizeof(triangleIndices) / sizeof(uint32_t));
		_triangleVA->setIndexBuffer(triangleIB);

		float squareVertices[5 * 4] = {

			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};
		_squareVA = overtime::vertexArray::create();
		overtime::ref<overtime::vertexBuffer> squareVB;
		squareVB = overtime::vertexBuffer::create(squareVertices, sizeof(squareVertices));

		squareVB->setLayout({
			{ overtime::shaderDataType::Float3, "position" },
			{ overtime::shaderDataType::Float2, "texCoord" }
			});
		_squareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		overtime::ref<overtime::indexBuffer> squareIB;
		squareIB = overtime::indexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		_squareVA->setIndexBuffer(squareIB);

		std::string colorVertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 position;
			
			uniform mat4 u_ViewProj;
			uniform mat4 u_Transform;			

			out vec3 v_Pos;

			void main()
			{
				v_Pos = position;
				gl_Position = u_ViewProj * u_Transform * vec4(position, 1.0);
			}
		)";
		std::string colorFragmentSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			in vec3 v_Pos;
			uniform vec3 u_Color;
			void main()
			{
				//vec3 col = 0.5 + 0.5*cos(v_Pos.xyx+vec3(0,2,4));

				//color = vec4(col,1.0);
				color = vec4(u_Color, 1.0);
			}
		)";
		_colorShader = overtime::shader::create("colorShader", colorVertexSrc, colorFragmentSrc);

		auto textureShader = _shaderLib.load({ {overtime::shader::type::vertex, "assets/shaders/texture.vert"},
						  {overtime::shader::type::fragment, "assets/shaders/texture.frag"} });

		_texture = overtime::texture2D::create("assets/resources/Screenshot.png");
		_cherryTexture = overtime::texture2D::create("assets/resources/cherry.png");
		std::dynamic_pointer_cast<overtime::openGLShader>(textureShader)->bind();
		std::dynamic_pointer_cast<overtime::openGLShader>(textureShader)->uploadUniformInt("u_Texture", 0);

	}
	void onUpdate(overtime::timeStep ts) override
	{
		_cameraControls.onUpdate(ts);

		

		overtime::rendererAPI::setClearColor({ 0, 0.6f, 0.6f, 1 });
		overtime::rendererAPI::clear();

		overtime::renderer::beginScene(_cameraControls.getCamera());
		std::dynamic_pointer_cast<overtime::openGLShader>(_colorShader)->bind();
		std::dynamic_pointer_cast<overtime::openGLShader>(_colorShader)->uploadUniformFloat3("u_Color", _squareColor);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				//transform = glm::rotate(transform, glm::radians(_camRotation), glm::vec3(0, 0, 1));
				overtime::renderer::submit(_squareVA, _colorShader, transform);
			}
		}
		_texture->bind();
		auto textureShader = _shaderLib.get("texture");
		overtime::renderer::submit(_squareVA, textureShader, glm::scale(glm::mat4(1.0f), glm::vec3(0.9f)));
		_cherryTexture->bind();
		//overtime::renderer::submit(_triangleVA, _colorShader, transform);
		overtime::renderer::submit(_squareVA, textureShader, glm::scale(glm::mat4(1.0f), glm::vec3(0.9f)));



		overtime::renderer::endScene();

	}
	void onImGuiRender()
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Color", glm::value_ptr(_squareColor));
		ImGui::End();
	}
	void onEvent(overtime::event& event) override
	{
		_cameraControls.onEvent(event);
	}

private:

	overtime::shaderLibrary _shaderLib;
	overtime::ref<overtime::vertexArray> _triangleVA;
	overtime::ref<overtime::vertexArray> _squareVA;
	overtime::ref<overtime::shader> _colorShader;
	overtime::ref<overtime::texture2D> _texture;
	overtime::ref<overtime::texture2D> _cherryTexture;

	overtime::orthographCameraController _cameraControls;


	glm::vec3 _squareColor = { 0.4f, 0.8f, 0.2f };
};

class Breeze : public overtime::application {
public:
	Breeze()
	{
		pushLayer(new testLayer());
	}
	~Breeze()
	{}

private:

};

overtime::application* overtime::createApplication()
{
	return new Breeze();

}
