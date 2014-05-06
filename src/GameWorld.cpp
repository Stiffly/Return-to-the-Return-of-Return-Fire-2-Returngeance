#include "PrecompiledHeader.h"
#include "GameWorld.h"

void GameWorld::Initialize()
{
	World::Initialize();

	m_ResourceManager.Preload("Model", "Models/Placeholders/PhysicsTest/Plane.obj");
	m_ResourceManager.Preload("Model", "Models/Placeholders/PhysicsTest/ArrowCube.obj");

	BindKey(GLFW_KEY_W, "+forward");
	BindKey(GLFW_KEY_S, "+backward");
	BindKey(GLFW_KEY_A, "+left");
	BindKey(GLFW_KEY_D, "+right");
	BindKey(GLFW_KEY_SPACE, "+up");
	BindKey(GLFW_KEY_LEFT_CONTROL, "+down");
	BindKey(GLFW_KEY_LEFT_ALT, "+slow");
	BindKey(GLFW_KEY_LEFT_SHIFT, "+fast");
	BindMouseButton(GLFW_MOUSE_BUTTON_1, "+attack");
	BindMouseButton(GLFW_MOUSE_BUTTON_2, "+attack2");
	BindMouseButton(GLFW_MOUSE_BUTTON_3, "+attack3");

	RegisterComponents();

	{
		auto camera = CreateEntity();
		auto transform = AddComponent<Components::Transform>(camera, "Transform");
		transform->Position.z = 20.f;
		transform->Position.y = 20.f;
		//transform->Orientation = glm::quat(glm::vec3(glm::pi<float>() / 8.f, 0.f, 0.f));
		auto cameraComp = AddComponent<Components::Camera>(camera, "Camera");
		cameraComp->FarClip = 2000.f;
		auto freeSteering = AddComponent<Components::FreeSteering>(camera, "FreeSteering");
		CommitEntity(camera);
	}


	{
		auto ground = CreateEntity();
		auto transform = AddComponent<Components::Transform>(ground, "Transform");
		transform->Position = glm::vec3(0, -5, 0);
		transform->Scale = glm::vec3(400.0f, 10.0f, 400.0f);
		transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		auto model = AddComponent<Components::Model>(ground, "Model");
		model->ModelFile = "Models/Placeholders/PhysicsTest/Cube.obj";
		auto box = AddComponent<Components::Box>(ground, "Box");
		box->Width = 200;
		box->Height = 5;
		box->Depth = 200;

		auto physics = AddComponent<Components::Physics>(ground, "Physics");
		physics->Mass = 10;
		physics->Static = true;

		CommitEntity(ground);
	}


	{
		auto jeep = CreateEntity();
		auto transform = AddComponent<Components::Transform>(jeep, "Transform");
		transform->Position = glm::vec3(0, 2, 0);

		auto physics = AddComponent<Components::Physics>(jeep, "Physics");
		physics->Mass = 1200;
		auto box = AddComponent<Components::Box>(jeep, "Box");
		box->Width = 1.487f;
		box->Height = 0.727f;
		box->Depth = 2.594f;
		auto vehicle = AddComponent<Components::Vehicle>(jeep, "Vehicle");

		AddComponent<Components::Input>(jeep, "Input");

		{
			auto chassis = CreateEntity(jeep);
			auto transform = AddComponent<Components::Transform>(chassis, "Transform");
			transform->Position = glm::vec3(0, -0.6577f, 0);
			auto model = AddComponent<Components::Model>(chassis, "Model");
			model->ModelFile = "Models/JeepV2/Chassi/chassi.OBJ";

		}

		{
			auto wheel = CreateEntity(jeep);
			auto transform = AddComponent<Components::Transform>(wheel, "Transform");
			transform->Position = glm::vec3(1.4f, 0.5546f - 0.6577f - 0.2, -0.9242f);
			transform->Scale = glm::vec3(1.0f);
			auto model = AddComponent<Components::Model>(wheel, "Model");
			model->ModelFile = "Models/JeepV2/WheelFront/wheelFront.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel, "Wheel");
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, 1.f, 0.f);
			Wheel->AxleID = 0;
			Wheel->Mass = 50;
			Wheel->Radius = 0.837f;
			Wheel->Steering = true;
			Wheel->SuspensionStrength = 40.f;
			Wheel->Friction = 4.0f;
			Wheel->ConnectedToHandbrake = true;
			CommitEntity(wheel);
		}

		{
			auto wheel = CreateEntity(jeep);
			auto transform = AddComponent<Components::Transform>(wheel, "Transform");
			transform->Position = glm::vec3(-1.4f, 0.5546f - 0.6577f - 0.2, -0.9242f);
			transform->Scale = glm::vec3(1.0f);
			transform->Orientation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 0, 1));
			auto model = AddComponent<Components::Model>(wheel, "Model");
			model->ModelFile = "Models/JeepV2/WheelFront/wheelFront.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel, "Wheel");
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, 1.f, 0.f);
			Wheel->AxleID = 0;
			Wheel->Mass = 50;
			Wheel->Radius = 0.837f;
			Wheel->Steering = true;
			Wheel->SuspensionStrength = 40.f;
			Wheel->Friction = 4.0f;
			Wheel->ConnectedToHandbrake = true;
			CommitEntity(wheel);
		}

		{
			auto wheel = CreateEntity(jeep);
			auto transform = AddComponent<Components::Transform>(wheel, "Transform");
			transform->Position = glm::vec3(0.2726f, 0.2805f - 0.6577f, 1.9307f);
			auto model = AddComponent<Components::Model>(wheel, "Model");
			model->ModelFile = "Models/JeepV2/WheelBack/wheelBack.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel, "Wheel");
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, 1.f, 0.f);
			Wheel->AxleID = 1;
			Wheel->Mass = 10;
			Wheel->Radius = 0.737f;
			Wheel->Steering = false;
			Wheel->SuspensionStrength = 50.f;
			Wheel->Friction = 4.0f;
			CommitEntity(wheel);
		}

		{
			auto wheel = CreateEntity(jeep);
			auto transform = AddComponent<Components::Transform>(wheel, "Transform");
			transform->Position = glm::vec3(-0.2726f, 0.2805f - 0.6577f, 1.9307f);
			transform->Orientation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 0, 1));
			auto model = AddComponent<Components::Model>(wheel, "Model");
			model->ModelFile = "Models/JeepV2/WheelBack/wheelBack.obj";
			auto Wheel = AddComponent<Components::Wheel>(wheel, "Wheel");
			Wheel->Hardpoint = transform->Position + glm::vec3(0.f, 1.f, 0.f);
			Wheel->AxleID = 1;
			Wheel->Mass = 10;
			Wheel->Radius = 0.737f;
			Wheel->Steering = false;
			Wheel->SuspensionStrength = 50.f;
			Wheel->Friction = 4.0f;
			CommitEntity(wheel);
		}
		CommitEntity(jeep);
	}

