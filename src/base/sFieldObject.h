#ifndef _FIELD_H_
#define _FIELD_H_

#include"IncludeAll.h"
#include"_BaseObject.h"
#include"DataBox.h"
#include"Camera.h"

#define FIELD_OBJ_NUM 3

class sFieldObject : public _BaseObject{

	mat world;
public:
	sFieldObject();
	bool InitField(float rotX, float rotY);
	bool Draw();
	bool DrawNoTransfer(int FLAG);
};

#endif
