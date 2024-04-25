#include "Enemy_Osiris.h"
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
#include <Optick/include/optick.h>
#include "Utils.cpp"


 


Enemy_Osiris::Enemy_Osiris() : Entity(EntityType::ENEMY_OSIRIS), maxHealth(250.0f), health(250.0f), speed(2.5f), attackDamage(50.0f){
	name.Create("osiris");

}

Enemy_Osiris::~Enemy_Osiris() {

}

bool Enemy_Osiris::Awake() {

	return true;
}

bool Enemy_Osiris::Start() {

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

	pbody = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
	pbody->entity = this;
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;


	originalPosition = app->map->WorldToMap(position.x, position.y);

	attackDamage = 20;
	viewDistance = 10;

	return true;
}

bool Enemy_Osiris::Update(float dt)
{
	OPTICK_EVENT();

	iPoint playerPos = app->entityManager->GetPlayer()->position;

	if (playerPos.x < position.x)
	{
		isFacingLeft = true;
	}
	else
	{
		isFacingLeft = false;
	}


	

	if (health <= 0)
	{
		if (nextState == EntityState::DEAD)
		{
			nextState = EntityState::REVIVING;
		}
		else
		{
			nextState = EntityState::DEAD;
		}
	}
	else if (app->map->pathfinding->GetDistance(app->entityManager->GetPlayer()->position, position) <= 20)
	{
		nextState = EntityState::ATTACKING;
	}
	else if (app->map->pathfinding->GetDistance(app->entityManager->GetPlayer()->position, position) <= 400)
	{
		nextState = EntityState::RUNNING;
	}
	else
	{
		nextState = EntityState::RUNNING;
	}

	


	switch (nextState)
	{
	case EntityState::RUNNING:
		Chase(dt, playerPos);
		break;
	case EntityState::ATTACKING:
		Attack(dt);
		break;
	case EntityState::DEAD:
		Die();
		break;
	case EntityState::REVIVING:
		Revive();
		break;
	case EntityState::IDLE:
		DoNothing(dt);
		break;
	default:
		break;
	}

	currentState = nextState;
	currentAnimation->Update();
	return true;
}


bool Enemy_Osiris::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();


	if (isFacingLeft) {
		app->render->DrawTexture(texture, position.x - 50, position.y - 25, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 50, position.y - 25, SDL_FLIP_NONE, &rect);
	}

	for (uint i = 0; i < lastPath.Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(lastPath.At(i)->x, lastPath.At(i)->y);
		if (app->physics->debug == true) {
			app->render->DrawTexture(app->map->pathfinding->mouseTileTex, pos.x, pos.y, SDL_FLIP_NONE);
		}
	}

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 16;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 16;
	return true;
}


bool Enemy_Osiris::CleanUp()
{
	app->physics->GetWorld()->DestroyBody(pbody->body);
	app->tex->UnLoad(texture);

	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}

void Enemy_Osiris::DoNothing(float dt)
{
	currentAnimation = &idleAnim;
	//printf("Osiris idle");
	pbody->body->SetLinearVelocity(b2Vec2_zero);

}

void Enemy_Osiris::Chase(float dt, iPoint playerPos)
{
	//printf("Osiris chasing");
	currentAnimation = &runAnim;
	Osirisfinding(dt, playerPos);

}

void Enemy_Osiris::Attack(float dt)
{
	//printf("Osiris attacking");
	currentAnimation = &attackAnim;		
	//sonido ataque
}

void Enemy_Osiris::Die() {
	pbody->body->SetLinearVelocity(b2Vec2_zero);

	if (!hasRevived)
	{
		nextState = EntityState::REVIVING;
		Revive();
	}
	else
	{
		app->entityManager->DestroyEntity(this);
		app->physics->GetWorld()->DestroyBody(pbody->body);
		app->tex->UnLoad(texture);
	}

}

void Enemy_Osiris::Revive()
{
	pbody->body->SetLinearVelocity(b2Vec2_zero);
	printf("Inside Revive function\n");

	if (!tempo)
	{
		reviveTimer.Start();
		tempo = true;
	}

	if (reviveTimer.CountDown(4) <= 0)
	{
		printf("Osiris: Time to revive, setting next state to IDLE\n");

		health = maxHealth;
		hasRevived = true;

		currentState = EntityState::IDLE;
		nextState = EntityState::IDLE;

		return;
	}
	printf("Condition not met for revival\n");
	nextState = EntityState::REVIVING;
}


// L07 DONE 6: Define OnCollision function for the player. 
void Enemy_Osiris::OnCollision(PhysBody* physA, PhysBody* physB) {
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
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}

bool Enemy_Osiris::Osirisfinding(float dt, iPoint playerPosP)
{
	iPoint playerPos = app->map->WorldToMap(playerPosP.x, playerPosP.y);
	iPoint enemyPos = app->map->WorldToMap(position.x, position.y);


	if (dist(playerPos, enemyPos) < viewDistance) {
		app->map->pathfinding->CreatePath(enemyPos, playerPos); // Calcula el camino desde la posicion del enemigo hacia la posicion del jugador
		lastPath = *app->map->pathfinding->GetLastPath();
	}
	else {
		app->map->pathfinding->CreatePath(enemyPos, originalPosition); // Calcula el camino desde la posicion del enemigo hacia la posicion del jugador
		lastPath = *app->map->pathfinding->GetLastPath();
	}





	b2Vec2 velocity = b2Vec2(0, 0);

	//Get the latest calculated path and draw
	

	if (lastPath.Count() > 1) { // Asegate de que haya al menos una posicion en el camino

		// Toma la primera posicion del camino como el objetivo al que el enemigo debe dirigirse
		iPoint targetPos = app->map->MapToWorld(lastPath.At(1)->x, lastPath.At(1)->y);

		// Calcula la direccion hacia el objetivo
		b2Vec2 direction(targetPos.x - position.x, targetPos.y - position.y);
		direction.Normalize();

		// Calcula la velocidad del movimiento
		velocity = b2Vec2(direction.x * speed, direction.y * speed);

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
	else {
		LOG("HA LLEGADO AL DESTINO");
	}

	// Aplica la velocidad al cuerpo del enemigo
	pbody->body->SetLinearVelocity(velocity);

	return true;
}

float Enemy_Osiris::GetHealth() const {
	return health;
}

void Enemy_Osiris::TakeDamage(float damage) {
	if (currentState != EntityState::DEAD) {
		health -= damage;
		printf("Enemy_Osiris has received  %f damage\n", damage);
	}
}

