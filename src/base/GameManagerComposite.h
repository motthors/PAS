#ifndef _COMPOSITE_H_
#define _COMPOSITE_H_

#include<vector>
#include"IncludeAll.h"
#include"SystemPointerBox.h"

class GMComposite : public SystemPointerWrap{

protected:

	std::vector<GMComposite*> GMAry;	// 子シーン
	GMComposite* pParent;       // 親シーン

public:

	GMComposite()
	{
		pParent = NULL;
	};
	virtual ~GMComposite()
	{
		UINT i;
		for(i=0;i<GMAry.size();++i){
			SAFE_DELETE(GMAry[i]);
		}
		GMAry.clear();
	}

	virtual void SetParent(GMComposite* GMC){pParent=GMC;}
	virtual void AddGM(GMComposite* pGM){GMAry.push_back(pGM);}	// 子シーンを追加
	
	virtual void ChangeMenuFlag(int flag, bool ResetFlag=false){};

	//virtual int Init(){};
	virtual bool Move()=0;
	virtual void Draw()=0;
};


#endif