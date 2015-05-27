#ifndef _MMAPRTS_SLIDEBAR_H_
#define _MMAPRTS_SLIDEBAR_H_

#include"MMParts_Draw.h"
#include"../base/Input.h"

class MMParts_SlideBar : public MMParts_Leaf{

protected:
	Input*	pInput = nullptr;
	float*	pVariable = nullptr;
	bool	ClickFlag = false;
	float	theta = 0;
	//float	MinVar = 0;
	//float	MaxVar = 0;
	float*	pMinVar = nullptr;
	float*	pMaxVar = nullptr;
	BOOL	PtrMinSetFlag = false;
	BOOL	PtrMaxSetFlag = false;
	float	BaseSize = 0;	//文字などのサイズが変わらないもの用

public:
	virtual ~MMParts_SlideBar();

	void SetVariable(float* pvar){ pVariable = pvar; }
	void SetMouseInput(Input* p){ pInput = p; }
	void SetBaseSize(float size){ BaseSize = size; }
	void SetMin(float max);
	void SetMax(float min);
	void SetMin_p(float* pmin);
	void SetMax_p(float* pmax);
	void SetPos(float startx, float starty, float dirx, float diry, float aspect);
	BYTE SelectJudge(float x, float y, float AspectDistortion);

	void ConvertVarToTheta();
	void ConvertThetaToVar();
	void ConvertThetaToDir();

	virtual void Move();
	virtual void Draw();
};


class MMParts_SlideBarPF : public MMParts_SlideBar{
	float**	ppVariable = nullptr;
public:
	void SetVariable(float** pvar){ ppVariable = pvar; }
	void Move();
};


class MMParts_SlideBarI : public MMParts_SlideBar{
	int* pVariable;
public:
	void SetVariable(int* pvar){ pVariable = pvar; }
};


#endif