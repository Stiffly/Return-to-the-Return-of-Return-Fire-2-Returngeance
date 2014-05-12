#ifndef Components_Physics_h__
#define Components_Physics_h__

#include "Component.h"

namespace Components
{

struct Physics : Component
{
	Physics()
		: Mass(0.f), Static(false){}

	float Mass;
	bool Static;

	virtual Physics* Clone() const override { return new Physics(*this); }
};

}

#endif // Components_Physics_h__
