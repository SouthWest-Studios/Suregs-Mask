#include "Enemy_Khurt.h"
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
#include "Item_Garra.h"
#include "BestiarioManager.h"
#include <Optick/include/optick.h>
#include "Utils.cpp"





Enemy_Khurt::Enemy_Khurt() : Entity(EntityType::ENEMY_KHURT) {
	name = ("khurt");
	state = EntityState::IDLE;
	currentState = state;
	nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(currentState)].next_state;

}

Enemy_Khurt::~Enemy_Khurt() {

}

bool Enemy_Khurt::Awake() {

	return true;
}

bool Enemy_Khurt::Start() {

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
	runAnim.LoadAnim("khurt", "runAnim", spritePositions);
	underAnim_start.LoadAnim("khurt", "underAnim_start", spritePositions);
	underAnim_process.LoadAnim("khurt", "underAnim_process", spritePositions);
	underAnim_end.LoadAnim("khurt", "underAnim_end", spritePositions);
	dieAnim.LoadAnim("khurt", "dieAnim", spritePositions);
	

	texture = app->tex->Load(config.attribute("texturePath").as_string());

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

	//printf("Speed: %f", speed);

	room = GetCurrentRoom();

	return true;
}

bool Enemy_Khurt::Update(float dt)
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
	else if (app->map->pathfinding->GetDistance(playerPos, position) <= attackDistance * 32)
	{
		nextState = EntityState::ATTACKING;
	}
	else if (app->map->pathfinding->GetDistance(playerPos, position) <= viewDistance * 32)
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
	case EntityState::IDLE:
		DoNothing(dt);
		break;
	default:
		break;
	}

	//if (poisonTimer < poisonDuration) {
	//	poisonTimer += 1 / dt;
	//	timeSinceLastTick += 1 / dt;

	//	if (timeSinceLastTick >= poisonTickRate && poisoned) {
	//		if (currentState != EntityState::DEAD && invulnerabilityTimer.ReadMSec() >= 500) {
	//			health -= poisonDamage;
	//			invulnerabilityTimer.Start();
	//			timerRecibirDanioColor.Start();
	//		}
	//		timeSinceLastTick -= poisonTickRate;
	//	}
	//}

	if (charging && dist(Antposition, position) > 350)
	{
		Stunned(dt);
		stunned = true;
	}

	if (chargeTimer.ReadSec() >= 5)
	{
		charging = false;
	}

	//if (app->map->pathfinding->GetDistance(playerPos, position) == (attackDistance + 5) * 32) {

	//	digging = true;
	//}
	//if (digging = true) {
	//	digTimer.Start();
	//	currentAnimation = &underAnim;
	//}

	//if (digTimer.ReadSec() >= 2) {
	//	nextState = EntityState::IDLE;
	//	digging = false;
	//}

	//VENENO <----------
	CheckPoison();
	//VENENO ---------->

	currentState = nextState;
	currentAnimation->Update();
	return true;
}


bool Enemy_Khurt::PostUpdate() {

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
		app->render->DrawTexture(texture, position.x - 25, position.y - 65, 0.5f,SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 40, position.y - 65, 0.5f,SDL_FLIP_NONE, &rect);
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


bool Enemy_Khurt::CleanUp()
{
	app->physics->GetWorld()->DestroyBody(pbodyFoot->body);
	app->physics->GetWorld()->DestroyBody(pbodySensor->body);
	app->tex->UnLoad(texture);
	lastPath.Clear();

	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}

void Enemy_Khurt::DoNothing(float dt)
{
	currentAnimation = &idleAnim;
	//printf("Khurt idle");
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);

}

void Enemy_Khurt::Chase(float dt, iPoint playerPos)
{
	//printf("Khurt chasing");
	if (!charging) {
		currentAnimation = &underAnim_start;
	}
	if (underAnim_start.HasFinished()) {
		underAnim_start.Reset();
		underProcess = true;
	}
	if (underProcess) {
		currentAnimation = &underAnim_process;
	}
	if (app->map->pathfinding->GetDistance(playerPos, position) <= (chargeAttackDistance + 1) * 32) {
		underProcess = false;
		currentAnimation = &underAnim_end;
	}
	if (underAnim_end.HasFinished()) {
		underFinished = true;
		underAnim_end.Reset();
	}
	if (underFinished) {
		/*underFinished = false;*/
		currentAnimation = &idleAnim;
	}

	if (chargeTimer.ReadSec() >= 5 && app->map->pathfinding->GetDistance(playerPos, position) <= chargeAttackDistance * 32 && app->map->pathfinding->GetDistance(playerPos, position) >= (attackDistance + 5) * 32)
	{
		Antposition = position;
		Charge(dt, playerPos);
	}
	else if (!charging)
	{
		Khurtfinding(dt, playerPos);
	}

}

