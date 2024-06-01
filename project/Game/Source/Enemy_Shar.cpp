#include "Enemy_Shar.h"
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
#include "ParticleSystem.h"
#include "Map.h"
#include "Physics.h"
#include "Log.h"
#include <Optick/include/optick.h>


Enemy_Shar::Enemy_Shar() : Entity(EntityType::ENEMY_SHAR), maxHealth(80.0f), health(80.0f), speed(2.5f), attackDamage(50.0f){

	name = ("shar");
	state = EntityState::IDLE;
	currentState = state;
	nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(currentState)].next_state;

}

Enemy_Shar::~Enemy_Shar() {

}

bool Enemy_Shar::Awake() {

	return true;
}

bool Enemy_Shar::Start() {

	OPTICK_EVENT();
	//position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	TSprite = config.attribute("Tsprite").as_int();
	SpriteX = config.attribute("sprite_x").as_int();
	SpriteY = config.attribute("sprite_y").as_int();
	Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

	idleAnim.LoadAnim("osiris", "idleAnim_osiris", spritePositions);
	runAnim.LoadAnim("osiris", "runAnim_osiris", spritePositions);
	attackAnim.LoadAnim("osiris", "attackAnim_osiris", spritePositions);
	dieAnim.LoadAnim("osiris", "dieAnim_osiris", spritePositions);

	texture = app->tex->Load(config.attribute("texturePath").as_string());

	pbodyFoot = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
	pbodyFoot->entity = this;
	pbodyFoot->listener = this;
	pbodyFoot->ctype = ColliderType::ENEMY;

	attackDamage = 30;

	return true;
}

bool Enemy_Shar::Update(float dt)
{
	OPTICK_EVENT();

	iPoint playerPos = app->entityManager->GetPlayer()->position;

	if (playerPos.x < position.x)
	{
		isFacingLeft = true;
	}
	else(isFacingLeft = false);


	switch (nextState) {
	case EntityState::DEAD:
		Die(dt);
		break;
	case EntityState::ATTACKING:
		Attack(dt);
		break;
	case EntityState::RUNNING:
		Chase(dt);
		break;
	case EntityState::IDLE:
		DoNothing(dt);
		break;
	default:
		break;
	}

	if (health <= 0) {
		nextState = EntityState::DEAD;
	}
	else if (app->map->pathfinding->GetDistance(app->entityManager->GetPlayer()->position, position) <= 20 /*Cambiar*/) {
		nextState = EntityState::ATTACKING;
	}
	else if (app->map->pathfinding->GetDistance(app->entityManager->GetPlayer()->position, position) <= 400/*Cambiar*/) {
		nextState = EntityState::RUNNING;
	}
	else {
		nextState = EntityState::IDLE;
	}

	currentAnimation->Update();

	return true;
}


bool Enemy_Shar::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();



	if (isFacingLeft) {
		app->render->DrawTexture(texture, position.x - 40, position.y - 100, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 40, position.y - 100, SDL_FLIP_NONE, &rect);
	}

	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 16;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 16;

	//for (uint i = 0; i < lastPath.Count(); ++i)
	//{
	//	iPoint pos = app->map->MapToWorld(lastPath.At(i)->x, lastPath.At(i)->y);
	//	if (app->physics->debug == true) {
	//		app->render->DrawTexture(app->map->pathfinding->mouseTileTex, pos.x, pos.y, SDL_FLIP_NONE);
	//	}
	//}

	return true;

}


bool Enemy_Shar::CleanUp()
{
	app->entityManager->DestroyEntity(pbodyFoot->entity);
	app->physics->DestroyBody(pbodyFoot);
	app->tex->UnLoad(texture);

	blood = nullptr;

	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}

void Enemy_Shar::DoNothing(float dt)
{
	currentAnimation = &idleAnim;
	//printf("Osiris idle");
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
}

void Enemy_Shar::Chase(float dt)
{
	//printf("Osiris chasing");
	currentAnimation = &runAnim;
	Sharfinding(dt);
}

void Enemy_Shar::Attack(float dt)
{
	//printf("Osiris attacking");
	currentAnimation = &attackAnim;
	//sonido ataque
}

