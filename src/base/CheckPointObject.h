#ifndef _CPO_H_
#define _CPO_H_

#include"IncludeAll.h"

#define CPO_HIDE	0
#define CPO_START	1
#define CPO_GOAL	2


typedef struct {
public:
	D3DXMATRIX worldMat;
	int ID;				//ステージ共通ID 0~10(まだ決めてない) 固有ID 100~
	
} CO;

class CPO{
public:

	D3DXVECTOR3 position;
	int x,y,z;
	D3DXMATRIX  rota;
	float		ratio;
	//D3DXQUATERNION q;

	int pointFlag;		//0:見えないポイント 1:スタート地点 2:ゴール 3:ゴール後タイム順位表示位置　n:ステージ固有ポイントID
	int ID;				//チェックポイントのID 1番から
	int nextPointID;	//次をどこのポイントにつなげるか
	int prevPointID;	//前はどのポイントにつながれてるか
	int blanchNum;		//分かれ道の数
	int connectFlag;	//ポイント間のガイドのつなぎ方 0:描かない n:ステージ固有ラインIDの指定 
	FLOAT connectPower;	//汎用変数 スプラインの基準ベクトルの大きさとかに

	int rotaflag;		//回転行列の変更をしたかどうか

	int objectNum;		//そのポイントに含まれている判定有りオブジェクトの数
	CO* pCOArray;

	int GuideNum;		//このポイントから伸びるガイドの数
	D3DXVECTOR3* pGuidePos;	//ガイドの位置のポインタ

	CPO(D3DXVECTOR3 pos=D3DXVECTOR3(0.0f,0.0f,0.0f)){
		position = pos;
		x=y=z=0;
		D3DXMatrixIdentity(&rota);
		ratio			= 70.0f;
		//D3DXQuaternionIdentity(&q);

		pointFlag		= 0;	
		ID				= -1;			
		nextPointID		= -1;
		prevPointID		= -1;
		blanchNum		= false;
		connectFlag		= 1;
		connectPower	= 1;
		rotaflag		= 0;
		objectNum		= 0;
		pCOArray		= NULL;
		GuideNum		= 0;
		pGuidePos		= NULL;
	};

};

#endif