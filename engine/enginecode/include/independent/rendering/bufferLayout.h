/** \file bufferLayout.h */
#pragma once

#include <vector>
#include "shaderDataType.h"

namespace Engine
{
	/** 
	\ Class VertexBufferElement
	Holds data about a single element in a vertex buffer layout 
	*/
	class VertexBufferElement
	{
	public:
		//doxygen comment these
		ShaderDataType m_dataType;
		uint32_t m_size;
		uint32_t m_offset;
		bool m_normalised;

		VertexBufferElement() {} 
		VertexBufferElement(ShaderDataType arg_type, bool arg_normalised = false) :
			m_dataType(arg_type), 
			m_size(SDT::size(arg_type)),
			m_offset(0), 
			m_normalised(arg_normalised)
		{}
	};


	/**
	\ Class UniformBufferElement
	Holds data about a single element in a uniform buffer layout
	*/
	class UniformBufferElement
	{
	public:
		//doxygen comment these
		const char* m_name;
		ShaderDataType m_dataType;
		uint32_t m_size;
		uint32_t m_offset;

		UniformBufferElement() {} //!< Default constructor
		UniformBufferElement(const char* arg_name, ShaderDataType arg_type) :  //!< Initiallizer constructor
			m_name(arg_name),
			m_dataType(arg_type),
			m_size(SDT::std104alignment(arg_type)),
			m_offset(0)
		{}
	};

	/**
	\ Class BufferLayout

	*/
	template <class G>
	class BufferLayout
	{
	public:
		BufferLayout<G>(const std::initializer_list<G>& arg_element) : m_elements(arg_element) { calcStrideAndOffset(); }
		BufferLayout<G>() {}
		inline uint32_t getStride() const { return m_stride; }
		void addElement(G arg_element);
		inline typename std::vector<G>::iterator begin() { return m_elements.begin(); }
		inline typename std::vector<G>::iterator end() { return m_elements.end(); }
		inline typename std::vector<G>::const_iterator begin() const { return m_elements.begin(); }
		inline typename std::vector<G>::const_iterator end() const { return m_elements.end(); }
	private:
		std::vector<G> m_elements; //!< Buffer elements
		uint32_t m_stride; //!< Size in bytes of the buffer line#
		void calcStrideAndOffset(); //!< Calculate stride and offsets based on elements
	};

	template <class G>
	void BufferLayout<G>::addElement(G arg_element)
	{
		m_elements.push_back(arg_element);
		calcStrideAndOffset();
	}

	template <class G>
	void BufferLayout<G>::calcStrideAndOffset()
	{
		uint32_t l_offset = 0; //!< local offset value

		for (auto& element : m_elements)
		{
			element.m_offset = l_offset;
			l_offset += element.m_size; //!< Adds the size of the calculated element to the local offset.
		}

		m_stride = l_offset;
	}
	

	using VertexBufferLayout = BufferLayout<VertexBufferElement>; //!< Whenever vertexBufferLayout is called, refer to BufferLayout<VBE>
	using UniformBufferLayout = BufferLayout<UniformBufferElement>;
}