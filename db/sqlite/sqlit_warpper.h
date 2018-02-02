/********************************************************************
created:	2015/09/24
created:	24:9:2015   14:16
filename: 	D:\KSYUN\new_client\pclite\src\include\db\db_base.h
file path:	D:\KSYUN\new_client\pclite\src\include\db
file base:	db_base
file ext:	h
author:		zhaohaibin

purpose:	
*********************************************************************/
#ifndef DB_BASE_H
#define DB_BASE_H

#include <string>
#include <cstdint>
#include <sqlite3/sqlite3.h>
#include "sqlit_stmt_warpper.h"

class sqlit_warpper
{
public:
	sqlit_warpper();
	~sqlit_warpper();

public:
	std::string error_msg();
	//db_path need utf8 encode
    int initialize_db(const std::string& db_path);

	//用于执行 sql语句(创建表，增，删，改)
	int exec_sql(const std::string& sql);
	int exec_sql(const char* sql);

	////////////////////////////////////////////////////////
	//step_1 sqlit_prepare
	//bind---根据需要选择
	//step_2 perform
	int sqlit_prepare(const char* sql, sqlit_stmt_wrapper& stmt);
	int perform(sqlit_stmt_wrapper& stmt);

	//////////////////////////////////////////////////事务相关接口////////////////////////////////////////
	//step_1	begin_transaction
	//step_2	其他数据库操作
	//step_3	没有错误commit_transaction否则rollback_transaction
	//开始一个事务
	int begin_transaction();

	//提交事务
	int commit_transaction();

	//回滚事务
	int rollback_transaction();

	////////////////////////////////////////////////////查询相关接口
	int exec_query(const std::string& sql, sqlit_stmt_wrapper& stmt);
	int exec_query(const char* sql, sqlit_stmt_wrapper& stmt);
private:
	sqlite3* m_p_sqlite;
};
#endif