void Enemy_Khurt::Attack(float dt)
{
	//printf("Khurt attacking");
	currentAnimation = &runAnim;
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero); //No se mueve mientras ataca

	//sonido ataque
}

void Enemy_Khurt::Die() {

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
		app->entityManager->GetPlayer()->maskZeroXP += 80;
		//printf("Current Mask 0 XP %i \n", app->entityManager->GetPlayer()->maskZeroXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK0)
	{
		app->entityManager->GetPlayer()->maskZeroXP += 80;
		//printf("Current Mask 0 XP %i \n", app->entityManager->GetPlayer()->maskZeroXP);
	}
	
	//Mask 1
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK1)
	{
		app->entityManager->GetPlayer()->maskOneXP += 80;
		//printf("Current Mask 1 XP %i \n", app->entityManager->GetPlayer()->maskOneXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK1)
	{
		app->entityManager->GetPlayer()->maskOneXP += 80;
		//printf("Current Mask 1 XP %i \n", app->entityManager->GetPlayer()->maskOneXP);
	}

	//Mask 2
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK2)
	{
		app->entityManager->GetPlayer()->maskTwoXP += 80;
		//printf("Current Mask 2 XP %i \n", app->entityManager->GetPlayer()->maskTwoXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK2)
	{
		app->entityManager->GetPlayer()->maskTwoXP += 80;
		//printf("Current Mask 2 XP %i \n", app->entityManager->GetPlayer()->maskTwoXP);
	}

	//Mask 3
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK3)
	{
		app->entityManager->GetPlayer()->maskThreeXP += 80;
		//printf("Current Mask 3 XP %i \n", app->entityManager->GetPlayer()->maskThreeXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK3)
	{
		app->entityManager->GetPlayer()->maskThreeXP += 80;
		//printf("Current Mask 3 XP %i \n", app->entityManager->GetPlayer()->maskThreeXP);
	}

	if (app->entityManager->GetIgory()->playerInFight) {
		app->map->DestroyEntity(this);
	}
}

// L07 DONE 6: Define OnCollision function for the player. 
void Enemy_Khurt::OnCollision(PhysBody* physA, PhysBody* physB) {
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

bool Enemy_Khurt::Khurtfinding(float dt, iPoint playerPosP)
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
		runAnim.Reset();

	}
	/*else {
		LOG("HA LLEGADO AL DESTINO");
	}*/

	// Aplica la velocidad al cuerpo del enemigo
	pbodyFoot->body->SetLinearVelocity(velocity);

	return true;
}

float Enemy_Khurt::GetHealth() const {
	return health;
}

void Enemy_Khurt::TakeDamage(float damage) {
	if (currentState != EntityState::DEAD && invulnerabilityTimer.ReadMSec() >= 500) {
		health -= damage;
		invulnerabilityTimer.Start();
		timerRecibirDanioColor.Start();
	}
}

void Enemy_Khurt::Charge(float dt, iPoint playerPos)
{
	printf("charging");
	currentAnimation = &chargeAnim;
	charging = true;

	b2Vec2 direction(playerPos.x - position.x, playerPos.y - position.y);
	direction.Normalize();

	b2Vec2 impulse = b2Vec2(direction.x * 5, direction.y * 5);
	pbodyFoot->body->ApplyLinearImpulse(impulse, pbodyFoot->body->GetWorldCenter(), true);

	stunTimer.Start();
	chargeTimer.Start();
}

void Enemy_Khurt::Stunned(float dt)
{
	if (stunTimer.ReadSec() <= 3) {
		printf("stunned");
		currentAnimation = &stunAnim;
		pbodyFoot->body->SetLinearVelocity(b2Vec2(0, 0));
	}
	else {
		stunTimer.Start();
		nextState = EntityState::IDLE;
		stunned = false;
		charging = false;
	}
}

void Enemy_Khurt::ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate) {
	this->poisonDamage = poisonDamage;
	this->poisonDuration = poisonDuration;
	this->poisonTickRate = poisonTickRate;

	this->poisoned = true;
	this->firstTimePoisonRecibed = true;

	poisonTimer.Start();
	poisonTickTimer.Start();


}

void Enemy_Khurt::CheckPoison() {
	float epsilon = 0.1f; //Para margen de error

	// Aplicar el primer tick de daño inmediatamente (si no, el primer tick no se aplica en el segundo 0.0)
	if (firstTimePoisonRecibed) {
		if (currentState != EntityState::DEAD) {
			health -= poisonDamage;
			invulnerabilityTimer.Start();
			timerRecibirDanioColor.Start();

			printf("Enemy_Khurt has received  %f damage of poison\n", poisonDamage);
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

				printf("Enemy_Khurt has received  %f damage of poison\n", poisonDamage);
			}
		}
	}
}
//VENENO ---------->


MapObject* Enemy_Khurt::GetCurrentRoom()
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