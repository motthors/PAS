#include"MMParts_SlideBar.h"
#include"../base/DefineRendering.h"

#define Bar_Length_Fix	0.1f
#define Bar_Theta_Min	1.0f



MMParts_SlideBar::~MMParts_SlideBar()
{
	if (PtrMinSetFlag)delete pMinVar;
	if (PtrMaxSetFlag)delete pMaxVar;
}

BYTE MMParts_SlideBar::SelectJudge(float x, float y, float aspect)
{
	float lenX = x_texel + x_Dir - x;
	float lenY = y_texel + y_Dir - y;
	lenY *= aspect;
	if ((lenX*lenX + lenY*lenY) < BaseSize*BaseSize*0.25)
	{
		ClickFlag = true;
		return 2;
	}
	return 3;
}

void MMParts_SlideBar::SetPos(float startx, float starty, float dirx, float diry, float aspect)
{
	if (*pVariable > *pMaxVar)*pVariable = *pMaxVar;
	if (*pVariable < *pMinVar)*pVariable = *pMinVar;
	ConvertVarToTheta();
	x_texel = startx;
	y_texel = starty;
	AspectDistortion = 1/aspect;
	ConvertThetaToDir();
}

void MMParts_SlideBar::SetMin(float min)
{
	pMinVar = new float;
	PtrMinSetFlag = true;
	*pMinVar = min;
}

void MMParts_SlideBar::SetMax(float max)
{
	pMaxVar = new float;
	PtrMaxSetFlag = true;
	*pMaxVar = max;
}

void MMParts_SlideBar::SetMin_p(float* pmin)
{
	pMinVar = pmin;
}

void MMParts_SlideBar::SetMax_p(float* pmax)
{
	pMaxVar = pmax;
}

void MMParts_SlideBar::ConvertVarToTheta()
{
	theta = ((*pVariable - *pMinVar) / (*pMaxVar - *pMinVar) * (twoPHI - 2.f)) + Bar_Theta_Min;
	if (fabs(theta) < Bar_Theta_Min) theta = Bar_Theta_Min / theta * fabs(theta);
}

void MMParts_SlideBar::ConvertThetaToVar()
{
	if (theta < 0)theta += twoPHI;
	*pVariable = ((theta - Bar_Theta_Min) / (twoPHI - 2.f) * (*pMaxVar - *pMinVar)) + *pMinVar;
}

void MMParts_SlideBar::ConvertThetaToDir()
{
	x_Dir = sin(theta)*(size-0.04f);
	y_Dir = cos(theta)*(size-0.04f);
	y_Dir *= AspectDistortion*0.88;
}

void MMParts_SlideBar::Move()
{
	////////////////////////��������@�}�E�X�N���b�N���ɃX���C�h�̑��쏈��
	if (ClickFlag)
	{
		//�}�E�X���W�̃s�N�Z�����W���e�N�Z�����W�֕ϊ�
		LONG x, y;
		float tx, ty;
		pInput->GetMouse_Absolute(&x, &y);
		DefRender.ConvertToTexelBaseCoord(&tx, &ty, x, y);
		//�p�x�v�Z
		theta = atan2((tx - x_texel) * AspectDistortion, (ty - y_texel));
		if (fabs(theta) < Bar_Theta_Min)
			theta = Bar_Theta_Min / theta * fabs(theta);
		ConvertThetaToVar();	//�l���f

		//�N���b�N���I�������t���O�|��
		if (0 == pInput->GetMouseButton(INPUT_MOUSE_LEFT))ClickFlag = false;
	}
	ConvertThetaToDir();	//�܂ݍ��W�v�Z
	ConvertVarToTheta();	//�l����p�x�v�Z
	ConvertThetaToVar();	//�l���f
	plaSize->Move();
}

void MMParts_SlideBar::Draw()
{
	//�x�[�X�`��
	float lasize = plaSize->GetValue();
	float fixX = x_Dir*lasize;
	float fixY = y_Dir*lasize;
	float TextHeight = lasize*BaseSize*0.2f;
	//�g  id=2:Donut
	pDrawer->DrawSquare_TexelStd(x_texel, y_texel, lasize*size, alpha, -2);
	//���j���[��
	pDrawer->Draw(x_texel, y_texel + size*1.8f - 0.05f, lasize*BaseSize*0.2*texwidthName, TextHeight, alpha, texidName);
	//���l
	pDrawer->DrawNumber(x_texel, y_texel + size*1.8f - 0.1f, TextHeight, alpha, *pVariable, log10(*pVariable) + 1, 6 - log10(*pVariable));

	//�܂ݕ`��
	pDrawer->DrawSquare_TexelStd(x_texel + fixX, y_texel + fixY, lasize*BaseSize*0.4f, alpha, -1);

}


///////////////////////////////////////////////////////////////////////////////////////////


void MMParts_SlideBarPF::Move()
{
	pVariable = *ppVariable;
	MMParts_SlideBar::Move();
}