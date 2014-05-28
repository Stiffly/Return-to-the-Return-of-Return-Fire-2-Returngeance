#ifndef GUI_WorldFrame_h__
#define GUI_WorldFrame_h__

#include "GUI/Frame.h"
#include "GUI/Viewport.h"
#include "RenderQueue.h"
#include "World.h"
#include "Systems/TransformSystem.h"
#include "Events/SetViewportCamera.h"
#include "Components/Transform.h"
#include "Components/Model.h"
#include "Components/Sprite.h"
#include "Components/PointLight.h"
#include "Components/BlendMap.h"


namespace GUI
{

class WorldFrame : public Frame
{
public:
	WorldFrame(Frame* parent, std::string name, std::shared_ptr<World> world)
		: Frame(parent, name)
		, m_World(world)
	{
		EVENT_SUBSCRIBE_MEMBER(m_ESetViewportCamera, &WorldFrame::OnSetViewportCamera);
	}

	void Update(double dt) override
	{
		if (!m_TransformSystem)
			m_TransformSystem = m_World->GetSystem<Systems::TransformSystem>();

		m_World->Update(dt);
	}

	void Draw(std::shared_ptr<Renderer> renderer) override
	{
		RenderQueue.Clear();
		renderer->ClearPointLights();

		for (auto &pair : *m_World->GetEntities())
		{
			EntityID entity = pair.first;

			auto transform = m_World->GetComponent<Components::Transform>(entity);
			if (!transform)
				continue;

			auto modelComponent = m_World->GetComponent<Components::Model>(entity);
			if (modelComponent)
			{
				auto modelAsset = ResourceManager->Load<Model>("Model", modelComponent->ModelFile);
				if (modelAsset)
				{
					Components::Transform absoluteTransform = m_TransformSystem->AbsoluteTransform(entity);
					glm::mat4 modelMatrix = glm::translate(glm::mat4(), absoluteTransform.Position)
										    * glm::toMat4(absoluteTransform.Orientation)
											* glm::scale(absoluteTransform.Scale);
					auto blendmapComponent = m_World->GetComponent<Components::BlendMap>(entity);
					if (blendmapComponent)
					{
						auto textureRed = ResourceManager->Load<Texture>("Texture", blendmapComponent->TextureRed);
						auto textureGreen = ResourceManager->Load<Texture>("Texture", blendmapComponent->TextureGreen);
						auto textureBlue = ResourceManager->Load<Texture>("Texture", blendmapComponent->TextureBlue);
						float textureRepeat = blendmapComponent->TextureRepeats;
						EnqueueBlendMapModel(modelAsset, textureRed, textureGreen, textureBlue, textureRepeat, modelMatrix);
					}
					else
					{
						EnqueueModel(modelAsset, modelMatrix);
					}

				}
			}

			auto spriteComponent = m_World->GetComponent<Components::Sprite>(entity);
			if (spriteComponent)
			{
				auto textureAsset = ResourceManager->Load<Texture>("Texture", spriteComponent->SpriteFile);
				if (textureAsset)
				{
					Components::Transform absoluteTransform = m_TransformSystem->AbsoluteTransform(entity);
					glm::quat orientation2D = glm::angleAxis(glm::eulerAngles(absoluteTransform.Orientation).z, glm::vec3(0, 0, -1));
					glm::mat4 modelMatrix = glm::translate(absoluteTransform.Position)
										  * glm::toMat4(orientation2D)
										  * glm::scale(absoluteTransform.Scale);
					EnqueueSprite(textureAsset, modelMatrix);
				}
			}

			auto pointLightComponent = m_World->GetComponent<Components::PointLight>(entity);
			if (pointLightComponent)
			{
				glm::vec3 position = m_TransformSystem->AbsolutePosition(entity);
				renderer->AddPointLightToDraw(
					position,
					pointLightComponent->Specular,
					pointLightComponent->Diffuse,
					pointLightComponent->specularExponent,
					pointLightComponent->ConstantAttenuation,
					pointLightComponent->LinearAttenuation,
					pointLightComponent->QuadraticAttenuation,
					pointLightComponent->Radius
					);
			}
		}
	}

protected:
	std::shared_ptr<World> m_World;

private:
	EventRelay<Frame, Events::SetViewportCamera> m_ESetViewportCamera;
	bool OnSetViewportCamera(const Events::SetViewportCamera &event)
	{
		// Search next layer for viewports and update cameras
		auto itpair = m_Children[m_Layer + 1].equal_range(event.ViewportFrame);
		for (auto it = itpair.first; it != itpair.second; ++it)
		{
			auto viewportFrame = std::dynamic_pointer_cast<GUI::Viewport>(it->second);
			if (!viewportFrame)
				continue;

			viewportFrame->SetCameraEntity(event.CameraEntity);
		}

		return true;
	}

	std::shared_ptr<Systems::TransformSystem> m_TransformSystem;

	void EnqueueModel(Model* model, glm::mat4 modelMatrix)
	{
		for (auto texGroup : model->TextureGroups)
		{
			ModelJob job;
			job.TextureID = texGroup.Texture->ResourceID;
			job.DiffuseTexture = *texGroup.Texture;
			job.NormalTexture = (texGroup.NormalMap) ? *texGroup.NormalMap : 0;
			job.SpecularTexture = (texGroup.SpecularMap) ? *texGroup.SpecularMap : 0;
			job.VAO = model->VAO;
			job.StartIndex = texGroup.StartIndex;
			job.EndIndex = texGroup.EndIndex;
			job.ModelMatrix = modelMatrix;
					
			RenderQueue.Add(job);
		}
	}

	void EnqueueBlendMapModel(Model* model, Texture* textureRed, Texture* textureGreen,  Texture* textureBlue, float textureRepeat, glm::mat4 modelMatrix)
	{
		for (auto texGroup : model->TextureGroups)
		{
			BlendMapModelJob job;
			job.TextureID = texGroup.Texture->ResourceID;
			job.DiffuseTexture = *texGroup.Texture;
			job.NormalTexture = (texGroup.NormalMap) ? *texGroup.NormalMap : 0;
			job.SpecularTexture = (texGroup.SpecularMap) ? *texGroup.SpecularMap : 0;
			job.BlendMapTextureRed = (*textureRed);
			job.BlendMapTextureGreen = (*textureGreen);
			job.BlendMapTextureBlue = (*textureBlue);
			job.TextureRepeat = textureRepeat;
			job.VAO = model->VAO;
			job.StartIndex = texGroup.StartIndex;
			job.EndIndex = texGroup.EndIndex;
			job.ModelMatrix = modelMatrix;

			RenderQueue.Add(job);
		}
	}

	void EnqueueSprite(Texture* texture, glm::mat4 modelMatrix)
	{
		SpriteJob job;
		job.TextureID = texture->ResourceID;
		job.Texture = *texture;
		job.ModelMatrix = modelMatrix;

		RenderQueue.Add(job);
	}
};

}

#endif // GUI_WorldFrame_h__
