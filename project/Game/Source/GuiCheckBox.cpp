#include "GuiCheckBox.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "App.h"
#include "Textures.h"
#include "Scene_Testing.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, SDL_Rect bounds2, const char* text) : GuiControl(GuiControlType::CHECKBOX, id), click(false)
{
	this->bounds = bounds;
	this->bounds2 = bounds2;
	this->text = text;

	canClick = true;
	drawBasic = false;
	click = false;

	img = app->tex->Load("Assets/Textures/Interfaz/CheckBoxUnMarked.png");
	img2 = app->tex->Load("Assets/Textures/Interfaz/CheckBoxMarked.png");
	img3 = app->tex->Load("Assets/Textures/Interfaz/CheckBoxSelected.png");
}

GuiCheckBox::~GuiCheckBox()
{

}

bool GuiCheckBox::Update(float dt)
{
	

	return false;
}

bool GuiCheckBox::PostUpdate()
{

	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);
		
		SDL_Rect bouunds;
		bouunds.x = bounds.x + bounds2.x;
		bouunds.y = bounds.y + bounds2.y;


		bouunds.w = 80;
		bouunds.h = 80;
		

		mouseX *= 2;

		mouseY *= 2;

		if (selected) {

			state = GuiControlState::FOCUSED;

			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
				state = GuiControlState::PRESSED;
			}

			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
				NotifyObserver();
				click = !click;

				app->audio->PlayFx(select_audio);
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}
	
		
		if (!click)
		{
			switch (state)
			{
			case GuiControlState::DISABLED:
				
				break;
			case GuiControlState::NORMAL:
				app->render->DrawTexture(img, bounds.x, bounds.y, SDL_FLIP_NONE, 0, 0, true);
				break;
			case GuiControlState::FOCUSED:
				app->render->DrawTexture(img3, bounds.x, bounds.y, SDL_FLIP_NONE, 0, 0, true);
				break;
			case GuiControlState::PRESSED:
				app->render->DrawTexture(img, bounds.x, bounds.y, SDL_FLIP_NONE, 0, 0, true);				
				break;
			}
			app->render->DrawText(text.GetString(), bounds.x + bounds2.x - 200, bounds.y + bounds2.y, bounds.w, bounds.h);
		}
		if (click)
		{
			switch (state)
			{
			case GuiControlState::DISABLED:
				/*app->render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);*/
				break;
			case GuiControlState::NORMAL:
				app->render->DrawTexture(img2, bounds.x, bounds.y, SDL_FLIP_NONE, 0, 0, 0, 0, true);
				break;
			case GuiControlState::FOCUSED:
				app->render->DrawTexture(img2, bounds.x, bounds.y, SDL_FLIP_NONE, 0, 0, 0, 0, true);
				break;
			case GuiControlState::PRESSED:
				app->render->DrawTexture(img2, bounds.x, bounds.y, SDL_FLIP_NONE, 0, 0, 0, 0, true);
				break;
			}
			app->render->DrawText(text.GetString(), bounds.x + bounds2.x - 200, bounds.y + bounds2.y, bounds.w, bounds.h);
		}
		//L15: DONE 4: Draw the button according the GuiControl State
		

		

		/*app->render->DrawRectangle(bouunds, 0, 255, 0, 255, true, false);*/
		
	}
	else
	{
		app->render->DrawRectangle({ 0,0,0,0 }, 0, 20, 255, true, false);
	}

	
	

	return false;
}

