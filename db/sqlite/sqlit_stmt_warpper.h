#ifndef _SQLIT_STMT_WARPPER_H
#define _SQLIT_STMT_WARPPER_H

#include <sqlite3/sqlite3.h>
#include <string>
#include <cstdint>

class sqlit_blob
{
public:
	void append(const char* p_buf, unsigned int buf_len)
	{
		m_val.append(p_buf, buf_len);
	}
	void append(const std::string& str)
	{
		m_val.append(str);
	}
	uint64_t size()
	{
		return m_val.size();
	}

	const char* get()
	{
		return m_val.c_str();
	}

	void reset()
	{
		m_val.clear();
	}
private:
	std::string m_val;
};

class sqlit_stmt_wrapper
{
public:
	sqlit_stmt_wrapper();
	~sqlit_stmt_wrapper();

	sqlite3_stmt*& sqlit_stmt();

	template<typename T>
	int fetch_row(T& val)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val, m_p_sqlit_stmt, 0);
		}
		return ret;
	}

	template<typename T1, typename T2>
	int fetch_row(T1& val_1, T2& val_2)
	{	
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3>
	int fetch_row(T1& val_1, T2& val_2, T3& val_3)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4>
	int fetch_row(T1& val_1, T2& val_2, T3& val_3, T4& val_4)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	int fetch_row(T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	int fetch_row(T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5, T6& val_6)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
			get(val_6, m_p_sqlit_stmt, 5);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	int fetch_row(T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5, T6& val_6, T7& val_7)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
			get(val_6, m_p_sqlit_stmt, 5);
			get(val_7, m_p_sqlit_stmt, 6);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, 
		typename T6, typename T7, typename T8>
		int fetch_row(
		T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5, 
		T6& val_6, T7& val_7, T8& val_8)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
			get(val_6, m_p_sqlit_stmt, 5);
			get(val_7, m_p_sqlit_stmt, 6);
			get(val_8, m_p_sqlit_stmt, 7);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, 
		typename T6, typename T7, typename T8, typename T9>
		int fetch_row(
		T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5,
		T6& val_6, T7& val_7, T8& val_8, T9& val_9)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
			get(val_6, m_p_sqlit_stmt, 5);
			get(val_7, m_p_sqlit_stmt, 6);
			get(val_8, m_p_sqlit_stmt, 7);
			get(val_9, m_p_sqlit_stmt, 8);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, 
		typename T6, typename T7, typename T8, typename T9, typename T10>
		int fetch_row(
		T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5, 
		T6& val_6, T7& val_7, T8& val_8, T9& val_9, T10& val_10)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
			get(val_6, m_p_sqlit_stmt, 5);
			get(val_7, m_p_sqlit_stmt, 6);
			get(val_8, m_p_sqlit_stmt, 7);
			get(val_9, m_p_sqlit_stmt, 8);
			get(val_10, m_p_sqlit_stmt, 9);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, 
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11>
		int fetch_row(
		T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5, 
		T6& val_6, T7& val_7, T8& val_8, T9& val_9, T10& val_10, T11& val_11)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
			get(val_6, m_p_sqlit_stmt, 5);
			get(val_7, m_p_sqlit_stmt, 6);
			get(val_8, m_p_sqlit_stmt, 7);
			get(val_9, m_p_sqlit_stmt, 8);
			get(val_10, m_p_sqlit_stmt, 9);
			get(val_11, m_p_sqlit_stmt, 10);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5, 
		typename T6, typename T7, typename T8, typename T9, typename T10, 
		typename T11, typename T12>
		int fetch_row(
		T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5,
		T6& val_6, T7& val_7, T8& val_8, T9& val_9, T10& val_10, 
		T11& val_11, T12& val_12)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
			get(val_6, m_p_sqlit_stmt, 5);
			get(val_7, m_p_sqlit_stmt, 6);
			get(val_8, m_p_sqlit_stmt, 7);
			get(val_9, m_p_sqlit_stmt, 8);
			get(val_10, m_p_sqlit_stmt, 9);
			get(val_11, m_p_sqlit_stmt, 10);
			get(val_12, m_p_sqlit_stmt, 11);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11, typename T12, typename T13>
		int fetch_row(
		T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5, 
		T6& val_6, T7& val_7, T8& val_8, T9& val_9, T10& val_10, 
		T11& val_11, T12& val_12, T13& val_13)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
			get(val_6, m_p_sqlit_stmt, 5);
			get(val_7, m_p_sqlit_stmt, 6);
			get(val_8, m_p_sqlit_stmt, 7);
			get(val_9, m_p_sqlit_stmt, 8);
			get(val_10, m_p_sqlit_stmt, 9);
			get(val_11, m_p_sqlit_stmt, 10);
			get(val_12, m_p_sqlit_stmt, 11);
			get(val_13, m_p_sqlit_stmt, 12);
		}
		return ret;
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11, typename T12, typename T13, typename T14>
		int fetch_row(
		T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5, 
		T6& val_6, T7& val_7, T8& val_8, T9& val_9, T10& val_10, 
		T11& val_11, T12& val_12, T13& val_13, T14& val_14)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
			get(val_6, m_p_sqlit_stmt, 5);
			get(val_7, m_p_sqlit_stmt, 6);
			get(val_8, m_p_sqlit_stmt, 7);
			get(val_9, m_p_sqlit_stmt, 8);
			get(val_10, m_p_sqlit_stmt, 9);
			get(val_11, m_p_sqlit_stmt, 10);
			get(val_12, m_p_sqlit_stmt, 11);
			get(val_13, m_p_sqlit_stmt, 12);
			get(val_14, m_p_sqlit_stmt, 13);
		}
		return ret;
	}
	template<typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11, typename T12, typename T13, typename T14,typename T15>
		int fetch_row(
		T1& val_1, T2& val_2, T3& val_3, T4& val_4, T5& val_5, 
		T6& val_6, T7& val_7, T8& val_8, T9& val_9, T10& val_10, 
		T11& val_11, T12& val_12, T13& val_13, T14& val_14, T15& val_15)
	{
		int ret = sqlite3_step(m_p_sqlit_stmt);
		if( ret == SQLITE_ROW)
		{
			get(val_1, m_p_sqlit_stmt, 0);
			get(val_2, m_p_sqlit_stmt, 1);
			get(val_3, m_p_sqlit_stmt, 2);
			get(val_4, m_p_sqlit_stmt, 3);
			get(val_5, m_p_sqlit_stmt, 4);
			get(val_6, m_p_sqlit_stmt, 5);
			get(val_7, m_p_sqlit_stmt, 6);
			get(val_8, m_p_sqlit_stmt, 7);
			get(val_9, m_p_sqlit_stmt, 8);
			get(val_10, m_p_sqlit_stmt, 9);
			get(val_11, m_p_sqlit_stmt, 10);
			get(val_12, m_p_sqlit_stmt, 11);
			get(val_13, m_p_sqlit_stmt, 12);
			get(val_14, m_p_sqlit_stmt, 13);
			get(val_15, m_p_sqlit_stmt, 14);

		}
		return ret;
	}

	//定义bind
	int bind(int index, sqlit_blob& blob, sqlite3_destructor_type destructor =  SQLITE_TRANSIENT)
	{
		return sqlite3_bind_blob(m_p_sqlit_stmt, index, blob.get(), (int)blob.size(), destructor);
	}

	int bind(int index, const std::string& str, sqlite3_destructor_type destructor =  SQLITE_TRANSIENT)
	{
		return sqlite3_bind_text(m_p_sqlit_stmt, index, str.c_str(), str.size(), destructor);
	}

	int bind(int index, int val)
	{
		return sqlite3_bind_int(m_p_sqlit_stmt, index, val);
	}

	int bind(int index, int64_t val )
	{
		return sqlite3_bind_int64(m_p_sqlit_stmt, index, val);
	}

