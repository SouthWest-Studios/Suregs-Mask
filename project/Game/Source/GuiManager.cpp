#include "GuiManager.h"
#include "App.h"
#include "Textures.h"
#include "GuiCheckBox.h"
#include "GuiControlButton.h"
#include "GuiControlSlider.h"
#include "Audio.h"
#include "Scene_Menu.h"
#include "Menu.h"

GuiManager::GuiManager(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	button_audio = app->audio->LoadAudioFx("button_fx");

	return true;
}

// L15: DONE1: Implement CreateGuiControl function that instantiates a new GUI control and add it to the list of controls
GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds,  Module* observer, SDL_Rect sliderBounds, SDL_Rect bounds2,int minValue , int maxValue)
{
	GuiControl* guiControl = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:
		guiControl = new GuiControlButton(id, bounds, text);
		break;
	case GuiControlType::CHECKBOX:
		guiControl = new GuiCheckBox(id, bounds, bounds2, text);

		break;
	case GuiControlType::SLIDER:
		guiControl = new GuiControlSlider(id, bounds, minValue, maxValue, text);

		break;
	}

	//Set the observer
	guiControl->observer = observer;

	// Created GuiControls are add it to the list of controls
	guiControlsList.Add(guiControl);

	return guiControl;
}

	




bool GuiManager::Update(float dt)
{	

	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		control->data->Update(dt);
		control = control->next;
	}
	GuiControl* control1 = nullptr;
	control1 = GetControlById(pointerId);
	if (control1 != nullptr) {
		control1->selected = true;
	}

	if (app->scene_menu->active || app->menu->ventana == 4) {
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
			NavigateUp();
			app->audio->PlayFx(button_audio);
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
			NavigateDown();
			app->audio->PlayFx(button_audio);
		}
		if (app->menu->music != nullptr)
		{
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN && app->menu->music->selected == false && app->menu->sfx->selected == false) {
				NavigateRight();
				app->audio->PlayFx(button_audio);
			}
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && app->menu->music->selected == false && app->menu->sfx->selected == false) {
				NavigateLeft();
				app->audio->PlayFx(button_audio);
			}
		}
		
	}

	return true;
}

bool GuiManager::PostUpdate()
{
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		control->data->PostUpdate();
		control = control->next;
	}
	return true;
}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		RELEASE(control);
	}


	return false;
}

void GuiManager::DestroyGuiControl(GuiControl* controlToDestroy)
{
	ListItem<GuiControl*>* control;
	for (control = guiControlsList.start; control != NULL; control = control->next)
	{
		if (control->data == controlToDestroy) {
			guiControlsList.Del(control);
			break;
		}
	}
}

void GuiManager::NavigateUp()
{
	GuiControl* control = nullptr;
	//Boton con id pointerId se deselecciona
	control = GetControlById(pointerId);
	if (control != nullptr) {
		control->selected = false;
	}
	pointerId--;
	if (pointerId < minId) {
		pointerId = maxId;
	}
	//Boton con id pointerId se selecciona
	control = GetControlById(pointerId);
	if (control != nullptr) {
		control->selected = true;
	}
}

void GuiManager::NavigateDown()
{
	GuiControl* control = nullptr;
	//Boton con id pointerId se deselecciona
	control = GetControlById(pointerId);
	if (control != nullptr) {
		control->selected = false;
	}
	pointerId++;
	if (pointerId > maxId) {
		pointerId = minId;
	}
	//Boton con id pointerId se selecciona
	control = GetControlById(pointerId);
	if (control != nullptr) {
		control->selected = true;
	}
}

void GuiManager::NavigateRight()
{
	GuiControl* control = nullptr;
	//Boton con id pointerId se deselecciona
	control = GetControlById(pointerId);
	if (control != nullptr) {
		control->selected = false;
	}
	pointerId+=columnSize;
	if (pointerId > maxId) {
		pointerId = minId;
	}
	//Boton con id pointerId se selecciona
	control = GetControlById(pointerId);
	if (control != nullptr) {
		control->selected = true;
	}
}

void GuiManager::NavigateLeft()
{
	GuiControl* control = nullptr;
	//Boton con id pointerId se deselecciona
	control = GetControlById(pointerId);
	if (control != nullptr) {
		control->selected = false;
	}
	pointerId -= columnSize;
	if (pointerId > maxId) {
		pointerId = minId;
	}
	//Boton con id pointerId se selecciona
	control = GetControlById(pointerId);
	if (control != nullptr) {
		control->selected = true;
	}
}

GuiControl* GuiManager::GetControlById(int id)
{
	ListItem<GuiControl*>* control;
	for (control = guiControlsList.start; control != NULL; control = control->next) {
		if (id == control->data->id) {
			return control->data;
		}
	}

	return nullptr;
}





