#include "PrecompiledHeader.h"
#include "GameWorld.h"

void GameWorld::Initialize()
{
	World::Initialize();


	{
		auto camera = CreateEntity();
		auto transform = AddComponent<Components::Transform>(camera, "Transform");
		transform->Position.z = 70.f;
		transform->Position.y = 25.f;
		transform->Orientation = glm::quat(glm::vec3(glm::pi<float>() / 8.f, 0.f, 0.f));
		auto cameraComp = AddComponent<Components::Camera>(camera, "Camera");
		cameraComp->FarClip = 2000.f;
		AddComponent(camera, "Input");
		auto freeSteering = AddComponent<Components::FreeSteering>(camera, "FreeSteering");
	}


	{
		auto ground = CreateEntity();
		auto transform = AddComponent<Components::Transform>(ground, "Transform");
		transform->Position = glm::vec3(0, -5, 0);
		transform->Scale = glm::vec3(1000.0f, 10.0f, 1000.0f);
		transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		auto model = AddComponent<Components::Model>(ground, "Model");
		model->ModelFile = "Models/Placeholders/PhysicsTest/Cube.obj";
		auto box = AddComponent<Components::Box>(ground, "Box");
		box->Width = 500;
		box->Height = 5;
		box->Depth = 500;

		auto physics = AddComponent<Components::Physics>(ground, "Physics");
		physics->Mass = 10;
		physics->Static = true;
	}


	{
		auto car = CreateEntity();
		auto transform = AddComponent<Components::Transform>(car, "Transform");
		transform->Position = glm::vec3(0, 5, 0);
		transform->Scale = glm::vec3(3, 1, 5);
		transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		auto model = AddComponent<Components::Model>(car, "Model");
		model->ModelFile = "Models/Placeholders/PhysicsTest/Cube2.obj";
		auto physics = AddComponent<Components::Physics>(car, "Physics");
		physics->Mass = 1200;
		auto box = AddComponent<Components::Box>(car, "Box");
		box->Width = 1.5f;
		box->Height = 0.5f;
		box->Depth = 2.5f;
		auto vehicle = AddComponent<Components::Vehicle>(car, "Vehicle");
		AddComponent<Components::Input>(car, "Input");
	}

	/*for(int i = 0; i < 83; i++)
	{
		auto light = CreateEntity();
		auto transform = AddComponent<Components::Transform>(light, "Transform");
		transform->Position = glm::vec3((float)(2*i)*glm::sin((float)i), 3, (float)(2*i)*glm::cos((float)i));

		auto pointLight = AddComponent<Components::PointLight>(light, "PointLight");
		pointLight->Specular = glm::vec3(0.1f, 0.1f, 0.1f);
		pointLight->Diffuse = glm::vec3(0.05f, 0.36f, 1.f);
		pointLight->constantAttenuation = 0.03f;
		pointLight->linearAttenuation = 0.009f;
		pointLight->quadraticAttenuation = 0.07f;
		pointLight->spotExponent = 0.0f;

		auto model = AddComponent<Components::Model>(light, "Model");
		model->ModelFile = "Models/Placeholders/PhysicsTest/PointLight.obj";
	}*/

	for(int i = 0; i < 100; i++)
	{
		auto ball = CreateEntity();
		auto transform = AddComponent<Components::Transform>(ball, "Transform");
		transform->Position = glm::vec3(i/5.f, 5 + i*2, i/5.f);
		transform->Scale = glm::vec3(1.0f, 1.0f, 1.0f);
		transform->Orientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		auto model = AddComponent<Components::Model>(ball, "Model");
		model->ModelFile = "Models/Placeholders/PhysicsTest/Sphere.obj";
		auto sphere = AddComponent<Components::Sphere>(ball, "Sphere");
		sphere->Radius = 0.5;
		auto physics = AddComponent<Components::Physics>(ball, "Physics");
		physics->Mass = 1;

		auto pointLight = AddComponent<Components::PointLight>(ball, "PointLight");
		pointLight->Specular = glm::vec3(0.1f, 0.1f, 0.1f);
		pointLight->Diffuse = glm::vec3(0.05f, 0.36f, 1.f);
		/*pointLight->constantAttenuation = 0.13f;
		pointLight->linearAttenuation = 0.009f;
		pointLight->quadraticAttenuation = 0.007f;*/
		pointLight->constantAttenuation = 0.03f;
		pointLight->linearAttenuation = 0.119f;
		pointLight->quadraticAttenuation = 0.07f;
		pointLight->spotExponent = 0.0f;
	}

	/*{
		auto entity = CreateEntity();
		AddComponent(entity, "Transform");
		auto emitter = AddComponent<Components::SoundEmitter>(entity, "SoundEmitter");
		emitter->Path = "Sounds/korvring.wav";
		emitter->Loop = true;
		GetSystem<Systems::SoundSystem>("SoundSystem")->PlaySound(emitter);
	}*/
}

