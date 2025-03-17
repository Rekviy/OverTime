#pragma once
#ifndef BUFFER_H
#define BUFFER_H 
namespace overtime {
	class vertexBuffer {
	public:
		virtual ~vertexBuffer() {}
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		
		static vertexBuffer* create(float* vertices, uint32_t size);
	};

	class indexBuffer {
	public:
		virtual ~indexBuffer() {}
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static indexBuffer* create(uint32_t* indices, uint32_t size);
	};
}


#endif //BUFFER_H