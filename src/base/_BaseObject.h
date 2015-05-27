#ifndef _BASE_OBJECT_H_
#define _BASE_OBJECT_H_

#include<list>
#include"IncludeAll.h"
#include"SystemPointerBox.h"

// オブジェクト基本クラス
class _BaseObject : public SystemPointerWrap{
protected:

	int meshID;
	int texID;

	////////////////////
	vec			position;
	POSINDEX	posidx;
	mat			rota;
	float		ratio;
	vec			q;	//QUATERNION

	TCHAR meshFilename[100];
	TCHAR texFilename[100];

	//子オブジェクト描画
	void DrawChild();

	//PosIdxが移動したときのフラグ
	BYTE MovIdxFlag;

	BOOL DeleteFlag; //オブジェクト消去のお知らせ　falseで消去

public:

	//子オブジェクトリスト
	std::list<sp<_BaseObject>> ChildObjectList;

	_BaseObject();
	virtual ~_BaseObject();
	
	void			SetFileName(TCHAR* mesh,TCHAR *tex);
	virtual bool	LoadFile();

	vec			GetPos();
	POSINDEX*	GetPosIdx();
	mat			GetRota();
	vec			GetDirec();
	vec			GetUp();
	vec			GetPitch();

	BYTE	GetMovIdxFlag(){ return MovIdxFlag; }

	void SetPos(vec *pos);
	void SetPosIdx(POSINDEX *pidx);
	void SetPosIdx(int x,int y,int z);
	void SetDirec(vec *dir);
	void SetUp(vec *up);
	void SetPitch(vec *pitch);
	void SetRatio(float r){ratio = r;}

	void SetRota(mat *rota);

	virtual void Move();
	virtual bool Draw();
	virtual void DebugDraw(){};

	//子オブジェクトをリストに登録
	virtual size_t RegistObject(sp<_BaseObject> spObj);
	//オブジェクトの譲渡
	virtual void UnderTake( sp<_BaseObject> &spObj);
	
};
	
//BaseObject間の距離計算省略関数
vec* DistanceBetBaseObj(vec* out, _BaseObject* p1, _BaseObject* p2);


#endif