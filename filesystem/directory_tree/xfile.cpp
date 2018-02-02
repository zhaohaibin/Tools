#include "xfile.h"
#include <boost/filesystem.hpp>
#include "../misc/string_helper/string_helper.h"

namespace kingfile
{
	namespace filesystem
	{
		xfile::xfile()
			: m_path(L"")
			, m_name(L"")
			, m_mtime(0)
			, m_size(0)
			, m_version(0)
			, m_opVer(0)
			, m_xid(0)
			, m_pxid(0)
			, m_type(XFILE_FOLDER)
			, m_role(file_role::EKP_FILE_ROLE_UNKOWN)
			, m_locked(0)
		{
		}

		xfile::xfile(const xfile& xFile )
		{
			m_path = xFile.m_path;
			m_name = xFile.m_name;
			m_mtime = xFile.m_mtime;
			m_size = xFile.m_size;
			m_version = xFile.m_version;
			m_opVer = xFile.m_opVer;
			m_xid = xFile.m_xid;
			m_pxid = xFile.m_pxid;
			m_type = xFile.m_type;
			m_role = xFile.m_role;
			m_sha1 = xFile.m_sha1;
			m_space_type = xFile.m_space_type;
			m_locked = xFile.m_locked;
		}

		xfile::~xfile()
		{
		}

		shared_ptr<xfile> xfile::create(const local_path& path, string& strError )
		{
			shared_ptr<xfile> spXfile(new xfile());
			try
			{
				bool isDir = boost::filesystem::is_directory(path.path());
				if(isDir)
				{
					spXfile->m_type = XFILE_FOLDER;
				}else if (boost::filesystem::is_regular_file(path.path()))
				{
					spXfile->m_type = XFILE_FILE;
					spXfile->m_size  = boost::filesystem::file_size(path.path());

				}
				else
				{
					strError  = "路径过长或文件系统错误,无法处理:" + path.path().string();
					spXfile.reset();
					return spXfile;
				}
				spXfile->m_path = path.generic_string();
				spXfile->m_name = path.file_name();
				//std::replace(spXfile->m_path.begin(), spXfile->m_path.end(), L'\\', L'/');
				//int pos = spXfile->m_path.rfind('/');
				//spXfile->m_name = path.substr(pos+1,path.size()-pos-1);
				time_t mtime = boost::filesystem::last_write_time(path.path());
				if(mtime < 0)
					spXfile->m_mtime = 0;
				else
					spXfile->m_mtime = mtime;
			}catch(boost::filesystem::filesystem_error error)
			{
				strError = error.what();
				spXfile.reset();
			}
			return spXfile;
		}

		bool xfile::is_directory()
		{
			return m_type == XFILE_FOLDER;
		}

		void xfile::to_json( Json::Value& jobj )
		{
			jobj["name"]				= string_helper::wstring_to_utf8(m_name);
			jobj["path"]				= string_helper::wstring_to_utf8(m_path);
			jobj["size"]				= m_size;
			jobj["mtime"]			= m_mtime;
			jobj["version"]			= m_version;
			jobj["opver"]				= m_opVer;
			jobj["xid"]					= m_xid;
			jobj["pxid"]				= m_pxid;
			jobj["type"]				= m_type;
			jobj["sha1"]				= m_sha1;
			jobj["role"]				= m_role;
			jobj["space_type"]		= m_space_type;
			jobj["status"]				= m_status;
			jobj["locked"]			= m_locked;
		}

		void xfile::parse( const Json::Value& jobj )
		{
			m_name			= string_helper::utf8_to_wstring(jobj["name"].asString());
			m_mtime			= jobj["mtime"].asUInt64();
			m_version			= jobj["file_version"].asInt64();
			m_opVer			= jobj["op_version"].asUInt64();
			m_xid				= jobj["xid"].asUInt64();
			m_pxid				= jobj["parent_xid"].asUInt64();
			m_sha1				= jobj["sha1"].asString();
			m_status			= jobj["status"].asUInt();
			if (jobj["is_share"].isNumeric())
				m_space_type =	(SPACE_TYPE)jobj["is_share"].asInt();
			if(jobj.isMember("locked") == true)
			{
				m_locked = jobj["locked"].asInt();
			}else
			{
				m_locked = 0;
			}
			if(m_space_type == SPACE_PRIVATE)
				m_role			= file_role::EKP_FILE_ROLE_OWNER;
			else if(m_space_type == SPACE_PUBLIC)
				m_role			= file_role::role_identiy(jobj["role"].asUInt64());
			if(m_sha1.empty() == false)
			{
				m_type = XFILE_FILE;
				m_size	= jobj["xsize"].asUInt64();
			}
			else
			{
				m_size	= 0;
				m_type = XFILE_FOLDER;
			}
		}

		std::string xfile::to_json_string()
		{
			Json::Value jobj;
			to_json(jobj);
			return Json::FastWriter().write(jobj);
		}

		std::string xfile::utf8_path()
		{
			return string_helper::wstring_to_utf8(m_path);
		}

		std::string xfile::utf8_name()
		{
			return string_helper::wstring_to_utf8(m_name);
		}

		std::wstring xfile::file_info()
		{
			wstring str = L"path:" + m_path+L", name:"+m_name + L", xid:" + string_helper::cast_to_string<wstring>(m_xid);
			str += L" pxid:" + string_helper::cast_to_string<wstring>(m_pxid);
			return str;
		}

		bool xfile_desc_comp( xfile_shared_ptr l, xfile_shared_ptr r )
		{
			wstring lPath = l->m_path;
			wstring rPath = r->m_path;
			transform(lPath.begin(), lPath.end(), lPath.begin(), tolower);
			transform(rPath.begin(), rPath.end(), rPath.begin(), tolower);
			return lPath > rPath;
		}

		bool xfile_asce_comp( xfile_shared_ptr l, xfile_shared_ptr r )
		{
			return xfile_desc_comp(r, l);
		}

	}
}