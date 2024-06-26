#include "Enemy_Khurt_Variation.h"
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
#include "Item_Garra.h"
#include "BestiarioManager.h"
#include "InventoryManager.h"
#include <Optick/include/optick.h>
#include "Utils.cpp"


Enemy_Khurt_Variation::Enemy_Khurt_Variation() : Entity(EntityType::ENEMY_KHURT_VARIATION) {
	name = ("khurt_variation");
	state = EntityState_Khurt::IDLE;
	currentState = state;
	nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(currentState)].next_state;

}

Enemy_Khurt_Variation::~Enemy_Khurt_Variation() {

}

bool Enemy_Khurt_Variation::Awake() {

	return true;
}

bool Enemy_Khurt_Variation::Start() {

	OPTICK_EVENT();
	//position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	TSprite = config.attribute("Tsprite").as_int();
	SpriteX = config.attribute("sprite_x").as_int();
	SpriteY = config.attribute("sprite_y").as_int();
	Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

	idleAnim.LoadAnim("khurt", "idleAnim", spritePositions);
	chargeAnim.LoadAnim("khurt", "chargeAnim", spritePositions);
	stunAnim.LoadAnim("khurt", "stunAnim", spritePositions);
	/*runAnim.LoadAnim("khurt", "runAnim", spritePositions);*/
	underAnim_start.LoadAnim("khurt", "underAnim_start", spritePositions);
	underAnim_process.LoadAnim("khurt", "underAnim_process", spritePositions);
	underAnim_end.LoadAnim("khurt", "underAnim_end", spritePositions);
	dieAnim.LoadAnim("khurt", "dieAnim", spritePositions);


	//texture = app->tex->Load(config.attribute("texturePath").as_string());
	texture = app->entityManager->textureKhurt;

	khurt_get_damage_fx = app->audio->LoadAudioFx("khurt_get_damage_fx");
	khurt_get_damageAlt_fx = app->audio->LoadAudioFx("khurt_get_damageAlt_fx");

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
	chargeAttackDistance = config.attribute("chargeAttackDistance").as_float();

	chargeTimer.Start();

	////printf("Speed: %f", speed);

	room = GetCurrentRoom();

	return true;
}

bool Enemy_Khurt_Variation::Update(float dt)
{
	OPTICK_EVENT();
	//Pone el sensor del cuerpo en su posicion
	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
	pbodySensor->body->SetTransform(b2Vec2(pbodyPos.p.x, pbodyPos.p.y - 1), 0);

	iPoint playerPos = app->entityManager->GetPlayer()->position;

	if (health <= 0)
	{
		desiredState = EntityState_Khurt::DEAD;
	}
	else if (app->entityManager->GetIgory() != nullptr && app->entityManager->GetIgory()->isDead) {
		health = 0;
	}
	else if (app->map->pathfinding->GetDistance(playerPos, position) <= attackDistance * 32)
	{
		//printf("ATTACKING \n");
		underAnim_start.Reset();
		desiredState = EntityState_Khurt::ATTACKING;
		isUnderground = false;
	}
	else if (app->map->pathfinding->GetDistance(playerPos, position) <= viewDistance * 32)
	{
		//printf("RUNNING \n");
		underAnim_end.Reset();
		desiredState = EntityState_Khurt::RUNNING;
		isUnderground = true;

	}
	else
	{
		desiredState = EntityState_Khurt::IDLE;
		isUnderground = false;
	}
	//if (charging && dist(Antposition, position) > 350 || charging && timechargingTimer.ReadSec() > 0.8)
	//{
	//	stunned = true;
	//	Stunned(dt);
	//}

	//if (chargeTimer.ReadSec() >= 5)
	//{
	//	charging = false;
	//}

	stateMachine(dt, playerPos);

	CheckPoison();

	currentState = nextState;
	currentAnimation->Update();
	return true;
}


bool Enemy_Khurt_Variation::PostUpdate() {

	SDL_SetTextureColorMod(texture, 198, 115, 255);

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
		app->render->DrawTexture(texture, position.x - 25, position.y - 65, 0.5f, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 40, position.y - 65, 0.5f, SDL_FLIP_NONE, &rect);
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


bool Enemy_Khurt_Variation::CleanUp()
{

	app->entityManager->DestroyEntity(this);
	app->physics->DestroyBody(pbodyFoot);
	pbodyFoot = nullptr;
	app->physics->DestroyBody(pbodySensor);
	pbodySensor = nullptr;
	//app->tex->UnLoad(texture);

	lastPath.Clear();

	blood = nullptr;

	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}

void Enemy_Khurt_Variation::DoNothing(float dt)
{
	currentAnimation = &idleAnim;
	////printf("Khurt idle");
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);

}

void Enemy_Khurt_Variation::DigUnderground()
{
	currentAnimation = &underAnim_start;
	isUnderground = true;
	if (underAnim_start.HasFinished()) {
		desiredState = EntityState_Khurt::MOVING_UNDERGROUND;
		//printf("MOVING UNDERGROUND \n");
	}
}

