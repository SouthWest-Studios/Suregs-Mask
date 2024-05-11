#include "EntityManager.h"
#include "Missions.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene_testing.h"
#include "inventity.h"
#include "ItemInv.h"
#include "Defs.h"
#include "Log.h"
#include "SString.h"
#include "InventoryManager.h"
#include "Hud.h"

Mission::Mission() : id(0), description(""), completed(false) {}


Mission::~Mission() {}

bool Mission::Start() {
    // Implementación específica del inicio de la misión, si es necesario
    return true;
}

bool Mission::CleanUp() {
    // Implementación específica de la limpieza de la misión, si es necesario
    return true;
}

bool Mission::Update(float dt) {
    // Implementación específica de la actualización de la misión, si es necesario
    return true;
}

bool Mission::PostUpdate() {
    // Implementación específica de la actualización posterior de la misión, si es necesario
    return true;
}

// Implementación de los métodos virtuales puros de la clase base
bool Mission::IsCompleted() const {
    return completed;
}

void Mission::SetCompleted(bool value) {
    completed = value;
}

void Mission::AddRequiredItem(uint itemId) {
    // Agregar el objeto requerido a la lista de objetos requeridos
    requiredItems.push_back(itemId);
}

void Mission::RemoveRequiredItem(uint itemId) {
    auto it = std::find(requiredItems.begin(), requiredItems.end(), itemId);
    if (it != requiredItems.end()) {
        requiredItems.erase(it);
    }
}

void Mission::SetRequiredEntity(Entity* entity) {
    requiredEntity = entity;
}

