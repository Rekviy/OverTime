#include "Breeze.h"
#include "renderer/openGL/openGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class testLayer : public overtime::layer {
public:
	testLayer() : layer("testLayer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f, -1.0f, 1.0f), m_CamPos(0.0f)
	{
		m_VertexArray.reset(overtime::vertexArray::create());
		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			  0.0f, 0.5f, 0.0f,
		};
		std::shared_ptr<overtime::vertexBuffer> vBuffer;
		vBuffer.reset(overtime::vertexBuffer::create(vertices, sizeof(vertices)));

		overtime::bufferLayout layout = {
			{overtime::shaderDataType::Float3, "a_Pos"}
		};

		vBuffer->setLayout(layout);
		m_VertexArray->addVertexBuffer(vBuffer);

		unsigned indices[3] = { 0,1,2 };
		std::shared_ptr<overtime::indexBuffer> iBuffer;
		iBuffer.reset(overtime::indexBuffer::create(indices, 3));
		m_VertexArray->setIndexBuffer(iBuffer);

		std::string vertexSrc = R"(
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

		std::string fragmentSrc = R"(
			layout(location = 0) out vec4 color;
			in vec3 v_Pos;
			uniform vec3 u_Color;
			void main()
			{
				//vec3 col = 0.5 + 0.5*cos(v_Pos.xyx+vec3(0,2,4));
				
				// Output to screen
				//color = vec4(col,1.0);
				color = vec4(u_Color, 1.0);
			}
		)";


		m_Shader.reset(overtime::shader::create(vertexSrc, fragmentSrc));
	}
	void onUpdate(overtime::timeStep ts) override
	{
		//OT_INFO("FrameTime: {0}s; {1}ms", ts.getSeconds(), ts.getMilliseconds());
		if (overtime::input::isKeyPressed(OT_KEY_W))
			m_CamPos.y += m_CamMoveSpeed * ts;
		else if (overtime::input::isKeyPressed(OT_KEY_S))
			m_CamPos.y -= m_CamMoveSpeed * ts;
		if (overtime::input::isKeyPressed(OT_KEY_A))
			m_CamPos.x -= m_CamMoveSpeed * ts;
		else if (overtime::input::isKeyPressed(OT_KEY_D))
			m_CamPos.x += m_CamMoveSpeed * ts;
		if (overtime::input::isKeyPressed(OT_KEY_E))
			m_CamRotation += m_CamRotationSpeed * ts;
		else if (overtime::input::isKeyPressed(OT_KEY_Q))
			m_CamRotation -= m_CamRotationSpeed * ts;

		OT_INFO("camera x:{0} y:{1} angle:{2}", m_CamPos.x, m_CamPos.y, m_CamRotation);

		overtime::rendererAPI::setClearColor({ 0, 0.6f, 0.6f, 1 });
		overtime::rendererAPI::clear();

		//m_Camera.setPosition(m_CamPos);
		m_Camera.setRotation(m_CamRotation);

		overtime::renderer::beginScene(m_Camera);
		std::dynamic_pointer_cast<overtime::openGLShader>(m_Shader)->bind();
		std::dynamic_pointer_cast<overtime::openGLShader>(m_Shader)->uploadUniformFloat3("u_Color", m_SquareColor);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int i = 0; i < 3; i++) {
			glm::vec3 pos(m_CamPos.x + i * 0.1f, m_CamPos.y + i * 0.1f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			transform = glm::rotate(transform, glm::radians(m_CamRotation), glm::vec3(0, 0, 1));
			overtime::renderer::submit(m_VertexArray, m_Shader, transform);
		}


		overtime::renderer::endScene();

	}
	void onImGuiRender()
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}
	void onEvent(overtime::event& event) override
	{}

private:
	glm::vec3 m_CamPos;
	float m_CamMoveSpeed = 5.0f;
	float m_CamRotation = 0.0f;
	float m_CamRotationSpeed = 45.0f;

	std::shared_ptr<overtime::vertexArray> m_VertexArray;
	std::shared_ptr<overtime::shader> m_Shader;
	overtime::orthographCamera m_Camera;

	glm::vec3 m_SquareColor = { 0.4f, 0.8f, 0.2f };
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
