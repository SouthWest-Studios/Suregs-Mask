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
#include "Enemy_Osiris.h"
#include "Enemy_Muur.h"
#include "Item_Hueso.h"
#include "EntityManager.h"
#include <Optick/include/optick.h>
#include "Boss_Igory.h"
#include "Utils.cpp"





Boss_Igory::Boss_Igory() : Entity(EntityType::BOSS_IGORY) {
	name = ("boss_igory");

}

Boss_Igory::~Boss_Igory() {

}

bool Boss_Igory::Awake() {

	return true;
}

bool Boss_Igory::Start() {

	OPTICK_EVENT();
	//position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	TSprite = config.attribute("Tsprite").as_int();
	SpriteX = config.attribute("sprite_x").as_int();
	SpriteY = config.attribute("sprite_y").as_int();
	Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

	atqCargado_fase2_boss_Igory.LoadAnim("boss_igory", "atqCargado_fase2_boss_Igory", spritePositions);
	atqCargado_fase1_boss_Igory.LoadAnim("boss_igory", "atqCargado_fase1_boss_Igory", spritePositions);
	walk_boss_Igory.LoadAnim("boss_igory", "walk_boss_Igory", spritePositions);
	cura_boss_Igory.LoadAnim("boss_igory", "cura_boss_Igory", spritePositions);
	dash_boss_Igory.LoadAnim("boss_igory", "dash_boss_Igory", spritePositions);
	atq1_boss_Igory.LoadAnim("boss_igory", "atq1_boss_Igory", spritePositions);
	geneSure_boss_Igory.LoadAnim("boss_igory", "geneSure_boss_Igory", spritePositions);
	idle_boss_Igory.LoadAnim("boss_igory", "idle_boss_Igory", spritePositions);
	atq2_boss_Igory.LoadAnim("boss_igory", "atq2_boss_Igory", spritePositions);
	atq3_boss_Igory.LoadAnim("boss_igory", "atq3_boss_Igory", spritePositions);
	dead_boss_Igory.LoadAnim("boss_igory", "dead_boss_Igory", spritePositions);
	hit_boss_Igory.LoadAnim("boss_igory", "hit_boss_Igory", spritePositions);


	texture = app->tex->Load(config.attribute("texturePath").as_string());


	pbodyFoot = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
	pbodyFoot->entity = this;
	pbodyFoot->listener = this;
	pbodyFoot->ctype = ColliderType::BOSS_INUIT;

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
	//FASE_Igory
	fase = FASE_Igory::FASE_ONE;
	lifeLow40 = maxHealth * 0.8;
	lifeLow40 = maxHealth * 0.4;

	room = GetCurrentRoom();
	generaTimeColdDown.Start();
	return true;
}

bool Boss_Igory::Update(float dt)
{
	OPTICK_EVENT();
	//Pone el sensor del cuerpo en su posicion
	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
	pbodySensor->body->SetTransform(b2Vec2(pbodyPos.p.x, pbodyPos.p.y - 1), 0);
	iPoint playerPos = app->entityManager->GetPlayer()->position;



	if (health <= 0)
	{
		desiredState = EntityState_Boss_Igory::DEAD;
	}
	else if (inTakeHit)
	{
		desiredState = EntityState_Boss_Igory::TAKEHIT;
	}
	/*else if (GeneraColdDown(10))
	{
		desiredState = EntityState_Boss_Igory::GENERATESUREG;
	}
	if (goColdDown) {
		if (GeneraColdDown(5)) {
			generaTimeColdDown.Start();
			goColdDown = false;
			inSuregAni = false;
		}
		else {
			desiredState = EntityState_Boss_Igory::IDLE;
		}


	}*/
	else if (app->map->pathfinding->GetDistance(playerPos, position) <= attackDistance * 64 && ataqColdDown && !inTakeHit)
	{
		desiredState = EntityState_Boss_Igory::IDLE;
	}
	else if (app->map->pathfinding->GetDistance(playerPos, position) <= attackDistance * 64 && ataqColdDown == false && !inTakeHit && !inSuregAni)
	{
		desiredState = EntityState_Boss_Igory::ATTACKING_BASIC;
	}
	else if (playerInFight && currentState != EntityState_Boss_Igory::ATTACKING_BASIC)
	{
		desiredState = EntityState_Boss_Igory::RUNNING;

		if (inAtack) {
			if (atackCube != nullptr) {
				app->physics->GetWorld()->DestroyBody(atackCube->body);
				atackCube = nullptr;
			}
			inAtack = false;
			atqGoNext = true;
		}
	}
	else if (!playerInFight)
	{
		desiredState = EntityState_Boss_Igory::IDLE;
	}
	else
	{
		desiredState = EntityState_Boss_Igory::IDLE;
	}
	stateMachine(dt, playerPos);

	if (health <= lifeLow40) {
		fase = FASE_Igory::FASE_CHANGE;
	}

	if (checkColdDown) {
		AtqColdDown();
	}


	switch (fase)
	{
	case FASE_Igory::FASE_ONE:
		break;
	case FASE_Igory::FASE_CHANGE:
		break;
	case FASE_Igory::FASE_TWO:
		break;
	case FASE_Igory::FASE_THREE:

		break;
	}


	showAnimation();
	resetAnimation();
	currentAnimation->Update();
	return true;
}


