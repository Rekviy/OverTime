#pragma once
#ifndef OPENGLTEXTURE_H
#define OPENGLTEXTURE_H

#include "renderer/texture.h"
namespace overtime {
	class openGLTexture2D : public texture2D {
	public:
		openGLTexture2D(const std::string& path);
		~openGLTexture2D() override;
		virtual int32_t getWidth() const override { return m_Width; }
		virtual int32_t getHeight() const { return m_Height; }
		virtual void bind(uint32_t slot = 0) const override;
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererId;

	};
}

#endif