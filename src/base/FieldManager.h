#ifndef _FIELD_MANAGER_H_
#define _FIELD_MANAGER_H_
/*//////////////////////////////////////////////////
			フィールドマネージャクラス
オープンワールドを想定したフィールド処理クラス
描画の有無の決定などのマネジメント
動かないオブジェクトと動くオブジェクトを別々に管理
（動くオブジェクトは専用のクラスをつくる）
今後実装予定として、自動で細かいオブジェクトの配置など
//////////////////////////////////////////////////////*/
class FieldManager;

#include"IncludeAll.h"
#include"SystemPointerBox.h"

#include"sFieldObject.h"

#define FIELD_OBJ_NUM 3

class FieldManager : public SystemPointerWrap{

	sFieldObject *SpaceDome;
	sFieldObject *Planet;
	std::list<sp<sFieldObject>> ObjectList;

public:
	FieldManager();
	~FieldManager();

	int Init();

	void SetPosIdx(POSINDEX* pidx);
	void Move();
	void Draw();
};

#endif