bool Boss_Igory::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idle_boss_Igory; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	if (timerRecibirDanioColor.ReadMSec() <= 100) {
		float alpha = (100 - timerRecibirDanioColor.ReadMSec()) / 100;
		SDL_SetTextureAlphaMod(texture, static_cast<Uint8>(255 * alpha)); // Ajusta la opacidad

	}
	else {
		SDL_SetTextureAlphaMod(texture, 255);
	}

	if (isFacingLeft) {
		app->render->DrawTexture(texture, position.x - 200, position.y - 200, 0.8, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 260, position.y - 200, 0.8, SDL_FLIP_NONE, &rect);
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

	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
		app->map->generaSureg(fase, position);
	}

	return true;
}


bool Boss_Igory::CleanUp()
{
	if (pbodyFoot != nullptr) {
		app->physics->GetWorld()->DestroyBody(pbodyFoot->body);
	}
	if (pbodySensor != nullptr) {
		app->physics->GetWorld()->DestroyBody(pbodySensor->body);
	}
	if (areaSensor != nullptr) {
		app->physics->GetWorld()->DestroyBody(areaSensor->body);
	}

	app->tex->UnLoad(texture);
	lastPath.Clear();

	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}


void Boss_Igory::resetAnimation()
{
	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "atq1_boss_Igory") {
		if (atackCube != nullptr) {
			app->physics->GetWorld()->DestroyBody(atackCube->body);
			atackCube = nullptr;
		}
		inAtack = false;
		startColdDown = true;
		checkColdDown = true;
	}
	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "atq2_boss_Igory") {
		if (atackCube != nullptr) {
			app->physics->GetWorld()->DestroyBody(atackCube->body);
			atackCube = nullptr;
		}
		inAtack = false;
		startColdDown = true;
		checkColdDown = true;
	}
	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "atq3_boss_Igory") {

		if (atackCube != nullptr) {
			app->physics->GetWorld()->DestroyBody(atackCube->body);
			atackCube = nullptr;
		}
		inAtack = false;
		startColdDown = true;
		checkColdDown = true;
	}
	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "hit_boss_Igory") {
		inTakeHit = false;
d		hit_boss_Igory.Reset();
		if (atackCube != nullptr) {
			app->physics->GetWorld()->DestroyBody(atackCube->body);
			atackCube = nullptr;
		}
		inAtack = false;
		/*startColdDown = true;
		checkColdDown = true;*/
	}

	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "geneSure_boss_Igory") {
		geneSure_boss_Igory.Reset();
		generaTimeColdDown.Start();
		showSuregAni = false;
		goColdDown = true;
	}

}

void Boss_Igory::showAnimation()
{
	if (inAtack &&!inSuregAni) {
		if (attackTime == 0) {
			currentAnimation = &atq1_boss_Igory;
		}
		else if (attackTime == 1) {
			currentAnimation = &atq2_boss_Igory;
		}
		else if (attackTime == 2) {
			currentAnimation = &atq3_boss_Igory;
		}

	}

	if (showSuregAni) {
		currentAnimation = &geneSure_boss_Igory;
	}
}



