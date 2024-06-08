#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiCheckBox.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "InventoryManager.h"
#include "NotesManager.h"
#include "BestiarioManager.h"
#include "Menu.h"
#include "Scene_Menu.h"
#include "Scene_Testing.h"
#include "ModuleFadeToBlack.h"
#include "Menu_Equipo.h"
#include "ElevatorMenu.h"
#include "Scene_Pueblo.h"
#include "Hud.h"
#include "Scene_Pueblo_Tutorial.h"
#include "Scene_Logos.h"
#include "Scene_intro.h"

#include "SDL_mixer/include/SDL_mixer.h"

Menu::Menu(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name = ("menu");
}

// Destructor
Menu::~Menu()
{}

// Called before render is available
bool Menu::Awake(pugi::xml_node config)
{
	LOG("menu");
	bool ret = true;

	

	return ret;
}

// Called before the first frame
bool Menu::Start()
{
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	/*fondoInventario = app->tex->Load(config.child("texture1").attribute("texturePath1").as_string());
	fondoEquipo = app->tex->Load(config.child("texture2").attribute("texturePath2").as_string());
	fondoDiario = app->tex->Load(config.attribute("texturePath3").as_string());
	fondoAjustes = app->tex->Load(config.attribute("texturePath4").as_string());*/
	fondoInventario = app->tex->Load("Assets/Textures/Interfaz/libro01.png");
	fondoEquipo = app->tex->Load("Assets/Textures/Interfaz/Equipo.png");
	fondoDiario = app->tex->Load("Assets/Textures/Interfaz/Diario/Diario.png");
	fondoAjustes = app->tex->Load("Assets/Textures/Interfaz/Ajustes.png"); 
	fondoAjustesMando = app->tex->Load("Assets/Textures/Interfaz/controles_mando.png");

	change_inventory_fx = app->audio->LoadAudioFx("change_inventory_fx");
	inventory_fx = app->audio->LoadAudioFx("inventory_fx");

	return true;
}

// Called each loop iteration
bool Menu::PreUpdate()
{
	
	if (a == 0)
	{
		Start();
		a++;
	}
	return true;
}

