
#include "EntityManager.h"
#include "CommerceManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene_testing.h"
#include "inventity.h"
#include "InventoryManager.h"
#include "SwordInv.h"
#include "GarraInv.h"
#include "ArmaduraInv.h"
#include "DiamanteInv.h"
#include "ItemInv.h"
#include "OjoInv.h"
#include "Defs.h"
#include "Log.h"
#include "SString.h"
#include "Utils.cpp"
#include "Hud.h"

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
	

	backgroundTradePathTexture				= (char*)config.child("backgroundTradePathTexture").attribute("texturePath").as_string();
	backgroundTradeHoverPathTexture			= (char*)config.child("backgroundTradeHoverPathTexture").attribute("texturePath").as_string();
	backgroundSelectAllPathTexture			= (char*)config.child("backgroundSelectAllPathTexture").attribute("texturePath").as_string();
	backgroundSelectAllHoverPathTexture		= (char*)config.child("backgroundSelectAllHoverPathTexture").attribute("texturePath").as_string();

	backgroundTradeItemPathTexture			= (char*)config.child("backgroundTradeItemTexture").attribute("texturePath").as_string();

	backgroundSliderPathTexture				= (char*)config.child("backgroundSliderPathTexture").attribute("texturePath").as_string();
	knobSliderPathTexture					= (char*)config.child("knobSliderPathTexture").attribute("texturePath").as_string();

	backgroundConfirmPathTexture			= (char*)config.child("backgroundConfirmPathTexture").attribute("texturePath").as_string();
	backgroundConfirmHoverPathTexture		= (char*)config.child("backgroundConfirmHoverPathTexture").attribute("texturePath").as_string();
	backgroundButtonDisabledPathTexture		= (char*)config.child("backgroundButtonDisabledPathTexture").attribute("texturePath").as_string();


	backgroundButtonPathTexture				= (char*)config.child("backgroundButtonPathTexture").attribute("texturePath").as_string();
	backgroundButtonHoverPathTexture		= (char*)config.child("backgroundButtonHoverPathTexture").attribute("texturePath").as_string();
	backgroundMoneyPathTexture				= (char*)config.child("backgroundMoneyPathTexture").attribute("texturePath").as_string();

	backgroundDescriptionPathTexture		= (char*)config.child("backgroundDescriptionPathTexture").attribute("texturePath").as_string();

	
	commerceNode = config.child("commerce");
	

	return ret;

}

bool CommerceManager::Start() {

	bool ret = true;

	while (commerceNode != NULL) {

		Commerce* commerce = CreateCommerce(CommerceType::BASICO, commerceNode.attribute("id").as_uint(), LoadTrades(commerceNode.child("trades")));

		commerce->positionGeneral = iPoint(commerceNode.attribute("positionGeneralX").as_int(), commerceNode.attribute("positionGeneralY").as_int());
		commerce->positionList = iPoint(commerceNode.attribute("positionListX").as_int(), commerceNode.attribute("positionListY").as_int());
		commerce->positionInList = iPoint(commerceNode.attribute("positionInListX").as_int(), commerceNode.attribute("positionInListY").as_int());
		commerce->tradeSpacing = commerceNode.attribute("tradeSpacing").as_int();
		commerce->itemSpacing = commerceNode.attribute("itemSpacing").as_int();
		commerce->npcPosition = iPoint(commerceNode.attribute("npcPositionX").as_int(), commerceNode.attribute("npcPositionY").as_int());

		commerce->itemsRequestedSpacing = commerceNode.attribute("itemsRequestedSpacing").as_int();

		commerce->npcPathTexture = (char*)commerceNode.attribute("NPCTexturePath").as_string();

		commerces.push_back(commerce);

		commerceNode = commerceNode.next_sibling();
	}

	//PlayCommerce(1);

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
	commerce->backgroundTradePathTexture				= backgroundTradePathTexture;
	commerce->backgroundTradeHoverPathTexture			= backgroundTradeHoverPathTexture;
	commerce->backgroundSelectAllPathTexture			= backgroundSelectAllPathTexture;
	commerce->backgroundSelectAllHoverPathTexture		= backgroundSelectAllHoverPathTexture;
	commerce->backgroundTradeItemPathTexture			= backgroundTradeItemPathTexture;
	commerce->backgroundSliderPathTexture				= backgroundSliderPathTexture;
	commerce->knobSliderPathTexture						= knobSliderPathTexture;
	commerce->backgroundConfirmPathTexture				= backgroundConfirmPathTexture;
	commerce->backgroundConfirmHoverPathTexture			= backgroundConfirmHoverPathTexture;
	commerce->backgroundButtonDisabledPathTexture		= backgroundButtonDisabledPathTexture;

	commerce->backgroundButtonPathTexture				= backgroundButtonPathTexture;
	commerce->backgroundButtonHoverPathTexture			= backgroundButtonHoverPathTexture;
	commerce->backgroundMoneyPathTexture				= backgroundMoneyPathTexture;

	commerce->backgroundDescriptionPathTexture			= backgroundDescriptionPathTexture;

	

	commerce->SetTrades(trades);

	

	return commerce;
}

void CommerceManager::PlayCommerce(uint id)
{
	for (int i = 0; i < commerces.size(); i++) {
		if (commerces.at(i)->GetId() == id) {
			commerces.at(i)->LoadTextures();
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

		std::vector<int> itemsOfferedInts = splitToInts(tradeNode.attribute("itemOffered").as_string(), ',');
		std::vector<int> quantityOfferedInts = splitToInts(tradeNode.attribute("quantityItemOffered").as_string(), ',');
		std::vector<int> itemsRequestedInts = splitToInts(tradeNode.attribute("itemRequested").as_string(), ',');
		std::vector<int> quantityRequestedInts = splitToInts(tradeNode.attribute("quantityItemRequested").as_string(), ',');

		for (int i = 0; i < itemsOfferedInts.size(); i++) {
			trade->itemsOffered.push_back(app->inventoryManager->CreateItem((InventityType)itemsOfferedInts.at(i), false));
			trade->quantityOffered.push_back(quantityOfferedInts.at(i));
		}

		for (int i = 0; i < itemsRequestedInts.size(); i++) {
			trade->itemsRequested.push_back(app->inventoryManager->CreateItem((InventityType)itemsRequestedInts.at(i), false));
			trade->quantityRequested.push_back(quantityRequestedInts.at(i));
		}

		
		/*trade->itemsRequested.push_back(app->inventoryManager->CreateItem((InventityType)tradeNode.attribute("itemRequested").as_int()));
		trade->quantityRequested.push_back(tradeNode.attribute("quantityItemRequested").as_int());*/
		trades.push_back(trade);
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
			app->entityManager->active = false;
			app->hud->isAnyMenuShowing = true;
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








