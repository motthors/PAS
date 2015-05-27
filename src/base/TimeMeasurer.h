#ifndef _TIME_MEASURER_H_
#define _TIME_MEASURER_H_

#include"IncludeAll.h"


class TimeMeasurer{

	bool flag;	//タイマーが動いているかどうか
	DWORD time;
	DWORD save;

public:
	TimeMeasurer();
	~TimeMeasurer();

	void Start();	//計測スタート(再スタート)
	void Stop();	//計測中断
	void Reset();	//リセット

	DWORD GetTime();	//タイマーの現在の値取得
};

#endif