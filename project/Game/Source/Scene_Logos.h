#ifndef __SCENE_LOGOS_H__
#define __SCENE_LOGOS_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "Entity.h"

struct SDL_Texture;

class Scene_Logos : public Module
{
public:

	Scene_Logos(App* app, bool start_enabled = true);

	// Destructor
	virtual ~Scene_Logos();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	enum EntityState;

	

private:
	SDL_Texture* sceneLogosTexture = nullptr;
	Animation SPosition;
	SDL_Rect* spritePositions = nullptr;
	pugi::xml_node config;
	int TSprite;
	int SpriteX;
	int SpriteY;
	int Photowidth;
	Animation* currentAnimation = nullptr;

	Animation sceneLogos;
	Timer timerIntro;

	int intro_fx;
};

#endif // __SCENE_LOGOS_H__