#ifndef __GUICONTROLSLIDER_H__
#define __GUICONTROLSLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiControlSlider : public GuiControl
{

public:

	GuiControlSlider(uint32 id, SDL_Rect bounds, int minValue, int maxValue, const char* text);
	virtual ~GuiControlSlider();

	// Called each loop iteration
	bool Update(float dt);
	bool PostUpdate();
	void Draw();

	SDL_Texture* knob;
	SDL_Texture* slider;
	
	int minValue, maxValue, value;
private:

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;
};

#endif // __GUICONTROLSLIDER_H__