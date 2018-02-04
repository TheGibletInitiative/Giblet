//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <stdexcept>
 #include <cstdint>


namespace Giblet { namespace CodeHook { namespace x86
{

	class OpcodeIterator
	{
	private:

		union
		{
			const uint8_t	*m_Pointer;
			const uint16_t	*m_Pointer16;
			const uint32_t	*m_Pointer32;
			const uint64_t	*m_Pointer64;
		};


	public:

		explicit OpcodeIterator()
		{
			m_Pointer = NULL;
		}


		explicit OpcodeIterator(const uint8_t *ptr)
		{
			m_Pointer = ptr;
		}


		OpcodeIterator(const OpcodeIterator &src)
		{
			*this = src;
		}


		OpcodeIterator &operator=(const OpcodeIterator &src)
		{
			m_Pointer = src.m_Pointer;
			return *this;
		}


		size_t operator-(const OpcodeIterator &src) const
		{
			return m_Pointer - src.m_Pointer;
		}


		uint8_t operator[](size_t index)	const
		{
			if (!m_Pointer)
			{
				throw std::runtime_error("pointer is null");
			}

			return m_Pointer[index];
		}


		void clear()
		{
			m_Pointer = NULL;
		}


		const void *c_data() const
		{
			return m_Pointer;
		}


		uint8_t NextI8()
		{
			if (!m_Pointer)
			{
				throw std::runtime_error("pointer is null");
			}

			return *(m_Pointer++);
		}


		uint16_t NextI16()
		{
			if (!m_Pointer16)
			{
				throw std::runtime_error("pointer is null");
			}

			return *(m_Pointer16++);
		}


		uint32_t NextI32()
		{
			if (!m_Pointer32)
			{
				throw std::runtime_error("pointer is null");
			}

			return *(m_Pointer32++);
		}


		uint64_t NextI64()
		{
			if (!m_Pointer64)
			{
				throw std::runtime_error("pointer is null");
			}

			return *(m_Pointer64++);
		}


		uint64_t getAddress() const
		{
			return reinterpret_cast<const uint64_t>(m_Pointer);
		}

	};

}}}
