#ifndef RenderQueue_h__
#define RenderQueue_h__

#include <cstdint>
#include <forward_list>

#include "ResourceManager.h"
#include "Texture.h"
#include "Model.h"

class RenderQueue;

struct RenderJob
{
	friend class RenderQueue;

protected:
	uint64_t Hash;

	virtual void CalculateHash() = 0;

	bool operator<(const RenderJob& rhs)
	{
		return this->Hash < rhs.Hash;
	}
};

struct ModelJob : RenderJob
{
	unsigned int TextureID;
	GLuint DiffuseTexture;
	GLuint NormalTexture;
	GLuint SpecularTexture;
	GLuint VAO;
	unsigned int StartIndex;
	unsigned int EndIndex;
	glm::mat4 ModelMatrix;

	void CalculateHash() override
	{
		Hash = TextureID;
	}
};

struct SpriteJob : RenderJob
{
	unsigned int TextureID;
	GLuint Texture;
	glm::mat4 ModelMatrix;

	void CalculateHash() override
	{
		Hash = TextureID;
	}
};

class RenderQueue
{
public:
	template <typename T>
	void Add(T &job)
	{
		job.CalculateHash();
		m_Jobs.push_front(std::shared_ptr<T>(new T(job)));
		m_Jobs.sort();
	}

	void Clear()
	{
		m_Jobs.clear();
	}

private:
	std::forward_list<std::shared_ptr<RenderJob>> m_Jobs;
};

#endif // RenderQueue_h__
