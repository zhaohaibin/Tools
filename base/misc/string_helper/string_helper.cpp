#include "string_helper.h"
#include <codecvt>
#include <algorithm>

namespace string_helper
{
	std::string wstring_to_utf8( const std::wstring& str )
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>,wchar_t> conv;
		return conv.to_bytes(str);
	}

	std::wstring utf8_to_wstring( const std::string& str )
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>,wchar_t> conv;
		return conv.from_bytes(str.c_str());
	}

	std::wstring lower_string( const std::wstring& str )
	{
		std::wstring lowerStr = str;
		std::transform(lowerStr.begin(), lowerStr.end(),lowerStr.begin(),towlower);
		return lowerStr;
	}
}