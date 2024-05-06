#ifndef __MISSIONMANAGER_H__
#define __MISSIONMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "inventity.h"
#include "List.h"
#include "Textures.h"
#include "Missions.h"

#include <vector> // Necesario para std::vector
#include <cstdint> // Necesario para uint32_t

class Mission;

struct Missions;

enum class MissionType // Tipo de misiones 
{
	//De momento solo secundria
	SECUANDARIA,
	UNKNOWN
};


class MissionManager : public Module
{
public:

	MissionManager(App* app, bool start_enabled = true);

	// Destructor
	virtual ~MissionManager();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Funciones que tendrá el MissionManager
	// 
	//Mission* CreateCommerce(CommerceType type, uint id, std::vector<Trade*> trades);
	//void PlayCommerce(uint id);

	//bool AnyCommerceActive();

	//std::vector<Trade*> LoadTrades(pugi::xml_node nodeTrade);



public:





private:

	// Variables 

	//char* backgroundPathTexture;

	//char* backgroundTradePathTexture;
	//char* backgroundTradeHoverPathTexture;
	//char* backgroundSelectAllPathTexture;
	//char* backgroundSelectAllHoverPathTexture;

	//char* backgroundTradeItemPathTexture;

	//char* backgroundSliderPathTexture;
	//char* knobSliderPathTexture;

	//char* backgroundConfirmPathTexture;
	//char* backgroundConfirmHoverPathTexture;
	//char* backgroundButtonDisabledPathTexture;

	//char* backgroundButtonPathTexture;
	//char* backgroundButtonHoverPathTexture;
	//char* backgroundMoneyPathTexture;

	//char* backgroundDescriptionPathTexture;

	//std::vector<Commerce*> commerces;

	//pugi::xml_node commerceNode;

};

#endif // __MISSIONMANAGER_H__