// Called each loop iteration
bool Menu::Update(float dt)
{
	if (app->input->GetButton(PAUSE) == KEY_DOWN && !app->scene_logos->active && !app->scene_intro->active && !app->scene_menu->active )
	{
		app->guiManager->minId = 100;
		app->guiManager->maxId = 104;
		app->guiManager->pointerId = 100;

		/*app->guiManager->columnSize = 2;*/
		
		if (menuu) {
			// Si el menú está abierto, inicia la animación de salida
			StartExitAnimation();
		}
		else {
			// Si el menú está cerrado, alterna entre abrir y cerrar el menú
			ToggleMenu();
		}

		app->audio->StopFx(-1);
		app->audio->PlayFx(inventory_fx);
	}

	if (app->input->GetButton(APP_EXIT) == KEY_DOWN && app->notesManager->zoomIn == false && app->bestiarioManager->zoomIn == false && app->ascensor->abierto == false)
	{
		if (!menuu)
		{
			ventana = 4;
		}
		
		if (menuu) {
			StartExitAnimation();
		}
		app->audio->StopFx(-1);
		app->audio->PlayFx(inventory_fx);
	}
	if (menuu)
	{/*
		app->hud->active = false;*/
		//app->entityManager->active = false;
		app->physics->active = false;
		app->hud->active = false;
		if (ventana == 0)
		{
			ventana++;
		}
		
		if (app->input->GetButton(CAMBIAR_PESTANA_RIGHT) == KEY_DOWN && !app->scene_pueblo->GetRod()->fishing.rodReady  && !app->scene_pueblo_tutorial->GetRod()->fishing.rodReady)
		{
			if (ventana == 5)
			{
				ventana = 1;
			}
			else
			{
				ventana++;
			}

			app->audio->PlayFx(change_inventory_fx);
		}
		if (app->input->GetButton(CAMBIAR_PESTANA_LEFT) == KEY_DOWN)
		{
			if (ventana == 1)
			{
				ventana = 5;
			}
			else
			{
				ventana--;
			}

			app->audio->PlayFx(change_inventory_fx);
		}

	}
	else {
		app->entityManager->active = true;
		app->physics->active = true;
		app->hud->Enable();
		ventana = 0;
	}
	
	

	

	uint windowWidth, windowHeight;
	app->win->GetWindowSize(windowWidth, windowHeight);

	if (ventana == 4)
	{

		contadormenu++;
		if (title != nullptr )
		{
			
			title->state = GuiControlState::NORMAL;
			fullScreen->state = GuiControlState::NORMAL;
			vsync->state = GuiControlState::NORMAL;
			music->state = GuiControlState::FOCUSED;
			sfx->state = GuiControlState::NORMAL;
		}
		
		else
		{
			
			SDL_Rect MusicPos = { windowWidth / 2 - 390 ,windowHeight / 2 - 94, 230, 50 };
			music = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 100, "MUSIC", MusicPos, this, { 0, 0, 20, 20 }, { 0,0,0,0 },0 ,100);

			SDL_Rect SfxPos = { windowWidth / 2 - 390 ,windowHeight / 2 + 20, 230, 50 };
			sfx = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 101, "SFX", SfxPos, this, { 0, 0, 20, 20 }, { 0,0,0,0 }, 0, 100);
	
			SDL_Rect FullScreen = { windowWidth / 2 - 300 ,windowHeight / 2 + 134, 230,50 };
			fullScreen = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 102, "FULLSCREEN", FullScreen, this, { 0,0,0,0 }, { -50,0,0,0 });
			
			SDL_Rect vSyncpos = { windowWidth / 2 -167 ,windowHeight / 2 + 133, 200, 50 };
			vsync = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 103, "VSYNC", vSyncpos, this, { 0, 0, 20, 20 } );

			SDL_Rect TitlePos;
		
			
			if (!app->scene_pueblo->active)
			{
				TitlePos = { (int)windowWidth / 2 - 330 ,(int)windowHeight / 2 + 170, 230,30 };

			}
			else
			{
				TitlePos = { (int)windowWidth / 2 - 330 ,(int)windowHeight / 2 + 200, 230,30 };
			}
			
			title = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 104, "VOLVER AL MENU", TitlePos, this, {0,0,0,0});

			if (app->win->fullscreen)
			{
				fullScreen->click = true;
				fullScreenActive = true;
			}
			else
			{
				fullScreen->click = false;
				fullScreenActive = false;
			}

			if (vsyncActive)
			{
				vsync->click = true;
				vsyncActive = true;
			}
			else
			{
				vsync->click = false;
				vsyncActive = false;
			}

			//Cargar la barra de audio del save_game
			((GuiControlSlider*)music)->value = app->audio->volumeMusic;
			((GuiControlSlider*)sfx)->value = app->audio->volumeFx;
		}

		



	}
	else
	{
		

			if (title != nullptr)
			{
				app->guiManager->DestroyGuiControl(title);
				title = nullptr;
				/*title->state = GuiControlState::DISABLED;
				title = nullptr;*/
				app->guiManager->DestroyGuiControl(fullScreen);
				/*fullScreen->state = GuiControlState::DISABLED;
				fullScreen = nullptr;*/
				app->guiManager->DestroyGuiControl(vsync);
				/*vsync->state = GuiControlState::DISABLED;
				vsync = nullptr;*/
				app->guiManager->DestroyGuiControl(music);
				/*music->state = GuiControlState::DISABLED;
				music = nullptr;*/
				app->guiManager->DestroyGuiControl(sfx);
				/*sfx->state = GuiControlState::DISABLED;
				sfx = nullptr;*/

				contadormenu = 0;
			}

		
	}
	if (ventana == 4)
	{

		if (music != nullptr)
		{
			int newVolume = ((GuiControlSlider*)music)->value;
			// Asegurarse de que el nuevo volumen esta dentro de los l�mites v�lidos (0-128 para SDL Mixer)
			newVolume = std::max(0, std::min(128, newVolume));
			// Establecer el volumen de la musica
			Mix_VolumeMusic(newVolume);
			app->audio->volumeMusic = newVolume;
			int a = 0;
			musicNum = newVolume;
			audio = true;
		}

		if (sfx != nullptr)
		{
			int newSFXVolume = ((GuiControlSlider*)sfx)->value;
			newSFXVolume = std::max(0, std::min(128, newSFXVolume));

			// Iterar sobre cada canal activo y ajustar su volumen
			for (const auto& pair : app->audio->activeChannels)
			{
				int channel = pair.second; // Obtener el canal asociado al ID del efecto de sonido
				Mix_Volume(channel, newSFXVolume);
			}

			app->audio->volumeFx = newSFXVolume;
			sfxNum = newSFXVolume;
		}

		

		if (fullScreen->selected && (app->input->GetButton(SELECT) == KEY_DOWN && !app->scene_pueblo->GetRod()->fishing.rodReady  && !app->scene_pueblo_tutorial->GetRod()->fishing.rodReady ))
		{
			fullScreen->click = !fullScreen->click;
		}
		if (vsync->selected && (app->input->GetButton(SELECT) == KEY_DOWN && !app->scene_pueblo->GetRod()->fishing.rodReady  && !app->scene_pueblo_tutorial->GetRod()->fishing.rodReady ))
		{
			vsync->click = !vsync->click;
		}
		if (title->selected && (app->input->GetButton(SELECT) == KEY_DOWN && !app->scene_pueblo->GetRod()->fishing.rodReady  && !app->scene_pueblo_tutorial->GetRod()->fishing.rodReady ))
		{
			title->click = true;
		}
	}
	
	if (fullScreen != nullptr && fullScreen->click == true && fullScreenActive == false)
	{
		fullScreenActive = true;
		app->win->ToggleFullscreen();
	}

	if (fullScreen != nullptr && fullScreen->click == false && fullScreenActive == true)
	{
		fullScreenActive = false;
		app->win->ToggleFullscreen();
	}

	if (vsync != nullptr && vsync->click == true && vsyncActive == false)
	{
		vsyncActive = true;
	}

	if (vsync != nullptr && vsync->click == false && vsyncActive == true)
	{
		vsyncActive = false;
	}
	if (title != nullptr && title->click == true)
	{
		/*app->scene_menu->active = true;*/
		menuu = false;
		/*app->guiManager->active = false;*/
		/*app->scene_testing->active = false;*/
		if (app->scene_pueblo->active)
		{
			app->SaveRequest();
			app->hud->playSaveIcon = true;
		}

		app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_menu);
		
		title->click = false;
		if (title != nullptr)
		{
			app->guiManager->DestroyGuiControl(title);
			title = nullptr;
			/*title->state = GuiControlState::DISABLED;
			title = nullptr;*/
			app->guiManager->DestroyGuiControl(fullScreen);
			/*fullScreen->state = GuiControlState::DISABLED;
			fullScreen = nullptr;*/
			app->guiManager->DestroyGuiControl(vsync);
			/*vsync->state = GuiControlState::DISABLED;
			vsync = nullptr;*/
			app->guiManager->DestroyGuiControl(music);
			/*music->state = GuiControlState::DISABLED;
			music = nullptr;*/
			app->guiManager->DestroyGuiControl(sfx);
			/*sfx->state = GuiControlState::DISABLED;
			sfx = nullptr;*/

			contadormenu = 0;
		}
		/*if (title != nullptr)
		{

			title->state = GuiControlState::DISABLED;
			title = nullptr;
			fullScreen->state = GuiControlState::DISABLED;
			fullScreen = nullptr;
			vsync->state = GuiControlState::DISABLED;
			vsync = nullptr;
			music->state = GuiControlState::DISABLED;
			music = nullptr;
			sfx->state = GuiControlState::DISABLED;
			sfx = nullptr;

			contadormenu = 0;
		}*/

	}

	if (ventana == 1)
	{
		app->inventoryManager->mostrar = true;
	}
	else
	{
		app->inventoryManager->mostrar = false;
	}
	if (ventana == 2)
	{
		app->menuEquipo->mostrar = true;
	}
	else
	{
		app->menuEquipo->mostrar = false;
	}

	if (ventana == 3)
	{
		app->notesManager->mostrar = true;
		app->bestiarioManager->mostrar = true;
	}
	else
	{
		app->notesManager->mostrar = false;
		app->bestiarioManager->mostrar = false;
	}
	return true;
}





