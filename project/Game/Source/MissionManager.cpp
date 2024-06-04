#include "EntityManager.h"
#include "MissionManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene_testing.h"
#include "inventity.h"
#include "InventoryManager.h"
#include "ItemInv.h"
#include "Defs.h"
#include "Log.h"
#include "SString.h"
#include "Utils.cpp"
#include "Hud.h"
#include "DialogManager.h"
#include "Dialog.h"
#include "DialogTriggerEntity.h"

#include "PugiXml/src/pugixml.hpp"

MissionManager::MissionManager(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name = ("missionmanager");
}

// Destructor
MissionManager::~MissionManager()
{}

// Called before render is available
bool MissionManager::Awake(pugi::xml_node config)
{
	LOG("Loading Mission Manager");
	bool ret = true;

	return ret;

}

bool MissionManager::Start() {
    bool ret = true;

    return ret;
}


// Called before quitting
bool MissionManager::CleanUp()
{
	bool ret = true;

	return ret;
}

bool MissionManager::Update(float dt)
{
    /*//printf("AAA");*/
    // Recorrer todas las misiones activas
    for (Mission* mission : activeMissions) {
        switch (mission->state) {
        case Mission::State::NotStarted:
            //printf("Mision NotStarted: %d", mission->GetId());
            // Si la misión no ha comenzado, el id del diálogo debería ser 1001
            ChangeDialogTriggerId(mission->GetId(), 1001);
            break;
        case Mission::State::InProgress:
            //printf("Mision InProgress: %d", mission->GetId());
            // Si la misión está en progreso, el id del diálogo debería ser 1002
            ChangeDialogTriggerId(mission->GetId(), 1002);
            break;
        case Mission::State::Completed:
            //printf("Mision Completed: %d", mission->GetId());
            // Si la misión está completada, el id del diálogo debería ser 1003
            ChangeDialogTriggerId(mission->GetId(), 1003);
            break;
        }
    }
    return true;
}


bool MissionManager::PostUpdate()
{
	bool ret = true;

	return ret;
}

void MissionManager::StartMission(Mission* mission) {
    // Verificar si la misión ya está en curso
    if (!IsMissionActive(mission)) {
        // Si no está en curso, agregarla a la lista de misiones activas
        //printf("Start Mision\n");
        activeMissions.push_back(mission);
        //printf("Mision añadida a la lista de misiones activas\n");
        // Cambiar el estado de la misión a InProgress
        mission->state = Mission::State::InProgress;
        //printf("Estado de la mision cambiado a InProgress\n");
    }
    else {
        //printf("La mision ya está en curso\n");
    }
}


void MissionManager::UpdateMissionProgress(Mission* mission) {
    //printf("Actualizando el progreso de la misión\n");
    // Completar la lógica de UpdateMissionProgress
}

void MissionManager::CompleteMission(Mission* mission) {
    //printf("Completando la misión\n");
    // Aquí podrías agregar lógica adicional, como recompensas, actualización de estadísticas, etc.
    // Por ahora, simplemente eliminaremos la misión de la lista de misiones activas
    auto it = std::find(activeMissions.begin(), activeMissions.end(), mission);
    if (it != activeMissions.end()) {
        //printf("Misión completada\n");
        activeMissions.erase(it);
    }
    else {
        //printf("La misión no se encontró en la lista de misiones activas\n");
    }
}

void MissionManager::LoadMissionsFromXML(const char* filename) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename);

    if (result) {
        pugi::xml_node missionsNode = doc.child("missions");

        // Recorre cada nodo de misión
        for (pugi::xml_node missionNode : missionsNode.children("mission")) {
            uint id = missionNode.attribute("id").as_uint();
        }
    }
}

void MissionManager::HandleMissionEvent(const std::string& event_type, Mission* mission, int newId) {
    //printf("Manejando el evento de misión: %s\n", event_type.c_str());
    // Cambiar el id del DialogTrigger
    ChangeDialogTriggerId(mission->GetId(), newId);

    // Verificar el tipo de evento de misión
    if (event_type == "activation") {
        // Activar la misión con la mision específica
        //printf("Llamando a StartMission\n");
        StartMission(mission);
    }
    else if (event_type == "progress") {
        // Actualizar el progreso de la misión con la mision específica
        //printf("Llamando a UpdateMissionProgress\n");
        UpdateMissionProgress(mission);
    }
    else if (event_type == "completion") {
        // Completar la misión con la mision específica
        //printf("Llamando a CompleteMission\n");
        CompleteMission(mission);
    }
}


void MissionManager::HandleDialogEvent(const DialogEvent& dialog_event) {
    //printf("Llamando a HandleDialogEvent");
    // Verificar si el evento tiene un evento de misión asociado
    if (dialog_event.mission_event != nullptr) {
        const MissionEvent& mission_event = *(dialog_event.mission_event);
        // Obtener el tipo y el ID de la misión del evento
        std::string event_type = mission_event.type;
        uint mission_id = mission_event.mission_id;
        //printf("Evento de misión: %s, ID de misión: %d\n", event_type.c_str(), mission_id);
        // Buscar la misión por su ID
        Mission* mission = FindMissionById(mission_id);
        if (mission != nullptr) {
            // Manejar el evento de misión
            HandleMissionEvent(event_type, mission, mission_id);
        }
    }
}


Mission* MissionManager::FindMissionById(uint mission_id) {
    for (Mission* mission : activeMissions) {
        if (mission->GetId() == mission_id) {
            return mission;
        }
    }
    return nullptr; // Devuelve nullptr si no se encuentra la misión
}


// Función para procesar un diálogo
void MissionManager::ProcessDialog(Dialog* dialog) {
    // Manejar cualquier evento asociado al diálogo
    if (dialog->has_event()) {
        const DialogEvent& dialog_event = dialog->event();
        if (dialog_event.mission_event != nullptr) {
            const MissionEvent& mission_event = *(dialog_event.mission_event);
            // Obtener el tipo y el ID de la misión del evento
            std::string event_type = mission_event.type;
            uint mission_id = mission_event.mission_id;
            // Buscar la misión por su ID
            Mission* mission = FindMissionById(mission_id);
            if (mission != nullptr) {
                // Manejar el evento de misión
                HandleMissionEvent(event_type, mission, mission_id);
            }
        }
    }
}

void MissionManager::ChangeDialogTriggerId(uint old_id, uint new_id) {
    // Buscar el DialogTrigger por su id antiguo
    DialogTrigger* dialogTrigger = FindDialogTriggerById(old_id);
    if (dialogTrigger != nullptr) {
        // Cambiar el id del DialogTrigger
        dialogTrigger->SetDialogId(new_id);
        // Actualizar el diálogo
        dialogTrigger->UpdateDialog();
    }
}


DialogTrigger* MissionManager::FindDialogTriggerById(uint dialog_trigger_id) {
    for (DialogTrigger* dialogTrigger : dialogTriggers) {
        if (dialogTrigger->GetDialogId() == dialog_trigger_id) {
            return dialogTrigger;
        }
    }
    return nullptr;
}


Missiones* MissionManager::CreateMission(pugi::xml_node missionNode) {
    Missiones* mission = new Missiones();
    mission->id = missionNode.attribute("id").as_uint();
    mission->entityName = missionNode.child("entityName").text().as_string();
    mission->itemName = missionNode.child("itemName").text().as_string();
    mission->itemQuantity = missionNode.child("itemQuantity").text().as_uint();
    mission->reward = missionNode.child("reward").text().as_uint();
    return mission;
}