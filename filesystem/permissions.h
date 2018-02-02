#ifndef _PERMISSIONS_H
#define _PERMISSIONS_H


#include <boost/filesystem.hpp>

//��ȡ�ļ�Ȩ��
boost::filesystem::perms permissions(const boost::filesystem::path& file)
{
	boost::filesystem::file_status status = boost::filesystem::status(file);
	return status.permissions();
}

//�޸��ļ�Ȩ��
void modify_perms(const boost::filesystem::path& file, boost::filesystem::perms power)
{
	boost::system::error_code ec;
	boost::filesystem::permissions(file, power,ec);
}

//����Ƿ��ж�Ӧ��Ȩ��
//@param power������Ȩ��
//@param targetPowerĿ��Ȩ�ޣ����power���Ƿ����targetPower
//����Ŀ��Ȩ�޷���true,���򷵻�false
bool perms_check(boost::filesystem::perms power, boost::filesystem::perms targetPower)
{
	return (power&targetPower)==targetPower;
}
#endif