// Función de easing (easeOutBounce ya definida)



bool Menu::PostUpdate()
{
	uint windowWidth, windowHeight;
	app->win->GetWindowSize(windowWidth, windowHeight);

	// Inicializar currentY a una posición predeterminada
	int currentY = windowHeight / 8 - 71;

	if (menuu || animating || animatingExit)
	{
		if (animating) {
			animationTime += app->dt;

			float progress = animationTime / 400.0f; // Duración de la animación de 1 segundo (1000 ms)
			if (progress >= 1.0f) {
				progress = 1.0f;
				animating = false;
			}
			float easedProgress = easeOutCubic(progress);

			// Calcular la nueva posición Y usando easedProgress para la entrada
			int startY = windowHeight; // Comienza desde fuera de la pantalla (parte inferior)
			int endY = windowHeight / 8 - 71; // Posición final
			currentY = startY + (endY - startY) * easedProgress;
		}
		else if (animatingExit) {
			animationTime += app->dt;
			animatingExit2 = true;
			float progress = animationTime / 400.0f; // Duración de la animación de 1 segundo (1000 ms)
			if (progress >= 1.0f) {
				progress = 1.0f;
				animatingExit = false;
				menuu = false; // Ocultar el menú al finalizar la animación de salida
			}
			float easedProgress = easeOutCubic(progress);

			// Calcular la nueva posición Y usando easedProgress para la salida
			int startY = windowHeight / 8 - 71; // Posición inicial
			int endY = windowHeight; // Posición final fuera de la pantalla
			currentY = startY + (endY - startY) * easedProgress;
		}

		// Crear y renderizar la textura semitransparente
		Uint8 alpha = 188;  // Valor de transparencia (0-255)
		SDL_Texture* transparentTexture = CreateTransparentTexture(app->render->renderer, windowWidth, windowHeight, alpha);
		if (transparentTexture != nullptr) {
			SDL_SetTextureBlendMode(transparentTexture, SDL_BLENDMODE_BLEND);
			SDL_RenderCopy(app->render->renderer, transparentTexture, nullptr, nullptr);
			SDL_DestroyTexture(transparentTexture);
		}

		// Dibujar las texturas del inventario en la posición calculada o final
		if (ventana == 1) {
			app->render->DrawTexture(fondoInventario, windowWidth / 8 + 40, currentY, SDL_FLIP_NONE, 0, 0);
		}
		if (ventana == 2) {
			app->render->DrawTexture(fondoEquipo, windowWidth / 8 + 40, currentY+1, SDL_FLIP_NONE, 0, 0);
		}
		if (ventana == 3) {
			app->render->DrawTexture(fondoDiario, windowWidth / 8 + 40, currentY+1, SDL_FLIP_NONE, 0, 0);
		}
		if (ventana == 4) {
			app->render->DrawTexture(fondoAjustes, windowWidth / 8 + 40, currentY+2, SDL_FLIP_NONE, 0, 0);
			if (app->scene_pueblo->active)
			{
				SDL_Rect TitlePoss = { (int)windowWidth / 2 - 290 ,(int)windowHeight / 2 + 165, 140,30 };
				if (app->guiManager->pointerId == 104)
				{
					app->render->DrawText("GUARDAR Y", TitlePoss.x, TitlePoss.y, TitlePoss.w, TitlePoss.h, 0, 50, 35, 130);
				}
				else
				{
					app->render->DrawText("GUARDAR Y", TitlePoss.x, TitlePoss.y, TitlePoss.w, TitlePoss.h, 0, 25, 0, 70);
				}
				
			}
		}
		if (ventana == 5) {
			app->render->DrawTexture(fondoAjustesMando, windowWidth / 8 + 40, currentY+2, SDL_FLIP_NONE, 0, 0);
		}
		if (!animatingExit)
		{
			animatingExit2 = false;
		}
	}

	bool ret = true;
	return ret;
}