void GameWorld::Update(double dt)
{
	World::Update(dt);
}

void GameWorld::RegisterComponents()
{
	m_ComponentFactory.Register("Camera", []() { return new Components::Camera(); });
	m_ComponentFactory.Register("DirectionalLight", []() { return new Components::DirectionalLight(); });
	m_ComponentFactory.Register("Input", []() { return new Components::Input(); });
	m_ComponentFactory.Register("Model", []() { return new Components::Model(); });
	m_ComponentFactory.Register("ParticleEmitter", []() { return new Components::ParticleEmitter(); });
	m_ComponentFactory.Register("PointLight", []() { return new Components::PointLight(); });
	m_ComponentFactory.Register("SoundEmitter", []() { return new Components::SoundEmitter(); });
	m_ComponentFactory.Register("Sprite", []() { return new Components::Sprite(); });
	m_ComponentFactory.Register("Template", []() { return new Components::Template(); });
	m_ComponentFactory.Register("Transform", []() { return new Components::Transform(); });
	m_ComponentFactory.Register("FreeSteering", []() { return new Components::FreeSteering(); });

	m_ComponentFactory.Register("Physics", []() { return new Components::Physics(); });
	m_ComponentFactory.Register("Sphere", []() { return new Components::Sphere(); });
	m_ComponentFactory.Register("Box", []() { return new Components::Box (); });

	m_ComponentFactory.Register("Vehicle", []() { return new Components::Vehicle(); });
}

void GameWorld::RegisterSystems()
{
	m_SystemFactory.Register("TransformSystem", [this]() { return new Systems::TransformSystem(this); });
	//m_SystemFactory.Register("LevelGenerationSystem", [this]() { return new Systems::LevelGenerationSystem(this); });
	m_SystemFactory.Register("InputSystem", [this]() { return new Systems::InputSystem(this, m_Renderer); });
	//m_SystemFactory.Register("CollisionSystem", [this]() { return new Systems::CollisionSystem(this); });
	////m_SystemFactory.Register("ParticleSystem", [this]() { return new Systems::ParticleSystem(this); });
	//m_SystemFactory.Register("PlayerSystem", [this]() { return new Systems::PlayerSystem(this); });
	m_SystemFactory.Register("FreeSteeringSystem", [this]() { return new Systems::FreeSteeringSystem(this); });
	m_SystemFactory.Register("SoundSystem", [this]() { return new Systems::SoundSystem(this); });

	m_SystemFactory.Register("PhysicsSystem", [this]() { return new Systems::PhysicsSystem(this); });

	m_SystemFactory.Register("RenderSystem", [this]() { return new Systems::RenderSystem(this, m_Renderer); });


}

void GameWorld::AddSystems()
{
	AddSystem("TransformSystem");
	//AddSystem("LevelGenerationSystem");
	AddSystem("InputSystem");
	//AddSystem("CollisionSystem");
	////AddSystem("ParticleSystem");
	//AddSystem("PlayerSystem");
	AddSystem("FreeSteeringSystem");
	AddSystem("SoundSystem");

	AddSystem("PhysicsSystem");

	AddSystem("RenderSystem");


}