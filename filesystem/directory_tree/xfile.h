#ifndef _XFILE_H
#define _XFILE_H

#include <string>
#include <cstdint>
#include <memory>
#include "JsonCpp/json.h"
#include "../role/role.h"
#include "../filesystem/local_path.h"

using namespace std;

namespace kingfile
{
	namespace filesystem
	{
		enum XFILE_TYPE
		{
			XFILE_FOLDER,
			XFILE_FILE
		};

		enum SPACE_TYPE
		{
			SPACE_PRIVATE,
			SPACE_PUBLIC
		};

		class xfile
		{
		public:
			xfile();
			xfile(const xfile& xFile);
			~xfile();

		public:
			static shared_ptr<xfile> create(const local_path& path, string& strError);

			bool is_directory();
			void to_json(Json::Value& jobj);
			string to_json_string();
			wstring file_info();
			void parse(const Json::Value& jobj);
			string utf8_path();
			string utf8_name();
		public:
			wstring			m_name;
			wstring			m_path;
			uint64_t		m_size;
			uint64_t		m_mtime;
			int64_t			m_version;
			uint64_t		m_opVer;
			uint64_t		m_xid;
			uint64_t		m_pxid;
			XFILE_TYPE	m_type;
			string			m_sha1;
			int				m_status;//0正常，1被删除
			file_role::EKP_FILE_ROLE m_role;
			SPACE_TYPE m_space_type;
			int				m_locked;//0正常，1被锁定
		};

		typedef  shared_ptr<xfile> xfile_shared_ptr;

		template<typename T>
		xfile_shared_ptr create_xfile(const local_path& path, string& strError)
		{
			return T::create(path, strError);
		}

		bool xfile_desc_comp(xfile_shared_ptr l, xfile_shared_ptr r);
		bool xfile_asce_comp(xfile_shared_ptr l, xfile_shared_ptr r);
	}
}
#endif


