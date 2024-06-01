#include "Enemy_Muur_Variation.h"
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
#include "BestiarioManager.h"
#include <Optick/include/optick.h>
#include "Utils.cpp"


Enemy_Muur_Variation::Enemy_Muur_Variation() : Entity(EntityType::ENEMY_MUUR_VARIATION) {
	name = ("muur");
	state = EntityState_Enemy::IDLE;
	currentState = state;
	nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(currentState)].next_state;

}

Enemy_Muur_Variation::~Enemy_Muur_Variation() {

}

bool Enemy_Muur_Variation::Awake() {

	return true;
}

bool Enemy_Muur_Variation::Start() {

	OPTICK_EVENT();

	TSprite = config.attribute("Tsprite").as_int();
	SpriteX = config.attribute("sprite_x").as_int();
	SpriteY = config.attribute("sprite_y").as_int();
	Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

	runAnim.LoadAnim("muur", "runAnim", spritePositions);
	stunAnim.LoadAnim("muur", "stunAnim", spritePositions);
	attackAnim.LoadAnim("muur", "attackAnim", spritePositions);
	idleAnim.LoadAnim("muur", "idleAnim", spritePositions);
	chargeAnim.LoadAnim("muur", "chargeAnim", spritePositions);
	reciebeDamage.LoadAnim("muur", "reciebeDamage", spritePositions);
	dieAnim.LoadAnim("muur", "dieAnim", spritePositions);

	texture = app->tex->Load(config.attribute("texturePath").as_string());

	muur_get_damage_fx = app->audio->LoadAudioFx("muur_get_damage_fx");

	pbodyFoot = app->physics->CreateCircle(position.x, position.y, 15, bodyType::DYNAMIC);
	pbodyFoot->entity = this;
	pbodyFoot->listener = this;
	pbodyFoot->ctype = ColliderType::ENEMY;

	pbodySensor = app->physics->CreateRectangleSensor(position.x, position.y, 10, 15, bodyType::DYNAMIC);
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

	room = GetCurrentRoom();

	return true;
}

bool Enemy_Muur_Variation::Update(float dt)
{
	OPTICK_EVENT();

	//printf("\nheal: %f",health);
	// Pone el sensor del cuerpo en su posicion
	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
	pbodySensor->body->SetTransform(b2Vec2(pbodyPos.p.x, pbodyPos.p.y - 0.5), 0);

	iPoint playerPos = app->entityManager->GetPlayer()->position;

	// L�gica de cambio de estado
	if (health <= 0)
	{
		nextState = EntityState_Enemy::DEAD;
	}
	else if (app->map->pathfinding->GetDistance(playerPos, position) <= attackDistance * 32)
	{
		nextState = EntityState_Enemy::ATTACKING;
	}
	else if (app->map->pathfinding->GetDistance(playerPos, position) <= viewDistance * 32)
	{
		nextState = EntityState_Enemy::RUNNING;
	}
	else if (app->map->pathfinding->GetDistance(playerPos, position) >= viewDistance * 32 && app->entityManager->GetIgory()->playerInFight)
	{
		nextState = EntityState_Enemy::RUNNING;
	}
	else
	{
		nextState = EntityState_Enemy::IDLE;
	}

	// Manejo de estados
	switch (nextState)
	{
	case EntityState_Enemy::RUNNING:
		Chase(dt, playerPos);
		break;
	case EntityState_Enemy::ATTACKING:
		Attack(dt);
		break;
	case EntityState_Enemy::DEAD:
		Die();
		break;
	case EntityState_Enemy::IDLE:
		DoNothing(dt);
		break;
	default:
		break;
	}

	if (charging && dist(Antposition, position) > 350 || charging && timechargingTimer.ReadSec() > 0.8)
	{
		isStunned = true;
		Stunned(dt);
	}

	if (chargeTimer.ReadSec() >= 5)
	{
		charging = false;
	}

	CheckPoison();

	// Actualiza la animaci�n actual
	if (currentAnimation != nullptr) {
		currentAnimation->Update();
	}
	currentState = nextState;

	return true;
}

