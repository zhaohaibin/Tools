#ifndef _BOOST_PATH_HELPER_H
#define _BOOST_PATH_HELPER_H
#include "boost/filesystem.hpp"

//path conv to string
template<typename T>
class path_conv
{
public:
	path_conv(const boost::filesystem::path& path)
		: m_path(path)
	{

	}
	T conv_to_string(){return T();}
private:
	const boost::filesystem::path& m_path;
};

template<>
class path_conv<string>
{
public:
	path_conv(const boost::filesystem::path& path)
		: m_path(path)
	{

	}
	string conv_to_string()
	{
		return m_path.generic_string();
	}
private:
	const boost::filesystem::path& m_path;
};

template<>
class path_conv<wstring>
{
public:
	path_conv(const boost::filesystem::path& path)
		: m_path(path)
	{

	}
	wstring conv_to_string()
	{
		return m_path.generic_wstring();
	}
private:
	const boost::filesystem::path& m_path;
};

//从路径中提取文件名
template<typename T>
class path_file_name
{

};

template<>
class path_file_name<string>
{
public:
	path_file_name(const boost::filesystem::path& path)
		: m_path(path)
	{

	}

	string name()
	{
		return m_path.filename().generic_string();
	}
private:
	const boost::filesystem::path& m_path;
};

//取路径中文件的名字
template<>
class path_file_name<wstring>
{
public:
	path_file_name(const boost::filesystem::path& path)
		: m_path(path)
	{

	}
	wstring name()
	{
		return m_path.filename().generic_wstring();
	}
private:
	const boost::filesystem::path& m_path;
};
#endif //_BOOST_PATH_HELPER_H