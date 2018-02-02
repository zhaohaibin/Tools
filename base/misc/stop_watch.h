#ifndef _STOP_WATCH_H
#define _STOP_WATCH_H
#include <ctime>
#include <functional>

class stop_watch
{
public:
	stop_watch(void);
	stop_watch(std::function<void(double)>);
	~stop_watch(void);

	double elapsed();
	void restart();
private:
	clock_t m_start_time;
	std::function<void(double)> m_end_func;
};

#endif //_STOP_WATCH_H


