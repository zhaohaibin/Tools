#include "stop_watch.h"


stop_watch::stop_watch(void)
{
	m_start_time = clock();
}

stop_watch::stop_watch( std::function<void(double)> end_func)
	: m_end_func(end_func)
{
	m_start_time = clock();
}


stop_watch::~stop_watch(void)
{
	clock_t total = clock()-m_start_time;
	double second = double(total)/CLK_TCK;
	if(m_end_func)
		m_end_func(second);
}

double stop_watch::elapsed()
{
	clock_t total = clock() - m_start_time;
	return double(total/CLK_TCK);
}

void stop_watch::restart()
{
	m_start_time = clock();
}
