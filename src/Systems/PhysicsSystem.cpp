#include "PrecompiledHeader.h"

// We�re not using anything product specific yet. We undef these so we don�t get the usual
// product initialization for the products.
#undef HK_FEATURE_PRODUCT_AI
#undef HK_FEATURE_PRODUCT_ANIMATION
#undef HK_FEATURE_PRODUCT_CLOTH
#undef HK_FEATURE_PRODUCT_DESTRUCTION_2012
#undef HK_FEATURE_PRODUCT_DESTRUCTION
#undef HK_FEATURE_PRODUCT_BEHAVIOR
#undef HK_FEATURE_PRODUCT_PHYSICS_2012
//#undef HK_FEATURE_PRODUCT_PHYSICS
// Also we�re not using any serialization/versioning so we don�t need any of these.
#define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700
#define HK_EXCLUDE_FEATURE_RegisterVersionPatches
#define HK_EXCLUDE_FEATURE_RegisterReflectedClasses
#define HK_EXCLUDE_FEATURE_MemoryTracker
#define HK_CLASSES_FILE "Common/Serialize/classlist/hkClasses.h"
#include "Common/Serialize/Util/hkBuiltinTypeRegistry.cxx"
#define HK_COMPAT_FILE "Common/Compat/hkCompatVersions.h"
// This include generates an initialization function based on the products
// and the excluded features.
#include <Common/Base/keycode.cxx>
#include <Common/Base/Config/hkProductFeatures.cxx>
#include "PhysicsSystem.h"
#include "World.h"



Systems::PhysicsSystem::PhysicsSystem(World* world) : System(world)
{
	{
		hkMemorySystem::FrameInfo finfo(500 * 1024);	// Allocate 500KB of Physics solver buffer
		hkMemoryRouter* memoryRouter = hkMemoryInitUtil::initDefault(hkMallocAllocator::m_defaultMallocAllocator, finfo);
		hkBaseSystem::init(memoryRouter, HavokErrorReport);


		hkpWorldCinfo worldInfo;
		worldInfo.setupSolverInfo(hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM);
		worldInfo.m_gravity = hkVector4(0.0f, -9.8f, 0.0f);
		worldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_FIX_ENTITY; // just fix the entity if the object falls off too far

		// You must specify the size of the broad phase - objects should not be simulated outside this region
		worldInfo.setBroadPhaseWorldSize(1000.0f);
		m_PhysicsWorld = new hkpWorld(worldInfo);
	}
	// Register all collision agents, even though only box - box will be used in this particular example.
	// It's important to register collision agents before adding any entities to the world.
	hkpAgentRegisterUtil::registerAllAgents(m_PhysicsWorld->getCollisionDispatcher());
	
	//
	// Initialize the visual debugger so we can connect remotely to the simulation
	// The context must exist beyond the use of the VDB instance, and you can make
	// whatever contexts you like for your own viewer types.
	//
	hkpPhysicsContext* context = new hkpPhysicsContext;
	hkpPhysicsContext::registerAllPhysicsProcesses(); // all the physics viewers
	context->addWorld(m_PhysicsWorld); // add the physics world so the viewers can see it
	SetupVisualDebugger(context);

	//SetupPhysics(m_PhysicsWorld);
}

void Systems::PhysicsSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register("Physics", []() { return new Components::Physics(); });
}

void Systems::PhysicsSystem::Update(double dt)
{	
	m_PhysicsWorld->stepDeltaTime(0.0166f);

	// Step the visual debugger
	StepVisualDebugger();
}

void Systems::PhysicsSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{	
	auto transformComponent = m_World->GetComponent<Components::Transform>(entity, "Transform");
	if (!transformComponent)
		return;

	if (m_RigidBodies.find(entity) == m_RigidBodies.end())
	{
		SetUpPhysicsState(entity, parent);
	}
	else
	{
		hkVector4 position = m_RigidBodies[entity]->getPosition();
		transformComponent->Position = glm::vec3(position(0), position(1), position(2));
		hkQuaternion orientation = m_RigidBodies[entity]->getRotation();
		transformComponent->Orientation = glm::quat(orientation(3),orientation(0), orientation(1), orientation(2));
	}


}

void Systems::PhysicsSystem::SetUpPhysicsState(EntityID entity, EntityID parent)
{
	auto transformComponent = m_World->GetComponent<Components::Transform>(entity, "Transform");
	if (!transformComponent)
		return;

	auto physicsComponent = m_World->GetComponent<Components::Physics>(entity, "Physics");
	if (!physicsComponent)
		return;

	auto sphereComponent = m_World->GetComponent<Components::Sphere >(entity, "Sphere");
	auto boxComponent = m_World->GetComponent<Components::Box >(entity, "Box");
	
	
	hkpConvexShape* shape;
	hkpRigidBodyCinfo rigidBodyInfo;
	hkMassProperties massProperties;
	

	if (sphereComponent)
	{
		shape = new hkpSphereShape(sphereComponent->Radius);
		rigidBodyInfo.m_shape = shape;
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
		
		hkpInertiaTensorComputer::computeSphereVolumeMassProperties(sphereComponent->Radius, physicsComponent->Mass, massProperties);
		
	}
	else if (boxComponent)
	{
		shape = new hkpBoxShape(hkVector4(boxComponent->Width, boxComponent->Height, boxComponent->Depth));
		rigidBodyInfo.m_shape = shape;
		rigidBodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
		hkReal thickness = 0.1;
		hkpInertiaTensorComputer::computeBoxSurfaceMassProperties(hkVector4(boxComponent->Width - thickness, boxComponent->Height - thickness, boxComponent->Depth - thickness), physicsComponent->Mass, thickness, massProperties);
	}
	else
	{
		return;
	}
	
	rigidBodyInfo.m_position.set(transformComponent->Position.x, transformComponent->Position.y, transformComponent->Position.z);
	rigidBodyInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
	rigidBodyInfo.m_centerOfMass = massProperties.m_centerOfMass;
	rigidBodyInfo.m_mass = massProperties.m_mass;

	// Create RigidBody
	hkpRigidBody* rigidBody = new hkpRigidBody(rigidBodyInfo);
	shape->removeReference();

	m_PhysicsWorld->addEntity(rigidBody);
	m_RigidBodies[entity] = rigidBody;
	rigidBody->removeReference();

}

void Systems::PhysicsSystem::TearDownPhysicsState(EntityID entity, EntityID parent)
{	

}

void Systems::PhysicsSystem::OnComponentCreated(std::string type, std::shared_ptr<Component> component)
{

}

void Systems::PhysicsSystem::OnComponentRemoved(std::string type, Component* component)
{

}


void Systems::PhysicsSystem::SetupVisualDebugger(hkpPhysicsContext* worlds)
{
	// Setup the visual debugger
	hkArray<hkProcessContext*> contexts;
	contexts.pushBack(worlds);

	m_VisualDebugger = new hkVisualDebugger(contexts);
	m_VisualDebugger->serve();

	// Allocate memory for internal profiling information
	// You can discard this if you do not want Havok profiling information
	hkMonitorStream& stream = hkMonitorStream::getInstance();
	stream.resize(500 * 1024);	// 500K for timer info
	stream.reset();

}

void Systems::PhysicsSystem::StepVisualDebugger()
{
	// Step the debugger
	m_VisualDebugger->step();

	// Reset internal profiling info for next frame
	hkMonitorStream::getInstance().reset();
}

void HK_CALL Systems::PhysicsSystem::HavokErrorReport(const char* msg, void*)
{
	LOG_DEBUG("%s", msg);
}
