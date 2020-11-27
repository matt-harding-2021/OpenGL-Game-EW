/** \file bufferLayout.cpp */

#include "engine_pch.h"
#include "rendering/bufferLayout.h"

namespace Engine
{

	void BufferLayout::addElement(BufferElement element)
{
	m_elements.push_back(element);
	calcStrideAndOffset();
}

void BufferLayout::calcStrideAndOffset()
{
	uint32_t l_offset = 0; //!< local offset value

	for (auto& element : m_elements)
	{
		element.m_offset = l_offset;
		l_offset += element.m_size; //!< Adds the size of the calculated element to the local offset.
	}

	m_stride = l_offset;
}
}