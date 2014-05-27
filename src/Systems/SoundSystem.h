#ifndef SoundEmitter_h__
#define SoundEmitter_h__
#include <AL/al.h>
#include <AL/alc.h>
#include <vector>

#include "System.h"
#include "Components/Transform.h"
#include "Components/SoundEmitter.h"
#include "Events/PlaySound.h"
#include "Sound.h"

namespace Systems
{

class SoundSystem : public System
{
public:
	SoundSystem(World* world, std::shared_ptr<::EventBroker> eventBroker, std::shared_ptr<::ResourceManager> resourceManager)
		: System(world, eventBroker, resourceManager)
	{ }

	void RegisterComponents(ComponentFactory* cf) override;
	void RegisterResourceTypes(std::shared_ptr<::ResourceManager> rm) override;
	void Initialize() override;

	void Update(double dt) override;
	void UpdateEntity(double dt, EntityID entity, EntityID parent) override;
	void OnComponentCreated(std::string type, std::shared_ptr<Component> component) override;
	void OnComponentRemoved(EntityID entity, std::string type, Component* component) override;
	void PlaySound(Components::SoundEmitter* emitter, std::string path); // Use if you want to play a temporary .wav file not from component
	void PlaySound(std::shared_ptr<Components::SoundEmitter> emitter); // Use if you want to play .wav file from component // imon no hate plx T.T
	void StopSound(std::shared_ptr<Components::SoundEmitter> emitter);

private:
	ALuint LoadFile(std::string fileName);
	ALuint CreateSource();

	//File-info
	//char type[4];
	//unsigned long size, chunkSize;
	//short formatType, channels;
	//unsigned long sampleRate, avgBytesPerSec;
	//short bytesPerSample, bitsPerSample;
	//unsigned long dataSize;

	// Events
	EventRelay<SoundSystem, Events::PlaySound> m_EPlaySound;
	bool OnPlaySound(const Events::PlaySound &event);

	std::map<Component*, ALuint> m_Sources;
	std::map<std::string, ALuint> m_BufferCache; // string = fileName
};

}
#endif // !SoundEmitter_h__

