#include "Enemy_Boorok.h"
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
#include "Item_Viscera.h"
#include "Item_Diente.h"
#include <Optick/include/optick.h>
#include "Utils.cpp"


Enemy_Boorok::Enemy_Boorok() : Entity(EntityType::ENEMY_BOOROK) {
	name = ("boorok");
	state = EntityState::IDLE;
	currentState = state;
	nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(currentState)].next_state;

}

Enemy_Boorok::~Enemy_Boorok() {

}

bool Enemy_Boorok::Awake() {

	return true;
}

bool Enemy_Boorok::Start() {

	OPTICK_EVENT();

	TSprite = config.attribute("Tsprite").as_int();
	SpriteX = config.attribute("sprite_x").as_int();
	SpriteY = config.attribute("sprite_y").as_int();
	Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

	idleAnim.LoadAnim("boorok", "idleAnim", spritePositions);
	sleepAnim.LoadAnim("boorok", "sleepAnim", spritePositions);
	runAnim.LoadAnim("boorok", "runAnim", spritePositions);
	attackAnim.LoadAnim("boorok", "attackAnim", spritePositions);
	dieAnim.LoadAnim("boorok", "dieAnim", spritePositions);
	chargeAttackAnim.LoadAnim("boorok", "chargeAttackAnim", spritePositions);

	texture = app->tex->Load(config.attribute("texturePath").as_string());

	boorok_get_damage_fx = app->audio->LoadAudioFx("boorok_get_damage_fx");
	boorok_get_damageAlt_fx = app->audio->LoadAudioFx("boorok_get_damageAlt_fx");
	boorok_death_fx = app->audio->LoadAudioFx("boorok_death_fx");

	pbodyFoot = app->physics->CreateCircle(position.x, position.y, 30, bodyType::DYNAMIC);
	pbodyFoot->entity = this;
	pbodyFoot->listener = this;
	pbodyFoot->ctype = ColliderType::ENEMY;

	pbodySensor = app->physics->CreateRectangleSensor(position.x, position.y, 60, 100, bodyType::DYNAMIC);
	pbodySensor->entity = this;
	pbodySensor->listener = this;
	pbodySensor->ctype = ColliderType::UNKNOWN;

	originalPosition = app->map->WorldToMap(position.x, position.y);

	maxHealth = config.attribute("maxHealth").as_float();
	health = maxHealth;
	healthIncrement = 0.05;
	speed = config.attribute("speed").as_float();
	attackDamage = config.attribute("attackDamage").as_float();
	attackDistance = config.attribute("attackDistance").as_float();
	viewDistance = config.attribute("viewDistance").as_float();
	chargeattackDistance = config.attribute("chargeattackDistance").as_float();
	areaattackdamage = config.attribute("areaattackdamage").as_float();

	chargeAttackTimer.Start();
	recoveryTimer.Start();

	return true;
}

bool Enemy_Boorok::Update(float dt)
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
		nextState = EntityState::IDLE;
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
		DoNothing(dt, playerPos);
		break;
	default:
		break;
	}

	CheckPoison();


	currentState = nextState;
	currentAnimation->Update();
	return true;
}


