#ifndef _MENUMANAGER_PARTS_H_
#define _MENUMANAGER_PARTS_H_

#include"../base/IncludeAll.h"


class MMParts_Base{
public:
	//variable
	TCHAR			Name[10];
	MMParts_Base*	Parent;
	MMParts_Base*	Brother;
	MMParts_Base*	Child;
	int				ChildNum;
	//function
	MMParts_Base();
	virtual ~MMParts_Base();

	void SetName(TCHAR* name);

	virtual void Appear(){}
	virtual void Select(){}	
	virtual bool Cancel(){ return false; } //ÉÅÉjÉÖÅ[èIóπÇ»ÇÁTrueÇï‘Ç∑ÇÊÇ§Ç…
	virtual BYTE SelectJudge(float x, float y, float){ return 0; }
	virtual void Inactive(){}
	virtual void Active(){}

	virtual void Move(){}
	virtual void Draw(){}
};

#endif