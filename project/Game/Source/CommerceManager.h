#ifndef __COMMERCEMANAGER_H__
#define __COMMERCEMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "inventity.h"
#include "List.h"
#include "Textures.h"
#include "Commerce.h"

#include <vector> // Necesario para std::vector
#include <cstdint> // Necesario para uint32_t

class Commerce;

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

	std::vector<Trade*> LoadTrades(pugi::xml_node nodeTrade);

	

public:





private:

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

	std::vector<Commerce*> commerces;
	
};

#endif // __COMMERCEMANAGER_H__
