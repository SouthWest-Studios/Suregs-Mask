#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiCheckBox : public GuiControl
{

public:

	GuiCheckBox(uint32 id, SDL_Rect bounds, SDL_Rect bounds2, const char* text);
	virtual ~GuiCheckBox();

	// Called each loop iteration
	bool Update(float dt);

	bool PostUpdate();

	bool click;
private:

	int mouseX, mouseY;
	

	bool canClick = true;
	bool drawBasic = false;

	SDL_Texture* img;
	SDL_Texture* img2;
};

#endif // __GUICHECKBOX_H__