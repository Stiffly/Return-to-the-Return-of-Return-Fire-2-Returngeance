#ifndef Components_Sprite_h__
#define Components_Sprite_h__

#include <string>

#include "Component.h"
#include "Color.h"

namespace Components
{

struct Sprite : Component
{
	std::string SpriteFile;
	Color Color;
	
	virtual Sprite* Clone() const override { return new Sprite(*this); }
};

}
#endif // !Components_Sprite_h__