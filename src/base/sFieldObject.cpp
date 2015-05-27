#include"sFieldObject.h"


sFieldObject::sFieldObject(){}

bool sFieldObject::InitField(float rotx, float roty)
{
	world = XMMatrixIdentity();
	rota = XMMatrixIdentity();
	world = XMMatrixScaling(ratio,ratio,ratio);

	XMFLOAT3 f;
	XMStoreFloat3(&f, position);
	world = XMMatrixMultiply(world, XMMatrixTranslation(f.x,f.y,f.z));

	mat camY,camX;

	camX = XMMatrixRotationAxis(XMVectorSet(0, 1, 0, 0), rotx);
	camY = XMMatrixRotationAxis(XMVectorSet(1, 0, 0, 0), roty);

	rota = XMMatrixMultiply(rota, camY );    
	rota = XMMatrixMultiply(rota, camX);

	world = XMMatrixMultiply(rota, world);
	//M_MatrixOffset(&world,&position);

	return _BaseObject::LoadFile();
}

bool sFieldObject::Draw()
{
	//pMatrixBox->SetMatrix(&world, &posidx, meshID, texID,MATBOX_NEAR);
	return true;
}

bool sFieldObject::DrawNoTransfer(int FLAG)
{
	//world._41 -= pcam;
	//world._42 -= pcam;
	//world._43 -= pcam;
	POSINDEX pi = {0,0,0};
	//pMatrixBox->SetMatrix(&world, &pi, meshID, texID, FLAG);
	return true;
}
