#include "GuiControlButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "Log.h"

GuiControlButton::GuiControlButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	select_audio = app->audio->LoadAudioFx("select_fx");
}

GuiControlButton::~GuiControlButton()
{
	button = app->tex->Load("Assets/Textures/Interfaz/textura_boton.png");

}

bool GuiControlButton::Start()
{
	if (button == nullptr) {
		LOG("Error con las texturas de los botones");
		return false;
	}
	return false;
}

bool GuiControlButton::Update(float dt)
{
	


	return false;
}

bool GuiControlButton::PostUpdate()
{
	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		//If the position of the mouse if inside the bounds of the button 
		if (selected) {

			state = GuiControlState::FOCUSED;

			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
				state = GuiControlState::PRESSED;
				
			}

			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
				NotifyObserver();
				click = true;

				app->audio->PlayFx(select_audio);
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
			app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h, app->render->buttonFont, 25, 0 , 70);
			break;
		case GuiControlState::FOCUSED:
			
			app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h, app->render->buttonFont, 204, 204, 255);
			break;
		case GuiControlState::PRESSED:
			
			app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h, app->render->buttonFont, 25, 0, 70);
			break;
		}

		

	}
	

	return false;
}