/*

 	{
 		// Front Right Wheel
 		auto ent = CreateEntity(car);
 		auto transform = AddComponent<Components::Transform>(ent, "Transform");
 		transform->Scale = glm::vec3(1)/glm::vec3(3, 1, 5);
 		transform->Position = glm::vec3(1.1f, -1.5f, -1.3f);
 		auto Wheel = AddComponent<Components::Wheel>(ent, "Wheel");
 		Wheel->Hardpoint = glm::vec3(1.1f, 0.f, -1.3f);// HACK: make into component
 		Wheel->AxleID = 0;
 		Wheel->Mass = 10;
 		Wheel->Radius = 0.5f;
 		Wheel->Steering = true;
 		Wheel->SuspensionStrength = 20.f;
 		auto model = AddComponent<Components::Model>(ent, "Model");
 		model->ModelFile = "Models/Placeholders/PhysicsTest/Cube2.obj";
 		CommitEntity(ent);
 	}
 	{
 		// Front Left Wheel
 		auto ent = CreateEntity(car);
 		auto transform = AddComponent<Components::Transform>(ent, "Transform");
 		transform->Scale = glm::vec3(1)/glm::vec3(3, 1, 5);
 		transform->Position = glm::vec3(-1.1f, -1.5f, -1.3f);
 		auto Wheel = AddComponent<Components::Wheel>(ent, "Wheel");
 		Wheel->Hardpoint = glm::vec3(-1.1f, 0.f, -1.3f);
 		Wheel->AxleID = 0;
 		Wheel->Mass = 10;
 		Wheel->Radius = 0.5f;
 		Wheel->Steering = true;
 		Wheel->SuspensionStrength = 20.f;
 		auto model = AddComponent<Components::Model>(ent, "Model");
 		model->ModelFile = "Models/Placeholders/PhysicsTest/Cube2.obj";
 		CommitEntity(ent);
 	}
 	{
 		// Back Right Wheel
 		auto ent = CreateEntity(car);
 		auto transform = AddComponent<Components::Transform>(ent, "Transform");
 		transform->Scale = glm::vec3(1)/glm::vec3(3, 1, 5);
 		transform->Position = glm::vec3(1.1f, -1.5f, 1.3f);
 		auto Wheel = AddComponent<Components::Wheel>(ent, "Wheel");
 		Wheel->Hardpoint = glm::vec3(1.1f, 0.f, 1.3f);
 		Wheel->AxleID = 1;
 		Wheel->Mass = 10;
 		Wheel->Radius = 0.5f;
 		Wheel->Steering = false;
 		Wheel->ConnectedToHandbrake = true;
 		Wheel->SuspensionStrength = 20.f;
 		auto model = AddComponent<Components::Model>(ent, "Model");
 		model->ModelFile = "Models/Placeholders/PhysicsTest/Cube2.obj";
 		CommitEntity(ent);
 	}
 	{
 		// Back Left Wheel
 		auto ent = CreateEntity(car);
 		auto transform = AddComponent<Components::Transform>(ent, "Transform");
 		transform->Scale = glm::vec3(1)/glm::vec3(3, 1, 5);
 		transform->Position = glm::vec3(-1.1f, -1.5f, 1.3f);
 		auto Wheel = AddComponent<Components::Wheel>(ent, "Wheel");
 		Wheel->Hardpoint = glm::vec3(-1.1f, 0.f, 1.3f);
 		Wheel->AxleID = 1;
 		Wheel->Mass = 10;
 		Wheel->Radius = 0.5f;
 		Wheel->Steering = false;
 		Wheel->ConnectedToHandbrake = true;
 		Wheel->SuspensionStrength = 20.f;
 		auto model = AddComponent<Components::Model>(ent, "Model");
 		model->ModelFile = "Models/Placeholders/PhysicsTest/Cube2.obj";
 		CommitEntity(ent);
 	}
 	
 	CommitEntity(car);
 }
*/

	for (int i = 0; i < 10; i++)
	{
		auto cube = CreateEntity();
		auto transform = AddComponent<Components::Transform>(cube, "Transform");
		transform->Position = glm::vec3(20, 10 + i * 2, 0);
		transform->Scale = glm::vec3(1);
		transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		auto model = AddComponent<Components::Model>(cube, "Model");
		model->ModelFile = "Models/Placeholders/PhysicsTest/Cube2.obj";

		auto physics = AddComponent<Components::Physics>(cube, "Physics");
		physics->Mass = 100;
		auto box = AddComponent<Components::Box>(cube, "Box");
		box->Width = 0.5f;
		box->Height = 0.5f;
		box->Depth = 0.5f;
		CommitEntity(cube);
	}

	{
		auto entity = CreateEntity();
		AddComponent(entity, "Transform");
		auto emitter = AddComponent<Components::SoundEmitter>(entity, "SoundEmitter");
		emitter->Path = "Sounds/korvring.wav";
		emitter->Loop = true;
		//GetSystem<Systems::SoundSystem>("SoundSystem")->PlaySound(emitter);
		CommitEntity(entity);
	}
}

