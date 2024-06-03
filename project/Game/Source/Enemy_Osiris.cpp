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
#include "ParticleSystem.h"
#include "Map.h"
#include "Physics.h"
#include "Item_Hueso.h"
#include "BestiarioManager.h"
#include "EntityManager.h"
#include "Boss_Igory.h"
#include <Optick/include/optick.h>
#include "Utils.cpp"


 


Enemy_Osiris::Enemy_Osiris() : Entity(EntityType::ENEMY_OSIRIS){
	name = ("osiris");

	state = EntityState_Enemy::IDLE;
	nextState = EntityState_Enemy::IDLE;
	currentState = state;
	desiredState = nextState;
	nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;

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

	idleAnim.LoadAnim("osiris", "idleAnim", spritePositions);
	runAnim.LoadAnim("osiris", "runAnim", spritePositions);
	attackAnim.LoadAnim("osiris", "attackAnim", spritePositions);
	dieAnim.LoadAnim("osiris", "dieAnim", spritePositions);
	takeDamageAnim.LoadAnim("osiris", "takeDamagekAnim", spritePositions);
	reviveAnim.LoadAnim("osiris", "reviveAnim", spritePositions);



	texture = app->tex->Load(config.attribute("texturePath").as_string());

	osiris_get_damage_fx = app->audio->LoadAudioFx("osiris_get_damage_fx");
	osiris_death_fx = app->audio->LoadAudioFx("osiris_death_fx");

	pbodyFoot = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
	pbodyFoot->entity = this;
	pbodyFoot->listener = this;
	pbodyFoot->ctype = ColliderType::ENEMY;

	pbodySensor = app->physics->CreateRectangleSensor(position.x, position.y, 40, 60, bodyType::DYNAMIC);
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

	//app->entityManager->objectsToDraw.push_back({
	//	texture,
	//	position.x, // x
	//	position.y, // y
	//	position.y + 98, // anchorY
	//	100, // width
	//	100, // height
	//	NULL, // currentFrame
	//	isFacingLeft
	//});

	room = GetCurrentRoom();

	return true;
}

bool Enemy_Osiris::Update(float dt)
{
	OPTICK_EVENT();
	//printf("\nEnemy_Osiris");
	//Pone el sensor del cuerpo en su posicion
	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
	pbodySensor->body->SetTransform(b2Vec2(pbodyPos.p.x, pbodyPos.p.y - 1), 0);

	iPoint playerPos = app->entityManager->GetPlayer()->position;


	if (health <= 0)
	{
		
		if (currentState == EntityState_Enemy::DEAD) {
			desiredState = EntityState_Enemy::REVIVING;
		}
		else
		{
			desiredState = EntityState_Enemy::DEAD;
		}
		
	}
	else if (app->entityManager->GetIgory()->isDead) {
		health = 0;
	}
	else if (app->map->pathfinding->GetDistance(playerPos, position) <= attackDistance * 32)
	{
		desiredState = EntityState_Enemy::ATTACKING;
	}
	else if (app->map->pathfinding->GetDistance(playerPos, position) <= viewDistance*32)
	{
		desiredState = EntityState_Enemy::RUNNING;
	}
	else
	{
		desiredState = EntityState_Enemy::RUNNING;
	}

	UpdateAttackSensor(dt);

	stateMachine(dt, playerPos);

	//VENENO <----------
	CheckPoison();
	//VENENO ---------->

	currentAnimation->Update();
	return true;
}


bool Enemy_Osiris::PostUpdate() {

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
		app->render->DrawTexture(texture, position.x - 50, position.y - 95, 0.7f, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 85, position.y - 95, 0.7f, SDL_FLIP_NONE, &rect);
	}


	//Efecto da�o
	


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


bool Enemy_Osiris::CleanUp()
{
	app->physics->GetWorld()->DestroyBody(pbodyFoot->body);
	app->physics->GetWorld()->DestroyBody(pbodySensor->body);
	app->tex->UnLoad(texture);
	lastPath.Clear();

	blood = nullptr;

	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}

void Enemy_Osiris::DoNothing(float dt)
{
	currentAnimation = &idleAnim;
	//printf("Osiris idle");
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);

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
	if (canAttack) {

		pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);//No se mueve mientras ataca

		if (isFacingLeft) {
			sensor = app->physics->CreateRectangle(position.x + 65, position.y -15, 40, 70, bodyType::DYNAMIC);
			sensor->ctype = ColliderType::PROJECTILE;
			sensor->listener = this;
			sensor->body->GetFixtureList()->SetSensor(true);
		}
		else {
			sensor = app->physics->CreateRectangle(position.x - 35, position.y - 15, 40, 70, bodyType::DYNAMIC);
			sensor->ctype = ColliderType::PROJECTILE;
			sensor->listener = this;
			sensor->body->GetFixtureList()->SetSensor(true);
		}

		attackCooldownTimer.Start();
		canAttack = false;

		currentAnimation = &attackAnim;
	}

	//sonido ataque
}