//SDL_Texture* CreateGradientTexturee(SDL_Renderer* renderer, int width, int height) {
//	SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
//	if (surface == nullptr) {
//		/*std::cerr << "Error creating surface: " << SDL_GetError() << std::endl;*/
//		return nullptr;
//	}
//
//	// Lock the surface to directly manipulate the pixels
//	SDL_LockSurface(surface);
//
//	Uint32* pixels = (Uint32*)surface->pixels;
//	for (int y = 0; y < height; ++y) {
//		// Calculate the alpha value for this row
//		Uint8 alpha = (Uint8)((float)y / height * 255);
//
//		for (int x = 0; x < width; ++x) {
//			pixels[y * width + x] = SDL_MapRGBA(surface->format, 0, 0, 0, alpha);
//		}
//	}
//
//	SDL_UnlockSurface(surface);
//
//	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
//	if (texture == nullptr) {
//		/*std::cerr << "Error creating texture: " << SDL_GetError() << std::endl;*/
//	}
//
//	SDL_FreeSurface(surface);
//	return texture;
//}
// Called before quitting
bool Menu::CleanUp()
{
	app->tex->UnLoad(fondoInventario); 
	app->tex->UnLoad(fondoEquipo);
	app->tex->UnLoad(fondoDiario);
	app->tex->UnLoad(fondoAjustes);
	app->tex->UnLoad(fondoAjustesMando);
	LOG("Freeing Scene_Intro");

	return true;
}

