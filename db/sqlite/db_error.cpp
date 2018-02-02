#include "db_error.h"

#include <sqlite3/sqlite3.h>
namespace db_error
{
	const error_determine	errorDetermine; 
	error_determine::error_determine()
	{
		init();
	}

	error_determine::~error_determine()
	{

	}

	bool error_determine::is_error( int code ) const
	{
		set<int>::iterator it = m_errorCodeSet.find(code);
		if(it != m_errorCodeSet.end())
			return true;
		return false;
	}

	void error_determine::init()
	{
		m_errorCodeSet.insert(SQLITE_ERROR);
		m_errorCodeSet.insert(SQLITE_INTERNAL);
		m_errorCodeSet.insert(SQLITE_PERM);
		m_errorCodeSet.insert(SQLITE_ABORT);
		m_errorCodeSet.insert(SQLITE_BUSY);
		m_errorCodeSet.insert(SQLITE_LOCKED);
		m_errorCodeSet.insert(SQLITE_NOMEM);
		m_errorCodeSet.insert(SQLITE_READONLY);
		m_errorCodeSet.insert(SQLITE_INTERRUPT);
		m_errorCodeSet.insert(SQLITE_IOERR);
		m_errorCodeSet.insert(SQLITE_CORRUPT);
		m_errorCodeSet.insert(SQLITE_NOTFOUND);
		m_errorCodeSet.insert(SQLITE_FULL);
		m_errorCodeSet.insert(SQLITE_CANTOPEN);
		m_errorCodeSet.insert(SQLITE_PROTOCOL);
		m_errorCodeSet.insert(SQLITE_EMPTY);
		m_errorCodeSet.insert(SQLITE_SCHEMA);
		m_errorCodeSet.insert(SQLITE_TOOBIG);
		m_errorCodeSet.insert(SQLITE_CONSTRAINT);
		m_errorCodeSet.insert(SQLITE_MISMATCH);
		m_errorCodeSet.insert(SQLITE_MISUSE);
		m_errorCodeSet.insert(SQLITE_NOLFS);
		m_errorCodeSet.insert(SQLITE_AUTH);
		m_errorCodeSet.insert(SQLITE_FORMAT);
		m_errorCodeSet.insert(SQLITE_RANGE);
		m_errorCodeSet.insert(SQLITE_NOTADB);


		m_errorCodeSet.insert(SQLITE_IOERR_READ);
		m_errorCodeSet.insert(SQLITE_IOERR_SHORT_READ);
		m_errorCodeSet.insert(SQLITE_IOERR_WRITE);
		m_errorCodeSet.insert(SQLITE_IOERR_FSYNC);
		m_errorCodeSet.insert(SQLITE_IOERR_DIR_FSYNC);
		m_errorCodeSet.insert(SQLITE_IOERR_TRUNCATE);
		m_errorCodeSet.insert(SQLITE_IOERR_FSTAT);
		m_errorCodeSet.insert(SQLITE_IOERR_UNLOCK);
		m_errorCodeSet.insert(SQLITE_IOERR_RDLOCK);
		m_errorCodeSet.insert(SQLITE_IOERR_DELETE);
		m_errorCodeSet.insert(SQLITE_IOERR_BLOCKED);
		m_errorCodeSet.insert(SQLITE_IOERR_NOMEM);
		m_errorCodeSet.insert(SQLITE_IOERR_ACCESS);
		m_errorCodeSet.insert(SQLITE_IOERR_CHECKRESERVEDLOCK);
		m_errorCodeSet.insert(SQLITE_IOERR_LOCK);
		m_errorCodeSet.insert(SQLITE_IOERR_CLOSE);
		m_errorCodeSet.insert(SQLITE_IOERR_DIR_CLOSE);
		m_errorCodeSet.insert(SQLITE_IOERR_SHMOPEN);
		m_errorCodeSet.insert(SQLITE_IOERR_SHMSIZE);
		m_errorCodeSet.insert(SQLITE_IOERR_SHMLOCK);
		m_errorCodeSet.insert(SQLITE_IOERR_SHMMAP);
		m_errorCodeSet.insert(SQLITE_IOERR_SEEK);
		m_errorCodeSet.insert(SQLITE_LOCKED_SHAREDCACHE);
		m_errorCodeSet.insert(SQLITE_BUSY_RECOVERY);
		m_errorCodeSet.insert(SQLITE_CANTOPEN_NOTEMPDIR);
		m_errorCodeSet.insert(SQLITE_CORRUPT_VTAB);
		m_errorCodeSet.insert(SQLITE_READONLY_RECOVERY);
		m_errorCodeSet.insert(SQLITE_READONLY_CANTLOCK);
		m_errorCodeSet.insert(SQLITE_ABORT_ROLLBACK);
		m_errorCodeSet.insert(DATA_INVALID);
	}

	bool is_error( int code )
	{
		return errorDetermine.is_error(code);
	}
}