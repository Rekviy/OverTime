set(OT_RENDERER_HEADER
	src/renderer/renderer.h
	src/renderer/rendererAPI.h
	src/renderer/renderContext.h
	src/renderer/renderCommand.h
	src/renderer/shader.h
	src/renderer/buffer.h
	src/renderer/vertexArray.h
	src/renderer/camera.h
	src/renderer/texture.h	
)

set(OT_RENDERER_SRC
	src/renderer/renderer.cpp
	src/renderer/rendererAPI.cpp
	src/renderer/shader.cpp
	src/renderer/buffer.cpp
	src/renderer/vertexArray.cpp
	src/renderer/camera.cpp
	src/renderer/texture.cpp
)
set(OT_RENDERER2D_HEADER
	src/renderer/renderer2D.h

)

set(OT_RENDERER2D_SRC
	src/renderer/renderer2D.cpp

)
set(OT_OPENGL_RENDERER_HEADER
	src/renderer/openGL/openGLContext.h
	src/renderer/openGL/openGLBuffer.h
	src/renderer/openGL/openGLVertexArray.h
	src/renderer/openGL/openGLRenderCommand.h
	src/renderer/openGL/openGLShader.h
	src/renderer/openGL/openGLTexture.h
)

set(OT_OPENGL_RENDERER_SRC
	src/renderer/openGL/openGLContext.cpp
	src/renderer/openGL/openGLBuffer.cpp
	src/renderer/openGL/openGLVertexArray.cpp
	src/renderer/openGL/openGLRenderCommand.cpp
	src/renderer/openGL/openGLShader.cpp
	src/renderer/openGL/openGLTexture.cpp
)

list(APPEND OT_RENDERER_HEADER ${OT_OPENGL_RENDERER_HEADER} ${OT_RENDERER2D_HEADER})
list(APPEND OT_RENDERER_SRC ${OT_OPENGL_RENDERER_SRC} ${OT_RENDERER2D_SRC})