bool Boss_Igory::AtqColdDown()
{
	if (startColdDown) {
		//printf("\nStart");
		atackTimeColdDown.Start();
		startColdDown = false;
	}

	if (atackTimeColdDown.ReadMSec() >= 500) {
		//printf("\nEnd");
		ataqColdDown = false;
		checkColdDown = false;
		startColdDown = true;
		atqGoNext = true;
		return true;
	}
	else
	{
		//printf("\nColdDown");
		ataqColdDown = true;
		return false;
	}

	atackTimeColdDown.Start();

}

void Boss_Igory::takeHit()
{
	currentAnimation = &hit_boss_Igory;
}



void Boss_Igory::stateMachine(float dt, iPoint playerPos)
{
	//printf("\ncurrentState: %d, desiredState: %d", static_cast<int>(currentState), static_cast<int>(desiredState));
	nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;
	switch (nextState) {
	case EntityState_Boss_Igory::IDLE:
		DoNothing(dt);
		break;
	case EntityState_Boss_Igory::RUNNING:
		Chase(dt, playerPos);
		break;
	case EntityState_Boss_Igory::ATTACKING_BASIC:

		pbodyFoot->body->SetLinearVelocity(b2Vec2_zero); //No se mueve mientras ataca

		if (!inAtack) {
			Attack(dt);
		}

		break;
	case EntityState_Boss_Igory::ATTACKING_CHARGE:
		break;
	case EntityState_Boss_Igory::ATTACKING_DASHI:
		break;
	case EntityState_Boss_Igory::DEAD:
		Die();
		break;
	case EntityState_Boss_Igory::FASE_CHANGE:
		break;
	case EntityState_Boss_Igory::TAKEHIT:
		takeHit();
		break;
	case EntityState_Boss_Igory::GENERATESUREG:
		printf("\ngeneraSureg");
		showSuregAni = true;
		inSuregAni = true;
		break;
	case EntityState_Boss_Igory::HEAL:
		break;
	case EntityState_Boss_Igory::NONE:
		desiredState = EntityState_Boss_Igory::IDLE;
		break;

	default:
		break;
	}
	currentState = nextState;


}




void Boss_Igory::DoNothing(float dt)
{
	currentAnimation = &idle_boss_Igory;
	//printf("Osiris idle");
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);

}

void Boss_Igory::Chase(float dt, iPoint playerPos)
{
	////printf("Osiris chasing");
	currentAnimation = &walk_boss_Igory;
	Bossfinding(dt, playerPos);

}

void Boss_Igory::Attack(float dt)
{
	if (atqGoNext) {
		attackTime++;
		atqGoNext = false;
	}
	//printf("\nattackTime: %d", attackTime);
	switch (attackTime)
	{
	case 1:
		atq3_boss_Igory.Reset();
		inAtack = true;
		printf("\nataque1");
		atackCube = app->physics->CreateRectangleSensor(position.x, position.y, 60, 120, STATIC);
		break;
	case 2:
		atq1_boss_Igory.Reset();
		inAtack = true;
		printf("\nataque2");
		atackCube = app->physics->CreateRectangleSensor(position.x, position.y, 60, 120, STATIC);
		break;
	case 3:
		atq2_boss_Igory.Reset();
		inAtack = true;
		printf("\nataque3");
		atackCube = app->physics->CreateRectangleSensor(position.x, position.y, 60, 120, STATIC);
		attackTime = 0;
		break;

	default:
		break;
	}


}


