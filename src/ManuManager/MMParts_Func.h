#ifndef _MMPARTS_FUNC_H_
#define _MMPARTS_FUNC_H_

#include"MMParts_Draw.h"

class MMParts_Func : public MMParts_Draw{
	function<void()> f;
public:

	MMParts_Func(function<void()> func){ f = func; }
	BYTE SelectJudge(float x, float y, float AspectDistortion)
	{ 
		if (MMParts_Draw::SelectJudge(x, y, AspectDistortion))
		{
			f();
		}
		return 0;
	}
};

#endif