#pragma once
#ifndef RENDERER_H
#define RENDERER_H


namespace overtime {
	enum class rendererAPI { None = 0, OpenGL = 1 };

	class renderer {
	public:
		inline static rendererAPI getAPI() { return s_RendererAPI; }
	private:
		static rendererAPI s_RendererAPI;
	};
}
#endif