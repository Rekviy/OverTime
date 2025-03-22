#include "rendererAPI.h"
#include "openGL/openGLRenderCommand.h"

namespace overtime {
	rendererAPI::API rendererAPI::s_RendererAPI = API::OpenGL;
	renderCommand* rendererAPI::s_RenderCommands = new openGLRenderCommand;

}