#include "ModuleFadeToBlack.h"
#include "Map.h"
#include "EntityManager.h"

#include "App.h"
#include "Window.h"
#include "Module.h"
#include "SDL/include/SDL_render.h"
#include "Log.h"

ModuleFadeToBlack::ModuleFadeToBlack(App* app, bool start_enabled) : Module(app, start_enabled)
{



	
}

ModuleFadeToBlack::~ModuleFadeToBlack()
{

}

bool ModuleFadeToBlack::Start()
{
	LOG("Preparing Fade Screen");


	uint winW, winH;
	app->win->GetWindowSize(winW, winH);

	// Convertir winW y winH a int
	int screenWidth = static_cast<int>(winW * app->win->GetScale());
	int screenHeight = static_cast<int>(winH * app->win->GetScale());

	screenRect = { 0, 0, screenWidth, screenHeight };

	currentStep = Fade_Step::NONE;

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
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{

			// TODO 1: Enable / Disable the modules received when FadeToBlacks(...) gets called
			moduleToDisable->sceneReload = false;
			moduleToDisable->Disable();
			app->map->Disable();
			app->entityManager->Disable();

			pugi::xml_document configFile;
			pugi::xml_node config;
			pugi::xml_parse_result file = configFile.load_file("config.xml");

			config = configFile.child("config").child(moduleToEnable->name.GetString());

			moduleToEnable->sceneReload = true;
			moduleToEnable->Awake(config);
			moduleToEnable->Enable();
			app->map->Enable();
			app->entityManager->Enable();

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