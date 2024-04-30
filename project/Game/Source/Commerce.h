#ifndef __COMMERCE_H__
#define __COMMERCE_H__

#include "Module.h"
#include "Entity.h"
#include "inventity.h"
#include "List.h"
#include "Textures.h"
#include "CommerceManager.h"

struct Trade {
	std::vector<Inventity*> itemsOffered;
	std::vector<Inventity*> itemsRequested;
	std::vector<uint32_t> quantityOffered;
	std::vector<uint32_t> quantityRequested;
	uint32_t quantityTraded;
};

class Commerce
{
public:

	Commerce(uint id);

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

	bool CloseCommerce();


	void SetTrades(std::vector<Trade*> trades);

	uint GetId();
	

public:

	char* backgroundPathTexture;

	char* sellerPathTexture;

	char* backgroundTradePathTexture;
	char* backgroundTradeHoverPathTexture;
	char* backgroundSelectAllPathTexture;
	char* backgroundSelectAllHoverPathTexture;

	char* backgroundSliderPathTexture;
	char* knobSliderPathTexture;

	char* backgroundConfirmPathTexture;
	char* backgroundConfirmHoverPathTexture;


	bool active; //Para saber si tiene que llamar al update o postUpdate del comercio o no.


	iPoint positionGeneral;
	iPoint positionList;
	iPoint positionInList;
	uint tradeSpacing;
	uint itemSpacing;


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



	

	std::vector<Trade*> trades;

	int pointerIndex;
	
};

#endif // __COMMERCE_H__
