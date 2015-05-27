#include"TimeMeasurer.h"
#include<MMSystem.h>

TimeMeasurer::TimeMeasurer()
{
	flag = false;
	time = 0;
	save = 0;
}
TimeMeasurer::~TimeMeasurer()
{

}

void TimeMeasurer::Start()
{
	flag = true;
	timeBeginPeriod(1);
	time = timeGetTime() - save;
	timeEndPeriod(1);
}
void TimeMeasurer::Stop()
{
	flag = false;
	timeBeginPeriod(1);
	save = timeGetTime() - time;
	timeEndPeriod(1);
	time = 0;
}
void TimeMeasurer::Reset()
{
	flag = false;
	save = 0;
}

DWORD TimeMeasurer::GetTime()
{
	DWORD ans;
	if(flag)
	{
		timeBeginPeriod(1);
		ans = timeGetTime() - time;
		timeEndPeriod(1);
	}
	else
		ans = save;
	return ans;
}
