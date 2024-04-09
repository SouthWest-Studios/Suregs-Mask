#include "GuiManager.h"
#include "App.h"
#include "Textures.h"
#include "GuiCheckBox.h"
#include "GuiControlButton.h"
#include "Audio.h"

GuiManager::GuiManager(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	return true;
}

// L15: DONE1: Implement CreateGuiControl function that instantiates a new GUI control and add it to the list of controls
GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds,  Module* observer, SDL_Rect sliderBounds, SDL_Rect bounds2)
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



