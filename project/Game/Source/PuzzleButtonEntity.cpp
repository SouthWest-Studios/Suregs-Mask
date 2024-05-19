#include "PuzzleButtonEntity.h"
#include "DialogManager.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene_Testing.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

PuzzleButtonEntity::PuzzleButtonEntity() : Entity(EntityType::PUZZLE_BUTTON)
{
	name = ("puzzle_button_entity");
}

PuzzleButtonEntity::~PuzzleButtonEntity() {}

bool PuzzleButtonEntity::Awake() {

	return true;
}

bool PuzzleButtonEntity::Start() {

	pbody = app->physics->CreateRectangleSensor(position.x+10, position.y, 32, 32, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PUZZLE_BUTTON_ENTITY;


	return true;
}

bool PuzzleButtonEntity::Update(float dt)
{
	
	return true;
}

bool PuzzleButtonEntity::CleanUp()
{
	if (pbody != nullptr) {
		app->physics->DestroyBody(pbody);
	}
	return true;
}

void PuzzleButtonEntity::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
		case ColliderType::PLAYER:
			LOG("COLISION CON PLAYER HOLIS");
			break;
	}
}


