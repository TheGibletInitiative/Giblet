//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#pragma once
#include <Protocols/YMsg/Parser.h>


namespace Giblet { namespace Utility
{

	template<class Type_>
	class SharedPtrAsRefArg
	{
	public:

		SharedPtrAsRefArg() = delete;
		SharedPtrAsRefArg(const SharedPtrAsRefArg&) = default;
		explicit SharedPtrAsRefArg(std::shared_ptr<Type_> ptr)
			: ptr_(ptr)
		{
			if (!ptr_)
			{
				throw std::runtime_error("ptr cannot be null");
			}
		}

		operator std::shared_ptr<Type_>()
		{
			return ptr_;
		}

		operator std::shared_ptr<const Type_>() const
		{
			return ptr_;
		}

		operator Type_&()
		{
			return *ptr_;
		}

		operator const Type_&() const
		{
			return *ptr_;
		}


	private:

		std::shared_ptr<Type_> ptr_;
	};

	template<class Type_>
	SharedPtrAsRefArg<Type_> MakeSharedPtrAsRefArg(std::shared_ptr<Type_> ptr)
	{
		return SharedPtrAsRefArg<Type_>(ptr);
	}

}}
