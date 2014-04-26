#ifndef ParticleSystem_h__
#define ParticleSystem_h__

#include "System.h"
#include "Components/Transform.h"
#include "Components/ParticleEmitter.h"
#include "Components/Particle.h"
#include "Components/Model.h"
#include "Components/Physics.h"
#include "Components/Box.h"
#include "Components/PointLight.h"
#include "Color.h"
#include <GLFW/glfw3.h>

namespace Systems
{

	struct ParticleData
	{
		EntityID ParticleID;
		double SpawnTime;
		float Scale;
		Color color;
	};

class ParticleSystem : public System
{
public:
	ParticleSystem(World* world);
	void RegisterComponents(ComponentFactory* cf) override;

	void Update(double dt) override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;
private:
	void SpawnParticles(EntityID emitterID, glm::vec3 pos, float spawnCount, float spreadAngle, double lifeTime, double dt);
	std::map<EntityID, std::list<ParticleData>> m_ParticleEmitter;
	std::map<EntityID, double> m_TimeSinceLastSpawn;

	float RandomizeAngle(float spreadAngle);
	
};

}


#endif // !ParticleSystem_h__