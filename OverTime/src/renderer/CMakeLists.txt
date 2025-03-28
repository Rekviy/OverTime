set(OT_RENDERER_HEADER
	src/renderer/renderer.h
	src/renderer/rendererAPI.h
	src/renderer/renderContext.h
	src/renderer/renderCommand.h
	src/renderer/shader.h
	src/renderer/buffer.h
	src/renderer/vertexArray.h
	src/renderer/camera.h
	
)
set(OT_RENDERER_SRC
	src/renderer/renderer.cpp
	src/renderer/rendererAPI.cpp
	src/renderer/shader.cpp
	src/renderer/buffer.cpp
	src/renderer/vertexArray.cpp
	src/renderer/camera.cpp
)

set(OT_OPENGL_RENDERER_HEADER
	src/renderer/openGL/openGLContext.h
	src/renderer/openGL/openGLBuffer.h
	src/renderer/openGL/openGLVertexArray.h
	src/renderer/openGL/openGLRenderCommand.h
	src/renderer/openGL/openGLShader.h
)

set(OT_OPENGL_RENDERER_SRC
	src/renderer/openGL/openGLContext.cpp
	src/renderer/openGL/openGLBuffer.cpp
	src/renderer/openGL/openGLVertexArray.cpp
	src/renderer/openGL/openGLRenderCommand.cpp
	src/renderer/openGL/openGLShader.cpp
)

list(APPEND OT_RENDERER_HEADER ${OT_OPENGL_RENDERER_HEADER})
list(APPEND OT_RENDERER_SRC ${OT_OPENGL_RENDERER_SRC})