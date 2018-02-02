#ifndef _SCOPE_GUARD_H
#define _SCOPE_GUARD_H
#include <functional>

class scope_guard
{
public:
	scope_guard(std::function<void(void)> exit_scope_func);
	~scope_guard(void);

	void dismiss();

private:
	scope_guard(scope_guard const&);
	scope_guard& operator=(scope_guard const&);
private:
	bool m_dismiss;
	std::function<void(void)> m_exit_scope;	
};

#endif //_SCOPE_GUARD_H