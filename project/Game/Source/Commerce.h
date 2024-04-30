#ifndef __COMMERCE_H__
#define __COMMERCE_H__

#include "Module.h"
#include "Entity.h"
#include "inventity.h"
#include "List.h"
#include "Textures.h"

struct Trade {
	std::vector<Inventity> itemsOffered;
	std::vector<Inventity> itemsRequested;
	std::vector<uint32_t> quantityOffered;
	std::vector<uint32_t> quantityRequested;
	uint32_t quantityTraded;
};

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

	bool SelectTrade(uint id, bool add = true);
	bool SelectAllTrade(uint id, bool add = true);
	bool ApplyTrades();

	bool LoadTextures();
	

public:

	char* backgroundPathexture;

	char* sellerPathTexture;

	char* backgroundTradePathTexture;
	char* backgroundTradeHoverPathTexture;
	char* backgroundSelectAllPathTexture;
	char* backgroundSelectAllHoverPathTexture;

	char* backgroundSliderPathTexture;
	char* knobSliderPathTexture;

	char* backgroundConfirmPathTexture;
	char* backgroundConfirmHoverPathTexture;



private:

	uint id;



	SDL_Texture* backgroundTexture;

	SDL_Texture* sellerTexture;

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

	List<Trade*> trades;
	
};

#endif // __COMMERCE_H__
