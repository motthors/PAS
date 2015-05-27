//#ifndef _BASE_SYSTEM_H_
//#define _BASE_SYSTEM_H_
//
//#include"IncludeAll.h"
//#include"VirtualController.h"
//#include"SoundBox.h"
//#include"FPSCounter.h"
//#include"DataBox.h"
//#include"Camera.h"
//
//// オブジェクト基本クラス
//class _BaseSystem{
//
//protected:
//
//	LPD3DXFONT pd3dfont;
//
//	VCon		*pInput;
//	SoundBox	*pSoundBox;
//	FPSCounter	*pfpsc;
//	DataBox		*pDataBox;
//	Camera		*pcam;
//
//public:
//	_BaseSystem(){
//		pd3dfont	=NULL;
//		pInput		=NULL;
//		pSoundBox	=NULL;
//		pfpsc		=NULL;
//		pDataBox	=NULL;
//		pcam		=NULL;
//	};
//	void CopySystemPointer(_BaseSystem *p){
//		this->pd3dfont	=p->pd3dfont;
//		this->pInput	=p->pInput;
//		this->pSoundBox	=p->pSoundBox;
//		this->pfpsc		=p->pfpsc;
//		this->pDataBox	=p->pDataBox;
//		this->pcam		=p->pcam;
//	}
//
//	void SetFontDevice(LPD3DXFONT font){pd3dfont=font;};
//	void SetInputDevice(VCon* input){pInput=input;};
//	void SetSoundDevice(SoundBox* S){pSoundBox=S;};
//	void SetCounterDevice(FPSCounter* fps){pfpsc=fps;};
//	void SetDataBoxDevice(DataBox* db){pDataBox=db;};
//	void SetCameraDevice(_Camera* pc){pcam=pc;};
//
//};{{{{{{
//
//
//#endif