bool Enemy_Muur_Variation::PostUpdate() {

	SDL_SetTextureColorMod(texture, 198, 115, 255);

	if (currentAnimation == nullptr) {
		currentAnimation = &idleAnim;
	}
	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	if (timerRecibirDanioColor.ReadMSec() <= 100) {
		float alpha = (100 - timerRecibirDanioColor.ReadMSec()) / 100;
		SDL_SetTextureAlphaMod(texture, static_cast<Uint8>(255 * alpha)); // Ajusta la opacidad
	}
	else {
		SDL_SetTextureAlphaMod(texture, 255);
	}

	if (isFacingLeft) {
		app->render->DrawTexture(texture, position.x - 30, position.y - 50, 0.5f, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 30, position.y - 50, 0.5f, SDL_FLIP_NONE, &rect);
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



bool Enemy_Muur_Variation::CleanUp()
{
	app->physics->GetWorld()->DestroyBody(pbodyFoot->body);
	app->physics->GetWorld()->DestroyBody(pbodySensor->body);
	app->tex->UnLoad(texture);
	lastPath.Clear();

	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}

void Enemy_Muur_Variation::DoNothing(float dt)
{
	currentAnimation = &idleAnim;
	//printf("Muur idle");
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);

}

void Enemy_Muur_Variation::Chase(float dt, iPoint playerPos)
{
	//printf("Muur chasing");
	currentAnimation = &runAnim;
	if (chargeTimer.ReadSec() >= 5 && app->map->pathfinding->GetDistance(playerPos, position) <= chargeattackDistance * 32 && app->map->pathfinding->GetDistance(playerPos, position) >= (attackDistance + 5) * 32)
	{
		Antposition = position;
		Charge(dt, playerPos);
	}
	else if (!charging)
	{
		Muurfinding(dt, playerPos);
	}

}

void Enemy_Muur_Variation::Attack(float dt)
{
	//printf("Muur attacking");
	currentAnimation = &attackAnim;
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
	//sonido ataque
}

void Enemy_Muur_Variation::Die() {
	app->audio->PlayFx(muur_get_damage_fx);



	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
	currentAnimation = &dieAnim;

	if (dieAnim.HasFinished())
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
		if (randomValue <= 0.30f) {
			Item_Cola* cola = (Item_Cola*)app->entityManager->CreateEntity(EntityType::ITEM_COLA);
			cola->config = configNode.child("entities_data").child("item_cola");
			cola->position = iPoint(position.x, position.y);
			cola->Start();
		}

		app->bestiarioManager->CreateItem("muur");
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
		if (app->entityManager->GetIgory()->playerInFight) {
			app->map->DestroyEntity(this);
		}
	}
}

// L07 DONE 6: Define OnCollision function for the player. 
void Enemy_Muur_Variation::OnCollision(PhysBody* physA, PhysBody* physB) {
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

bool Enemy_Muur_Variation::Muurfinding(float dt, iPoint playerPosP)
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

float Enemy_Muur_Variation::GetHealth() const {
	return health;
}

void Enemy_Muur_Variation::TakeDamage(float damage) {
	if (currentState != EntityState_Enemy::DEAD && invulnerabilityTimer.ReadMSec() >= 500) {
		health -= damage;
		invulnerabilityTimer.Start();
		timerRecibirDanioColor.Start();
		currentAnimation = &reciebeDamage;
	}
}

//VENENO <----------
void Enemy_Muur_Variation::ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate) {
	this->poisonDamage = poisonDamage;
	this->poisonDuration = poisonDuration;
	this->poisonTickRate = poisonTickRate;

	this->poisoned = true;
	this->firstTimePoisonRecibed = true;

	poisonTimer.Start();
	poisonTickTimer.Start();


}

void Enemy_Muur_Variation::CheckPoison() {
	float epsilon = 0.1f; //Para margen de error

	// Aplicar el primer tick de da�o inmediatamente (si no, el primer tick no se aplica en el segundo 0.0)
	if (firstTimePoisonRecibed) {
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

void Enemy_Muur_Variation::Charge(float dt, iPoint playerPos) {
	if (chargeTimer.ReadSec() >= 5)
	{
		/*printf("charge");*/
		currentAnimation = &chargeAnim;
		pbodyFoot->body->SetLinearVelocity(b2Vec2(0, 0));
		if (chargeAnim.HasFinished())
		{
			timechargingTimer.Start();
			charging = true;

			b2Vec2 direction(playerPos.x - position.x, playerPos.y - position.y);
			direction.Normalize();

			b2Vec2 impulse = b2Vec2(direction.x * 5, direction.y * 5);
			pbodyFoot->body->ApplyLinearImpulse(impulse, pbodyFoot->body->GetWorldCenter(), true);

			stunTimer.Start();
			chargeTimer.Start();
		}
	}
}

void Enemy_Muur_Variation::Stunned(float dt) {
	pbodyFoot->body->SetLinearVelocity(b2Vec2(0, 0));

	if (stunAnim.HasFinished() && stunTimer.ReadSec() >= 2)
	{
		pbodyFoot->body->SetLinearVelocity(b2Vec2(0, 0));
		nextState = EntityState_Enemy::IDLE;
		isStunned = false;
		charging = false;
		chargeTimer.Start();
		stunAnim.Reset();
	}
	else {
		isStunned = true;
		currentAnimation = &stunAnim;
	}
}


MapObject* Enemy_Muur_Variation::GetCurrentRoom()
{
	//salas peque�as
	for (ListItem<MapObject*>* item = app->map->smallRoomsList.start; item != nullptr; item = item->next)
	{
		MapObject* room = item->data;

		// el jugador est� dentro de la sala
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

		// el jugador est� dentro de la sala
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

		// el jugador est� dentro de la sala
		if (position.x >= room->x && position.x <= room->x + room->width &&
			position.y >= room->y && position.y <= room->y + room->height)
		{
			return room;
		}
	}

	return nullptr;
}