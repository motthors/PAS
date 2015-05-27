#ifndef _BASE_MANAGER_H_
#define _BASE_MANAGER_H_
//////////////////////////////////////
/*		ベースマネージャー
GMの一番下。フィールドクラスはここで保持・実行
/////////////////////////////////////*/

#include"IncludeAll.h"
#include"GameManagerComposite.h"

//#include"FlightManager.h"
//#include"MenuManager.h"
//#include"MainGameMgr.h"
#include"../PAS/PASManager.h"
//#include"CourseEditManager.h"
//#include"FreeFlightManager.h"

#include"FieldManager.h"

class BaseManager :public GMComposite{

	bool EndFlag;

public:

	//MenuManager*			MenuM;
	//MainGame*				MGM;
	//BattleManager*			BTLM;
	//CourseEditManager*		CEditM;
	//FreeFlightM*			FFM;
	PASManager*				PASM;

	GMComposite*			NowGMC;

	FieldManager			*pfield;

	int					PlayerNum;

	void SetMainGameInfo();
	void SetBattleGameInfo();

public:

	BaseManager();
	~BaseManager();

	void ChangeMenuFlag(int flag, bool ResetFlag=false);

	void Init();
	bool Move();
	void Draw();
	void DebugDraw();

};

#endif