void Enemy_Khurt_Variation::MoveUnderground(float dt, iPoint playerPos)
{
	////printf("Khurt chasing");
	currentAnimation = &underAnim_start;
	if (underAnim_start.HasFinished()) {
		currentAnimation = &underAnim_process;
		Khurtfinding(dt, playerPos);
	}


	//if (chargeTimer.ReadSec() >= 5 && app->map->pathfinding->GetDistance(playerPos, position) <= chargeAttackDistance * 32 && app->map->pathfinding->GetDistance(playerPos, position) >= (attackDistance + 5) * 32)
	//{
	//	Antposition = position;
	//	desiredState = EntityState_Khurt::DIGGING_OUT;
	//	/*DigOut(dt, playerPos);*/
	//}
	//else if (!charging)
	//{
	//	Khurtfinding(dt, playerPos);
	//}
}

void Enemy_Khurt_Variation::DigOut(float dt, iPoint playerPos)
{
	currentAnimation = &underAnim_end;
	isUnderground = false;
	if (underAnim_end.HasFinished()) {
		/*Charge(dt, playerPos);*/
		desiredState = EntityState_Khurt::ATTACKING;
		//printf("ATTACKING \n");
	}
}

void Enemy_Khurt_Variation::Charge(float dt, iPoint playerPos)
{
}

void Enemy_Khurt_Variation::Stunned(float dt)
{
	stunned = true;
	pbodyFoot->body->SetLinearVelocity(b2Vec2(0, 0));
	currentAnimation = &stunAnim;
	if (stunAnim.HasFinished()) {
		stunned = false;
		desiredState = EntityState_Khurt::RUNNING;
	}
	/*if (stunAnim.HasFinished() && stunTimer.ReadSec() >= 2)
	{
		pbodyFoot->body->SetLinearVelocity(b2Vec2(0, 0));
		desiredState = EntityState_Khurt::IDLE;
		stunned = false;
		charging = false;
		chargeTimer.Start();
		stunAnim.Reset();
	}
	else {
		stunned = true;
		currentAnimation = &stunAnim;
	}*/
}


void Enemy_Khurt_Variation::Attack(float dt)
{
	//printf("Khurt attacking");
	currentAnimation = &underAnim_end;
	if (underAnim_end.HasFinished()) {
		currentAnimation = &chargeAnim;
		pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
		//No se mueve mientras ataca

		if (chargeAnim.HasFinished()) {
			app->entityManager->GetPlayer()->TakeDamage(attackDamage);
			desiredState = EntityState_Khurt::RUNNING;
			//printf("RUNNING \n");
		}
	}

	//sonido ataque
}

void Enemy_Khurt_Variation::stateMachine(float dt, iPoint playerPos)
{
	////printf("\ncurrentState: %d, desiredState: %d", static_cast<int>(currentState), static_cast<int>(desiredState));
	nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;
	switch (nextState) {
	case EntityState_Khurt::IDLE:
		DoNothing(dt);
		break;
	case EntityState_Khurt::RUNNING:
		if (!isUnderground) {
			DigUnderground();
		}
		else {
			MoveUnderground(dt, playerPos);
		}
		break;
	case EntityState_Khurt::ATTACKING:
		Attack(dt);
		break;
	case EntityState_Khurt::DEAD:
		Die();
		break;
	case EntityState_Khurt::STUNNED:
		Stunned(dt);
		break;
	case EntityState_Khurt::DIGGING_UNDERGROUND:
		DigUnderground();
		break;
	case EntityState_Khurt::MOVING_UNDERGROUND:
		MoveUnderground(dt, playerPos);
		break;
	case EntityState_Khurt::DIGGING_OUT:
		DigOut(dt, playerPos);
		break;
	case EntityState_Khurt::NONE:
		desiredState = EntityState_Khurt::IDLE;
		break;

	default:
		break;
	}
	currentState = nextState;

}


