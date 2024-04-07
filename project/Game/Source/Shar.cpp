#include "Shar.h"
#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene_Testing.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Window.h"
#include "Pathfinding.h"
#include "Map.h"
#include "Physics.h"




Shar::Shar() : Entity(EntityType::ENEMY_SHAR)
{
	name.Create("shar");

}

Shar::~Shar() {

}

bool Shar::Awake() {

	return true;
}

bool Shar::Start() {


	//position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	TSprite = config.attribute("Tsprite").as_int();
	SpriteX = config.attribute("sprite_x").as_int();
	SpriteY = config.attribute("sprite_y").as_int();
	Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

	idleAnim.LoadAnim("osiris", "idleAnim", spritePositions);
	runAnim.LoadAnim("osiris", "runAnim", spritePositions);
	attackAnim.LoadAnim("osiris", "attackAnim", spritePositions);
	dieAnim.LoadAnim("osiris", "dieAnim", spritePositions);

	texture = app->tex->Load(config.attribute("texturePath").as_string());

	pbody = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;


	//Estadisticas
	health = 100.0f;
	maxHealth = 100.0f;
	speed = 0.2f;

	return true;
}

bool Shar::Update(float dt)
{
	iPoint playerPos = app->entityManager->GetPlayer()->position;

	if (playerPos.x > position.x)
	{
		isFacingLeft = true;
	}
	else(isFacingLeft = false);


	if (health <= 0) {
		nextState = EntityState::DEAD;
	}
	else if (abs(playerPos.x - position.x) <= 30 && abs(playerPos.y - position.y) <= 30 /*cambiar si hace falta*/) {
		nextState = EntityState::ATTACKING;
	}
	else if (abs(playerPos.x - position.x) <= 100 && abs(playerPos.y - position.y) <= 100/*cambiar si hace falta*/) {
		nextState = EntityState::RUNNING;
	}
	else {
		nextState = EntityState::IDLE;
	}

	switch (nextState) {
	case EntityState::RUNNING:
		Chase(dt);
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


	currentAnimation->Update();
	return true;
}


bool Shar::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();



	if (isFacingLeft) {
		app->render->DrawTexture(texture, position.x - 40, position.y - 100, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 40, position.y - 100, SDL_FLIP_NONE, &rect);
	}

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 16;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 16;
	return true;
}


bool Shar::CleanUp()
{
	return true;
}

void Shar::DoNothing(float dt)
{
	currentAnimation = &idleAnim;
	//printf("Osiris idle");
}

void Shar::Chase(float dt)
{
	//printf("Osiris chasing");
	currentAnimation = &runAnim;
	//iPoint playerPos = app->entityManager->GetPlayer()->position;
	//path->CreatePath(position, playerPos);
}

void Shar::Attack(float dt)
{
	//printf("Osiris attacking");
	currentAnimation = &attackAnim;
	//sonido ataque
}

void Shar::Die(float dt) {
	state = EntityState::DEAD;
}

// L07 DONE 6: Define OnCollision function for the player. 
void Shar::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		//restar vida al player
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}

void Shar::SetPlayer(Player* player)
{
	this->player = player;
}


