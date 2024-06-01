
#include "EntityManager.h"
#include "QuestManager.h"
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

QuestManager::QuestManager(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name = ("questmanager");
}

// Destructor
QuestManager::~QuestManager()
{}

// Called before render is available
bool QuestManager::Awake(pugi::xml_node config)
{
	LOG("Loading Quest Manager");
	bool ret = true;


	return ret;

}

bool QuestManager::Start() {
	bool ret = true;


	return ret;
}

// Called before quitting
bool QuestManager::CleanUp()
{
	bool ret = true;


	return ret;
}

bool QuestManager::CreateQuest(int questLineID, int questID, std::string questTitle)
{
	bool ret = false;

	QuestLine* questLine = FindQuestLine(questLineID);

	if (questLine != nullptr) {
		ret = true;
		Quest* quest = new Quest{ questID, questTitle };
		questLine->quests.push_back(quest);
		
	}

	return ret;
}

std::string QuestManager::GetQuestTitle(int questLineID, int questID)
{
	return FindQuest(FindQuestLine(questLineID), questID)->questTitle;
}

void QuestManager::UpdateQuestLine(int questLineID)
{
	FindQuestLine(questLineID)->questIndex++;
}

QuestLine* QuestManager::FindQuestLine(int questLineID)
{
	for (int i = 0; i < questLines.size(); i++) {
		if (questLines.at(i)->questLineID == questLineID) {
			return questLines.at(i);
		}
	}
	return nullptr;
}

Quest* QuestManager::FindQuest(QuestLine* questLine, int questID)
{
	for (int i = 0; i < questLine->quests.size(); i++) {
		if (questLine->quests.at(i)->questID == questID) {
			return questLine->quests.at(i);
		}
	}
	return nullptr;
}

bool QuestManager::LoadState(pugi::xml_node node)
{
	return false;
}

bool QuestManager::SaveState(pugi::xml_node node)
{
	return false;
}








