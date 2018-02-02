#include "directory_helper.h"
#include "../filesystem/file_system_helper.h"
#include "boost/lexical_cast.hpp"
#include "../user_db/table/table_helper/db_snapshot_table_helper.h"

namespace kingfile
{
	namespace filesystem
	{
		namespace directory_helper
		{
			shared_ptr<directory_tree> conv_to_directory_tree( shared_ptr<server_directory_tree> spTree, KingFileError& error )
			{
				shared_ptr<directory_tree> spDirectoryTree(new directory_tree());
				shared_ptr<KingFileError> spError(new KingFileError());
				bool ret = spTree->for_each(std::bind(_insert_to_directory_tree, std::placeholders::_1, spDirectoryTree, spError));
				if(ret == false)
				{
					spDirectoryTree->clear();
					error = *spError;
					return NULL;
				}
				return spDirectoryTree;
			}

			bool copy( const local_path& rootPath, shared_ptr<directory_tree>spTree, const local_path& targetPath, string& error )
			{
				bool ret = true;
				shared_ptr<string> spError(new string);
				ret = spTree->for_each(std::bind(_copy, rootPath, std::placeholders::_1, targetPath, spError));
				if(ret == false)
				{
					error = *spError;
				}
				return ret;
			}

			bool conv_to_vec( shared_ptr<directory_tree>spTree, shared_ptr<vector<xfile_shared_ptr>>& spXfileVec )
			{
				return spTree->for_each(std::bind(_conv_to_vec, std::placeholders::_1, spXfileVec));
			}

			bool _insert_to_directory_tree( shared_ptr<server_directory_tree::node> spNode, shared_ptr<directory_tree> spTree , shared_ptr<KingFileError> spError)
			{
				bool ret = true;
				ret = spTree->insert(spNode->file(), *spError);
				return ret;
			}

			bool _copy( const local_path& rootPath, shared_ptr<directory_tree::node> spNode, const local_path& path, shared_ptr<string> spError )
			{
				try
				{
					xfile_shared_ptr spXfile = spNode->file();
					local_path targetPath = path;
					targetPath.append(spNode->parent()->file()->m_path);
					targetPath.append(spXfile->m_name);
					//targetPath += spNode->parent()->file()->m_path;
					//targetPath += "/" + spXfile->m_name;
					if(spXfile->is_directory())
					{
						string error;
						if( make_dirs(targetPath, error) == false)
						{
							*spError = error;
							return false;
						}
					}else
					{
						//string fromPaht = rootPath + spXfile->m_path;
						local_path fromPath = rootPath;
						fromPath.append(spXfile->m_path);
						boost::filesystem::copy_file(fromPath.path(), targetPath.path());
					}
				}catch(boost::filesystem::filesystem_error error)
				{
					*spError = error.what();
					return false;
				}
				return true;
			}

			bool _conv_to_vec( shared_ptr<directory_tree::node> spNode, shared_ptr<vector<xfile_shared_ptr>>& spXfileVec )
			{
				spXfileVec->push_back(spNode->file());
				return true;
			}

			shared_ptr<directory_tree> clone( shared_ptr<directory_tree>spTree , KingFileError& error)
			{
				shared_ptr<directory_tree> spTreeCloen(new directory_tree());
				shared_ptr<KingFileError> spError(new KingFileError());
				bool ret = spTree->for_each(std::bind(_copy_insert_to_directory, std::placeholders::_1, spTreeCloen, spError));
				if(ret == true)
					return spTreeCloen;
				else
				{
					error = *spError;
					spTreeCloen->clear();
					return NULL;
				}
			}

			bool _copy_insert_to_directory( shared_ptr<directory_tree::node> spNode, shared_ptr<directory_tree> spTree , shared_ptr<KingFileError> spError)
			{
				xfile_shared_ptr spXfile(new xfile(*spNode->file()));
				return spTree->insert(spXfile, *spError);
			}

			shared_ptr<directory_tree> load_local_file_tree( const sync_path& syncPath, KingFileError& kError )
			{
				//初始化根
				local_path diskRootPath = local_path(syncPath.m_local_path);
				local_path rootPath(L"/"+syncPath.m_name);
				shared_ptr<directory_tree> spDirectorTree(new directory_tree());
				string error;

				local_path rootDiskPath = diskRootPath; rootDiskPath.append(syncPath.m_name);
				xfile_shared_ptr spXfile = xfile::create(rootDiskPath, error);
				spXfile->m_path = rootPath.generic_string();
				spDirectorTree->insert(spXfile, kError);

				vector<xfile_shared_ptr> localXfileVec;
				wstring preError = L"构建LocalSnapshot---";
				string strError;
				bool ret = enum_directory(rootDiskPath, localXfileVec, NULL,strError);
				if(ret == false)
				{
					kError.m_str = preError + CodeTools::ASCII_TO_W(strError);
					kError.m_code = KING_FILE_IO_ACCESS_ERROR;
					return NULL;
				}

				for(size_t i = 0; i < localXfileVec.size(); ++i)
				{
					local_path path = local_path(localXfileVec[i]->m_path).erase_parent_path(diskRootPath);
					localXfileVec[i]->m_path = path.generic_string();
					localXfileVec[i]->m_space_type = (kingfile::filesystem::SPACE_TYPE)syncPath.m_space_type;
					if( spDirectorTree->insert(localXfileVec[i], kError) == false )
					{
						kError.m_str = preError + kError.m_str;
						ret = false;
						return NULL;
					}
				}
				return spDirectorTree;
			}

			bool anyXfileAsecComp(boost::any& l , boost::any& r)
			{
				return  kingfile::filesystem::xfile_asce_comp(boost::any_cast<xfile_shared_ptr>(l), boost::any_cast<xfile_shared_ptr>(r));
			}

			shared_ptr<directory_tree> load_db_file_tree( const sync_path& syncPath, KingFileError& kError )
			{
				wstring preError;
				xfile_shared_ptr spXfile(new xfile());
				local_path rootPath = local_path(syncPath.m_local_path).file_name();
				local_path folder_root_path(L"/"+syncPath.m_name);

				spXfile->m_name = syncPath.m_name;
				spXfile->m_path	= folder_root_path.generic_string();
				spXfile->m_type	= XFILE_FOLDER;
				spXfile->m_role	= (file_role::EKP_FILE_ROLE)syncPath.m_role;
				spXfile->m_xid		=  boost::lexical_cast<uint64_t>(syncPath.m_xid);
				spXfile->m_space_type	= (kingfile::filesystem::SPACE_TYPE)syncPath.m_space_type;
				shared_ptr<directory_tree> spDirectorTree(new directory_tree());
				bool ret = spDirectorTree->insert(spXfile, kError);

				vector<boost::any> dataVec;
				ret = snapshot_table_helper::read_all(syncPath.m_xid, dataVec, kError);
				if(ret == false)
				{
					kError.m_str = preError + kError.m_str;
					return NULL;
				}

				std::sort(dataVec.begin(), dataVec.end(), anyXfileAsecComp);

				for(size_t i = 0; i < dataVec.size(); ++i)
				{
					xfile_shared_ptr spXfile = boost::any_cast<xfile_shared_ptr>(dataVec[i]);
					xfile_shared_ptr spServerXfile(new xfile(*spXfile));
					ret = spDirectorTree->insert(spXfile, kError);
					if(ret == false)
					{
						kError.m_str = preError + kError.m_str;
						break;
					}
				}

				return spDirectorTree;
			}

		}
	}
}
