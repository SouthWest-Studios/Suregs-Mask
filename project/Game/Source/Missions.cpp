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
    // Implementaci�n espec�fica del inicio de la misi�n, si es necesario
    return true;
}

bool Mission::CleanUp() {
    // Implementaci�n espec�fica de la limpieza de la misi�n, si es necesario
    return true;
}

bool Mission::Update(float dt) {
    // Implementaci�n espec�fica de la actualizaci�n de la misi�n, si es necesario
    return true;
}

bool Mission::PostUpdate() {
    // Implementaci�n espec�fica de la actualizaci�n posterior de la misi�n, si es necesario
    return true;
}

// Implementaci�n de los m�todos virtuales puros de la clase base
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

