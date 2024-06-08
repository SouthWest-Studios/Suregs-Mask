#ifndef __QUESTMANAGER_H__
#define __QUESTMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "inventity.h"
#include "List.h"
#include "Textures.h"

#include <vector> // Necesario para std::vector
#include <cstdint> // Necesario para uint32_t

struct Quest
{
	int questID;
	std::string questTitle;
};

struct QuestLine
{
	int questLineID;
	std::vector<Quest*> quests;
	int questIndex = 1;
	bool completed = false;
	bool active = false;
};

class QuestManager : public Module
{
public:

	QuestManager(App* app, bool start_enabled = true);

	// Destructor
	virtual ~QuestManager();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called after Awake
	bool Start();

	// Called before quitting
	bool CleanUp();


	Quest* CreateQuest(int questLineID, int questID, std::string questTitle);
	QuestLine* CreateQuestLine(int questLineID);

	std::string GetQuestTitle(int questLineID, int questID);
	
	std::vector<Quest*> GetActiveQuest();

	void UpdateQuestLine(int questLineID, int newQuestTarget = -1, bool setActive = true);
	int GetQuestLineIndex(int questLineID);

	QuestLine* FindQuestLine(int questLineID);
	Quest* FindQuest(QuestLine* questLine, int questID);
	Quest* FindActualQuest(QuestLine* questLine);

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);
	

public:



private:

	std::vector<QuestLine*> questLines;

	
	
	
};

#endif // __QUESTMANAGER_H__
