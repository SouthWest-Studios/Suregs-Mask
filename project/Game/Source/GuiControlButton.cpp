#include "GuiControlButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "GuiManager.h"
#include "Menu.h"
#include "Scene_Pueblo.h"
#include "Scene_Pueblo_Tutorial.h"

GuiControlButton::GuiControlButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	select_fx = app->audio->LoadAudioFx("select_fx");
}

GuiControlButton::~GuiControlButton()
{
	

}



bool GuiControlButton::Update(float dt)
{
	


	return false;
}

bool GuiControlButton::PostUpdate()
{
	if (state != GuiControlState::DISABLED)
	{
		if (app->menu->animating == false && app->menu->animatingExit2 == false)
		{
			// L15: DONE 3: Update the state of the GUiButton according to the mouse position
			app->input->GetMousePosition(mouseX, mouseY);

			//If the position of the mouse if inside the bounds of the button 
			if (selected) {

				state = GuiControlState::FOCUSED;

				if ((app->input->GetButton(CONFIRM) == KEY_DOWN || app->input->GetButton(SELECT) == KEY_DOWN && !app->scene_pueblo->GetRod()->fishing.rodReady  && !app->scene_pueblo_tutorial->GetRod()->fishing.rodReady ) && !app->scene_pueblo->GetRod()->fishing.rodReady  && !app->scene_pueblo_tutorial->GetRod()->fishing.rodReady) {
					state = GuiControlState::PRESSED;

				}

				if ((app->input->GetButton(CONFIRM) == KEY_DOWN || app->input->GetButton(SELECT) == KEY_DOWN && !app->scene_pueblo->GetRod()->fishing.rodReady  && !app->scene_pueblo_tutorial->GetRod()->fishing.rodReady ) && !app->scene_pueblo->GetRod()->fishing.rodReady  && !app->scene_pueblo_tutorial->GetRod()->fishing.rodReady) {
					NotifyObserver();
					click = true;

					app->audio->PlayFx(select_fx);
				}
			}
			else {
				state = GuiControlState::NORMAL;
			}

			//L15: DONE 4: Draw the button according the GuiControl State
			switch (state)
			{
			case GuiControlState::DISABLED:
				app->render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);
				break;
			case GuiControlState::NORMAL:
				app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h, app->render->buttonFont, 25, 0, 70);
				break;
			case GuiControlState::FOCUSED:
				app->render->DrawTexture(app->guiManager->button, bounds.x - 30, bounds.y, 1);
				app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h, app->render->buttonFont, 50, 35, 130);
				break;
			case GuiControlState::PRESSED:
				app->render->DrawTexture(app->guiManager->button, bounds.x - 25, bounds.y + 5, 0.7f);
				app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h, app->render->buttonFont, 25, 0, 70);
				break;
			}

		}

	}
	

	return false;
}



