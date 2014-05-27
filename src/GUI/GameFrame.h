#ifndef GUI_GameFrame_h__
#define GUI_GameFrame_h__

#include "GUI/Frame.h"
#include "GUI/WorldFrame.h"
#include "GUI/Viewport.h"
#include "GUI/TextureFrame.h"

#include "GameWorld.h"

namespace GUI
{

class GameFrame : public Frame
{
public:
	GameFrame(Frame* parent, std::string name)
		: Frame(parent, name)
	{
		m_World = std::make_shared<GameWorld>(EventBroker, ResourceManager);
		auto worldFrame = new WorldFrame(this, "GameWorldFrame", m_World);
		{
			vp1 = new Viewport(worldFrame, "Viewport1", m_World);
			vp1->X = 0;
			vp1->Width = 640;
			vp2 = new Viewport(worldFrame, "Viewport2", m_World);
			vp2->X = vp1->Right();
			vp2->Width = 640;

			auto tex = new TextureFrame(vp1, "TextureFrameThingy");
			tex->SetTexture("Textures/GUI/hurt.png");
		}
		m_World->Initialize();
	}

private:
	std::shared_ptr<GameWorld> m_World;
	Viewport* vp1;
	Viewport* vp2;
};

}

#endif // GUI_GameFrame_h__
