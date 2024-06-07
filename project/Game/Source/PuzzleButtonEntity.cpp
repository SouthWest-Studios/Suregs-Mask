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
	std::string path = config.attribute("texturePath").as_string();
	texture = app->tex->Load(config.attribute("texturePath").as_string());

	puzzle_button_fx = app->audio->LoadAudioFx("puzzle_button_fx");

	return true;
}

bool PuzzleButtonEntity::Update(float dt)
{
	
	return true;
}

bool PuzzleButtonEntity::PostUpdate()
{
	if (!pressed && !playerOnHover) {
		app->render->DrawTexture(texture, position.x - 35, position.y - 32, 1.0f, SDL_FLIP_NONE, &notPressedRect);
	}
	else {
		//notPressed
		app->render->DrawTexture(texture, position.x - 35, position.y - 32, 1.0f, SDL_FLIP_NONE, &pressedRect);
	}

	playerOnHover = false;

	return true;
}

bool PuzzleButtonEntity::CleanUp()
{
	if (pbody != nullptr) {
		app->physics->DestroyBody(pbody);
		pbody = nullptr;
	}
	return true;
}

void PuzzleButtonEntity::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
		case ColliderType::PLAYER:
			if(pressed == false) app->audio->PlayFx(puzzle_button_fx);
			pressed = true;
			playerOnHover = true;
			LOG("PULSANDO BOTON ID: %d", buttonID);

			break;
	}
}


