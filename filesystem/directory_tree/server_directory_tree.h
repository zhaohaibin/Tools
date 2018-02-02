#ifndef _SERVER_DIRECTORY_TREE_H
#define _SERVER_DIRECTORY_TREE_H

#include<vector>
#include<map>
#include <functional>
#include"../xfile/xfile.h"
#include "directory_tree_node.h"
#include "../error/KingFileError.h"

namespace kingfile
{
	namespace filesystem
	{
		class server_directory_tree
		{
		public:
			typedef directory_tree_node<uint64_t> node;
		public:
			server_directory_tree(void);
			~server_directory_tree(void);

			bool init(map<uint64_t, xfile_shared_ptr>& xfiles, KingFileError& error);
			bool for_each(std::function<bool(shared_ptr<server_directory_tree::node>)> func);
			void clear();
		private:
			xfile_shared_ptr _extract_path(xfile_shared_ptr spXfile, 
				map<uint64_t, xfile_shared_ptr>& xfiles, 
				vector<xfile_shared_ptr>& xfilePath, KingFileError& error);
			bool _insert(vector<xfile_shared_ptr>& xfilePath, KingFileError& error);
			void _full_path(vector<xfile_shared_ptr>& xfilePath);
			bool _unparent(shared_ptr<node> spNode);
		private:
			shared_ptr<server_directory_tree::node> m_spRoot;
		};
	}
}
#endif

