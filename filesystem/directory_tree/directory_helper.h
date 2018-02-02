#ifndef _DIRECTORY_HELPER_H
#define _DIRECTORY_HELPER_H
#include "directory_tree.h"
#include "server_directory_tree.h"
#include "../filesystem/local_path.h"
#include "../user_db/user_db_manager.h"

namespace kingfile
{
	namespace filesystem
	{
		namespace directory_helper
		{
			shared_ptr<directory_tree> conv_to_directory_tree(shared_ptr<server_directory_tree> spTree, KingFileError& error);

			bool copy(const local_path& rootPath, shared_ptr<directory_tree>spTree, const local_path& targetPath, string& error);
			bool conv_to_vec(shared_ptr<directory_tree>spTree, shared_ptr<vector<xfile_shared_ptr>>& spXfileVec);
			shared_ptr<directory_tree> clone(shared_ptr<directory_tree>spTree, KingFileError& error);

			shared_ptr<directory_tree> load_local_file_tree(const sync_path& syncPath, KingFileError& kError);
			shared_ptr<directory_tree> load_db_file_tree(const sync_path& syncPath, KingFileError& kError);

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			bool _copy( const local_path& rootPath, shared_ptr<directory_tree::node> spNode, const local_path& path, shared_ptr<string> spError);

			bool _copy_insert_to_directory(shared_ptr<directory_tree::node> spNode, shared_ptr<directory_tree> spTree, shared_ptr<KingFileError> spError);
			bool _insert_to_directory_tree(shared_ptr<server_directory_tree::node> spNode, shared_ptr<directory_tree> spTree, shared_ptr<KingFileError> spError);

			bool _conv_to_vec(shared_ptr<directory_tree::node> spNode, shared_ptr<vector<xfile_shared_ptr>>& spXfileVec);


		}
	}
}

#endif

