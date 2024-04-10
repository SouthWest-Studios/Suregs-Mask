#include "GuiControlButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"

GuiControlButton::GuiControlButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;
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
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		//If the position of the mouse if inside the bounds of the button 
		if (selected) {

			state = GuiControlState::FOCUSED;

			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
				state = GuiControlState::PRESSED;
			}

			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
				NotifyObserver();
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
			app->render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
			break;
		case GuiControlState::FOCUSED:
			app->render->DrawRectangle(bounds, 0, 0, 20, 255, true, false);
			break;
		case GuiControlState::PRESSED:
			app->render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
			break;
		}

		app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h);

	}

	return false;
}

void GuiControlButton::NavigateUp(GuiControl* control)
{
	control->selected = false;
	pointerId--;
}




