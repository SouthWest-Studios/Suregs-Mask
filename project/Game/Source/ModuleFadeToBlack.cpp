#include "ModuleFadeToBlack.h"
#include "Map.h"
#include "EntityManager.h"
#include "Scene_Logos.h"
#include "Hud.h"
#include "App.h"
#include "Menu.h"
#include "Window.h"
#include "Module.h"
#include "SDL/include/SDL_render.h"
#include "Log.h"
#include "Textures.h"

ModuleFadeToBlack::ModuleFadeToBlack(App* app, bool start_enabled) : Module(app, start_enabled)
{

	name = ("fadeToBlack");

	
}

ModuleFadeToBlack::~ModuleFadeToBlack()
{

}

bool ModuleFadeToBlack::Start()
{
	LOG("Preparing Fade Screen");

	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	config = configFile.child("config").child(name.GetString());

	transitionPath = config.child("transition").attribute("texturepath").as_string();
	transition = app->tex->Load(transitionPath);

	uint winW, winH;
	app->win->GetWindowSize(winW, winH);

	// Convertir winW y winH a int
	int screenWidth = static_cast<int>(winW * app->win->GetScale());
	int screenHeight = static_cast<int>(winH * app->win->GetScale());

	screenRect = { 0, 0, screenWidth, screenHeight };
	transitionRect = { 0, 0, screenWidth, screenHeight };

	currentStep = Fade_Step::NONE;

	activeScene = app->scene_logos;

	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool ModuleFadeToBlack::Update(float dt)
{
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;
	
	if (currentStep == Fade_Step::TO_BLACK)
	{
		app->menu->menuu = false;
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{

			// TODO 1: Enable / Disable the modules received when FadeToBlacks(...) gets called
			moduleToDisable->sceneReload = false;
			app->map->Disable();
			moduleToDisable->Disable();
			app->entityManager->Disable();

			pugi::xml_document configFile;
			pugi::xml_node config;
			pugi::xml_parse_result file = configFile.load_file("config.xml");

			config = configFile.child("config").child(moduleToEnable->name.GetString());

			moduleToEnable->sceneReload = true;

			moduleToEnable->Enable();
			moduleToEnable->Awake(config);

			if(moduleToEnable != (Module*)app->scene_intro && moduleToEnable != (Module*)app->scene_menu && moduleToEnable != (Module*)app->scene_gameover){
				/*app->hud->Enable();*/
				app->map->Enable();
				//app->menu->Enable();
				app->menu->active = true;
			}
			else
			{
				/*app->hud->Disable();*/
				/*app->menu->active = false;*/
			}
			/*app->entityManager->Enable();*/
			activeScene = moduleToEnable;

			currentStep = Fade_Step::FROM_BLACK;
		}
	}
	else
	{
		
		--frameCount;
		if (frameCount <= 0)
		{
			currentStep = Fade_Step::NONE;
		}
	}

	/*transitionRect.x += static_cast <int>(1.0f * dt);*/

	return true;
}

bool ModuleFadeToBlack::PostUpdate()
{
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;

	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screenRect);

	/*int transitionX = transitionRect.x;
	int transitionY = transitionRect.y;

	SDL_SetTextureAlphaMod(transition, fadeRatio * 255.0f);
	app->render->DrawTexture(transition, transitionX, transitionY, 1.0f, SDL_FLIP_NONE, nullptr);*/

	//app->render->DrawTexture(transition, 0, 0, 1, SDL_FLIP_NONE, &screenRect, fadeRatio * 1.0f);
	

	return true;
}

bool ModuleFadeToBlack::FadeToBlack(Module* moduleToDisable, Module* moduleToEnable, float frames)
{
	bool ret = false;

	// If we are already in a fade process, ignore this call
	if (currentStep == Fade_Step::NONE)
	{
		currentStep = Fade_Step::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;

		// TODO 1: How do we keep track of the modules received in this function?
		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;

		ret = true;
	}

	return ret;
}