#include "sqlit_stmt_warpper.h"

sqlit_stmt_wrapper::sqlit_stmt_wrapper() :m_p_sqlit_stmt(0)
{

}

sqlit_stmt_wrapper::~sqlit_stmt_wrapper()
{
	release();
}

sqlite3_stmt*& sqlit_stmt_wrapper::sqlit_stmt()
{
	return m_p_sqlit_stmt;
}

void sqlit_stmt_wrapper::release()
{
	if(m_p_sqlit_stmt)
	{
		sqlite3_finalize(m_p_sqlit_stmt);
		m_p_sqlit_stmt = 0;
	}
}