#ifndef __COMMERCE_H__
#define __COMMERCE_H__

#include "Module.h"
#include "Entity.h"
#include "inventity.h"
#include "List.h"
#include "Textures.h"

class Commerce
{
public:

	Commerce();

	// Destructor
	virtual ~Commerce();

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	

public:





private:

	SDL_Texture* backgroundTexture;

	SDL_Texture* SellerTexture;

	SDL_Texture* backgroundTradeTexture;
	SDL_Texture* backgroundTradeHoverTexture;
	SDL_Texture* backgroundSelectAllTexture;
	SDL_Texture* backgroundSelectAllHoverTexture;

	SDL_Texture* backgroundSliderTexture;
	SDL_Texture* knobSliderTexture;

	SDL_Texture* backgroundConfirmTexture;
	SDL_Texture* backgroundConfirmHoverTexture;



	iPoint positionGeneral;
	iPoint positionList;
	iPoint positionInList;
	uint tradeSpacing;
	uint itemSpacing;
	
};

#endif // __COMMERCE_H__
