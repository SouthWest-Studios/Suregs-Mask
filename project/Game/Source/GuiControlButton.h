#ifndef __GUICONTROLBUTTON_H__
#define __GUICONTROLBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiControlButton : public GuiControl
{

public:

	GuiControlButton(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiControlButton();

	bool Start();
	// Called each loop iteration
	bool Update(float dt);

	bool PostUpdate();

	bool click = false;

private:
	
	SDL_Texture* button;
	
	int mouseX, mouseY;
	/*unsigned int click;*/

	bool canClick = true;
	
	bool drawBasic = false;

	int select_audio;
};

#endif // __GUICONTROLBUTTON_H__