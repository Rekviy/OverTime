#pragma once
#ifndef BUFFER_H
#define BUFFER_H 

#include "core/core.h"

#include <string>
#include <vector>

namespace overtime {

	enum class shaderDataType {
		None, Bool, Int, Int2, Int3, Int4, Float, Float2, Float3, Float4, Mat3, Mat4
	};

	static uint32_t shaderDataTypeSize(shaderDataType type)
	{
		switch (type) {
			case shaderDataType::Bool:   return 1;
			case shaderDataType::Int:
			case shaderDataType::Float:  return 4;
			case shaderDataType::Int2:
			case shaderDataType::Float2: return 8;
			case shaderDataType::Int3:
			case shaderDataType::Float3: return 12;
			case shaderDataType::Int4:
			case shaderDataType::Float4: return 16;
			case shaderDataType::Mat3:   return 4 * 3 * 3;
			case shaderDataType::Mat4:   return 4 * 4 * 4;

		}
		OT_CORE_ASSERT(false, "Unknown shaderDataType!");
		return 0;
	}
	struct bufferElement {
		std::string name;
		shaderDataType type;
		uint32_t size;
		uint32_t offset;
		bool normalized;
		bufferElement()
			:type(shaderDataType::None), size(shaderDataTypeSize(type)), offset(0), normalized(false)
		{}
		bufferElement(shaderDataType type, const std::string& name, bool normalized = false)
			:type(type), name(name), size(shaderDataTypeSize(type)), offset(0), normalized(normalized)
		{}

		uint32_t getComponentCount() const
		{
			switch (type) {
				case shaderDataType::Bool:   return 1;
				case shaderDataType::Int:
				case shaderDataType::Float:  return 1;
				case shaderDataType::Int2:
				case shaderDataType::Float2: return 2;
				case shaderDataType::Int3:
				case shaderDataType::Float3: return 3;
				case shaderDataType::Int4:
				case shaderDataType::Float4: return 4;
				case shaderDataType::Mat3:   return 3 * 3;
				case shaderDataType::Mat4:   return 4 * 4;

			}
			OT_CORE_ASSERT(false, "Unknown shaderDataType!");
			return 0;
		}
	};
	class bufferLayout {
	public:
		bufferLayout() {}
		bufferLayout(const std::initializer_list<bufferElement>& elements)
			:m_Elements(elements)
		{
			calculateOffsetAndStride();
		}
		inline uint32_t getStride() const { return m_Stride; }
		inline const std::vector<bufferElement>& getElements() const { return m_Elements; }
		std::vector<bufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<bufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<bufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<bufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void calculateOffsetAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& elem : m_Elements) {
				elem.offset = offset;
				offset += elem.size;
				m_Stride += elem.size;
			}
		}
		std::vector<bufferElement> m_Elements;
		uint32_t m_Stride;
	};

	class vertexBuffer {
	public:
		virtual ~vertexBuffer() {}
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual const bufferLayout& getLayout() const = 0;
		virtual void setLayout(const bufferLayout& layout) = 0;

		static ref<vertexBuffer> create(float* vertices, uint32_t size);
	};

	class indexBuffer {
	public:
		virtual ~indexBuffer() {}
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static ref<indexBuffer> create(uint32_t* indices, uint32_t size);
	};
}


#endif //BUFFER_H