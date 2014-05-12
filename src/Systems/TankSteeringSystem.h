#include <array>

#include "System.h"
#include "Events/TankSteer.h"
#include "Components/Transform.h"
#include "Components/TankSteering.h"
#include "Components/TowerSteering.h"
#include "Components/BarrelSteering.h"
#include "Components/Vehicle.h"
#include "InputController.h"

namespace Systems
{

	class TankSteeringSystem : public System
	{
	public:
		TankSteeringSystem(World* world, std::shared_ptr<::EventBroker> eventBroker)
			: System(world, eventBroker) { }

		void RegisterComponents(ComponentFactory* cf) override;
		void Initialize() override;

		void Update(double dt) override;
		void UpdateEntity(double dt, EntityID entity, EntityID parent) override;

	private:
		class TankSteeringInputController;
		std::unique_ptr<TankSteeringInputController> m_TankInputController;
		class TowerSteeringInputController;
		std::unique_ptr<TowerSteeringInputController> m_TowerInputController;
	};

	class TankSteeringSystem::TankSteeringInputController : InputController
	{
	public:
		TankSteeringInputController(std::shared_ptr<::EventBroker> eventBroker)
			: InputController(eventBroker)
		{
			m_Horizontal = 0.f;
			m_Vertical = 0.f;
			PositionX = 0;
			PositionY = 0;
			Handbrake = false;
		}

		float PositionY;
		float PositionX;
		bool Handbrake;
		void Update(double dt);
	protected:
		virtual bool OnCommand(const Events::InputCommand &event);
		virtual bool OnMouseMove(const Events::MouseMove &event);

	private:
		float m_Horizontal;
		float m_Vertical;
	};

	class TankSteeringSystem::TowerSteeringInputController : InputController
	{
	public:
		TowerSteeringInputController(std::shared_ptr<::EventBroker> eventBroker)
			: InputController(eventBroker)
		{
			m_TowerDirection = 0.f;
			m_BarrelDirection = 0.f;
			TowerDirection = 0.f;
			BarrelDirection = 0.f;
		}

		float TowerDirection;
		float BarrelDirection;
		void Update(double dt);
	protected:
		virtual bool OnCommand(const Events::InputCommand &event);
		virtual bool OnMouseMove(const Events::MouseMove &event);

	private:
	 float m_TowerDirection;
	float m_BarrelDirection;
	};

}