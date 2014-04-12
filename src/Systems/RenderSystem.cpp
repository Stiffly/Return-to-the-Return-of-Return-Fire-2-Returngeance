#include "PrecompiledHeader.h"
#include "RenderSystem.h"
#include "World.h"

void Systems::RenderSystem::OnComponentCreated(std::string type, std::shared_ptr<Component> component)
{
	if(type == "Model")
	{
		auto modelComponent = std::static_pointer_cast<Components::Model>(component);
	}
}

void Systems::RenderSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
{
	auto transformComponent = m_World->GetComponent<Components::Transform>(entity, "Transform");
	if (transformComponent == nullptr)
		return;

	// Draw models
	auto modelComponent = m_World->GetComponent<Components::Model>(entity, "Model");
	if (modelComponent != nullptr)
	{
		if (m_CachedModels.find(modelComponent->ModelFile) == m_CachedModels.end())
		{
			m_CachedModels[modelComponent->ModelFile] = std::make_shared<Model>(OBJ(modelComponent->ModelFile));
		}

		auto model = m_World->GetResourceManager()->Fetch<Model>(modelComponent->ModelFile);
		glm::vec3 position = m_TransformSystem->AbsolutePosition(entity);
		glm::quat orientation = m_TransformSystem->AbsoluteOrientation(entity);
		glm::vec3 scale = m_TransformSystem->AbsoluteScale(entity);
		m_Renderer->AddModelToDraw(model, position, orientation, scale, modelComponent->Visible, modelComponent->ShadowCaster);
	}

	auto pointLightComponent = m_World->GetComponent<Components::PointLight>(entity, "PointLight");
	if (pointLightComponent != nullptr)
	{
		glm::vec3 position = m_TransformSystem->AbsolutePosition(entity);
		m_Renderer->AddPointLightToDraw(
		    position,
		    pointLightComponent->Specular,
		    pointLightComponent->Diffuse,
		    pointLightComponent->constantAttenuation,
		    pointLightComponent->linearAttenuation,
		    pointLightComponent->quadraticAttenuation,
		    pointLightComponent->spotExponent);
	}

	auto cameraComponent = m_World->GetComponent<Components::Camera>(entity, "Camera");
	if (cameraComponent != nullptr)
	{
		m_Renderer->GetCamera()->Position(transformComponent->Position);
		m_Renderer->GetCamera()->Orientation(transformComponent->Orientation);

		m_Renderer->GetCamera()->FOV(cameraComponent->FOV);
		m_Renderer->GetCamera()->NearClip(cameraComponent->NearClip);
		m_Renderer->GetCamera()->FarClip(cameraComponent->FarClip);
	}
}

void Systems::RenderSystem::Initialize()
{
	m_TransformSystem = m_World->GetSystem<Systems::TransformSystem>("TransformSystem");
}

void Systems::RenderSystem::RegisterComponents(ComponentFactory* cf)
{
	cf->Register("Camera", []() { return new Components::Camera(); });
	cf->Register("Model", []() { return new Components::Model(); });
	cf->Register("Sprite", []() { return new Components::Sprite(); });
	cf->Register("PointLight", []() { return new Components::PointLight(); });
	cf->Register("DirectionalLight", []() { return new Components::DirectionalLight(); });
}

void Systems::RenderSystem::RegisterResourceTypes(ResourceManager* rm)
{
	rm->RegisterType("Texture", [](std::string resourceName) { return new Texture(resourceName); });
	rm->RegisterType("Model", [](std::string resourceName) { return new Model(OBJ(resourceName)); });
}





