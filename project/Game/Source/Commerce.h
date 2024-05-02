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
	std::vector<int> quantityOffered;
	std::vector<int> quantityRequested;
	int quantityTraded;
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

	int GetInventoryTradesQuantity(InventityType type);

	int GetTotalTradesSelected();

	uint GetId();
	

public:

	char* backgroundPathTexture;


	char* backgroundTradePathTexture;
	char* backgroundTradeHoverPathTexture;
	char* backgroundSelectAllPathTexture;
	char* backgroundSelectAllHoverPathTexture;

	char* backgroundTradeItemPathTexture;

	char* backgroundSliderPathTexture;
	char* knobSliderPathTexture;

	char* backgroundConfirmPathTexture;
	char* backgroundConfirmHoverPathTexture;

	char* backgroundButtonPathTexture;
	char* backgroundButtonHoverPathTexture;
	char* backgroundButtonDisabledPathTexture;
	char* backgroundMoneyPathTexture;

	char* backgroundDescriptionPathTexture;

	char* npcPathTexture;


	bool active; //Para saber si tiene que llamar al update o postUpdate del comercio o no.


	iPoint positionGeneral;
	iPoint positionList;
	iPoint positionInList;
	uint tradeSpacing;
	uint itemSpacing;
	uint itemsRequestedSpacing;

	iPoint npcPosition;


private:

	uint id;



	SDL_Texture* backgroundTexture;

	SDL_Texture* backgroundTradeTexture;
	SDL_Texture* backgroundTradeHoverTexture;
	SDL_Texture* backgroundSelectAllTexture;
	SDL_Texture* backgroundSelectAllHoverTexture;

	SDL_Texture* backgroundTradeItemTexture;

	SDL_Texture* backgroundSliderTexture;
	SDL_Texture* knobSliderTexture;

	SDL_Texture* backgroundConfirmTexture;
	SDL_Texture* backgroundConfirmHoverTexture;

	SDL_Texture* backgroundButtonTexture;
	SDL_Texture* backgroundButtonHoverTexture;
	SDL_Texture* backgroundButtonDisabledTexture;
	SDL_Texture* backgroundMoneyTexture;

	SDL_Texture* backgroundDescriptionTexture;

	SDL_Texture* npcTexture;



	

	std::vector<Trade*> trades;

	int pointerIndexF;
	int pointerIndexC;
	int scrollY;
	
};

#endif // __COMMERCE_H__
