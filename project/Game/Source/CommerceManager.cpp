
#include "EntityManager.h"
#include "CommerceManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene_testing.h"
#include "inventity.h"
#include "SwordInv.h"
#include "GarraInv.h"
#include "ArmaduraInv.h"
#include "DiamanteInv.h"
#include "ItemInv.h"
#include "OjoInv.h"
#include "Defs.h"
#include "Log.h"
#include "SString.h"

CommerceManager::CommerceManager(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("commercemanager");
}

// Destructor
CommerceManager::~CommerceManager()
{}

// Called before render is available
bool CommerceManager::Awake(pugi::xml_node config)
{
	LOG("Loading Commerce Manager");
	bool ret = true;



	backgroundPathTexture					= (char*)config.child("backgroundPathTexture").attribute("texturePath").as_string();
	sellerPathTexture						= (char*)config.child("sellerPathTexture").attribute("texturePath").as_string();
	backgroundTradePathTexture				= (char*)config.child("backgroundTradePathTexture").attribute("texturePath").as_string();
	backgroundTradeHoverPathTexture			= (char*)config.child("backgroundTradeHoverPathTexture").attribute("texturePath").as_string();
	backgroundSelectAllPathTexture			= (char*)config.child("backgroundSelectAllPathTexture").attribute("texturePath").as_string();
	backgroundSelectAllHoverPathTexture		= (char*)config.child("backgroundSelectAllHoverPathTexture").attribute("texturePath").as_string();
	backgroundSliderPathTexture				= (char*)config.child("backgroundSliderPathTexture").attribute("texturePath").as_string();
	knobSliderPathTexture					= (char*)config.child("knobSliderPathTexture").attribute("texturePath").as_string();
	backgroundConfirmPathTexture			= (char*)config.child("backgroundConfirmPathTexture").attribute("texturePath").as_string();
	backgroundConfirmHoverPathTexture		= (char*)config.child("backgroundConfirmHoverPathTexture").attribute("texturePath").as_string();


	pugi::xml_node commerceNode;
	commerceNode = config.child("commerce");
	while (commerceNode != NULL) {

		Commerce* commerce = CreateCommerce(CommerceType::BASICO, commerceNode.attribute("id").as_uint(), LoadTrades(commerceNode.child("trades")));
		
		commerces.push_back(commerce);

		commerceNode = commerceNode.next_sibling();
	}

	return ret;

}

bool CommerceManager::Start() {

	bool ret = true;


	return ret;
}

// Called before quitting
bool CommerceManager::CleanUp()
{
	bool ret = true;
	
	return ret;
}

Commerce* CommerceManager::CreateCommerce(CommerceType type, uint id, std::vector<Trade*> trades)
{
	Commerce* commerce = new Commerce(id);

	commerce->backgroundPathTexture						= backgroundPathTexture;
	commerce->sellerPathTexture							= sellerPathTexture;
	commerce->backgroundTradePathTexture				= backgroundTradePathTexture;
	commerce->backgroundTradeHoverPathTexture			= backgroundTradeHoverPathTexture;
	commerce->backgroundSelectAllPathTexture			= backgroundSelectAllPathTexture;
	commerce->backgroundSelectAllHoverPathTexture		= backgroundSelectAllHoverPathTexture;
	commerce->backgroundSliderPathTexture				= backgroundSliderPathTexture;
	commerce->knobSliderPathTexture						= knobSliderPathTexture;
	commerce->backgroundConfirmPathTexture				= backgroundConfirmPathTexture;
	commerce->backgroundConfirmHoverPathTexture			= backgroundConfirmHoverPathTexture;

	commerce->SetTrades(trades);

	

	return commerce;
}

void CommerceManager::PlayCommerce(uint id)
{
	for (int i = 0; i < commerces.size(); i++) {
		if (commerces.at(i)->GetId() == id) {
			commerces.at(i)->active = true;
			break;
		}
	}
}

std::vector<Trade*> CommerceManager::LoadTrades(pugi::xml_node nodeTrade)
{
	std::vector<Trade*> trades;

	pugi::xml_node tradeNode = nodeTrade.child("trade");

	while (tradeNode != NULL) {

		Trade* trade = new Trade();
		//trade->itemsOffered.push_back()

		tradeNode = tradeNode.next_sibling();
	}



	return trades;
}

bool CommerceManager::Update(float dt)
{

	bool ret = true;

	for (int i = 0; i < commerces.size(); i++) {
		if (commerces.at(i)->active) {
			commerces.at(i)->Update(dt);
		}
	}



	return ret;
}

bool CommerceManager::PostUpdate()
{
	bool ret = true;

	for (int i = 0; i < commerces.size(); i++) {
		if (commerces.at(i)->active) {
			commerces.at(i)->PostUpdate();
		}
	}
	
	return ret;
}








