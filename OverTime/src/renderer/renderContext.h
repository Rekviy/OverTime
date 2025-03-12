#pragma once
#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

namespace overtime {
	class renderContext {
	public:
		virtual void init() = 0;
		virtual void swap() = 0;
	};
}
#endif