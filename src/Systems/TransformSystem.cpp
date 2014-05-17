#include "PrecompiledHeader.h"
#include "TransformSystem.h"
#include "World.h"

//void Systems::TransformSystem::UpdateEntity(double dt, EntityID entity, EntityID parent)
//{
//	if (parent == 0)
//		return;
//
//	auto transform = m_World->GetComponent<Components::Transform>(entity);
//	auto parentTransform = m_World->GetComponent<Components::Transform>(parent);
//
//	transform->Position = parentTransform->Position + transform->RelativePosition;
//}

glm::vec3 Systems::TransformSystem::AbsolutePosition(EntityID entity)
{
	glm::vec3 absPosition;
	glm::quat accumulativeOrientation;

	do
	{
		auto transform = m_World->GetComponent<Components::Transform>(entity);
		//absPosition += transform->Position;
		entity = m_World->GetEntityParent(entity);
		auto transform2 = m_World->GetComponent<Components::Transform>(entity);
		if (entity == 0)
			absPosition += transform->Position;
		else
			absPosition = transform2->Orientation * (absPosition + transform->Position);
	} while (entity != 0);

	return absPosition * accumulativeOrientation;
}

glm::quat Systems::TransformSystem::AbsoluteOrientation(EntityID entity)
{
	glm::quat absOrientation;

	do
	{
		auto transform = m_World->GetComponent<Components::Transform>(entity);
		absOrientation = transform->Orientation * absOrientation;
		entity = m_World->GetEntityParent(entity);
	} while (entity != 0);

	return absOrientation;
}

glm::vec3 Systems::TransformSystem::AbsoluteScale(EntityID entity)
{
	glm::vec3 absScale(1);

	do
	{
		auto transform = m_World->GetComponent<Components::Transform>(entity);
		absScale *= transform->Scale;
		entity = m_World->GetEntityParent(entity);
	} while (entity != 0);

	return absScale;
}

Components::Transform Systems::TransformSystem::AbsoluteTransform(EntityID entity)
{
	glm::vec3 absPosition;
	glm::quat absOrientation;
	glm::vec3 absScale(1);

	do
	{
		auto transform = m_World->GetComponent<Components::Transform>(entity);
		entity = m_World->GetEntityParent(entity);
		auto transform2 = m_World->GetComponent<Components::Transform>(entity);

		// Position
		if (entity == 0)
			absPosition += transform->Position;
		else
			absPosition = transform2->Orientation * (absPosition + transform->Position);
		// Orientation
		absOrientation = transform->Orientation * absOrientation;
		// Scale
		absScale *= transform->Scale;
	} while (entity != 0);

	Components::Transform transform;
	transform.Position = absPosition;
	transform.Orientation = absOrientation;
	transform.Scale = absScale;

	return transform;
}