bool Menu::OnGuiMouseClickEvent(GuiControl* control)
{
	return false;
}

void Menu::SettingsInterface()
{
}

void Menu::ShowSettingsInterface()
{
}

void Menu::ShowCredits()
{
}

void Menu::DestroySettingsInterface()
{
}

void Menu::Fullscreen()
{
}

bool Menu::LoadState(pugi::xml_node node)
{

	return true;
}

bool Menu::SaveState(pugi::xml_node node)
{

	return true;
}

SDL_Texture* Menu::CreateTransparentTexture(SDL_Renderer* renderer, int width, int height, Uint8 alpha)
{
	SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	if (surface == nullptr) {
		/*std::cerr << "Error creating surface: " << SDL_GetError() << std::endl;*/
		return nullptr;
	}

	// Lock the surface to directly manipulate the pixels
	SDL_LockSurface(surface);

	Uint32* pixels = (Uint32*)surface->pixels;
	Uint32 color = SDL_MapRGBA(surface->format, 0, 0, 0, alpha);  // Black with the specified alpha

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			pixels[y * width + x] = color;
		}
	}

	SDL_UnlockSurface(surface);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == nullptr) {
		/*std::cerr << "Error creating texture: " << SDL_GetError() << std::endl;*/
	}

	SDL_FreeSurface(surface);
	return texture;
}

void Menu::StartAnimation()
{
	animationTime = 0.0f;
	animating = true;
	animatingExit = false;
}

void Menu::StartExitAnimation()
{
	animationTime = 0.0f;
	animating = false;
	animatingExit = true;
}

inline float Menu::easeOutBounce(float t)
{
	if (t < 1 / 2.75) {
		return 7.5625 * t * t;
	}
	else if (t < 2 / 2.75) {
		t -= 1.5 / 2.75;
		return 7.5625 * t * t + 0.75;
	}
	else if (t < 2.5 / 2.75) {
		t -= 2.25 / 2.75;
		return 7.5625 * t * t + 0.9375;
	}
	else {
		t -= 2.625 / 2.75;
		return 7.5625 * t * t + 0.984375;
	}
}

inline float Menu::easeOutCubic(float t)
{
	t--;
	return t * t * t + 1;
}

void Menu::ToggleMenu()
{
	if (menuu) {
		StartExitAnimation();
	}
	else {
		StartAnimation();
	}
	menuu = !menuu;
}


