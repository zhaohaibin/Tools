#ifndef _PERMISSIONS_H
#define _PERMISSIONS_H


#include <boost/filesystem.hpp>

//获取文件权限
boost::filesystem::perms permissions(const boost::filesystem::path& file)
{
	boost::filesystem::file_status status = boost::filesystem::status(file);
	return status.permissions();
}

//修改文件权限
void modify_perms(const boost::filesystem::path& file, boost::filesystem::perms power)
{
	boost::system::error_code ec;
	boost::filesystem::permissions(file, power,ec);
}

//检查是否有对应的权限
//@param power待检查的权限
//@param targetPower目标权限，检查power中是否存在targetPower
//存在目标权限返回true,否则返回false
bool perms_check(boost::filesystem::perms power, boost::filesystem::perms targetPower)
{
	return (power&targetPower)==targetPower;
}
#endif