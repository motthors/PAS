#ifndef _LINERADDER_H_
#define _LINERADDER_H_

class LinerAdder{

	float value;
	float rate;
	float base;
	float dir;
public:
	LinerAdder();

	void SetBaseValue(float Val);
	float GetValue();
	void SetAddRate(float rate);
	void Move();
};

class LinerAdder_autoDecrease{
	//0~1間をスイッチ押す間増加、押さないときは減少
	float value;
	float rate;
public:
	LinerAdder_autoDecrease();

	float GetValue();
	void SetAddRate(float rate);
	void Increase();
	void Move();
};

#endif