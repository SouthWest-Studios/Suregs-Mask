#include "Osiris.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Window.h"




Osiris::Osiris() : Entity(EntityType::OSIRIS)
{
	name.Create("osiris");

}

Osiris::~Osiris() {

}

bool Osiris::Awake() {






	return true;
}

bool Osiris::Start() {


	//position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	TSprite = config.attribute("Tsprite").as_int();
	SpriteX = config.attribute("sprite_x").as_int();
	SpriteY = config.attribute("sprite_y").as_int();
	Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

	idleAnim.LoadAnim("osiris", "idleAnim", spritePositions);
	runAnim.LoadAnim("osiris", "runAnim", spritePositions);


	texture = app->tex->Load(config.attribute("texturePath").as_string());

	pbody = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;

	return true;
}

bool Osiris::Update(float dt)
{
	nextState = EntityState::IDLE;



	/*if (inRodar) {
		Rodar(dt);

	}*/

	switch (nextState) {
	case EntityState::RUNNING:
		Run(dt);
		break;
	case EntityState::ATTACKING:
		Attack(dt);
		break;
	case EntityState::IDLE:
		DoNothing(dt);
		break;
	default:
		break;
	}
	//switch (state)
	//{
	//case EntityState::IDLE:
	//	currentAnimation = &idleAnim;
	//	break;
	//case EntityState::RUNNING:
	//	currentAnimation = &runAnim;
	//	break;
	//case EntityState::ATTACKING:
	//	//currentAnimation = &attackAnim;
	//	break;
	//default:
	//	//currentAnimation = &idleAnim;
	//	break;
	//}

	currentAnimation->Update();
	return true;
}


bool Osiris::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();


	if (isFacingLeft) {
		app->render->DrawTexture(texture, position.x - 50, position.y - 200, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 50, position.y - 200, SDL_FLIP_NONE, &rect);
	}

	return true;
}


bool Osiris::CleanUp()
{
	return true;
}

void Osiris::DoNothing(float dt)
{
	currentAnimation = &idleAnim;
	//printf("idle");
}

void Osiris::Run(float dt)
{
	//printf("runn");

	currentAnimation = &runAnim;
}

void Osiris::Attack(float dt)
{
}

// L07 DONE 6: Define OnCollision function for the player. 
void Osiris::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}


