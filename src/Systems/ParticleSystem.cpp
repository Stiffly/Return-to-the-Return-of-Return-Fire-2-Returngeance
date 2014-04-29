#include "PrecompiledHeader.h"
#include "ParticleSystem.h"


#include "World.h"

Systems::ParticleSystem::ParticleSystem(World *m_World) : System(m_World)
{
	
}

void Systems::ParticleSystem::Update(double dt)
{
	
}

void Systems::ParticleSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto transformComponent = m_World->GetComponent<Components::Transform>(entity, "Transform");
	if(!transformComponent)
		return;

	auto emitterComponent = m_World->GetComponent<Components::ParticleEmitter>(entity, "ParticleEmitter");
	if(emitterComponent)
	{
		emitterComponent->TimeSinceLastSpawn += dt;
		auto transformComponent = m_World->GetComponent<Components::Transform>(entity, "Transform");
		if(emitterComponent->TimeSinceLastSpawn > emitterComponent->SpawnFrequency)
		{
			SpawnParticles(entity);
			emitterComponent->TimeSinceLastSpawn = 0;
		}

		std::list<ParticleData>::iterator it;
		for(it = m_ParticleEmitter[entity].begin(); it != m_ParticleEmitter[entity].end();)
		{
			EntityID particleID = (it)->ParticleID;
			auto transformComponent = m_World->GetComponent<Components::Transform>(particleID, "Transform");
			auto particleComponent = m_World->GetComponent<Components::Particle>(particleID, "Particle");

			double timeLived = glfwGetTime() - it->SpawnTime; 
			if(timeLived > particleComponent->LifeTime)
			{
				m_World->RemoveEntity(particleID);
				it = m_ParticleEmitter[entity].erase(it);
			}
			else
			{
				// FIX: calculate once
				float timeProgress = timeLived / particleComponent->LifeTime; 
				// ColorInterpolation(timeProgress, particleComponent->ColorSpectrum, color);
				// Scale interpolation
				if(particleComponent->ScaleSpectrum.size() > 1)
					VectorInterpolation(timeProgress, particleComponent->ScaleSpectrum, transformComponent->Scale);
				// Velocity interpolation
				if(particleComponent->VelocitySpectrum.size() > 1)
					VectorInterpolation(timeProgress, particleComponent->VelocitySpectrum, transformComponent->Velocity);
				// Angular velocity interpolation
				if(particleComponent->AngularVelocitySpectrum.size() > 1)
					ScalarInterpolation(timeProgress, particleComponent->AngularVelocitySpectrum, it->AngularVelocity);
				//Angular velocity interpolation
				
				if(particleComponent->OrientationSpectrum.size() > 1)
					VectorInterpolation(timeProgress, particleComponent->OrientationSpectrum, it->Orientation);
				
				glm::vec3 v1 = glm::normalize(particleComponent->OrientationSpectrum[0]);
				glm::vec3 v2 = glm::normalize(it->Orientation);
				glm::vec3 v3 = glm::cross(v1,v2);
				float angle = glm::abs(glm::acos(glm::dot(v1, v2) / (v1.length() * v2.length())));
				transformComponent->Orientation = glm::angleAxis(angle, v3);


				//float alpha = it->AngularVelocity * dt;
				//transformComponent->Orientation = transformComponent->Orientation * it->Orientation;
				transformComponent->Position +=  transformComponent->Velocity * (float)dt;

				it++;
			}
		}
	}
}

void Systems::ParticleSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register("ParticleEmitter", []() { return new Components::ParticleEmitter(); });
	cf->Register("Particle", []() { return new Components::Particle(); });
}


