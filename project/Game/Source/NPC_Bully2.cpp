#include "NPC_Bully2.h"
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
#include "ModuleFadeToBlack.h"


NPCBully2::NPCBully2() : Entity(EntityType::NPC_BULLY_2)
{
	name.Create("npc_bully2");

}

NPCBully2::~NPCBully2() {

}

bool NPCBully2::Awake() {



	return true;
}

bool NPCBully2::Start() {


	//position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	TSprite = config.attribute("Tsprite").as_int();
	SpriteX = config.attribute("sprite_x").as_int();
	SpriteY = config.attribute("sprite_y").as_int();
	Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

	idleAnim.LoadAnim((char*)name.GetString(), "idleAnim", spritePositions);

	std::string texturePath = config.attribute("texturePath").as_string();

	texture = app->tex->Load(texturePath.c_str());

	//pbody = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
	//pbody->entity = this;
	//pbody->listener = this;
	//pbody->ctype = ColliderType::PLAYER;





	return true;
}

bool NPCBully2::Update(float dt)
{
	//nextState = EntityState::IDLE;


	currentAnimation->Update();
	return true;
}


bool NPCBully2::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x - 50, position.y - 200, SDL_FLIP_NONE, &rect);
	

	return true;
}


bool NPCBully2::CleanUp()
{
	//app->physics->GetWorld()->DestroyBody(pbody->body);
	SDL_DestroyTexture(texture);
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void NPCBully2::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	default:
		break;
	}
}




