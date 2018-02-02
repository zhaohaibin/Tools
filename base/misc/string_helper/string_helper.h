#ifndef _STRING_HELPER_H
#define _STRING_HELPER_H

#include <string>
#include <locale>
#include <cstdint>
#include "boost/lexical_cast.hpp"

namespace string_helper
{
	std::string wstring_to_utf8(const std::wstring& str);
	std::wstring utf8_to_wstring(const std::string& str);

	std::wstring lower_string(const std::wstring& str);


	template<typename T1, typename T2>
	T1 cast_to_string(T2 val)
	{
		return boost::lexical_cast<T1>(val);
	}
}
#endif //_STRING_HELPER_H