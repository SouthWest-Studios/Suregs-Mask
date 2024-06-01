#include "NPC_Padre.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene_Testing.h"
#include "Log.h"
#include "InventoryManager.h"
#include "Point.h"
#include "Physics.h"
#include "Window.h"
#include "DialogManager.h"
#include "Dialog.h"
#include "DialogTriggerEntity.h"
#include "ModuleFadeToBlack.h"
#include "PugiXml\src\pugixml.hpp"
#include <iostream>





NPCPadre::NPCPadre() : Entity(EntityType::NPC_PADRE)
{
	name = ("npc_padre");
}

NPCPadre::~NPCPadre() {}

bool NPCPadre::Awake() {
	return true;
}

bool NPCPadre::Start() {
	//position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	int TSprite = config.attribute("Tsprite").as_int();
	int SpriteX = config.attribute("sprite_x").as_int();
	int SpriteY = config.attribute("sprite_y").as_int();
	int Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

	idleAnim.LoadAnim("boss_igory", "idle_boss_Igory", spritePositions);

	std::string texturePath = config.attribute("texturePath").as_string();
	texture = app->tex->Load(texturePath.c_str());

	app->entityManager->objectsToDraw.push_back({
	    texture,
	    position.x + 200, // x
	    position.y - 400, // y
	    position.y + 183, // anchorY
	    185, // width
	    110, // height
	    true, // isFacingLeft
	    false // isDynamic
	    });

	pbody = app->physics->CreateRectangleSensor(position.x, position.y, 100, 100, bodyType::STATIC);
	pbody->entity = this;
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	parseResult = dialogoFile.load_file("dialogs.xml");
	dialogoPadre = dialogoFile.child("dialogues");
	dialogNode = find_child_by_attribute(dialogoPadre, "dialog", "id", "2001").child("sentences").child("sentence");
	return true;
}

bool NPCPadre::Update(float dt) {
	//nextState = EntityState::IDLE;

	currentAnimation->Update();


	/*for (pugi::xml_node itemNode = dialogNode; itemNode; itemNode = itemNode.next_sibling("sentence"))
	{
		app->dialogManager->AddDialog(app->dialogManager->CreateDialog(dialogNode, itemNode.attribute("name").as_string(), itemNode.attribute("facetexturepath").as_string()));
	}*/


	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		CleanUp();
	}
	return true;
}

bool NPCPadre::PostUpdate() {
	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x - 170, position.y - 135, 0.5f, SDL_FLIP_NONE, &rect);

	return true;
}

pugi::xml_node NPCPadre::find_child_by_attribute(pugi::xml_node parent, const char* name, const char* attr_name, const char* attr_value) {
	for (pugi::xml_node node : parent.children(name)) {
		if (std::strcmp(node.attribute(attr_name).value(), attr_value) == 0) {
			return node;
		}
	}
	return pugi::xml_node(); 
}
bool NPCPadre::CleanUp() {
	app->physics->GetWorld()->DestroyBody(pbody->body);
	app->tex->UnLoad(texture);
	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}

// L07 DONE 6: Define OnCollision function for the player.
void NPCPadre::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype) {
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::PLAYER:
		for (pugi::xml_node itemNode = dialogNode; itemNode; itemNode = itemNode.next_sibling("sentence"))
		{
			app->dialogManager->AddDialog(app->dialogManager->CreateDialog(dialogNode, itemNode.attribute("name").as_string(), itemNode.attribute("facetexturepath").as_string()));
		}
		break;
	default:
		break;
	}
}
