#include "PrecompiledHeader.h"
#include "GameWorld.h"

void GameWorld::Initialize()
{
	World::Initialize();

	ResourceManager->Preload("Model", "Models/Placeholders/PhysicsTest/Plane.obj");
	ResourceManager->Preload("Model", "Models/Placeholders/PhysicsTest/ArrowCube.obj");

	// Interface
	BindKey(GLFW_KEY_A, "interface_horizontal", -1.f);
	BindKey(GLFW_KEY_D, "interface_horizontal", 1.f);
	BindGamepadAxis(Gamepad::Axis::LeftX, "interface_horizontal", 1.f);
	BindGamepadButton(Gamepad::Button::Left, "interface_horizontal", -1.f);
	BindGamepadButton(Gamepad::Button::Right, "interface_horizontal", 1.f);
	BindKey(GLFW_KEY_W, "interface_vertical", 1.f);
	BindKey(GLFW_KEY_S, "interface_vertical", -1.f);
	BindGamepadAxis(Gamepad::Axis::LeftY, "interface_vertical", 1.f);
	BindGamepadButton(Gamepad::Button::Up, "interface_vertical", 1.f);
	BindGamepadButton(Gamepad::Button::Down, "interface_vertical", -1.f);
	BindKey(GLFW_KEY_SPACE, "interface_confirm", 1.f);
	BindGamepadButton(Gamepad::Button::A, "interface_confirm", 1.f);
	
	

	BindKey(GLFW_KEY_W, "vertical", 1.f);
	BindKey(GLFW_KEY_S, "vertical", -1.f);
	BindKey(GLFW_KEY_A, "horizontal", -1.f);
	BindKey(GLFW_KEY_D, "horizontal", 1.f);
	BindGamepadAxis(Gamepad::Axis::LeftX, "horizontal", 1.f);
	BindGamepadAxis(Gamepad::Axis::LeftY, "vertical", 1.f);

	BindKey(GLFW_KEY_UP, "barrel_rotation", 1.f);
	BindKey(GLFW_KEY_DOWN, "barrel_rotation", -1.f);
	BindKey(GLFW_KEY_LEFT, "tower_rotation", -1.f);
	BindKey(GLFW_KEY_RIGHT, "tower_rotation", 1.f);
	BindGamepadAxis(Gamepad::Axis::RightX, "tower_rotation", 1.f);
	BindGamepadAxis(Gamepad::Axis::RightY, "barrel_rotation", 1.f);

	BindKey(GLFW_KEY_SPACE, "handbrake", 1.f);
	BindGamepadButton(Gamepad::Button::A, "handbrake", 1.f);

	BindKey(GLFW_KEY_Z, "shoot", 1.f);
	BindGamepadAxis(Gamepad::Axis::RightTrigger, "shoot", 1.f);

	BindKey(GLFW_KEY_X, "use", 1.f);
	BindGamepadButton(Gamepad::Button::X, "use", 1.f);

	BindMouseButton(GLFW_MOUSE_BUTTON_1, "cam_lock", 1.f);
	BindKey(GLFW_KEY_Y, "cam_vertical", 1.f);
	BindKey(GLFW_KEY_H, "cam_vertical", -1.f);
	BindKey(GLFW_KEY_G, "cam_horizontal", -1.f);
	BindKey(GLFW_KEY_J, "cam_horizontal", 1.f);
	BindKey(GLFW_KEY_U, "cam_normal", 1.f);
	BindKey(GLFW_KEY_T, "cam_normal", -1.f);
	BindKey(GLFW_KEY_B, "cam_speed", -1.f);
	BindKey(GLFW_KEY_N, "cam_speed", 1.f);
	
	//BindGamepadButton(Gamepad::Button::Up, "Gamepad::Button::Up", 1.f);
	//BindGamepadButton(Gamepad::Button::Down, "Gamepad::Button::Down", 1.f);
	//BindGamepadButton(Gamepad::Button::Left, "Gamepad::Button::Left", 1.f);
	//BindGamepadButton(Gamepad::Button::Right, "Gamepad::Button::Right", 1.f);
	//BindGamepadButton(Gamepad::Button::Start, "Gamepad::Button::Start", 1.f);
	//BindGamepadButton(Gamepad::Button::Back, "Gamepad::Button::Back", 1.f);
	//BindGamepadButton(Gamepad::Button::LeftThumb, "Gamepad::Button::LeftThumb", 1.f);
	//BindGamepadButton(Gamepad::Button::RightThumb, "Gamepad::Button::RightThumb", 1.f);
	//BindGamepadButton(Gamepad::Button::LeftShoulder, "Gamepad::Button::LeftShoulder", 1.f);
	//BindGamepadButton(Gamepad::Button::RightShoulder, "Gamepad::Button::RightShoulder", 1.f);
	//BindGamepadButton(Gamepad::Button::A, "Gamepad::Button::A", 1.f);
	//BindGamepadButton(Gamepad::Button::B, "Gamepad::Button::B", 1.f);
	//BindGamepadButton(Gamepad::Button::X, "Gamepad::Button::X", 1.f);
	//BindGamepadButton(Gamepad::Button::Y, "Gamepad::Button::Y", 1.f);

	RegisterComponents();

	auto camera = CreateEntity();
	{
		auto transform = AddComponent<Components::Transform>(camera);
		transform->Position.z = 150.f;
		transform->Position.y = 10.f;
		//transform->Orientation = glm::quat(glm::vec3(glm::pi<float>() / 8.f, 0.f, 0.f));
		auto cameraComp = AddComponent<Components::Camera>(camera);
		cameraComp->FarClip = 2000.f;
		auto freeSteering = AddComponent<Components::FreeSteering>(camera);
	}
	{
		Events::SetViewportCamera e;
		e.CameraEntity = camera;
		e.ViewportFrame = "ViewportFreeCam";
		EventBroker->Publish(e);
	}

	{
		auto ground = CreateEntity();
		auto transform = AddComponent<Components::Transform>(ground);
		transform->Position = glm::vec3(0, -50, 0);
		//transform->Scale = glm::vec3(400.0f, 10.0f, 400.0f);
		transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		auto model = AddComponent<Components::Model>(ground);
		model->ModelFile = "Models/TestScene3/testScene2.obj";
		//model->ModelFile = "Models/Placeholders/Terrain/Terrain2.obj";

		auto physics = AddComponent<Components::Physics>(ground);
		physics->Mass = 10;
		physics->Static = true;
		physics->CollisionLayer = (int)Components::Physics::CollisionLayer::STATIC;

		auto groundshape = CreateEntity(ground);
		auto transformshape = AddComponent<Components::Transform>(groundshape);
		auto meshShape = AddComponent<Components::MeshShape>(groundshape);
		//meshShape->ResourceName = "Models/Placeholders/Terrain/Terrain2.obj";
		meshShape->ResourceName = "Models/TestScene3/testScene2.obj";


		CommitEntity(groundshape);
		CommitEntity(ground);
	}

	/*EntityID tank1 = CreateTank(1);
	{
	auto transform = GetComponent<Components::Transform>(tank1);
	transform->Position.z = -125.f;
	transform->Position.y = -20.f;
	transform->Orientation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
	Events::SetViewportCamera e;
	e.CameraEntity = GetProperty<EntityID>(tank1, "Camera");
	e.ViewportFrame = "Viewport1";
	EventBroker->Publish(e);
	}*/

	/*EntityID tank2 = CreateTank(2);
	{
		auto transform = GetComponent<Components::Transform>(tank2);
		transform->Position.x = 10.f;
		transform->Position.z = 50.f;

		Events::SetViewportCamera e;
		e.CameraEntity = GetProperty<EntityID>(tank2, "Camera");
		e.ViewportFrame = "Viewport2";
		EventBroker->Publish(e);
	}*/

	{
		auto flag = CreateEntity();
		auto transform = AddComponent<Components::Transform>(flag);
		transform->Position = glm::vec3(0, -50, 100);
		auto model = AddComponent<Components::Model>(flag);
		model->ModelFile = "Models/Flag/FishingRod/FishingRod.obj";
		{
			auto fish = CreateEntity(flag);
			auto transform = AddComponent<Components::Transform>(fish);
			transform->Position = glm::vec3(-1.3f, 1.0, 0);
			auto model = AddComponent<Components::Model>(fish);
			model->ModelFile = "Models/Flag/LeFish/Salmon.obj";
			CommitEntity(fish);
		}

		auto trigger = AddComponent<Components::Trigger>(flag);
		{
			auto shape = CreateEntity(flag);
			auto transform = AddComponent<Components::Transform>(shape);
			transform->Position = glm::vec3(-0.9f, 0.9f, 0);
			auto box = AddComponent<Components::BoxShape>(shape);
			box->Width = 1.1f;
			box->Depth = 0.7f;
			box->Height = 3.9f;
			CommitEntity(shape);
		}

		auto flagComponent = AddComponent<Components::Flag>(flag);

		CommitEntity(flag);
	}

	CreateGate(glm::vec3(0, -50, 150));
	glm::vec3 Position = glm::vec3(-3.93076, -49, 0);
	{
		auto garage = CreateEntity();
		auto transform = AddComponent<Components::Transform>(garage);
		transform->Position = Position;
		auto player = AddComponent<Components::Player>(garage);
		player->ID = 1;

		

		auto ElevatorBase = CreateEntity(garage);
		{	
			auto transform = AddComponent<Components::Transform>(ElevatorBase);
			auto model = AddComponent<Components::Model>(ElevatorBase);
			model->ModelFile = "Models/SpawnRamp/Main/Main.obj";
			auto physics = AddComponent<Components::Physics>(ElevatorBase);
			physics->Static = true;
			physics->CollisionLayer = (int)Components::Physics::CollisionLayer::STATIC;

			{
				auto mainShape = CreateEntity(ElevatorBase);
				auto transform = AddComponent<Components::Transform>(mainShape);
				transform->Position = glm::vec3(0.f, 0, 0.f);
				auto mesh = AddComponent<Components::MeshShape>(mainShape);
				mesh->ResourceName =  "Models/SpawnRamp/Main/CollisionShape.obj";
				CommitEntity(mainShape);
			}
			{
				auto StairsShape = CreateEntity(ElevatorBase);
				auto transform = AddComponent<Components::Transform>(StairsShape);
				transform->Position = glm::vec3(-7.48639f, 0.4809f, 3.79166f);
				auto box = AddComponent<Components::BoxShape>(StairsShape);
				box->Width = 1.211f;
				box->Height = 1.798f;
				box->Depth = 3.229f;
				CommitEntity(StairsShape);
			}
		}
		CommitEntity(ElevatorBase);

		auto leftHatch = CreateEntity(garage);
		{
			auto transform = AddComponent<Components::Transform>(leftHatch);
			transform->Position = glm::vec3(5.83103f, 1.3553f, -2.40268f);
			auto model = AddComponent<Components::Model>(leftHatch);
			model->ModelFile = "Models/SpawnRamp/LeftHatch/LeftHatch.obj";
			auto physics = AddComponent<Components::Physics>(leftHatch);
			physics->Static = true;
			physics->CollisionLayer = (int)Components::Physics::CollisionLayer::STATIC;
			auto rotate = AddComponent<Components::Rotate>(leftHatch);
			rotate->StartRotation = transform->Orientation;
			rotate->GoalRotation = transform->Orientation * glm::angleAxis(-glm::radians(110.f),glm::vec3(0, 0, 1)); 
			rotate->Time = 5.f;
			{
				auto shape = CreateEntity(leftHatch);
				auto transform = AddComponent<Components::Transform>(shape);
				transform->Position = glm::vec3(-2.79331, 0, 0);
				auto box = AddComponent<Components::BoxShape>(shape);
				box->Width = 2.317f;
				box->Height = 0.183f;
				box->Depth = 6.987f;
				CommitEntity(shape);
			}
			CommitEntity(leftHatch);
		}

		auto rightHatch = CreateEntity(garage);
		{
			auto transform = AddComponent<Components::Transform>(rightHatch);
			transform->Position = glm::vec3(-4.52423f, 1.3553f, -2.40268f);
			auto model = AddComponent<Components::Model>(rightHatch);
			model->ModelFile = "Models/SpawnRamp/RightHatch/RightHatch.obj";
			auto physics = AddComponent<Components::Physics>(rightHatch);
			physics->Static = true;
			physics->CollisionLayer = (int)Components::Physics::CollisionLayer::STATIC;
			auto rotate = AddComponent<Components::Rotate>(rightHatch);
			rotate->StartRotation = transform->Orientation;
			rotate->GoalRotation = transform->Orientation * glm::angleAxis(glm::radians(110.f),glm::vec3(0, 0, 1)); 
			rotate->Time = 5.f;
			{
				auto shape = CreateEntity(rightHatch);
				auto transform = AddComponent<Components::Transform>(shape);
				transform->Position = glm::vec3(2.79331, 0, 0);
				auto box = AddComponent<Components::BoxShape>(shape);
				box->Width = 2.317f;
				box->Height = 0.183f;
				box->Depth = 6.987f;
				CommitEntity(shape);
			}
			CommitEntity(rightHatch);
		}

		auto elevator = CreateEntity(garage);
		{
			auto transform = AddComponent<Components::Transform>(elevator);
			transform->Position = glm::vec3(0.69385f, 1.25222f, -2.39938f) + glm::vec3(0, -9.5f, 0);
			auto model = AddComponent<Components::Model>(elevator);
			model->ModelFile = "Models/SpawnRamp/Elevator/Elevator.obj";
			auto physics = AddComponent<Components::Physics>(elevator);
			physics->Static = true;
			physics->CollisionLayer = (int)Components::Physics::CollisionLayer::STATIC;
			auto move = AddComponent<Components::Move>(elevator);
			move->StartPosition = transform->Position;
			move->GoalPosition = transform->Position + glm::vec3(0, 9.5f, 0);
			move->Speed = 1.5f;
			{
				auto shape = CreateEntity(elevator);
				auto transform = AddComponent<Components::Transform>(shape);
				transform->Position = glm::vec3(0, 0, 0);
				auto box = AddComponent<Components::BoxShape>(shape);
				box->Width = 4.754f;
				box->Height = 0.141f;
				box->Depth = 6.86f;
				CommitEntity(shape);
			}

			auto spawnPoint = CreateEntity(elevator);
			{
				auto transform = AddComponent<Components::Transform>(spawnPoint);
				transform->Position.y = 1.f;
				auto spawnPointComponent = AddComponent<Components::SpawnPoint>(spawnPoint);
			}
			CommitEntity(spawnPoint);
		}
		CommitEntity(elevator);

		auto garageComponent = AddComponent<Components::Garage>(garage);
		garageComponent->DoorLeft = leftHatch;
		garageComponent->DoorRight = rightHatch;
		garageComponent->Elevator = elevator;
		CommitEntity(garage);
		
		{
			auto trigger = CreateEntity(garage);
			SetProperty(trigger, "Name", "BoundsTrigger");
			auto transform = AddComponent<Components::Transform>(trigger);
			transform->Position = glm::vec3(0, 44.87521f, 0);
			auto triggerComponent = AddComponent<Components::Trigger>(trigger);
			{
				auto shape = CreateEntity(trigger);
				auto transform = AddComponent<Components::Transform>(shape);
				transform->Position = glm::vec3(0, 0, 0);
				auto box = AddComponent<Components::BoxShape>(shape);
				box->Width = 72.98f;
				box->Height = 54.53f;
				box->Depth = 72.98f;
				CommitEntity(shape);
			}
			CommitEntity(trigger);
		}

		{
			auto trigger = CreateEntity(garage);
			SetProperty(trigger, "Name", "ElevatorShaftTrigger");
			auto transform = AddComponent<Components::Transform>(trigger);
			transform->Position = glm::vec3(0.69385f, -6.7997f, -2.0f);
			auto triggerComponent = AddComponent<Components::Trigger>(trigger);
			{
				auto shape = CreateEntity(trigger);
				auto transform = AddComponent<Components::Transform>(shape);
				transform->Position = glm::vec3(0, 0, 0);
				auto box = AddComponent<Components::BoxShape>(shape);
				box->Width = 2.0f;
				box->Height = 12.f;
				box->Depth = 2.0f;
				CommitEntity(shape);
			}
			CommitEntity(trigger);
		}
	}

	//for (int i = 0; i < 500; i++)
	//{
	//	auto Light = CreateEntity();
	//	auto transform = AddComponent<Components::Transform>(Light);
	//	transform->Position = glm::vec3((5 + (i*2.f))*cos(i*2.f), 3.f, (5 + (i*2.f))*sin(i*2.f));
	//	auto light = AddComponent<Components::PointLight>(Light);
	//	light->Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	//	light->Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	//	light->Radius = 15.f;
	//	light->specularExponent = 100.f;
	//	CommitEntity(Light);
	//	//auto model = AddComponent<Components::Model>(Light, "Model");
	//	//model->ModelFile = "Models/Placeholders/PhysicsTest/PointLight.obj";
	//}
}

