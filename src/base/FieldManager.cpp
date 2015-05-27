#include"FieldManager.h"
#include"LoadFileNameList.h"

FieldManager::FieldManager()
{
	Planet = nullptr;
	SpaceDome = nullptr;
}

FieldManager::~FieldManager()
{
	SAFE_DELETE(SpaceDome);
	SAFE_DELETE(Planet);
	ObjectList.clear();
}

int FieldManager::Init()
{	
	//Planet
	Planet = new sFieldObject;
	Planet->SetDataBoxDevice(pDataBox);
	Planet->SetMatrixBox(pMatrixBox);
	Planet->SetFileName(_T("data/field/space/PASPlanet_2.x"), _T("data/earth.png"));
	Planet->SetPos(&XMVectorSet(0, 0, 0, 0));
	Planet->SetRatio(pPASCT->GetRg());
	if (!Planet->InitField(PHI, 0))return FALSE;


	//station 3
	//sp<sFieldObject> pfield3;
	//pfield3.SetPtr(new sFieldObject());
	//pfield3->SetDataBoxDevice(pDataBox);
	//pfield3->SetMatrixBox(pMatrixBox);
	//pfield3->SetFileName(_T("data/field/station3.x"),_T("data/field/as6_tex_d.png"));
	//pfield3->SetPos(&D3DXVECTOR3(0.0f, -900.0f, -3500.0f));
	//pfield3->SetRatio(1000000.0f);
	//if(0>pfield3->InitField(0, 0.3f))return FALSE;
	//ObjectList.push_back(pfield3);


	//space dome
	SpaceDome = new sFieldObject();
	SpaceDome->SetDataBoxDevice(pDataBox);
	SpaceDome->SetMatrixBox(pMatrixBox);
	SpaceDome->SetFileName(LOAD_M_SPHERE, LOAD_T_FIELD_SPACE);
	SpaceDome->SetPos(&XMVectorSet(0, 0, 0, 0));
	SpaceDome->SetRatio(1000000.0f);
	if(!SpaceDome->InitField(0, 0))return FALSE;

	return FALSE;
}

void  FieldManager::Move()
{
	Planet->SetRatio( pPASCT->GetRg() );
}

void  FieldManager::Draw()
{
	SpaceDome->DrawNoTransfer(MATBOX_SPACE);
	Planet->Draw();

	//std::list<sp<sFieldObject>>::iterator it;
	//for(it=ObjectList.begin();it!=ObjectList.end();++it)
	//{
	//	(*it)->Draw();
	//}
}
