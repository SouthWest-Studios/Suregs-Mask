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


	bool active = false; //Para saber si tiene que llamar al update o postUpdate del comercio o no.


	iPoint positionGeneral;
	iPoint positionList;
	iPoint positionInList;
	uint tradeSpacing;
	uint itemSpacing;
	uint itemsRequestedSpacing;

	iPoint npcPosition;


private:

	uint id;



	SDL_Texture* backgroundTexture = nullptr;

	SDL_Texture* backgroundTradeTexture = nullptr;
	SDL_Texture* backgroundTradeHoverTexture = nullptr;
	SDL_Texture* backgroundSelectAllTexture = nullptr;
	SDL_Texture* backgroundSelectAllHoverTexture = nullptr;

	SDL_Texture* backgroundTradeItemTexture = nullptr;

	SDL_Texture* backgroundSliderTexture = nullptr;
	SDL_Texture* knobSliderTexture = nullptr;

	SDL_Texture* backgroundConfirmTexture = nullptr;
	SDL_Texture* backgroundConfirmHoverTexture = nullptr;

	SDL_Texture* backgroundButtonTexture = nullptr;
	SDL_Texture* backgroundButtonHoverTexture = nullptr;
	SDL_Texture* backgroundButtonDisabledTexture = nullptr;
	SDL_Texture* backgroundMoneyTexture = nullptr;

	SDL_Texture* backgroundDescriptionTexture = nullptr;

	SDL_Texture* npcTexture = nullptr;



	

	std::vector<Trade*> trades;

	int pointerIndexF;
	int pointerIndexC;
	int scrollY;
	
};

#endif // __COMMERCE_H__
