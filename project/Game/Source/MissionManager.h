#ifndef __MISSIONMANAGER_H__
#define __MISSIONMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "inventity.h"
#include "List.h"
#include "Textures.h"
#include "Missions.h"
#include "Dialog.h"
#include "DialogTriggerEntity.h"

#include <vector> // Necesario para std::vector
#include <cstdint> // Necesario para uint32_t

class Mission;
class DialogTrigger;

struct Missions;

enum class State;

struct Missiones {
	uint id;
	std::string entityName;
	std::string itemName;
	uint itemQuantity;
	uint reward;
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
	void StartMission(Mission* mission);
	void UpdateMissionProgress(Mission* mission);
	void CompleteMission(Mission* mission);
	void LoadMissionsFromXML(const char* filename);

	// Función para verificar si una misión está completada
	bool IsMissionCompleted(Mission* mission) const { return false; }

	// Función para obtener el progreso actual de una misión
	void GetMissionProgress(Mission* mission) const {}

	void ProcessDialog(Dialog* dialog);

	void HandleDialogEvent(const DialogEvent& dialog_event);
	void HandleMissionEvent(const std::string& event_type, Mission* mission, int newId);

	Mission* FindMissionById(uint mission_id);

	void ChangeDialogTriggerId(uint old_id, uint new_id);

	DialogTrigger* FindDialogTriggerById(uint dialog_trigger_id);

	Missiones* CreateMission(pugi::xml_node missionNode);

public:


private:

	// Variables 

	std::vector<Mission*> activeMissions;

	std::vector<DialogTrigger*> dialogTriggers;

	std::vector<Missiones*> missions;


	//verificar si una misión ya está en curso
	bool IsMissionActive(Mission* mission) {
		// Recorrer la lista de misiones activas y verificar si activa mision misión
		for (Mission* activeMission : activeMissions) {
			if (activeMission == mission) {
				return true; // La misión ya está activa
			}
		}
		return false; // La misión no está activa
	}
};

#endif // __MISSIONMANAGER_H__