bool Enemy_Boorok::PostUpdate() {

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
		app->render->DrawTexture(texture, position.x - 25, position.y - 100, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 120, position.y - 100, SDL_FLIP_NONE, &rect);
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


bool Enemy_Boorok::CleanUp()
{
	app->physics->GetWorld()->DestroyBody(pbodyFoot->body);
	app->physics->GetWorld()->DestroyBody(pbodySensor->body);
	app->tex->UnLoad(texture);
	lastPath.Clear();

	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}

void Enemy_Boorok::DoNothing(float dt, iPoint playerPos)
{
	//printf("Boorok Do nothing \n");
	if (dist(position, playerPos) > -1)
	{
		Sleeping();
	}
	chargeAttackTimer.Start();
	currentAnimation = &idleAnim;
}

void Enemy_Boorok::Chase(float dt, iPoint playerPos)
{
	currentAnimation = &runAnim;
	if (chargeAttackTimer.ReadSec() >= 5 && app->map->pathfinding->GetDistance(playerPos, position) <= chargeattackDistance * 32)
	{
		chargeAttack(playerPos);
		chargeAttackTimer.Start();
	}
	else
	{
		Boorokfinding(dt, playerPos);
	}
}

void Enemy_Boorok::Attack(float dt)
{
	//printf("Muur attacking");
	currentAnimation = &attackAnim;
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
	//sonido ataque
}

void Enemy_Boorok::Die() {

	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
	currentAnimation = &dieAnim;

	if (currentAnimation->HasFinished())
	{
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
		if (randomValue <= 0.35f) {
			Item_Viscera* visceras = (Item_Viscera*)app->entityManager->CreateEntity(EntityType::ITEM_VISCERAS);
			visceras->config = configNode.child("entities_data").child("item_viscera");
			visceras->position = iPoint(position.x, position.y);
			visceras->Start();
		}
		float randomValue2 = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

		// Determina si el item debe crearse basado en un 30% de probabilidad
		if (randomValue2 <= 0.30f) {
			Item_Diente* diente = (Item_Diente*)app->entityManager->CreateEntity(EntityType::ITEM_DIENTE);
			diente->config = configNode.child("entities_data").child("item_diente");
			diente->position = iPoint(position.x, position.y);
			diente->Start();
		}

		app->entityManager->GetPlayer()->maskZeroXP += 20;
		//printf("Current XP %i \n", app->entityManager->GetPlayer()->maskZeroXP);
	}
}

// L07 DONE 6: Define OnCollision function for the player. 
void Enemy_Boorok::OnCollision(PhysBody* physA, PhysBody* physB) {
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

bool Enemy_Boorok::Boorokfinding(float dt, iPoint playerPosP)
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

float Enemy_Boorok::GetHealth() const {
	return health;
}

void Enemy_Boorok::TakeDamage(float damage) {
	if (currentState != EntityState::DEAD && invulnerabilityTimer.ReadMSec() >= 500) {
		health -= damage;
		invulnerabilityTimer.Start();
		timerRecibirDanioColor.Start();
		app->audio->PlayRandomFx(boorok_get_damage_fx, boorok_get_damageAlt_fx, NULL);
	}
}

//VENENO <----------
void Enemy_Boorok::ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate) {
	this->poisonDamage = poisonDamage;
	this->poisonDuration = poisonDuration;
	this->poisonTickRate = poisonTickRate;

	this->poisoned = true;
	this->firstTimePoisonRecibed = true;

	poisonTimer.Start();
	poisonTickTimer.Start();


}

void Enemy_Boorok::CheckPoison() {
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

void Enemy_Boorok::Sleeping()
{
	//printf("Sleeping \n");

	pbodyFoot->body->SetLinearVelocity(b2Vec2(0, 0));
	currentAnimation = &sleepAnim;

	if (recoveryTimer.ReadSec() >= 0.5) {
		if (health < maxHealth) {
			health += health * healthIncrement;

			if (health > maxHealth) {
				health = maxHealth;
			}
		}
		recoveryTimer.Start();
	}
	//printf("Health: %f \n", health);
}

void Enemy_Boorok::chargeAttack(iPoint playerPos)
{
	currentAnimation = &chargeAttackAnim;

	printf("Charge attack");

	AreaAttack(playerPos);

	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);

	chargeAttackTimer.Start();


}

void Enemy_Boorok::AreaAttack(iPoint playerPos) {
	
	float distance = dist(position, playerPos);

	// Si el player esta dentro del radio del ataque
	if (distance <= chargeattackDistance * 32) {
		app->entityManager->GetPlayer()->TakeDamage(areaattackdamage);
		printf("Area attack \n");
	}
}
