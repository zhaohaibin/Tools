#include "scope_guard.h"


scope_guard::scope_guard(std::function<void(void)> exit_scope_func)
	: m_exit_scope(exit_scope_func)
	, m_dismiss(false)
{
}

scope_guard::scope_guard( scope_guard const& )
{

}


scope_guard::~scope_guard(void)
{
	if(m_dismiss == false)
	{
		m_exit_scope();
	}
}

void scope_guard::dismiss()
{
	m_dismiss = true;
}

scope_guard& scope_guard::operator=( scope_guard const& )
{
	return *this;
}
