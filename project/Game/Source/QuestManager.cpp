
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
	//Crear todas las misiones vacias, y luego el load pondra indices y tal

	pugi::xml_node questLineNode = config.child("questLines").child("questLine");

	while (questLineNode != NULL) {
		
		int questLineId = questLineNode.attribute("id").as_int();

		QuestLine* questLine = CreateQuestLine(questLineId);
		
		
		pugi::xml_node questNode = questLineNode.child("quest");
		while (questNode != NULL) {
			CreateQuest(questLineId, questNode.attribute("id").as_int(), questNode.attribute("questTitle").as_string());
			questNode = questNode.next_sibling("quest");
		}
		questLineNode = questLineNode.next_sibling("questLine");
	}



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

Quest* QuestManager::CreateQuest(int questLineID, int questID, std::string questTitle)
{
	Quest* quest = nullptr;

	QuestLine* questLine = FindQuestLine(questLineID);

	if (questLine != nullptr) {
		quest = new Quest{ questID, questTitle };
		questLine->quests.push_back(quest);
		
	}

	return quest;
}

QuestLine* QuestManager::CreateQuestLine(int questLineID)
{
	QuestLine* questLine = nullptr;
	if (FindQuestLine(questLineID) == nullptr) {
		questLine = new QuestLine;
		questLine->questLineID = questLineID;

		questLines.push_back(questLine);
	}
	return questLine;
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

	for (pugi::xml_node questLineNode = node.child("questLines").child("questLine"); questLineNode; questLineNode = questLineNode.next_sibling("questLine"))
	{
		QuestLine* questLine = FindQuestLine(questLineNode.attribute("id").as_int());

		if (questLine != nullptr) {
			questLine->questIndex = questLineNode.attribute("index").as_int();
			questLine->active = questLineNode.attribute("active").as_bool();
			questLine->completed = questLineNode.attribute("completed").as_bool();
		}

	}



	return true;
}

bool QuestManager::SaveState(pugi::xml_node node)
{

	pugi::xml_node questLinesNode = node.append_child("questLines");


	for (int i = 0; i < questLines.size(); i++){
		pugi::xml_node questLineNode = questLinesNode.append_child("questLine");
		questLineNode.append_attribute("id").set_value(questLines.at(i)->questLineID);
		questLineNode.append_attribute("index").set_value(questLines.at(i)->questIndex);
		questLineNode.append_attribute("active").set_value(questLines.at(i)->active);
		questLineNode.append_attribute("completed").set_value(questLines.at(i)->completed);


		//for (int j = 0; j < questLines.at(i)->quests.size(); i++) {
		//	pugi::xml_node questNode = questLineNode.append_child("quest");
		//	questNode.append_attribute("id").set_value(questLines.at(i)->quests.at(j)->questID);
		//	questNode.append_attribute("id").set_value(questLines.at(i)->quests.at(j)->questID);


		//}


	}



	return true;
}







