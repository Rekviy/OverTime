#include "openGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace overtime {
	openGLTexture2D::openGLTexture2D(const std::string& path)
		:m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		OT_CORE_ASSERT(data, "Failed to load image!");
		m_Height = height; m_Width = width;
		GLenum internalForm, dataForm = 0;

		switch (channels) {
			case 3:
				internalForm = GL_RGB8;
				dataForm = GL_RGB;
				break;
			case 4:
				internalForm = GL_RGBA8;
				dataForm = GL_RGBA;
				break;
			default:
				OT_CORE_ASSERT(true, "Format not supported!");
				return;
		}


		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererId);

		glTextureStorage2D(m_RendererId, 1, internalForm, m_Width, m_Height);

		glTextureParameteri(m_RendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererId, 0, 0, 0, m_Width, m_Height, dataForm, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
		
	}
	openGLTexture2D::~openGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererId);

	}
	void openGLTexture2D::bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererId);
	}
}