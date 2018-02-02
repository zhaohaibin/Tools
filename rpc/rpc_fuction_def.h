#pragma once
/********************************************************************
	created:	2017/05/10
	created:	10:5:2017   10:20
	filename: 	C:\kingfile\RPC\include\rpc_fuction_def.h
	file path:	C:\kingfile\RPC\include
	file base:	rpc_fuction_def
	file ext:	h
	author:		xiangzhenwei
	purpose:	RPC invoke sub function name define here
*********************************************************************/

#define  rpc_init_user_info			"init_user_info"
#define  rpc_init_client_context	"init_client_context"

#define  rpc_PostDownloadFilesTask	 "downloadFiles"
#define  rpc_PostDownloadFoldersTask "downloadFolders"
#define  rpc_PostUploadFilesTask	 "uploadFiles"
#define  rpc_PostUploadFoldersTask "uploadFolders"


#define  rpc_pauseDownloadFiles		"pauseDownloadFiles"
#define  rpc_pauseUploadFiles		"pauseUploadFiles"

#define  rpc_startDownloadFiles		 "startDownloadFiles"
#define  rpc_startUploadFiles		 "startUploadFiles"

#define  rpc_cancelDownloadFiles		"cancelDownloadFiles"
#define  rpc_cancelUploadFiles			"cancelUploadFiles"

#define  rpc_cancelAllDownloadFiles		"cancelAllDownloadFiles"
#define  rpc_cancelAllUploadFiles		"cancelAllUploadFiles"

#define  rpc_pauseAllDownloadFiles		"pauseAllDownloadFiles"
#define  rpc_pauseAllUploadFiles		"pauseAllUploadFiles"

#define  rpc_startAllDownloadFiles		"startAllDownloadFiles"
#define  rpc_startAllUploadFiles		"startAllUploadFiles"


#define rpc_page_list				"page_list"
#define rpc_share_root_list			"share_root_list"
#define rpc_xfile_rename			"xfile_rename"
#define rpc_xfile_delete			"xfile_delete"
#define rpc_xfile_batch_delete			"xfile_batch_delete"
#define rpc_xfile_create_dir			"xfile_create_dir"
#define rpc_xfile_create_public_root	"xfile_create_public_root"
#define rpc_xfile_get_sub_items			"xfile_get_sub_items"

#define rpc_insert_tf_items		"insert_tf_items"
#define rpc_update_tf_item		"update_tf_item"
#define rpc_complete_tf_item	"complete_tf_item"
#define rpc_complete_tf_items	"complete_tf_items"

#define rpc_folder_subscribe	"folder_subscribe"
#define rpc_folder_unsubscribe	"folder_unsubscribe"

#define rpc_file_lock			"file_lock"
#define rpc_file_unlock			"file_unlock"

#define rpc_xfile_move				"xfile_move"
#define rpc_xfile_copy				"xfile_copy"
#define rpc_xfile_batch_move		"xfile_batch_move"
#define rpc_xfile_batch_copy		"xfile_batch_copy"

#define rpc_online_edit_download_file		"online_edit_download_file"
#define rpc_online_edit_preview_download_file		"online_edit_preview_download_file" //预览文件,只读下载
#define rpc_online_edit_begin_show_pro		"online_edit_show_pro"
#define rpc_online_edit_update_pro			"online_edit_update_pro"
#define rpc_online_edit_end_show_pro		"online_edit_end_pro"

///DataBaseService
#define rpc_insertToDownloadFile	"insertToDownloadFile"
#define rpc_getFromDownloadFile	"getFromDownloadFile"
#define rpc_deleteInDownloadFile	"deleteInDownloadFile"
#define rpc_deleteItemsInDownloadFile	"deleteItemsInDownloadFile"
#define rpc_getMaxIDFromDownloadFile	"getMaxIDFromDownloadFile"
#define rpc_deleteAllInDownloadFile	"deleteAllInDownloadFile"
#define rpc_getDownloadTaskCount		"getDownloadTaskCount"

