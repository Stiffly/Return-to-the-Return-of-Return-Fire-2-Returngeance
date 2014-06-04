#ifndef TriggerSystem_h__
#define TriggerSystem_h__


#include "System.h"
#include "Components/Transform.h"
#include "Components/TankSteering.h"
#include "Events/SetVelocity.h"
#include "Events/ApplyForce.h"
#include "Events/ApplyPointImpulse.h"

#include <unordered_map>

#include "Components/Trigger.h"
#include "Components/TriggerExplosion.h"
#include "Events/EnterTrigger.h"
#include "Events/LeaveTrigger.h"
#include "Components/Flag.h"
#include "Components/TriggerMove.h"
#include "Components/TriggerRotate.h"
#include "Components/Move.h"
#include "Components/Rotate.h"
#include "Components/Player.h"
#include "Components/Team.h"
#include "Events/Move.h"
#include "Events/Rotate.h"
#include <math.h>
namespace Systems
{
	class TriggerSystem : public System
	{
	public:
		
		TriggerSystem(World* world, std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<::ResourceManager> resourceManager)
			: System(world, eventBroker, resourceManager) { }

		void Initialize() override;
		void RegisterComponents(ComponentFactory* cf) override;
		void Update(double dt) override;
		void UpdateEntity(double dt, EntityID entity, EntityID parent) override;
		//void OnComponentCreated(std::string type, std::shared_ptr<Component> component) override;
		void OnComponentRemoved(EntityID entity, std::string type, Component* component) override;
		void OnEntityCommit(EntityID entity) override;
		void OnEntityRemoved(EntityID entity) override;

		EventRelay<TriggerSystem, Events::EnterTrigger> m_EEnterTrigger;
		bool OnEnterTrigger(const Events::EnterTrigger &event);
		EventRelay<TriggerSystem, Events::LeaveTrigger> m_ELeaveTrigger;
		bool OnLeaveTrigger(const Events::LeaveTrigger &event);
	private:
		void Flag(EntityID entity, EntityID phantomEntity);
		void Explosion(EntityID entity, EntityID phantomEntity);
		void Move(EntityID entity, bool queue, bool swap);
		void Rotate(EntityID entity, bool queue, bool swap);
	};

}
#endif // TriggerSystem_h__