void Enemy_Osiris::UpdateAttackSensor(float dt)
{
	if (!canAttack && attackCooldownTimer.ReadMSec() >= 350.0f) {
		canAttack = true;
	}

	if (sensor != nullptr && canAttack) {
		canAttack = false;
		attackCooldownTimer.Start();

		if (attackSensorTimer.ReadMSec() >= 350.0f) {
			app->physics->DestroyBody(sensor);
			sensor = nullptr;
		}
	}
}


void Enemy_Osiris::Die() {

	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
	currentAnimation = &dieAnim;

	if (!hasRevived)
	{
		desiredState = EntityState_Enemy::REVIVING;
		Revive();
	}
	else
	{
		app->audio->PlayFx(osiris_death_fx);

		fPoint pos((float)position.x, (float)position.y);
		blood = app->psystem->AddEmiter(pos, EMITTER_TYPE_ENEMY_BLOOD);

		app->entityManager->DestroyEntity(this);
		app->physics->GetWorld()->DestroyBody(pbodyFoot->body);
		app->physics->GetWorld()->DestroyBody(pbodySensor->body);
		app->tex->UnLoad(texture);
		//CleanUp();
		pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
		if (parseResult) {
			configNode = configFile.child("config");
		}
		float randomValue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

		// Determina si el �tem debe crearse basado en un 30% de probabilidad
		if (randomValue <= 0.25f) {
			Item_Hueso* hueso = (Item_Hueso*)app->entityManager->CreateEntity(EntityType::ITEM_HUESO);
			hueso->config = configNode.child("entities_data").child("item_hueso");
			hueso->position = iPoint(position.x, position.y);
			hueso->Start();
		}
		
			app->bestiarioManager->CreateItem("osiris");
			
		
		//Mask 0
		if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK0)
		{
			app->entityManager->GetPlayer()->maskZeroXP += 40;
			//printf("Current Mask 0 XP %i \n", app->entityManager->GetPlayer()->maskZeroXP);
		}

		if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK0)
		{
			app->entityManager->GetPlayer()->maskZeroXP += 40;
			//printf("Current Mask 0 XP %i \n", app->entityManager->GetPlayer()->maskZeroXP);
		}

		//Mask 1
		if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK1)
		{
			app->entityManager->GetPlayer()->maskOneXP += 40;
			//printf("Current Mask 1 XP %i \n", app->entityManager->GetPlayer()->maskOneXP);
		}

		if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK1)
		{
			app->entityManager->GetPlayer()->maskOneXP += 40;
			//printf("Current Mask 1 XP %i \n", app->entityManager->GetPlayer()->maskOneXP);
		}

		//Mask 2
		if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK2)
		{
			app->entityManager->GetPlayer()->maskTwoXP += 40;
			//printf("Current Mask 2 XP %i \n", app->entityManager->GetPlayer()->maskTwoXP);
		}

		if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK2)
		{
			app->entityManager->GetPlayer()->maskTwoXP += 40;
			//printf("Current Mask 2 XP %i \n", app->entityManager->GetPlayer()->maskTwoXP);
		}

		//Mask 3
		if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK3)
		{
			app->entityManager->GetPlayer()->maskThreeXP += 40;
			//printf("Current Mask 3 XP %i \n", app->entityManager->GetPlayer()->maskThreeXP);
		}

		if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK3)
		{
			app->entityManager->GetPlayer()->maskThreeXP += 40;
			//printf("Current Mask 3 XP %i \n", app->entityManager->GetPlayer()->maskThreeXP);
		}
		
		//printf("\n SSS: %d", app->entityManager->GetIgory()->position);
		if (app->entityManager->GetIgory()->playerInFight) {
			app->map->DestroyEntity(this);
		}
		
	}

}

void Enemy_Osiris::Revive()
{
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
	currentAnimation = &reviveAnim;
	if (!tempo)
	{
		reviveTimer.Start();
		tempo = true;
		isReviving = true;
	}

	if (reviveTimer.CountDown(4) <= 0)
	{

		health = maxHealth;
		hasRevived = true;
		isReviving = false;
		//currentState = EntityState_Enemy::IDLE;
		desiredState = EntityState_Enemy::IDLE;

		return;
	}
	desiredState = EntityState_Enemy::REVIVING;
}


