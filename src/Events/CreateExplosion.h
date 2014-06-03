#ifndef Event_CreateExplosion_h__
#define Event_CreateExplosion_h__

#include "EventBroker.h"
#include <glm/common.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Events
{
	struct CreateExplosion : Event
	{
		CreateExplosion() 
			: Color(glm::vec4(0)) {}
		glm::vec3 Position;
		glm::vec4 Color;
		double LifeTime;
		int ParticlesToSpawn;
		std::string spritePath;
		glm::quat RelativeUpOrientation;
		float Speed;
		float SpreadAngle;
		std::vector<float> ParticleScale;
	};

}

#endif // Event_CreateExplosion_h__