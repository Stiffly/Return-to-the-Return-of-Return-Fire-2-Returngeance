#include "PrecompiledHeader.h"
#include "ParticleSystem.h"


#include "World.h"

Systems::ParticleSystem::ParticleSystem(World *m_World) : System(m_World)
{
	m_TimeSinceLastSpawn = 0;
}

void Systems::ParticleSystem::Update(double dt)
{
	m_TimeSinceLastSpawn += dt;
}

void Systems::ParticleSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto transformComponent = m_World->GetComponent<Components::Transform>(entity, "Transform");
	if(!transformComponent)
		return;

	auto emitterComponent = m_World->GetComponent<Components::ParticleEmitter>(entity, "ParticleEmitter");
	if(emitterComponent)
	{
		if(m_TimeSinceLastSpawn > emitterComponent->SpawnFrequency)
		{
			SpawnParticles(entity, emitterComponent->SpawnCount, emitterComponent->SpreadAngle);
			m_TimeSinceLastSpawn = 0;
		}
		std::cout<<m_TimeSinceLastSpawn<<std::endl;
		std::list<ParticleData>::iterator it;
		for(it = m_ParticleEmitter[entity].begin(); it == m_ParticleEmitter[entity].end();)
		{
			auto particleComponent = m_World->GetComponent<Components::Particle>(it->ParticleID, "Particle");
			
			double timeLived = glfwGetTime() - it->SpawnTime; 
			std::cout<<timeLived<<std::endl;
			if(timeLived > particleComponent->LifeTime)
			{
				m_ParticleEmitter[entity].erase(it);
				break;
			}
			else
			{
				it++;
			}

			// Interpolates the color for each color channel by the start and end value. Decides how much the color should be interpolated based on time.
			// How big fraction the color is multiplied with
			float timeProgress = timeLived / particleComponent->LifeTime; 
			// The difference between the start and end value
			float deltaColor = glm::abs(particleComponent->ColorSpectrum[0].r - particleComponent->ColorSpectrum[1].r); 
			it->color.r = particleComponent->ColorSpectrum[0].r + deltaColor * timeProgress;
			deltaColor = glm::abs(particleComponent->ColorSpectrum[0].g - particleComponent->ColorSpectrum[1].g);
			it->color.g = particleComponent->ColorSpectrum[0].g + deltaColor * timeProgress;
			deltaColor = glm::abs(particleComponent->ColorSpectrum[0].b - particleComponent->ColorSpectrum[1].b);
			it->color.b = particleComponent->ColorSpectrum[0].b + deltaColor * timeProgress;

			//Interpolates the scale of the particle
			float deltaScale = glm::abs(particleComponent->ScaleSpectrum[0] - particleComponent->ScaleSpectrum[1]);
			it->Scale = particleComponent->ScaleSpectrum[0] + deltaScale * timeProgress;

			//Interpolates the velocity of the particle
			float deltaVelocity = glm::abs(particleComponent->VelocitySpectrum[0].x - particleComponent->VelocitySpectrum[1].x);
			it->Velocity.x = particleComponent->VelocitySpectrum[0].x + deltaVelocity * timeProgress;
			deltaVelocity = glm::abs(particleComponent->VelocitySpectrum[0].y - particleComponent->VelocitySpectrum[1].y);
			it->Velocity.y = particleComponent->VelocitySpectrum[0].y + deltaVelocity * timeProgress;	
			deltaVelocity = glm::abs(particleComponent->VelocitySpectrum[0].z - particleComponent->VelocitySpectrum[1].z);
			it->Velocity.z = particleComponent->VelocitySpectrum[0].z + deltaVelocity * timeProgress;
		}
	}
}

void Systems::ParticleSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register("ParticleEmitter", []() { return new Components::ParticleEmitter(); });
	cf->Register("Particle", []() { return new Components::Particle(); });
}

void Systems::ParticleSystem::SpawnParticles(EntityID emitterID, float spawnCount, float spreadAngle)
{
	std::list<EntityID> particles;
	for(int i = 0; i < spawnCount; i++)
	{
		auto ent = m_World->CreateEntity();
		
		auto transform = m_World->AddComponent<Components::Transform>(ent, "Transform");
		transform->Position = glm::vec3(0);
		
		auto particle = m_World->AddComponent<Components::Particle>(ent, "Particle");
		particle->LifeTime = 4000;
		Color startColor = {.4f, .45f, .2f};
		particle->ColorSpectrum.push_back(startColor);
		Color endColor = {0.f, 45.f, 23.f};
		particle->ColorSpectrum.push_back(endColor);
		particle->ScaleSpectrum.push_back(1);
		particle->ScaleSpectrum.push_back(30);
		particle->VelocitySpectrum.push_back(glm::vec3(0, -.2, 0));
		particle->VelocitySpectrum.push_back(glm::vec3(0, -3, 0));

		ParticleData data;
		data.ParticleID = ent;
		data.SpawnTime = glfwGetTime();
		data.color = particle->ColorSpectrum[0];
		data.Scale = particle->ScaleSpectrum[0];

		m_ParticleEmitter[emitterID].push_back(data);
	}
}

void Systems::ParticleSystem::Draw(double dt)
{

}


