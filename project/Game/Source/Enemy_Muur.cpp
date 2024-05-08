﻿#include "Enemy_Muur.h"
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
#include "Item_Hueso.h"
#include "Item_Cola.h"
#include <Optick/include/optick.h>
#include "Utils.cpp"





Enemy_Muur::Enemy_Muur() : Entity(EntityType::ENEMY_MUUR) {
	name.Create("muur");

}

Enemy_Muur::~Enemy_Muur() {

}

bool Enemy_Muur::Awake() {

	return true;
}

bool Enemy_Muur::Start() {

	OPTICK_EVENT();

	TSprite = config.attribute("Tsprite").as_int();
	SpriteX = config.attribute("sprite_x").as_int();
	SpriteY = config.attribute("sprite_y").as_int();
	Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

	idleAnim.LoadAnim("muur", "idleAnim", spritePositions);
	runAnim.LoadAnim("muur", "runAnim", spritePositions);
	attackAnim.LoadAnim("muur", "attackAnim", spritePositions);
	chargeAnim.LoadAnim("muur", "chargeAnim", spritePositions);
	stunAnim.LoadAnim("muur", "stunAnim", spritePositions);
	dieAnim.LoadAnim("muur", "dieAnim", spritePositions);

	texture = app->tex->Load(config.attribute("texturePath").as_string());

	muur_get_damage_fx = app->audio->LoadAudioFx("muur_get_damage_fx");

	pbodyFoot = app->physics->CreateCircle(position.x, position.y, 15, bodyType::DYNAMIC);
	pbodyFoot->entity = this;
	pbodyFoot->listener = this;
	pbodyFoot->ctype = ColliderType::ENEMY;

	pbodySensor = app->physics->CreateRectangleSensor(position.x, position.y + 10, 10, 15, bodyType::DYNAMIC);
	pbodySensor->entity = this;
	pbodySensor->listener = this;
	pbodySensor->ctype = ColliderType::UNKNOWN;

	originalPosition = app->map->WorldToMap(position.x, position.y);

	maxHealth = config.attribute("maxHealth").as_float();
	health = maxHealth;
	speed = config.attribute("speed").as_float();
	attackDamage = config.attribute("attackDamage").as_float();
	attackDistance = config.attribute("attackDistance").as_float();
	viewDistance = config.attribute("viewDistance").as_float();
	chargeattackDistance = config.attribute("chargeattackDistance").as_float();

	chargeTimer.Start();


	return true;
}

bool Enemy_Muur::Update(float dt)
{
	OPTICK_EVENT();


	//Pone el sensor del cuerpo en su posicion
	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
	pbodySensor->body->SetTransform(b2Vec2(pbodyPos.p.x, pbodyPos.p.y - 1), 0);



	iPoint playerPos = app->entityManager->GetPlayer()->position;

	if (health <= 0)
	{
		nextState = EntityState::DEAD;
	}
	else if (app->map->pathfinding->GetDistance(playerPos, position) <= viewDistance * 32)
	{
		nextState = EntityState::RUNNING;
	}
	else if (app->map->pathfinding->GetDistance(playerPos, position) <= attackDistance * 32)
	{
		nextState = EntityState::ATTACKING;
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
	case EntityState::IDLE:
		DoNothing(dt);
		break;
	default:
		break;
	}

	if (charging && dist(Antposition, position) > 350)
	{
		Stunned(dt);
		isStunned = true;
	}

	if (chargeTimer.ReadSec() >= 5)
	{
		charging = false;
	}

	CheckPoison();


	currentState = nextState;
	currentAnimation->Update();
	return true;
}


bool Enemy_Muur::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();


	if (timerRecibirDanioColor.ReadMSec() <= 100) {
		float alpha = (100 - timerRecibirDanioColor.ReadMSec()) / 100;
		SDL_SetTextureAlphaMod(texture, static_cast<Uint8>(255 * alpha)); // Ajusta la opacidad

	}
	else {
		SDL_SetTextureAlphaMod(texture, 255);
	}



	if (isFacingLeft) {
		app->render->DrawTexture(texture, position.x, position.y - 30, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 25, position.y - 30, SDL_FLIP_NONE, &rect);
	}


	for (uint i = 0; i < lastPath.Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(lastPath.At(i)->x, lastPath.At(i)->y);
		if (app->physics->debug == true) {
			app->render->DrawTexture(app->map->pathfinding->mouseTileTex, pos.x, pos.y, SDL_FLIP_NONE);
		}
	}

	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 16;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 16;
	return true;
}


bool Enemy_Muur::CleanUp()
{
	app->physics->GetWorld()->DestroyBody(pbodyFoot->body);
	app->physics->GetWorld()->DestroyBody(pbodySensor->body);
	app->tex->UnLoad(texture);
	lastPath.Clear();

	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}

void Enemy_Muur::DoNothing(float dt)
{
	currentAnimation = &idleAnim;
	//printf("Muur idle");
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);

}

