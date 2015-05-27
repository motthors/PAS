#include"LinerAdder.h"
#include<math.h>

LinerAdder::LinerAdder()
{
	value = 0;
	rate = 0.01f;
	base = 0;
	dir = 0;
}

void LinerAdder::SetBaseValue(float Val)
{
	base = Val;
	if ((base - value) == 0.f)dir = 0;
	else dir = (base - value) / fabs(base - value);
}

float LinerAdder::GetValue()
{
	return value;
}

void LinerAdder::SetAddRate(float val)
{
	rate = val;
}


void LinerAdder::Move()
//‚PƒtƒŒ[ƒ€‚É‚P‰ñŒÄ‚Ô
{
	//float dir = (base - value)/fabs(base - value);
	if ((base-value)*dir < rate)
	{
		value = base;
		dir = 0;
	}
	else
	{
		value += rate*dir;
	}
}

////////////////////////////////////////////////

LinerAdder_autoDecrease::LinerAdder_autoDecrease()
{
	value = 0;
	rate = 0.01;
}

float LinerAdder_autoDecrease::GetValue()
{
	return value;
}

void LinerAdder_autoDecrease::SetAddRate(float val)
{
	rate = val;
}

void LinerAdder_autoDecrease::Increase()
{
	if(value >= 1.0f)
		value += rate;
	else 
		value += 2*rate;
}

void LinerAdder_autoDecrease::Move()
//‚PƒtƒŒ[ƒ€‚É‚P‰ñŒÄ‚Ô
{
	value -= rate;
	if(value < 0)
	{
		value = 0;
	}
}