void Systems::ParticleSystem::SpawnParticles(EntityID emitterID)
{
	auto emitterComponent = m_World->GetComponent<Components::ParticleEmitter>(emitterID, "ParticleEmitter");
	auto emitterTransform = m_World->GetComponent<Components::Transform>(emitterID, "Transform");
	glm::quat emitterOrientation = emitterTransform->Orientation;

	float tempSpeed =  4;
	glm::vec3 speed = glm::vec3(tempSpeed);

	for(int i = 0; i < emitterComponent->SpawnCount; i++)
	{
		auto ent = m_World->CloneEntity(emitterComponent->ParticleTemplate);

		auto particleTransform = m_World->GetComponent<Components::Transform>(ent, "Transform");
		particleTransform->Position = emitterTransform->Position;
		particleTransform->Scale = glm::vec3(1, 1, 1);

		//The emitter's orientation as "start value" times the default direction for emitter. Times the speed, and then rotate on x and y axis with the randomized spread angle. 
		float spreadAngle = emitterComponent->SpreadAngle;
		particleTransform->Velocity = emitterOrientation * glm::vec3(0, 0, -1) * speed * 
			glm::normalize(glm::angleAxis(RandomizeAngle(spreadAngle), glm::vec3(1, 0, 0))) * 
			glm::normalize(glm::angleAxis(RandomizeAngle(spreadAngle), glm::vec3(0, 1, 0))) *
			glm::normalize(glm::angleAxis(RandomizeAngle(spreadAngle), glm::vec3(0, 0, 1)));

		glm::vec3 testVel = glm::vec3(particleTransform->Velocity.x, -particleTransform->Velocity.y * 1.5, particleTransform->Velocity.z); //TEMP

		auto particle = m_World->AddComponent<Components::Particle>(ent, "Particle");
		particle->LifeTime = emitterComponent->LifeTime;
		particle->ScaleSpectrum.push_back(glm::vec3(1)); //TEMP
		//particle->ScaleSpectrum.push_back(glm::vec3(1,4,1)); //TEMP
		particle->VelocitySpectrum.push_back(particleTransform->Velocity); //TEMP
		particle->VelocitySpectrum.push_back(testVel); //TEMP
// 		particle->AngularVelocitySpectrum.push_back(0.f);
// 		particle->AngularVelocitySpectrum.push_back(-glm::pi<float>());
		particle->OrientationSpectrum = particle->VelocitySpectrum;

// 		Color startColor = {.4f, .45f, .2f};
// 		particle->ColorSpectrum.push_back(startColor);
// 		Color endColor = {0.f, 45.f, 23.f};
// 		particle->ColorSpectrum.push_back(endColor);

		

		ParticleData data;
		data.ParticleID = ent;
		data.SpawnTime = glfwGetTime();
		//data.AngularVelocity = particle->AngularVelocitySpectrum[0];
		data.Orientation = particle->OrientationSpectrum[0];

		m_ParticleEmitter[emitterID].push_back(data);
	}
}

//Randomizes between -spreadAngle/2 and spreadAngle/2
float Systems::ParticleSystem::RandomizeAngle(float spreadAngle)
{
	return ((float)rand() / ((float)RAND_MAX + 1) * spreadAngle) - spreadAngle/2;
}

//Interpolates the velocity of the particle
void Systems::ParticleSystem::VectorInterpolation(double timeProgress, std::vector<glm::vec3> spectrum, glm::vec3 &v)
{
	float dAxisValue = glm::abs(spectrum[0].x - spectrum[1].x);
	if(spectrum[0].x > spectrum[1].x)
		dAxisValue *= -1;
	v.x = spectrum[0].x + dAxisValue * timeProgress;
	dAxisValue = glm::abs(spectrum[0].y - spectrum[1].y);
	if (spectrum[0].y > spectrum[1].y)
		dAxisValue *= -1;
	v.y = spectrum[0].y + dAxisValue * timeProgress;	
	dAxisValue = glm::abs(spectrum[0].z - spectrum[1].z);
	if(spectrum[0].z > spectrum[1].z)
		dAxisValue *= -1;
	v.z = spectrum[0].z + dAxisValue * timeProgress;
}

// void Systems::ParticleSystem::ColorInterpolation(double timeProgress, std::vector<Color> spectrum, Color &c)
// {
// 	float dColor = glm::abs(spectrum[0].r - spectrum[1].r); 
// 	c.r = spectrum[0].r + dColor * timeProgress;
// 	dColor = glm::abs(spectrum[0].g - spectrum[1].g);
// 	c.g = spectrum[0].g + dColor * timeProgress;
// 	dColor = glm::abs(spectrum[0].b - spectrum[1].b);
// 	c.b = spectrum[0].b + dColor * timeProgress;
// }

void Systems::ParticleSystem::ScalarInterpolation(double timeProgress, std::vector<float> spectrum, float &alpha)
{
	float dAlpha = glm::abs(spectrum[0] - spectrum[1]);
	if(spectrum[0] > spectrum[1])
		dAlpha *= -1;
	alpha = spectrum[0] + dAlpha * timeProgress; 
}