void Enemy_Shar::Die(float dt) {

	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	if (parseResult) {
		configNode = configFile.child("config");
	}

	fPoint pos((float)position.x, (float)position.y);
	blood = app->psystem->AddEmiter(pos, EMITTER_TYPE_ENEMY_BLOOD);

	ojo = (Item_Ojo*)app->entityManager->CreateEntity(EntityType::ITEM_OJO);
	ojo->config = configNode.child("entities_data").child("item_ojo");
	ojo->position = iPoint(position.x, position.y);
	ojo->Start();

	app->entityManager->DestroyEntity(this);
	app->physics->GetWorld()->DestroyBody(pbodyFoot->body);
	app->tex->UnLoad(texture);

	//Mask XP

	//Mask 0
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK0)
	{
		app->entityManager->GetPlayer()->maskZeroXP += 20;
		//printf("Current Mask 0 XP %i \n", app->entityManager->GetPlayer()->maskZeroXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK0)
	{
		app->entityManager->GetPlayer()->maskZeroXP += 20;
		//printf("Current Mask 0 XP %i \n", app->entityManager->GetPlayer()->maskZeroXP);
	}

	//Mask 1
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK1)
	{
		app->entityManager->GetPlayer()->maskOneXP += 20;
		//printf("Current Mask 1 XP %i \n", app->entityManager->GetPlayer()->maskOneXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK1)
	{
		app->entityManager->GetPlayer()->maskOneXP += 20;
		//printf("Current Mask 1 XP %i \n", app->entityManager->GetPlayer()->maskOneXP);
	}

	//Mask 2
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK2)
	{
		app->entityManager->GetPlayer()->maskTwoXP += 20;
		//printf("Current Mask 2 XP %i \n", app->entityManager->GetPlayer()->maskTwoXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK2)
	{
		app->entityManager->GetPlayer()->maskTwoXP += 20;
		//printf("Current Mask 2 XP %i \n", app->entityManager->GetPlayer()->maskTwoXP);
	}

	//Mask 3
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK3)
	{
		app->entityManager->GetPlayer()->maskThreeXP += 20;
		//printf("Current Mask 3 XP %i \n", app->entityManager->GetPlayer()->maskThreeXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK3)
	{
		app->entityManager->GetPlayer()->maskThreeXP += 20;
		//printf("Current Mask 3 XP %i \n", app->entityManager->GetPlayer()->maskThreeXP);
	}
}

// L07 DONE 6: Define OnCollision function for the player. 
void Enemy_Shar::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		//restar vida al player
		break;
	case ColliderType::PLAYER_ATTACK:
		LOG("Collision Player_Attack");
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}

void Enemy_Shar::SetPlayer(Player* player)
{
	this->player = player;
}

bool Enemy_Shar::Sharfinding(float dt)
{
	iPoint playerPos = app->map->WorldToMap(app->entityManager->GetPlayer()->position.x, app->entityManager->GetPlayer()->position.y);
	iPoint enemyPos = app->map->WorldToMap(position.x, position.y);

	app->map->pathfinding->CreatePath(enemyPos, playerPos); // Calcula el camino desde la posicion del enemigo hacia la posicion del jugador
	lastPath = *app->map->pathfinding->GetLastPath();

	//Get the latest calculated path and draw
	for (uint i = 0; i < lastPath.Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(lastPath.At(i)->x, lastPath.At(i)->y);
		if (app->physics->debug == true) {
			app->render->DrawTexture(app->map->pathfinding->mouseTileTex, pos.x, pos.y, SDL_FLIP_NONE);
		}
	}

	if (lastPath.Count() > 1) { // Asegate de que haya al menos una posicion en el camino

		if (app->map->pathfinding->GetDistance(app->entityManager->GetPlayer()->position, position) > 400) {
			pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
		}

		// Toma la primera posicion del camino como el objetivo al que el enemigo debe dirigirse
		iPoint targetPos = app->map->MapToWorld(lastPath.At(1)->x, lastPath.At(1)->y);

		// Calcula la dirección hacia el objetivo
		b2Vec2 direction(targetPos.x - position.x, targetPos.y - position.y);
		direction.Normalize();

		// Calcula la velocidad del movimiento
		b2Vec2 velocity(direction.x * speed, direction.y * speed);

		// Aplica la velocidad al cuerpo del enemigo
		pbodyFoot->body->SetLinearVelocity(velocity);

		// Determina si el enemigo est?mirando hacia la izquierda o hacia la derecha
		if (direction.x < 0) {
			isFacingLeft = true;
		}
		else {
			isFacingLeft = false;
		}

		isAttacking = false;
		attackAnim.Reset();

	}
	return true;
}

float Enemy_Shar::GetHealth() const {
	return health;
}

void Enemy_Shar::TakeDamage(float damage) {

	health -= damage;
	printf("Enemy_Shar has received  %f damage\n", damage);
}

