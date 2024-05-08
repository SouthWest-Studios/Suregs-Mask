#include "NPC_Herrera.h"
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


NPCHerrera::NPCHerrera() : Entity(EntityType::NPC_HERRERA)
{
	name = ("npc_herrera");

}

NPCHerrera::~NPCHerrera() {

}

bool NPCHerrera::Awake() {



	return true;
}

bool NPCHerrera::Start() {


	//position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	TSprite = config.attribute("Tsprite").as_int();
	SpriteX = config.attribute("sprite_x").as_int();
	SpriteY = config.attribute("sprite_y").as_int();
	Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

	idleAnim.LoadAnim((char*)name.c_str(), "idleAnim", spritePositions);

	std::string texturePath = config.attribute("texturePath").as_string();

	texture = app->tex->Load(texturePath.c_str());
	
	app->entityManager->objectsToDraw.push_back({
		texture,
		position.x - 50, // x
		position.y - 200, // y
		position.y + 178, // anchorY
		138, // width
		189, // height
		true, // isFacingLeft
		false
	});

	//pbody = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
	//pbody->entity = this;
	//pbody->listener = this;
	//pbody->ctype = ColliderType::PLAYER;





	return true;
}

bool NPCHerrera::Update(float dt)
{
	//nextState = EntityState::IDLE;


	currentAnimation->Update();
	return true;
}


bool NPCHerrera::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	//app->render->DrawTexture(texture, position.x - 50, position.y - 200, SDL_FLIP_NONE, &rect);
	

	return true;
}


bool NPCHerrera::CleanUp()
{
	//app->physics->GetWorld()->DestroyBody(pbody->body);
	app->tex->UnLoad(texture);

	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void NPCHerrera::OnCollision(PhysBody* physA, PhysBody* physB) {
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





