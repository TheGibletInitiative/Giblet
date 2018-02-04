//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once



namespace Giblet { namespace CodeHook { namespace x86
{

	template<typename DataType_, typename BaseType_ = DataType_> struct PostByteT
	{
	public:

		PostByteT()
			:
			m_HasData(false),
			m_DataValue(0)
		{}


		PostByteT &operator=(const PostByteT &src)
		{
			m_HasData = src.m_HasData;
			m_DataValue = src.m_DataValue;
			return *this;
		}


		void SetDataValue(DataType_ data)
		{
			m_HasData = true;
			m_DataValue = data;
		}


		DataType_ GetDataValue() const
		{
			return m_DataValue;
		}


		bool HasData() const
		{
			return m_HasData;
		}


		void Reset()
		{
			m_DataValue = 0;
			m_HasData = false;
		}


	public:

		//	FIXME: This is UB!
		union
		{
			BaseType_	m_BitData;
			DataType_	m_DataValue;
		};


	private:

		bool	m_HasData;
	};

}}}
