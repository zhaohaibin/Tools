#ifndef _DIRECTORY_TREE_H
#define _DIRECTORY_TREE_H

#include <vector>
#include <functional>
#include "directory_tree_node.h"
#include "../filesystem/local_path.h"
#include "../error/KingFileError.h"

namespace kingfile
{
	namespace filesystem
	{
		class directory_tree
		{
		public:
			typedef local_path::string_type		tree_path_type;
			typedef directory_tree_node<tree_path_type> node;
			typedef map<tree_path_type, std::shared_ptr<node>> node_subitems_type;

		public:
			directory_tree(void);
			~directory_tree(void);

		public:
			void setCancelHandle(std::function<bool()> CancelHandle); //cancel handle
			bool insert(shared_ptr<xfile> spXfile, KingFileError& error);
			void erase(shared_ptr<xfile> spXfile);
			void clear();
			bool for_each(std::function<bool(shared_ptr<node>)> func);
			shared_ptr<node> find_parent(shared_ptr<xfile> spXfile);
			shared_ptr<node> find(const tree_path_type& path);

			node_subitems_type root_subitems();
		private:
			shared_ptr<node> _find(const tree_path_type& path);
			bool _for_each(shared_ptr<node> spRoot, std::function<bool(shared_ptr<node>)> func);

			void _path_split(const tree_path_type& path, const tree_path_type& split, vector<tree_path_type>& vec);
			void _path_split(const local_path& path, vector<tree_path_type>& vec);
			bool _unparent(shared_ptr<node> spNode);
			tree_path_type _lower_string(const tree_path_type& str);
		private:
			shared_ptr<node>		m_spRoot;
			std::function<bool()> m_CancelHandleCmd;
		};
	}
}
#endif