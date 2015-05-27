#include"_BaseObject.h"

_BaseObject::_BaseObject()
{	
	position = XMVectorSet(0,0,0,0);
	rota = XMMatrixIdentity();
	ratio	= 1.0f;
	q = XMQuaternionIdentity();
	ZeroMemory(meshFilename, sizeof(TCHAR)*100);
	ZeroMemory(texFilename,  sizeof(TCHAR)*100);
	posidx.x = 0;
	posidx.y = 0;
	posidx.z = 0;
	meshID	= -1;
	texID	= -1;
	MovIdxFlag = 0;
	DeleteFlag = true;
}

_BaseObject::~_BaseObject()
{
	ChildObjectList.clear();
}

void _BaseObject::SetFileName(TCHAR* mesh,TCHAR *tex)
{
	_tcscpy_s(meshFilename,100,mesh);
	_tcscpy_s(texFilename,100,tex);
}

bool _BaseObject::LoadFile()
{
	if(FAILED(meshID = pDataBox->GetMesh(meshFilename)))return FALSE;
	if(FAILED(texID  = pDataBox->GetTexture(texFilename)))return FALSE;
	return TRUE;
}

void _BaseObject::DrawChild()
{
	std::list<sp<_BaseObject>>::iterator it;
	for(it = ChildObjectList.begin(); it!=ChildObjectList.end();)
	{
		if(FAILED((*it)->Draw()))
		{
			UnderTake(*it);
			it = ChildObjectList.erase(it);
			continue;
		}
		++it;
	}
}

size_t _BaseObject::RegistObject(sp<_BaseObject> spObj)
{
	ChildObjectList.push_back(spObj);
	return 0;
}

void _BaseObject::UnderTake(sp<_BaseObject> &spObj)
{
	// 子オブジェクトの描画リストの中身をコピーする
	std::list< sp<_BaseObject> >::iterator it;
	for(it=spObj->ChildObjectList.begin(); it!=spObj->ChildObjectList.end(); it++)
	{
		ChildObjectList.push_back( (*it) );   // 自身にコピー
	}

	//引数の描画リストを空にする
	spObj->ChildObjectList.clear();

}

vec			_BaseObject::GetPos(){	return position;}
POSINDEX*	_BaseObject::GetPosIdx(){return &posidx;}
mat			_BaseObject::GetRota(){	return rota;}

vec	_BaseObject::GetDirec()
{
	XMFLOAT4X4 m;
	XMStoreFloat4x4(&m, rota);
	return XMVectorSet(m._31, m._32, m._33, 0);
}
vec _BaseObject::GetUp()
{
	XMFLOAT4X4 m;
	XMStoreFloat4x4(&m, rota);
	return XMVectorSet(m._21, m._22, m._23, 0);
}
vec _BaseObject::GetPitch()
{
	XMFLOAT4X4 m;
	XMStoreFloat4x4(&m, rota);
	return XMVectorSet(m._11, m._12, m._13, 0);
}

void _BaseObject::SetPos(vec *pos)
{
	position = *pos;
}
void _BaseObject::SetPosIdx(POSINDEX *pidx)
{
	posidx.x = pidx->x;
	posidx.y = pidx->y;
	posidx.z = pidx->z;
}
void _BaseObject::SetPosIdx(int x,int y,int z)
{
	posidx.x = x;
	posidx.y = y;
	posidx.z = z;
}
void _BaseObject::SetDirec(vec *dir){}
void _BaseObject::SetUp(vec *up){}
void _BaseObject::SetPitch(vec *pitch){}

//void _BaseObject::SetRota(D3DXMATRIX *r){rota = *r;}
void _BaseObject::SetRota(mat *r){rota = *r;}

void _BaseObject::Move()
{
	//Pos2PosIdx(&position, &posidx, &position);
	//MovIdxFlag = CheckTransPosIndex(&position, &posidx);
}

bool _BaseObject::Draw()
{
	//mat4* world = pMatrixBox->GetNowMatrixPointer();
	//*world = XMMatrixIdentity();
	//*world = XMMatrixScaling(ratio,ratio,ratio);
	//M_MatrixOffset(world, &position);
	//*world = XMMatrixMultiply(rota,*world);
	//
	//pMatrixBox->SetMatrix( &posidx, meshID, texID);
	return DeleteFlag;
}


vec* DistanceBetBaseObj(vec* out, _BaseObject* p1, _BaseObject* p2)
//BaseObject間の距離ベクトル計算省略関数
//p1 - p2
{
	POSINDEX idist;
	PosIdxSub(&idist, p1->GetPosIdx(), p2->GetPosIdx());
	vec vecdist;
	vecdist = XMVectorSubtract(p1->GetPos(), p2->GetPos());
	//PosIdx2Pos(out, &vecdist, &idist);
	return out;
}