void GameWorld::Update(double dt)
{
	World::Update(dt);
}

void GameWorld::RegisterComponents()
{
	m_ComponentFactory.Register<Components::Transform>([]() { return new Components::Transform(); }); 
	m_ComponentFactory.Register<Components::Template>([]() { return new Components::Template(); });	
	m_ComponentFactory.Register<Components::Player>([]() { return new Components::Player(); });	
	m_ComponentFactory.Register<Components::Flag>([]() { return new Components::Flag(); });
	m_ComponentFactory.Register<Components::Move>([]() { return new Components::Move(); });
	m_ComponentFactory.Register<Components::Rotate>([]() { return new Components::Rotate(); });
}

void GameWorld::RegisterSystems()
{
	m_SystemFactory.Register<Systems::TimerSystem>([this]() { return new Systems::TimerSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::DamageSystem>([this]() { return new Systems::DamageSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::TransformSystem>([this]() { return new Systems::TransformSystem(this, EventBroker, ResourceManager); });
	//m_SystemFactory.Register<Systems::LevelGenerationSystem>([this]() { return new Systems::LevelGenerationSystem(this); });
	m_SystemFactory.Register<Systems::InputSystem>([this]() { return new Systems::InputSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::DebugSystem>([this]() { return new Systems::DebugSystem(this, EventBroker, ResourceManager); });
	//m_SystemFactory.Register<Systems::CollisionSystem>([this]() { return new Systems::CollisionSystem(this); });
	m_SystemFactory.Register<Systems::ParticleSystem>([this]() { return new Systems::ParticleSystem(this, EventBroker, ResourceManager); });
	//m_SystemFactory.Register<Systems::PlayerSystem>([this]() { return new Systems::PlayerSystem(this); });
	m_SystemFactory.Register<Systems::FreeSteeringSystem>([this]() { return new Systems::FreeSteeringSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::TankSteeringSystem>([this]() { return new Systems::TankSteeringSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::WheelPairSystem>([this]() { return new Systems::WheelPairSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::SoundSystem>([this]() { return new Systems::SoundSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::PhysicsSystem>([this]() { return new Systems::PhysicsSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::TriggerSystem>([this]() { return new Systems::TriggerSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::FollowSystem>([this]() { return new Systems::FollowSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::GarageSystem>([this]() { return new Systems::GarageSystem(this, EventBroker, ResourceManager); });
	m_SystemFactory.Register<Systems::RenderSystem>([this]() { return new Systems::RenderSystem(this, EventBroker, ResourceManager); });
}

void GameWorld::AddSystems()
{
	AddSystem<Systems::TimerSystem>();
	AddSystem<Systems::DamageSystem>();
	AddSystem<Systems::TransformSystem>();
	//AddSystem<Systems::LevelGenerationSystem>();
	AddSystem<Systems::InputSystem>();
	AddSystem<Systems::DebugSystem>();
	//AddSystem<Systems::CollisionSystem>();
	AddSystem<Systems::ParticleSystem>();
	//AddSystem<Systems::PlayerSystem>();
	AddSystem<Systems::FreeSteeringSystem>();
	AddSystem<Systems::TankSteeringSystem>();
	AddSystem<Systems::WheelPairSystem>();
	AddSystem<Systems::SoundSystem>();
	AddSystem<Systems::PhysicsSystem>();
	AddSystem<Systems::TriggerSystem>();
	AddSystem<Systems::FollowSystem>();
	AddSystem<Systems::GarageSystem>();
	AddSystem<Systems::RenderSystem>();
}

void GameWorld::BindKey(int keyCode, std::string command, float value)
{
	Events::BindKey e;
	e.KeyCode = keyCode;
	e.Command = command;
	e.Value = value;
	EventBroker->Publish(e);
}

void GameWorld::BindMouseButton(int button, std::string command, float value)
{
	Events::BindMouseButton e;
	e.Button = button;
	e.Command = command;
	e.Value = value;
	EventBroker->Publish(e);
}

void GameWorld::BindGamepadAxis(Gamepad::Axis axis, std::string command, float value)
{
	Events::BindGamepadAxis e;
	e.Axis = axis;
	e.Command = command;
	e.Value = value;
	EventBroker->Publish(e);
}

void GameWorld::BindGamepadButton(Gamepad::Button button, std::string command, float value)
{
	Events::BindGamepadButton e;
	e.Button = button;
	e.Command = command;
	e.Value = value;
	EventBroker->Publish(e);
}

void GameWorld::CreateGate(glm::vec3 Position)
{
	{
		auto gateBase = CreateEntity();
		auto transform = AddComponent<Components::Transform>(gateBase);
		transform->Position = Position;
		auto model = AddComponent<Components::Model>(gateBase);
		model->ModelFile = "Models/Gate/Lift/Lift.obj";
		auto physics = AddComponent<Components::Physics>(gateBase);
		physics->Static = true;
		physics->CollisionLayer = (int)Components::Physics::CollisionLayer::STATIC;

		{
			auto leftBaseShape = CreateEntity(gateBase);
			auto transform = AddComponent<Components::Transform>(leftBaseShape);
			transform->Position = glm::vec3(8.5f, 3, 0);
			auto box = AddComponent<Components::BoxShape>(leftBaseShape);
			box->Width = 3.f/2;
			box->Height = 6.094f/2;
			box->Depth = 4.f/2;
			CommitEntity(leftBaseShape);
		}
		{
			auto RightBaseShape = CreateEntity(gateBase);
			auto transform = AddComponent<Components::Transform>(RightBaseShape);
			transform->Position = glm::vec3(-8.5f, 3, 0);
			auto box = AddComponent<Components::BoxShape>(RightBaseShape);
			box->Width = 3.f/2;
			box->Height = 6.094f/2;
			box->Depth = 4.f/2;
			CommitEntity(RightBaseShape);
		}
		{
			auto LeftTopShape = CreateEntity(gateBase);
			auto transform = AddComponent<Components::Transform>(LeftTopShape);
			transform->Position = glm::vec3(7.5485f, 9.385f, 0);
			auto box = AddComponent<Components::BoxShape>(LeftTopShape);
			box->Width = 1.747f/2;
			box->Height = 6.851f/2;
			box->Depth = 1.767f/2;
			CommitEntity(LeftTopShape);
		}
		{
			auto RightTopShape = CreateEntity(gateBase);
			auto transform = AddComponent<Components::Transform>(RightTopShape);
			transform->Position = glm::vec3(-7.5485f, 9.385f, 0);
			auto box = AddComponent<Components::BoxShape>(RightTopShape);
			box->Width = 1.747f/2;
			box->Height = 6.851f/2;
			box->Depth = 1.767f/2;
			CommitEntity(RightTopShape);
		}
		{
			auto RampShape = CreateEntity(gateBase);
			auto transform = AddComponent<Components::Transform>(RampShape);
			transform->Position = glm::vec3(0, 0.24f, 0);
			auto mesh = AddComponent<Components::MeshShape>(RampShape);
			mesh->ResourceName = "Models/Gate/Lift/RampCollision.obj";
			CommitEntity(RampShape);
		}
		CommitEntity(gateBase);
	}

	{
		auto gate = CreateEntity();
		auto transform = AddComponent<Components::Transform>(gate);
		transform->Position = Position;
		auto model = AddComponent<Components::Model>(gate);
		model->ModelFile = "Models/Gate/Gate/Gate.obj";		
		auto physics = AddComponent<Components::Physics>(gate);
		physics->Static = true;
		physics->CollisionLayer = (int)Components::Physics::CollisionLayer::STATIC;
		auto move = AddComponent<Components::Move>(gate);
		move->Speed = 5.f;
		move->GoalPosition = glm::vec3(Position.x, Position.y + 9.02345f, Position.z);
		move->StartPosition = glm::vec3(Position.x, Position.y, Position.z);
		{
			auto mainShape = CreateEntity(gate);
			auto transform = AddComponent<Components::Transform>(mainShape);
			transform->Position = glm::vec3(0.f, 3.09776f, 0.f);
			auto box = AddComponent<Components::BoxShape>(mainShape);
			box->Width = 6.963f;
			box->Height = 2.478f;
			box->Depth = 0.522f;
			CommitEntity(mainShape);
		}
		{
			auto lowerShape = CreateEntity(gate);
			auto transform = AddComponent<Components::Transform>(lowerShape);
			transform->Position = glm::vec3(0.f, 0.35f, 0.f);
			auto box = AddComponent<Components::BoxShape>(lowerShape);
			box->Width = 6.963f;
			box->Height = 0.308f;
			box->Depth = 0.222f;
			CommitEntity(lowerShape);
		}
		CommitEntity(gate);

		{
			auto gateTrigger = CreateEntity();
			auto transform = AddComponent<Components::Transform>(gateTrigger);
			transform->Position = glm::vec3(Position.x, Position.y + 2.8241, Position.z);
			auto trigger = AddComponent<Components::Trigger>(gateTrigger);
			auto triggerMove = AddComponent<Components::TriggerMove>(gateTrigger);
			triggerMove->Entity = gate;


			{
				auto shape = CreateEntity(gateTrigger);
				auto transform = AddComponent<Components::Transform>(shape);
				transform->Position = glm::vec3(0.f, 2.8241f, 0.f);
				auto box = AddComponent<Components::BoxShape>(shape);
				box->Width = 6.963f;
				box->Height = 3.104f;
				box->Depth = 10.356f;
				CommitEntity(shape);
			}
			CommitEntity(gateTrigger);
		}

	}
}

EntityID GameWorld::CreateJeep(int playerID)
{
	auto jeep = CreateEntity();
	auto transform = AddComponent<Components::Transform>(jeep);
	transform->Position = glm::vec3(0, 5, 0);
	transform->Orientation = glm::angleAxis(glm::pi<float>() / 2, glm::vec3(0, 1, 0));
	auto physics = AddComponent<Components::Physics>(jeep);
	physics->Mass = 1800;
	physics->Static = false;
	auto vehicle = AddComponent<Components::Vehicle>(jeep);
	vehicle->DownshiftRPM = 3500.f;
	vehicle->UpshiftRPM = 6000.f;
	
	AddComponent<Components::Input>(jeep);
	auto player = AddComponent<Components::Player>(jeep);
	player->ID = playerID;
	auto health = AddComponent<Components::Health>(jeep);
	health->Amount = 100.f;

	{
		auto shape = CreateEntity(jeep);
		auto transform = AddComponent<Components::Transform>(shape);
		auto meshShape = AddComponent<Components::MeshShape>(shape);
		meshShape->ResourceName = "Models/Jeep/Chassi/ChassiCollision.obj";
		CommitEntity(shape);
	}

	{
		auto chassis = CreateEntity(jeep);
		auto transform = AddComponent<Components::Transform>(chassis);
		transform->Position = glm::vec3(0, 0, 0); // 0.6577f
		auto model = AddComponent<Components::Model>(chassis);
		model->ModelFile = "Models/Jeep/Chassi/chassi.obj";
	}

	//Create wheels
	float wheelOffset = 0.4f;
	float springLength = 0.3f;
	float suspensionStrength = 35.f;
	{
		auto wheel = CreateEntity(jeep);
		auto transform = AddComponent<Components::Transform>(wheel);
		transform->Position = glm::vec3(1.9f, 0.5546f - wheelOffset, -0.9242f);
		transform->Scale = glm::vec3(1.0f);
		auto model = AddComponent<Components::Model>(wheel);
		model->ModelFile = "Models/Jeep/WheelFront/wheelFront.obj";
		auto Wheel = AddComponent<Components::Wheel>(wheel);
		Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
		Wheel->AxleID = 0;
		Wheel->Mass = 50;
		Wheel->Radius = 0.837f;
		Wheel->Steering = true;
		Wheel->SuspensionStrength = suspensionStrength;
		Wheel->Friction = 4.f;
		Wheel->ConnectedToHandbrake = true;
		CommitEntity(wheel);
	}

	{
		auto wheel = CreateEntity(jeep);
		auto transform = AddComponent<Components::Transform>(wheel);
		transform->Position = glm::vec3(-1.9f, 0.5546f - wheelOffset, -0.9242f);
		transform->Scale = glm::vec3(1.0f);
		transform->Orientation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 0, 1));
		auto model = AddComponent<Components::Model>(wheel);
		model->ModelFile = "Models/Jeep/WheelFront/wheelFront.obj";
		auto Wheel = AddComponent<Components::Wheel>(wheel);
		Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
		Wheel->AxleID = 0;
		Wheel->Mass = 50;
		Wheel->Radius = 0.837f;
		Wheel->Steering = true;
		Wheel->SuspensionStrength = suspensionStrength;
		Wheel->Friction = 4.f;
		Wheel->ConnectedToHandbrake = true;
		CommitEntity(wheel);
	}

	{
		auto wheel = CreateEntity(jeep);
		auto transform = AddComponent<Components::Transform>(wheel);
		transform->Position = glm::vec3(0.2726f, 0.2805f - wheelOffset, 1.9307f);
		auto model = AddComponent<Components::Model>(wheel);
		model->ModelFile = "Models/Jeep/WheelBack/wheelBack.obj";
		auto Wheel = AddComponent<Components::Wheel>(wheel);
		Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
		Wheel->AxleID = 1;
		Wheel->Mass = 50;
		Wheel->Radius = 0.737f;
		Wheel->Steering = false;
		Wheel->SuspensionStrength = suspensionStrength;
		Wheel->Friction = 4.f;
		Wheel->ConnectedToHandbrake = true;
		CommitEntity(wheel);
	}

	{
		auto wheel = CreateEntity(jeep);
		auto transform = AddComponent<Components::Transform>(wheel);
		transform->Position = glm::vec3(-0.2726f, 0.2805f - wheelOffset, 1.9307f);
		transform->Orientation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 0, 1));
		auto model = AddComponent<Components::Model>(wheel);
		model->ModelFile = "Models/Jeep/WheelBack/wheelBack.obj";
		auto Wheel = AddComponent<Components::Wheel>(wheel);
		Wheel->Hardpoint = transform->Position + glm::vec3(0.f, springLength, 0.f);
		Wheel->AxleID = 1;
		Wheel->Mass = 50;
		Wheel->Radius = 0.737f;
		Wheel->Steering = false;
		Wheel->SuspensionStrength = suspensionStrength;
		Wheel->Friction = 4.f;
		Wheel->ConnectedToHandbrake = true;
		CommitEntity(wheel);
	}

	CommitEntity(jeep);

	return jeep;	
}