void GameWorld::Update(double dt)
{
	World::Update(dt);
}

void GameWorld::RegisterComponents()
{
	m_ComponentFactory.Register("Transform", []() { return new Components::Transform(); }); 
	m_ComponentFactory.Register("Template", []() { return new Components::Template(); });	
}

void GameWorld::RegisterSystems()
{
	m_SystemFactory.Register("TransformSystem", [this]() { return new Systems::TransformSystem(this, m_EventBroker); });
	//m_SystemFactory.Register("LevelGenerationSystem", [this]() { return new Systems::LevelGenerationSystem(this); });
	m_SystemFactory.Register("InputSystem", [this]() { return new Systems::InputSystem(this, m_EventBroker); });
	m_SystemFactory.Register("DebugSystem", [this]() { return new Systems::DebugSystem(this, m_EventBroker); });
	//m_SystemFactory.Register("CollisionSystem", [this]() { return new Systems::CollisionSystem(this); });
	////m_SystemFactory.Register("ParticleSystem", [this]() { return new Systems::ParticleSystem(this); });
	//m_SystemFactory.Register("PlayerSystem", [this]() { return new Systems::PlayerSystem(this); });
	m_SystemFactory.Register("FreeSteeringSystem", [this]() { return new Systems::FreeSteeringSystem(this, m_EventBroker); });
	m_SystemFactory.Register("SoundSystem", [this]() { return new Systems::SoundSystem(this, m_EventBroker); });
	m_SystemFactory.Register("PhysicsSystem", [this]() { return new Systems::PhysicsSystem(this, m_EventBroker); });
	m_SystemFactory.Register("RenderSystem", [this]() { return new Systems::RenderSystem(this, m_EventBroker, m_Renderer); });
}

void GameWorld::AddSystems()
{
	AddSystem("TransformSystem");
	//AddSystem("LevelGenerationSystem");
	AddSystem("InputSystem");
	AddSystem("DebugSystem");
	//AddSystem("CollisionSystem");
	////AddSystem("ParticleSystem");
	//AddSystem("PlayerSystem");
	AddSystem("FreeSteeringSystem");
	AddSystem("SoundSystem");
	AddSystem("PhysicsSystem");
	AddSystem("RenderSystem");
}

void GameWorld::BindKey(int keyCode, std::string command)
{
	Events::BindKey e;
	e.KeyCode = keyCode;
	e.Command = command;
	m_EventBroker->Publish(e);
}

void GameWorld::BindMouseButton(int button, std::string command)
{
	Events::BindMouseButton e;
	e.Button = button;
	e.Command = command;
	m_EventBroker->Publish(e);
}