void Enemy_Muur::Chase(float dt, iPoint playerPos)
{
	//printf("Muur chasing");
	currentAnimation = &runAnim;
	if(chargeTimer.ReadSec() >= 5 && app->map->pathfinding->GetDistance(playerPos, position) <= chargeattackDistance * 32 && app->map->pathfinding->GetDistance(playerPos, position) >= (attackDistance + 5) * 32)
	{
		Antposition = position;
		Charge(dt, playerPos);
	}
	else if(!charging)
	{
		Muurfinding(dt, playerPos);
	}

}

void Enemy_Muur::Attack(float dt)
{
	//printf("Muur attacking");
	currentAnimation = &attackAnim;
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
	//sonido ataque
}

void Enemy_Muur::Die() {
	app->audio->PlayFx(muur_get_damage_fx);

	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
	currentAnimation = &dieAnim;

	app->entityManager->DestroyEntity(this);
	app->physics->GetWorld()->DestroyBody(pbodyFoot->body);
	app->physics->GetWorld()->DestroyBody(pbodySensor->body);
	app->tex->UnLoad(texture);


	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	if (parseResult) {
		configNode = configFile.child("config");
	}
	float randomValue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

	// Determina si el item debe crearse basado en un 30% de probabilidad
	if (randomValue <= 0.30f) {
		Item_Cola* cola = (Item_Cola*)app->entityManager->CreateEntity(EntityType::ITEM_COLA);
		cola->config = configNode.child("entities_data").child("item_cola");
		cola->position = iPoint(position.x, position.y);
		cola->Start();
	}
	app->entityManager->GetPlayer()->playerXP += 20;
	//printf("Current XP %i \n", app->entityManager->GetPlayer()->playerXP);


}

// L07 DONE 6: Define OnCollision function for the player. 
void Enemy_Muur::OnCollision(PhysBody* physA, PhysBody* physB) {
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

bool Enemy_Muur::Muurfinding(float dt, iPoint playerPosP)
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
		isFacingLeft = (direction.x >= 0);


		isAttacking = false;
		attackAnim.Reset();

	}
	/*else {
		LOG("HA LLEGADO AL DESTINO");
	}*/

	// Aplica la velocidad al cuerpo del enemigo
	pbodyFoot->body->SetLinearVelocity(velocity);

	return true;
}

float Enemy_Muur::GetHealth() const {
	return health;
}

void Enemy_Muur::TakeDamage(float damage) {
	if (currentState != EntityState::DEAD && invulnerabilityTimer.ReadMSec() >= 500) {
		health -= damage;
		invulnerabilityTimer.Start();
		timerRecibirDanioColor.Start();
	}
}

//VENENO <----------
void Enemy_Muur::ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate) {
	this->poisonDamage = poisonDamage;
	this->poisonDuration = poisonDuration;
	this->poisonTickRate = poisonTickRate;

	this->poisoned = true;
	this->firstTimePoisonRecibed = true;

	poisonTimer.Start();
	poisonTickTimer.Start();


}

void Enemy_Muur::CheckPoison() {
	float epsilon = 0.1f; //Para margen de error

	// Aplicar el primer tick de daño inmediatamente (si no, el primer tick no se aplica en el segundo 0.0)
	if (firstTimePoisonRecibed) {
		if (currentState != EntityState::DEAD) {
			health -= poisonDamage;
			invulnerabilityTimer.Start();
			timerRecibirDanioColor.Start();

			printf("Enemy_Osiris has received  %f damage of poison\n", poisonDamage);
		}
		firstTimePoisonRecibed = false;
	}

	if (poisonTimer.ReadSec() <= poisonDuration + epsilon && poisoned) {
		if (poisonTickTimer.ReadSec() >= poisonTickRate) {
			poisonTickTimer.Start(); // Reiniciar el temporizador de ticks de veneno
			if (currentState != EntityState::DEAD) {
				health -= poisonDamage;
				invulnerabilityTimer.Start();
				timerRecibirDanioColor.Start();

				printf("Enemy_Osiris has received  %f damage of poison\n", poisonDamage);
			}
		}
	}
}
//VENENO ---------->

void Enemy_Muur::Charge(float dt, iPoint playerPos) {
	printf("charge");
	currentAnimation = &chargeAnim;
	charging = true;

	b2Vec2 direction(playerPos.x - position.x, playerPos.y - position.y);
	direction.Normalize();

	b2Vec2 impulse = b2Vec2(direction.x * 5, direction.y * 5);
	pbodyFoot->body->ApplyLinearImpulse(impulse, pbodyFoot->body->GetWorldCenter(), true);

	stunTimer.Start();
	chargeTimer.Start();
}


void Enemy_Muur::Stunned(float dt) {
	if (stunTimer.ReadSec() <= 2) {
		printf("stunned");
		currentAnimation = &stunAnim;
		pbodyFoot->body->SetLinearVelocity(b2Vec2(0, 0));
	}
	else {
		stunTimer.Start(); 
		nextState = EntityState::IDLE; 
		isStunned = false;  
		charging = false;  
	}
}
