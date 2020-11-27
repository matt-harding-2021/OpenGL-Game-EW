/** \file bufferLayout.h */
#pragma once

#include <vector>
#include "shaderDataType.h"

namespace Engine
{
	/** 
	\ Class BufferElement
	Holds data about a single element in a buffer layout 
	*/
	class BufferElement
	{
	public:
		//doxygen comment these
		ShaderDataType m_dataType;
		uint32_t m_size;
		uint32_t m_offset;
		bool m_normalised;

		BufferElement() {} 
		BufferElement(ShaderDataType type, bool normalised = false) : 
			m_dataType(type), 
			m_size(SDT::size(type)),
			m_offset(0), 
			m_normalised(normalised) 
		{}
	};

	/**
	\ Class BufferLayout

	*/
	class BufferLayout
	{
	public:
		BufferLayout();
		BufferLayout(const std::initializer_list<BufferElement>& element) : m_elements(element) { calcStrideAndOffset(); }
		inline uint32_t getStride() const { return m_stride; }
		void addElement(BufferElement element);
		inline std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		inline std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		inline std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		inline std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
	private:
		std::vector<BufferElement> m_elements; //!< Buffer elements
		uint32_t m_stride; //!< Size in bytes of the buffer line#
		void calcStrideAndOffset(); //!< Calculate stride and offsets based on elements
	};

	
}