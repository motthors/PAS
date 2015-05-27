#include"MMParts_Draw.h"


MMParts_Draw::MMParts_Draw()
{
	pDrawer = nullptr;
	plaSize = new LinerAdder;
	plaSize->SetBaseValue(0.0f);
	plaSize->SetAddRate(0.2f);
}

MMParts_Draw::~MMParts_Draw()
{
	SAFE_DELETE(plaSize);
}

void MMParts_Draw::Init(int N, int I)
{
	texidName = N;
	texidImage = I;
}

void MMParts_Draw::SetPos(float startx, float starty, float dirx, float diry, float aspect)
{
	x_texel = startx;
	y_texel = starty;
	x_Dir = dirx;
	y_Dir = diry;
	AspectDistortion = aspect;
}

BYTE MMParts_Draw::SelectJudge(float x, float y, float AspectDistortion)
{
	float lenX = x_texel + x_Dir - x;
	float lenY = y_texel + y_Dir - y;
	lenY *= AspectDistortion;
	if ((lenX*lenX + lenY*lenY) < size*size)
		return 1;
	return 0;
}

void MMParts_Draw::Appear()
{
	plaSize->SetBaseValue(1.0f);
}

void MMParts_Draw::Select()
{

}

bool MMParts_Draw::Cancel()
{
	plaSize->SetBaseValue(0);
	return false;
}

void MMParts_Draw::Inactive()
//自分と子を非アクティブに
{
	alpha = 0.2f;
	auto itr = (MMParts_Draw*)Child;
	FOR(ChildNum)
	{
		itr->alpha = 0.2f;
		itr = (MMParts_Draw*)itr->Brother;
	}
}

void MMParts_Draw::Active()
{
	alpha = 1.0f;
	auto itr = (MMParts_Draw*)Child;
	FOR(ChildNum)
	{
		itr->alpha = 1.0f;
		itr = (MMParts_Draw*)itr->Brother;
	}
}

void MMParts_Draw::Drag(float x, float y)
{
	this->x_texel += x;
	this->y_texel += y;
	auto itr = (MMParts_Draw*)Child;
	FOR(ChildNum)
	{
		itr->Drag(x, y);
		itr = (MMParts_Draw*)itr->Brother;
	}
}

void MMParts_Draw::Move()
{
	plaSize->Move();
}

void MMParts_Draw::Draw()
{
	float lasize = plaSize->GetValue();
	float fixX = x_Dir*lasize;
	float fixY = y_Dir*lasize;
	pDrawer->DrawSquare_TexelStd(x_texel + fixX, y_texel + fixY, lasize*size, alpha, -1);
	pDrawer->Draw(x_texel + fixX, y_texel + fixY + 0.03f, lasize*size*0.2f*texwidthName, lasize*size*0.2f, alpha, texidName);
}
