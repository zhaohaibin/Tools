#include <deque>
#include "server_directory_tree.h"
#include "../filesystem/local_path.h"
#include "directory_tree_error.h"

namespace kingfile
{
	namespace filesystem
	{
		server_directory_tree::server_directory_tree(void)
			: m_spRoot(new server_directory_tree::node())
		{
			shared_ptr<xfile> spXfile(new xfile());
			m_spRoot->set_xfile(spXfile);
		}

		server_directory_tree::~server_directory_tree(void)
		{
			clear();
		}

		bool server_directory_tree::init( map<uint64_t, xfile_shared_ptr>& xfiles , KingFileError& error)
		{
			bool ret = true;
			xfile_shared_ptr spRet = NULL;
			map<uint64_t, xfile_shared_ptr>::iterator it = xfiles.begin();
			for( ; it != xfiles.end(); ++it)
			{
				vector<xfile_shared_ptr> xfilePath;
				spRet = _extract_path(it->second, xfiles, xfilePath, error);
				if(spRet)
				{
					ret = false;
					break;
				}
				_full_path(xfilePath);
				
				ret = _insert(xfilePath, error);
				if(ret == false)
					break;
			}
			return ret;
		}

		xfile_shared_ptr server_directory_tree::_extract_path( xfile_shared_ptr spXfile, 
			map<uint64_t, xfile_shared_ptr>& xfiles, 
			vector<xfile_shared_ptr>& xfilePath,
			KingFileError& error)
		{
			xfile_shared_ptr spRet = NULL;
			xfile_shared_ptr spLastXfile = spXfile;
			for(;;)
			{
				xfilePath.push_back(spLastXfile);
				uint64_t pxid = spLastXfile->m_pxid;
				if(pxid == 0)
					break;

				if(xfilePath.size() > 128)
				{
					error.m_code = DIR_TREE_PATH_TOO_DEEP;
					error.m_str = L"路径深度超过限制:";
					for(size_t i = 0; i < xfilePath.size(); ++i)
					{
						error.m_str += xfilePath[i]->file_info();
					}
					spRet = NULL;
					break;
				}else
				{
					map<uint64_t, xfile_shared_ptr>::iterator it = xfiles.find(pxid);
					if(it == xfiles.end())
					{
						error.m_code = DIR_TREE_LOST_PARENT;
						error.m_str = L"提取路径出错没有找到父节点："+ spLastXfile->file_info();
						spRet = spLastXfile;
						break;
					}else
					{
						spLastXfile = it->second;
					}
				}
			}
			return spRet;
		}

		bool server_directory_tree::_insert( vector<xfile_shared_ptr>& xfilePath, KingFileError& error )
		{
			bool ret = true;
			shared_ptr<server_directory_tree::node> spParentNode = m_spRoot;
			for(int i = xfilePath.size()-1; i >= 0; --i)
			{
				shared_ptr<server_directory_tree::node> spNode(new server_directory_tree::node());
				spNode->reset_parent(spParentNode);
				spNode->set_xfile(xfilePath[i]);

				shared_ptr<server_directory_tree::node> spFind = spParentNode->find(spNode->file()->m_xid);
				if( !spFind )
				{
					ret = spParentNode->insert_sub_node(spNode->file()->m_xid, spNode, error);
					if(ret)
						spFind = spNode;
					else
						return ret;
				}
				spParentNode = spFind;
			}
			return ret;
		}

		bool server_directory_tree::for_each( std::function<bool(shared_ptr<server_directory_tree::node>)> func )
		{
			bool ret = true;
			deque<shared_ptr<server_directory_tree::node>> dirNodeQueue;
			dirNodeQueue.push_back(m_spRoot);
			for(; dirNodeQueue.empty() == false; )
			{
				shared_ptr<server_directory_tree::node> spDir = dirNodeQueue.front();
				dirNodeQueue.pop_front();
				map<uint64_t, std::shared_ptr<server_directory_tree::node>> subItems = spDir->sub_items();
				map<uint64_t, std::shared_ptr<server_directory_tree::node>>::iterator it = subItems.begin();
				for(; it!= subItems.end(); ++it)
				{
					if(it->second->file()->m_type == XFILE_FOLDER)
						dirNodeQueue.push_back(it->second);
					ret = func(it->second);
					if(ret == false)
						return ret;;
				}
			}
			return ret;
		}

		void server_directory_tree::clear()
		{
			for_each(std::bind(&server_directory_tree::_unparent,this, std::placeholders::_1));
			shared_ptr<xfile> spXfile(new xfile());
			m_spRoot.reset(new server_directory_tree::node());
			m_spRoot->set_xfile(spXfile);
		}

		bool server_directory_tree::_unparent( shared_ptr<node> spNode )
		{
			spNode->reset_parent();
			return true;
		}

		void server_directory_tree::_full_path( vector<xfile_shared_ptr>& xfilePath )
		{
			//string path = "/";
			local_path path;
			path.append(L"/");
			for( int i = xfilePath.size()-1; i >= 0; --i)
			{
				path.append(xfilePath[i]->m_name);
				//path += xfilePath[i]->m_name;
				xfilePath[i]->m_path = path.generic_string();
				//path += "/";
			}
		}
	}
}