#include "PrecompiledHeader.h"
#include "TankSteeringSystem.h"
#include "World.h"

void Systems::TankSteeringSystem::RegisterComponents( ComponentFactory* cf )
{
	cf->Register<Components::TankSteering>([]() { return new Components::TankSteering(); });
	cf->Register<Components::TowerSteering>([]() { return new Components::TowerSteering(); });
	cf->Register<Components::BarrelSteering>([]() { return new Components::BarrelSteering(); });
}

void Systems::TankSteeringSystem::Initialize()
{
	m_TankInputController = std::unique_ptr<TankSteeringInputController>(new TankSteeringInputController(EventBroker));
	m_TowerInputController = std::unique_ptr<TowerSteeringInputController>(new TowerSteeringInputController(EventBroker));
}

void Systems::TankSteeringSystem::Update(double dt)
{
	m_TankInputController->Update(dt);
	m_TowerInputController->Update(dt);
}

void Systems::TankSteeringSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto tankSteeringComponent = m_World->GetComponent<Components::TankSteering>(entity);
	if(tankSteeringComponent)
	{
		Events::TankSteer e;
		e.Entity = entity;
		e.PositionX = m_TankInputController->PositionX;
		e.PositionY = m_TankInputController->PositionY;
		e.Handbrake = m_TankInputController->Handbrake;
		EventBroker->Publish(e);
	}

	auto towerSteeringComponent = m_World->GetComponent<Components::TowerSteering>(entity);
	if(towerSteeringComponent)
	{
		auto transformComponent = m_World->GetComponent<Components::Transform>(entity);
		glm::quat orientation =  glm::angleAxis(towerSteeringComponent->TurnSpeed * m_TowerInputController->TowerDirection * (float)dt, towerSteeringComponent->Axis);
		transformComponent->Orientation *= orientation;
	}

	auto barrelSteeringComponent = m_World->GetComponent<Components::BarrelSteering>(entity);
	if(barrelSteeringComponent)
	{
		auto transformComponent = m_World->GetComponent<Components::Transform>(entity);
		auto absoluteTransform = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(entity);
		glm::quat orientation =  glm::angleAxis(barrelSteeringComponent->TurnSpeed * m_TowerInputController->BarrelDirection * (float)dt, barrelSteeringComponent->Axis);
		transformComponent->Orientation *= orientation;

		if(m_TowerInputController->Shoot && m_TimeSinceLastShot[entity] > 1.0)
		{
			EntityID clone = m_World->CloneEntity(barrelSteeringComponent->ShotTemplate);
			auto templateAbsoluteTransform = m_World->GetSystem<Systems::TransformSystem>()->AbsoluteTransform(barrelSteeringComponent->ShotTemplate);
			auto cloneTransform = m_World->GetComponent<Components::Transform>(clone);
			cloneTransform->Position = templateAbsoluteTransform.Position;
			cloneTransform->Orientation = absoluteTransform.Orientation * cloneTransform->Orientation;
			Events::SetVelocity e;
			e.Entity = clone;
			e.Velocity = absoluteTransform.Orientation * (glm::vec3(0.f, 0.f, -1.f) * barrelSteeringComponent->ShotSpeed);
			EventBroker->Publish(e);
			m_TimeSinceLastShot[entity] = 0;


			auto clonePhysicsComponent = m_World->GetComponent<Components::Physics>(clone);
			//1,670m/s
			//25kg
			EntityID baseParent = m_World->GetEntityBaseParent(entity);
			Events::ApplyPointImpulse ePointImpulse ;
			ePointImpulse.Entity = baseParent;
			ePointImpulse.Position = absoluteTransform.Position;
			ePointImpulse.Impulse = glm::normalize(absoluteTransform.Orientation * glm::vec3(0, 0, 1))  * clonePhysicsComponent->Mass * 1670.f;
			EventBroker->Publish(ePointImpulse);
		}

		m_TimeSinceLastShot[entity] += dt;
	}
}

void Systems::TankSteeringSystem::TankSteeringInputController::Update( double dt )
{
	PositionX = m_Horizontal;
	PositionY = m_Vertical;
}

void Systems::TankSteeringSystem::TowerSteeringInputController::Update( double dt )
{
	TowerDirection = m_TowerDirection;
	BarrelDirection = m_BarrelDirection;
	Shoot = m_Shoot;
}

bool Systems::TankSteeringSystem::TankSteeringInputController::OnCommand(const Events::InputCommand &event)
{
	float val;
	if(abs(event.Value) < 0.3f)
	{
		val = 0.f;
	}
	else
	{
		val = event.Value;
	}
	
	if (event.Command == "horizontal")
	{
		m_Horizontal = val;
		m_Vertical = -0.4f;
	}
	else if (event.Command == "vertical")
	{
		m_Vertical = -val;
	}
	
	else if (event.Command == "handbrake")
	{
		Handbrake = val > 0;
	}

	return true;
}

bool Systems::TankSteeringSystem::TowerSteeringInputController::OnCommand( const Events::InputCommand &event )
{
	float val = event.Value;
	if(event.Command == "tower_rotation")
	{
		m_TowerDirection = -val;
	}
	else if(event.Command == "barrel_rotation")
	{
		m_BarrelDirection = val;
	}

	else if (event.Command == "shoot")
	{
		m_Shoot = val > 0;
	}
	return true;
}

bool Systems::TankSteeringSystem::TowerSteeringInputController::OnMouseMove( const Events::MouseMove &event )
{
	return false;
}

bool Systems::TankSteeringSystem::TankSteeringInputController::OnMouseMove( const Events::MouseMove &event )
{
	return false;
}




