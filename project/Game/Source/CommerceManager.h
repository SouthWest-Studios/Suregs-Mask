#ifndef __COMMERCEMANAGER_H__
#define __COMMERCEMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "inventity.h"
#include "List.h"
#include "Textures.h"
#include "Commerce.h"
#include "CommerceHerreria.h"

#include <vector> // Necesario para std::vector
#include <cstdint> // Necesario para uint32_t

class Commerce;
class CommerceHerreria;

struct Trade;

enum class CommerceType
{
	BASICO,
	HERRERIA,
	UNKNOWN 
};


class CommerceManager : public Module
{
public:

	CommerceManager(App* app, bool start_enabled = true);

	// Destructor
	virtual ~CommerceManager();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


	Commerce* CreateCommerce(CommerceType type, uint id, std::vector<Trade*> trades);
	void PlayCommerce(uint id);

	bool AnyCommerceActive();

	std::vector<Trade*> LoadTrades(pugi::xml_node nodeTrade);

	

public:





private:

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
	char* backgroundButtonDisabledPathTexture;

	char* backgroundButtonPathTexture;
	char* backgroundButtonHoverPathTexture;
	char* backgroundMoneyPathTexture;

	char* backgroundDescriptionPathTexture;

	std::vector<Commerce*> commerces;

	pugi::xml_node commerceNode;
	
};

#endif // __COMMERCEMANAGER_H__