void Boss_Igory::Die() {

	//Mask XP

	//Mask 0
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK0)
	{
		app->entityManager->GetPlayer()->maskZeroXP += 1000;
		//printf("Current Mask 0 XP %i \n", app->entityManager->GetPlayer()->maskZeroXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK0)
	{
		app->entityManager->GetPlayer()->maskZeroXP += 1000;
		//printf("Current Mask 0 XP %i \n", app->entityManager->GetPlayer()->maskZeroXP);
	}

	//Mask 1
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK1)
	{
		app->entityManager->GetPlayer()->maskOneXP += 1000;
		//printf("Current Mask 1 XP %i \n", app->entityManager->GetPlayer()->maskOneXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK1)
	{
		app->entityManager->GetPlayer()->maskOneXP += 1000;
		//printf("Current Mask 1 XP %i \n", app->entityManager->GetPlayer()->maskOneXP);
	}

	//Mask 2
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK2)
	{
		app->entityManager->GetPlayer()->maskTwoXP += 1000;
		//printf("Current Mask 2 XP %i \n", app->entityManager->GetPlayer()->maskTwoXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK2)
	{
		app->entityManager->GetPlayer()->maskTwoXP += 1000;
		//printf("Current Mask 2 XP %i \n", app->entityManager->GetPlayer()->maskTwoXP);
	}

	//Mask 3
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK3)
	{
		app->entityManager->GetPlayer()->maskThreeXP += 1000;
		//printf("Current Mask 3 XP %i \n", app->entityManager->GetPlayer()->maskThreeXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK3)
	{
		app->entityManager->GetPlayer()->maskThreeXP += 1000;
		//printf("Current Mask 3 XP %i \n", app->entityManager->GetPlayer()->maskThreeXP);
	}

}

void Boss_Igory::Revive()
{

}



bool Boss_Igory::Bossfinding(float dt, iPoint playerPosP)
{

	iPoint playerPos = app->map->WorldToMap(playerPosP.x, playerPosP.y);
	iPoint enemyPos = app->map->WorldToMap(position.x, position.y);




	if (playerInFight && !inSuregAni) {
		app->map->pathfinding->CreatePath(enemyPos, playerPos); // Calcula el camino desde la posicion del enemigo hacia la posicion del jugador
		lastPath = *app->map->pathfinding->GetLastPath();
	}


	//else {
	//	app->map->pathfinding->CreatePath(enemyPos, originalPosition); // Calcula el camino desde la posicion del enemigo hacia la posicion del jugador
	//	lastPath = *app->map->pathfinding->GetLastPath();
	//}


	b2Vec2 velocity = b2Vec2(0, 0);

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
		//attackAnim.Reset();

	}
	pbodyFoot->body->SetLinearVelocity(velocity);


	return true;
}

void Boss_Igory::deleteCollision(PhysBody* phy)
{

	if (phy != nullptr) {
		phy->body->SetLinearVelocity(b2Vec2(0, 0));
		//phy->body->GetWorld()->DestroyBody(phy->body);
		app->physics->GetWorld()->DestroyBody(phy->body);
		phy = nullptr;
		//printf("delete");
	}
	//printf("\ndelete0");
}

float Boss_Igory::GetHealth() const {
	return health;
}

void Boss_Igory::TakeDamage(float damage) {


}




bool Boss_Igory::TimerColdDown(float time)
{
	//printf("\nataqueTimeClodDown%: %f", ataqueTimeClodDown);
	ataqueTimeClodDown = atackTimeColdDown.CountDown(time);
	if ((float)ataqueTimeClodDown == 0) {
		return true;
	}
	else
	{
		return false;
	}
}

bool Boss_Igory::GeneraColdDown(float time)
{
	float fgenraTimeColdDown = generaTimeColdDown.CountDown(time);
	//printf("\nataqueTimeClodDown%: %f", fgenraTimeColdDown);
	if ((float)fgenraTimeColdDown == 0) {
		return true;
	}
	else
	{
		return false;
	}
}



void Boss_Igory::ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate) {
	this->poisonDamage = poisonDamage;
	this->poisonDuration = poisonDuration;
	this->poisonTickRate = poisonTickRate;
	this->poisonTimer = 0.0f;
	this->timeSinceLastTick = 0.0f;
	this->poisoned = true;
}

void Boss_Igory::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::BOSS_INUIT:
		break;
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		//restar vida al player
		break;
	case ColliderType::PLAYER_ATTACK:
		LOG("Collision Player_Attack");
		if (fase != FASE_Igory::FASE_CHANGE && app->entityManager->GetPlayer()->checkAtk == true) {
			inTakeHit = true;
			health -= app->entityManager->GetPlayer()->currentStats.attackDamage;
			timerRecibirDanioColor.Start();
			printf("\n BossHeal %f", health);
			app->entityManager->GetPlayer()->checkAtk = false;
		}
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}

void Boss_Igory::OnEndCollision(PhysBody* physA, PhysBody* physB) {
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


MapObject* Boss_Igory::GetCurrentRoom()
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