void Enemy_Khurt_Variation::Die() {

	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
	currentAnimation = &dieAnim;

	fPoint pos((float)position.x, (float)position.y);
	blood = app->psystem->AddEmiter(pos, EMITTER_TYPE_ENEMY_BLOOD);




	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	if (parseResult) {
		configNode = configFile.child("config");
	}
	float randomValue = (float(std::rand() % 101) / 100);

	// Determina si el item debe crearse basado en un 30% de probabilidad
	if (randomValue <= 0.25f) {
		Item_Garra* garra = (Item_Garra*)app->entityManager->CreateEntity(EntityType::ITEM_GARRA);
		garra->config = configNode.child("entities_data").child("item_garra");
		garra->position = iPoint(position.x, position.y);
		garra->Start();
	}
	app->bestiarioManager->CreateItem("khurt");
	//Mask XP

	//Mask 0
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK0)
	{
		app->inventoryManager->maskZeroXP += 80;
		////printf("Current Mask 0 XP %i \n", app->inventoryManager->maskZeroXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK0)
	{
		app->inventoryManager->maskZeroXP += 80;
		////printf("Current Mask 0 XP %i \n", app->inventoryManager->maskZeroXP);
	}

	//Mask 1
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK1)
	{
		app->inventoryManager->maskOneXP += 80;
		////printf("Current Mask 1 XP %i \n", app->inventoryManager->maskOneXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK1)
	{
		app->inventoryManager->maskOneXP += 80;
		////printf("Current Mask 1 XP %i \n", app->inventoryManager->maskOneXP);
	}

	//Mask 2
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK2)
	{
		app->inventoryManager->maskTwoXP += 80;
		////printf("Current Mask 2 XP %i \n", app->inventoryManager->maskTwoXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK2)
	{
		app->inventoryManager->maskTwoXP += 80;
		////printf("Current Mask 2 XP %i \n", app->inventoryManager->maskTwoXP);
	}

	//Mask 3
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK3)
	{
		app->inventoryManager->maskThreeXP += 80;
		////printf("Current Mask 3 XP %i \n", app->inventoryManager->maskThreeXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK3)
	{
		app->inventoryManager->maskThreeXP += 80;
		////printf("Current Mask 3 XP %i \n", app->inventoryManager->maskThreeXP);
	}

	if (app->entityManager->GetIgory() != nullptr && app->entityManager->GetIgory()->playerInFight) {
		app->map->DestroyEntity(this);
	}

	CleanUp();
}

// L07 DONE 6: Define OnCollision function for the player. 
void Enemy_Khurt_Variation::OnCollision(PhysBody* physA, PhysBody* physB) {
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

bool Enemy_Khurt_Variation::Khurtfinding(float dt, iPoint playerPosP)
{
	iPoint playerPos = app->map->WorldToMap(playerPosP.x, playerPosP.y);
	iPoint enemyPos = app->map->WorldToMap(position.x, position.y);


	int pathLength = app->map->pathfinding->CreatePath(enemyPos, playerPos);

	if (dist(playerPos, enemyPos) < viewDistance && pathLength > 0 && pathLength < viewDistance) {

		lastPath = *app->map->pathfinding->GetLastPath();
	}
	else {
		if (app->entityManager->GetIgory() != nullptr && app->entityManager->GetIgory()->playerInFight) {
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
		runAnim.Reset();

	}
	/*else {
		LOG("HA LLEGADO AL DESTINO");
	}*/

	// Aplica la velocidad al cuerpo del enemigo
	pbodyFoot->body->SetLinearVelocity(velocity);

	return true;
}

float Enemy_Khurt_Variation::GetHealth() const {
	return health;
}

void Enemy_Khurt_Variation::TakeDamage(float damage) {
	if (currentState != EntityState_Khurt::DEAD && invulnerabilityTimer.ReadMSec() >= 500) {
		health -= damage;
		invulnerabilityTimer.Start();
		timerRecibirDanioColor.Start();
		app->audio->PlayRandomFx(khurt_get_damage_fx, khurt_get_damageAlt_fx, NULL);
	}
}


void Enemy_Khurt_Variation::ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate) {
	this->poisonDamage = poisonDamage;
	this->poisonDuration = poisonDuration;
	this->poisonTickRate = poisonTickRate;

	this->poisoned = true;
	this->firstTimePoisonRecibed = true;

	poisonTimer.Start();
	poisonTickTimer.Start();


}

void Enemy_Khurt_Variation::CheckPoison() {
	float epsilon = 0.1f; //Para margen de error

	// Aplicar el primer tick de da�o inmediatamente (si no, el primer tick no se aplica en el segundo 0.0)
	if (firstTimePoisonRecibed) {
		if (currentState != EntityState_Khurt::DEAD) {
			health -= poisonDamage;
			invulnerabilityTimer.Start();
			timerRecibirDanioColor.Start();

			//printf("Enemy_Khurt has received  %f damage of poison\n", poisonDamage);
		}
		firstTimePoisonRecibed = false;
	}

	if (poisonTimer.ReadSec() <= poisonDuration + epsilon && poisoned) {
		if (poisonTickTimer.ReadSec() >= poisonTickRate) {
			poisonTickTimer.Start(); // Reiniciar el temporizador de ticks de veneno
			if (currentState != EntityState_Khurt::DEAD) {
				health -= poisonDamage;
				invulnerabilityTimer.Start();
				timerRecibirDanioColor.Start();

				//printf("Enemy_Khurt has received  %f damage of poison\n", poisonDamage);
			}
		}
	}
}
//VENENO ---------->


MapObject* Enemy_Khurt_Variation::GetCurrentRoom()
{
	//salas peque�as
	for (ListItem<MapObject*>* item = app->map->smallRoomsList.start; item != nullptr; item = item->next)
	{
		MapObject* room = item->data;

		// el jugador est?dentro de la sala
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

		// el jugador est?dentro de la sala
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

		// el jugador est?dentro de la sala
		if (position.x >= room->x && position.x <= room->x + room->width &&
			position.y >= room->y && position.y <= room->y + room->height)
		{
			return room;
		}
	}

	return nullptr;
}