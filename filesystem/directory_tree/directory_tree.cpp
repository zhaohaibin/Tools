#include <deque>
#include <algorithm>
#include "directory_tree.h"
#include "../xfile/xfile.h"
#include "../misc/string_helper/string_helper.h"
#include "directory_tree_error.h"

namespace kingfile
{
	namespace filesystem
	{
		directory_tree::directory_tree(void)
			: m_spRoot(new directory_tree::node())
			,m_CancelHandleCmd(NULL)
		{
			shared_ptr<xfile> spXfile(new xfile());
			m_spRoot->set_xfile(spXfile);
		}

		directory_tree::~directory_tree(void)
		{
		}

		bool directory_tree::insert( shared_ptr<xfile> spXfile, KingFileError& error)
		{
			bool ret = false;
			shared_ptr<directory_tree::node> spParent = find_parent(spXfile);
			if(spParent)
			{
				shared_ptr<directory_tree::node> spNode = directory_tree::node::create(spParent, spXfile);
				directory_tree::tree_path_type nodeKey = _lower_string(spNode->file()->m_path);
				ret = spParent->insert_sub_node(nodeKey, spNode, error);
			}else
			{
				error.m_code = DIR_TREE_LOST_PARENT;
				error.m_str = L"没有找到父节点:"+spXfile->file_info();
			}
			return ret;
		}

		void directory_tree::erase( shared_ptr<xfile> spXfile )
		{
			shared_ptr<directory_tree::node> spParent = find_parent(spXfile);
			shared_ptr<directory_tree::node> spNode = find(spXfile->m_path);
			if(spParent && spNode)
			{
				_for_each(spNode, std::bind(&directory_tree::_unparent, this, std::placeholders::_1));
				spNode->reset_parent();
				spParent->erase_sub_node(_lower_string(spXfile->m_path));
			}
		}

		bool directory_tree::for_each( std::function<bool(shared_ptr<directory_tree::node>)> func )
		{
			return _for_each(m_spRoot, func);
		}

		shared_ptr<directory_tree::node> directory_tree::find_parent( shared_ptr<xfile> spXfile )
		{
			local_path path = local_path(spXfile->m_path).parent_path();
			directory_tree::tree_path_type parentPaht = _lower_string(path.generic_string());//_lower_string(spXfile->m_path.substr(0, spXfile->m_path.rfind('/')));
			return _find(parentPaht);
		}

		shared_ptr<directory_tree::node> directory_tree::_find( const tree_path_type& path )
		{
			vector<tree_path_type> ancestorsPath;
			_path_split(path,ancestorsPath);
			shared_ptr<directory_tree::node> spNode = m_spRoot;
			for(size_t i = 0; i < ancestorsPath.size(); ++i)
			{
				if(ancestorsPath[i] == L"/")
				{
					spNode = m_spRoot;
					continue;
				}
				spNode = spNode->find(ancestorsPath[i]);
				if(!spNode)
					break;
			}
			return spNode;
		}

		bool directory_tree::_for_each( 
			shared_ptr<directory_tree::node> spRoot, 
			std::function<bool(shared_ptr<directory_tree::node>)> func )
		{
			if(spRoot->file()->is_directory() == false)
				return false;

			deque<shared_ptr<directory_tree::node>>	dirNodeQueue;
			dirNodeQueue.push_back(spRoot);
			for(; dirNodeQueue.empty() == false;  )
			{
				if (m_CancelHandleCmd)
					if (m_CancelHandleCmd())
						return false; // 外部取消

				shared_ptr<directory_tree::node> spDir = dirNodeQueue.front();
				dirNodeQueue.pop_front();
				node_subitems_type subItems = spDir->sub_items();
				node_subitems_type::iterator it = subItems.begin();
				for( ;  it != subItems.end(); ++it)
				{
					if(it->second->file()->m_type == XFILE_FOLDER)
						dirNodeQueue.push_back(it->second);
					if( func(it->second) == false)
						return false;
				}
			}
			return true;
		}

		void directory_tree::_path_split( const tree_path_type& path, const tree_path_type& split, vector<tree_path_type>& vec )
		{
			tree_path_type splitPath;
			tree_path_type::const_iterator it = path.begin();
			if(path.empty())
				return;
			for(; it != path.end(); ++it)
			{
				tree_path_type temp;
				temp += *it;
				if(temp == split)
				{
					if(splitPath.length() > 0)
						vec.push_back(splitPath);
				}
				splitPath += *it;
			}
			vec.push_back(splitPath);
		}

		void directory_tree::_path_split( const local_path& path, vector<tree_path_type>& vec )
		{
			boost::filesystem::path::iterator it = path.path().begin();
			for( it; it != path.path().end(); ++it)
			{
				if(vec.size() > 0)
				{
					local_path subPath(vec[vec.size()-1]);
					subPath.append(local_path(*it).generic_string());
					vec.push_back(subPath.generic_string());
				}else
				{
					vec.push_back(local_path(*it).generic_string());
				}				
			}
		}

		shared_ptr<directory_tree::node> directory_tree::find( const directory_tree::tree_path_type& path )
		{
			return _find(_lower_string(path));
		}

		void directory_tree::clear()
		{
			for_each(std::bind(&directory_tree::_unparent,this, std::placeholders::_1));
			shared_ptr<xfile> spXfile(new xfile());
			m_spRoot = shared_ptr<directory_tree::node>(new directory_tree::node());
			m_spRoot->set_xfile(spXfile);
		}

		bool directory_tree::_unparent( shared_ptr<directory_tree::node> spNode )
		{
			spNode->reset_parent();
			return true;
		}

		directory_tree::node_subitems_type directory_tree::root_subitems()
		{
			return m_spRoot->sub_items();
		}

		directory_tree::tree_path_type directory_tree::_lower_string( const directory_tree::tree_path_type& str )
		{
			return string_helper::lower_string(str);
		}
		void directory_tree::setCancelHandle(std::function<bool()> CancelHandle)
		{
			m_CancelHandleCmd = CancelHandle;
		}
	}
}