#define rpc_insertToDownloadFolder	"insertToDownloadFolder"
#define rpc_getFromDownloadFolder	"getFromDownloadFolder"
#define rpc_deleteInDownloadFolder	"deleteInDownloadFolder"

#define rpc_insertToDownloadFolderParse	"insertToDownloadFolderParse"
#define rpc_getMaxIDFromDownloadFolderParse	"getMaxIDFromDownloadFolderParse"
#define rpc_deleteInDownloadFolderParse	"deleteInDownloadFolderParse"
#define rpc_getFromDownloadFolderParse	"getFromDownloadFolderParse"

#define rpc_insertToUploadFile	"insertToUploadFile"
#define rpc_getFromUploadFile	"getFromUploadFile"
#define rpc_deleteInUploadFile	"deleteInUploadFile"
#define rpc_deleteItemsInUploadFile	"deleteItemsInUploadFile"
#define rpc_getMaxIDFromUploadFile "getMaxIDFromUploadFile"
#define rpc_deleteAllInUploadFile "deleteAllInUploadFile"
#define rpc_getUploadTaskCount "getUploadTaskCount"

#define rpc_insertToUploadFolder	"insertToUploadFolder"
#define rpc_getFromUploadFolder	"getFromUploadFolder"
#define rpc_getMaxIDFromUploadFolder "getMaxIDFromUploadFolder"
#define rpc_deleteFromUploadFolder "deleteFromUploadFolder"

#define rpc_insertToUploadFolderParse "insertToUploadFolderParse"
#define rpc_getFromUploadFolderParse "getFromUploadFolderParse"
#define rpc_getMaxIDFromUploadFolderParse	"getMaxIDFromUploadFolderParse"
#define rpc_deleteFromUploadFolderParse	"deleteFromUploadFolderParse"

#define rpc_insertToCompleteFile	"insertToCompleteFile"
#define rpc_deleteItemInCompleteFile "deleteItemInCompleteFile"
#define rpc_deleteItemsInCompleteFile "deleteItemsInCompleteFile"
#define rpc_deleteAllInCompleteFile	"deleteAllInCompleteFile"
#define rpc_getFromCompleteFile "getFromCompleteFile"
#define rpc_getMaxIDFromCompleteFile "getMaxIDFromCompleteFile"

#define rpc_insertToFileChangeMessage "insertToFileChangeMessage"
#define rpc_deleteItemInFileChangeMessage "deleteItemInFileChangeMessage"
#define rpc_deleteAllInFileChangeMessage "deleteAllInFileChangeMessage"
#define rpc_loadItemsInFileChangeMessage "loadItemsInFileChangeMessage"

#define rpc_message_received_new	"message_received_new" // 消息服务调用UI
#define rpc_message_clear_all		"message_clear_all"	   // UI调用消息服务
#define rpc_message_load_history	"message_load_history" // UI调用消息服务
#define rpc_message_delete_one		"message_delete_one"   // UI调用消息服务

#define rpc_gloabal_content_search	"gloabal_content_search" // 全文件搜索


#define rpc_startXfileTransferService "startXfileTransferService"

#define rpc_cancleAllUploadXifles "cancleAllUploadXifles"

#define rpc_serviceInitialized "serviceInitialized"			//服务初始化完成

#define rpc_initializeTaskCount "initializeTaskCount"
#define rpc_taskCountChanged "taskCountChanged"
#define rpc_show_msg_box	 "show_msg_box"
#define rpc_cancelAllDownloadFolderParse "cancelAllDownloadTasks"
#define rpc_set_tf_thread_count "set_tf_thread_count"
#define rpc_get_root_create_limit_info "get_root_create_limit_info"

#define rpc_itemUploadComplete "itemUploadComplete"
#define rpc_itemDownloadComplete "itemDownloadComplete"
#define rpc_addUploadFolders "addUploadFolders"
#define rpc_updateDownloadFileTempMergeFile "updateDownloadFileTempMergeFile"