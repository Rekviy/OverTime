#pragma once
#ifndef OPENGLTEXTURE_H
#define OPENGLTEXTURE_H

#include "renderer/texture.h"

#include <glad/glad.h>

namespace overtime {
	class openGLTexture2D : public texture2D {
	public:
		openGLTexture2D(const std::string& path);
		openGLTexture2D(uint32_t width, uint32_t height);
		~openGLTexture2D() override;
		virtual void setData(void* data, uint32_t size) override;
		virtual int32_t getWidth() const override { return m_Width; }
		virtual int32_t getHeight() const { return m_Height; }
		virtual void bind(uint32_t slot = 0) const override;
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererId;
		GLenum m_InternalFormat, m_DataFormat = 0;
	};
}

#endif