private:
	//定义取出当前行中的指定列的值
	template<typename T>
	void get(T& val, sqlite3_stmt* p_stmt, const int index){}

	void get(int& val, sqlite3_stmt* p_stmt, const int index)
	{
		val = sqlite3_column_int(p_stmt, index);
	}

	void get(int64_t& val, sqlite3_stmt* p_stmt, const int index)
	{
		val =  sqlite3_column_int64(p_stmt, index);
	}

	void get(uint64_t& val, sqlite3_stmt* p_stmt, const int index)
	{
		val = sqlite3_column_int64(p_stmt, index);
	}

	void get(double& val, sqlite3_stmt* p_stmt, const int index)
	{
		val = sqlite3_column_double(p_stmt, index);
	}

	void get(std::string& val , sqlite3_stmt* p_stmt, const int index)
	{
		val.clear();
		val.append((const char*)sqlite3_column_text(p_stmt, index), sqlite3_column_bytes(p_stmt, index) );
	}

	void get(sqlit_blob& val, sqlite3_stmt* p_stmt, const int index)
	{
		val.reset();
		const char* pSrc = (const char*)sqlite3_column_blob(p_stmt, index);
		int srcLen = sqlite3_column_bytes(p_stmt, index);
		if(pSrc != NULL && srcLen != 0 )
		{
			val.append(pSrc,srcLen);
		}
		//val.append((const char*)sqlite3_column_blob(p_stmt, index), sqlite3_column_bytes(p_stmt, index) );
	}

	void release();
private:
	sqlite3_stmt* m_p_sqlit_stmt;
};
#endif