// L07 DONE 6: Define OnCollision function for the player. 
void Enemy_Osiris::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::PLAYER:
		if (physA->ctype == ColliderType::PROJECTILE) {
			app->entityManager->GetPlayer()->TakeDamage(attackDamage);
		}
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
		if (app->entityManager->GetIgory()->playerInFight) {
			app->map->pathfinding->CreatePath(enemyPos, playerPos); // Calcula el camino desde la posicion del enemigo hacia la posicion del jugador
			lastPath = *app->map->pathfinding->GetLastPath();
		}
		else
		{
			app->map->pathfinding->CreatePath(enemyPos, originalPosition); // Calcula el camino desde la posicion del enemigo hacia la posicion del jugador
			lastPath = *app->map->pathfinding->GetLastPath();
		}
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

float Enemy_Osiris::GetHealth() const {
	return health;
}

void Enemy_Osiris::TakeDamage(float damage) {

	if (currentState != EntityState_Enemy::DEAD && invulnerabilityTimer.ReadMSec() >= 500) {
		health -= damage;
		invulnerabilityTimer.Start();
		timerRecibirDanioColor.Start();

		printf("Enemy_Osiris has received  %f damage\n", damage);
		if (currentState == EntityState_Enemy::REVIVING) {
			if (!hasRevived) {
				hasRevived = true;
			}
		}
		
	}
	app->audio->PlayFx(osiris_get_damage_fx);
}

void Enemy_Osiris::stateMachine(float dt, iPoint playerPos)
{
	//printf("\ncurrentState: %d, desiredState: %d", static_cast<int>(currentState), static_cast<int>(desiredState));
	nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;
	switch (nextState) {
	case EntityState_Enemy::IDLE:
		DoNothing(dt);
		break;
	case EntityState_Enemy::RUNNING:
		Chase(dt, playerPos);
		break;
	case EntityState_Enemy::ATTACKING:
		Attack(dt);
		break;
	case EntityState_Enemy::DEAD:
		Die();
		break;
	case EntityState_Enemy::REVIVING:
		Revive();
		break;
	case EntityState_Enemy::DASHI:
		break;
	case EntityState_Enemy::NONE:

		desiredState = EntityState_Enemy::IDLE;
		break;

	default:
		break;
	}
	currentState = nextState;

}


//VENENO <----------
void Enemy_Osiris::ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate) {
	this->poisonDamage = poisonDamage;
	this->poisonDuration = poisonDuration;
	this->poisonTickRate = poisonTickRate;
	
	this->poisoned = true;
	this->firstTimePoisonRecibed = true;

	poisonTimer.Start();
	poisonTickTimer.Start();


}

void Enemy_Osiris::CheckPoison() {
	float epsilon = 0.1f; //Para margen de error

    // Aplicar el primer tick de daño inmediatamente (si no, el primer tick no se aplica en el segundo 0.0)
	if(firstTimePoisonRecibed) {
		if (currentState != EntityState_Enemy::DEAD) {
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
            if (currentState != EntityState_Enemy::DEAD) {
                health -= poisonDamage;
                invulnerabilityTimer.Start();
                timerRecibirDanioColor.Start();

                printf("Enemy_Osiris has received  %f damage of poison\n", poisonDamage);
            }
        }
    }
}
//VENENO ---------->

MapObject* Enemy_Osiris::GetCurrentRoom()
{
	//salas pequeñas
	for (ListItem<MapObject*>* item = app->map->smallRoomsList.start; item != nullptr; item = item->next)
	{
		MapObject* room = item->data;

		// el jugador está dentro de la sala
		if (position.x >= room->x && position.x <= room->x + room->width &&
			position.y >= room->y && position.y <= room->y + room->height)
		{
			return room;
		}
	}

	//salas grandes
	for (ListItem<MapObject*>* item = app->map->largeRoomsList.start; item != nullptr; item = item->next)
	{
		MapObject* room = item->data;

		// el jugador está dentro de la sala
		if (position.x >= room->x && position.x <= room->x + room->width &&
			position.y >= room->y && position.y <= room->y + room->height)
		{
			return room;
		}
	}

	//salas l
	for (ListItem<MapObject*>* item = app->map->LRoomsList.start; item != nullptr; item = item->next)
	{
		MapObject* room = item->data;

		// el jugador está dentro de la sala
		if (position.x >= room->x && position.x <= room->x + room->width &&
			position.y >= room->y && position.y <= room->y + room->height)
		{
			return room;
		}
	}

	return nullptr;
}