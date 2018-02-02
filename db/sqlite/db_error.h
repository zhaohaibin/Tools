#ifndef _DB_ERROR_H
#define _DB_ERROR_H

#include <set>
using namespace std;

#define DATA_INVALID -1
namespace db_error
{
	class error_determine
	{
	public:
		error_determine();
		~error_determine();

		bool is_error(int code) const;

	private:
		void init();
	private:
		set<int> m_errorCodeSet;
	};

	const extern error_determine	errorDetermine; 

	bool is_error(int code);
}
#endif