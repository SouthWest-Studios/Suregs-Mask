#include "Boss_Inuit.h"

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
#include <Optick/include/optick.h>
#include "Utils.cpp"





Boss_Inuit::Boss_Inuit() : Entity(EntityType::BOSS_INUIT) {
	name = ("boss_inuit");

}

Boss_Inuit::~Boss_Inuit() {

}

bool Boss_Inuit::Awake() {

	return true;
}

bool Boss_Inuit::Start() {

	OPTICK_EVENT();
	//position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	TSprite = config.attribute("Tsprite").as_int();
	SpriteX = config.attribute("sprite_x").as_int();
	SpriteY = config.attribute("sprite_y").as_int();
	Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

	idleAnim.LoadAnim("boss_inuit", "idleAnim_boss_inuit", spritePositions);
	/*runAnim.LoadAnim("osiris", "runAnim", spritePositions);
	attackAnim.LoadAnim("osiris", "attackAnim", spritePositions);
	dieAnim.LoadAnim("osiris", "dieAnim", spritePositions);*/

	texture = app->tex->Load(config.attribute("texturePath").as_string());

	pbodyFoot = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
	pbodyFoot->entity = this;
	pbodyFoot->listener = this;
	pbodyFoot->ctype = ColliderType::ENEMY;

	pbodySensor = app->physics->CreateRectangleSensor(position.x, position.y, 40, 60, bodyType::DYNAMIC);
	pbodySensor->entity = this;
	pbodySensor->listener = this;
	pbodySensor->ctype = ColliderType::UNKNOWN;



	areaSensor = app->physics->CreateCircle(position.x, position.y, 500, bodyType::STATIC, true);
	areaSensor->entity = this;
	areaSensor->listener = this;
	areaSensor->ctype = ColliderType::BOSSAREA;


	originalPosition = app->map->WorldToMap(position.x, position.y);

	maxHealth = config.attribute("maxHealth").as_float();
	health = maxHealth;
	speed = config.attribute("speed").as_float();
	attackDamage = config.attribute("attackDamage").as_float();
	attackDistance = config.attribute("attackDistance").as_float();
	viewDistance = config.attribute("viewDistance").as_float();


	//printf("Speed: %f", speed);
	return true;
}

bool Boss_Inuit::Update(float dt)
{
	OPTICK_EVENT();
	//Pone el sensor del cuerpo en su posicion
	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
	pbodySensor->body->SetTransform(b2Vec2(pbodyPos.p.x, pbodyPos.p.y - 1), 0);
	iPoint playerPos = app->entityManager->GetPlayer()->position;

	if (health <= 0)
	{
		desiredState = EntityState_Boss_Inuit::DEAD;
	}
	else if (app->map->pathfinding->GetDistance(playerPos, position) <= attackDistance * 32)
	{
		desiredState = EntityState_Boss_Inuit::ATTACKING_BASIC;
	}
	else if (app->map->pathfinding->GetDistance(playerPos, position) <= viewDistance * 32 && currentState != EntityState_Boss_Inuit::ATTACKING_BASIC)
	{
		desiredState = EntityState_Boss_Inuit::RUNNING;
	}
	else
	{
		desiredState = EntityState_Boss_Inuit::RUNNING;
	}
	stateMachine(dt, playerPos);


	if (atackCube != nullptr && playerInBossArea == false) {
		app->physics->GetWorld()->DestroyBody(atackCube->body);
		atackCube = nullptr;
	}



	switch (fase)
	{
	case FASE::FASE_ONE:
		break;
	case FASE::FASE_CHANGE:
		break;
	case FASE::FASE_TWO:
		break;
	}

	currentAnimation->Update();
	return true;
}


bool Boss_Inuit::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();


	//if (timerRecibirDanioColor.ReadMSec() <= 100) {
	//	float alpha = (100 - timerRecibirDanioColor.ReadMSec()) / 100;
	//	SDL_SetTextureAlphaMod(texture, static_cast<Uint8>(255 * alpha)); // Ajusta la opacidad

	//}
	//else {
	//	SDL_SetTextureAlphaMod(texture, 255);
	//}


	if (isFacingLeft) {
		app->render->DrawTexture(texture, position.x - 70, position.y - 200, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 70, position.y - 200, SDL_FLIP_NONE, &rect);
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


bool Boss_Inuit::CleanUp()
{
	app->physics->GetWorld()->DestroyBody(pbodyFoot->body);
	app->physics->GetWorld()->DestroyBody(pbodySensor->body);
	app->physics->GetWorld()->DestroyBody(areaSensor->body);
	app->tex->UnLoad(texture);
	lastPath.Clear();

	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}

void Boss_Inuit::DoNothing(float dt)
{
	//currentAnimation = &idleAnim;
	////printf("Osiris idle");
	//pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);

}

void Boss_Inuit::Chase(float dt, iPoint playerPos)
{
	////printf("Osiris chasing");
	//currentAnimation = &runAnim;
	Bossfinding(dt, playerPos);

}

void Boss_Inuit::Attack(float dt)
{
	////printf("Osiris attacking");
	//currentAnimation = &attackAnim;

	attackTime++;
	switch (attackTime)
	{
	case 1:
		inAtack = true;
		printf("\nataque1");

		atackCube = app->physics->CreateRectangleSensor(position.x, position.y, 60, 120, STATIC);

		break;
	case 2:
		inAtack = true;
		printf("\nataque2");
		atackCube = app->physics->CreateRectangleSensor(position.x, position.y, 60, 120, STATIC);
		break;
	case 3:
		inAtack = true;
		printf("\nataque3");
		atackCube = app->physics->CreateRectangleSensor(position.x, position.y, 60, 120, STATIC);
		break;

	case 4:
		inAtack = true;
		printf("\nataque4");
		playerDireccion = calculate_direction();
		printplayerDireccion = directionToString(playerDireccion);
		printf("\n PlayerDireccion %s", printplayerDireccion.c_str());

		attackTime = 0;
		break;

	default:
		break;
	}

	atackTimeColdDown.Start();

}

void Boss_Inuit::Die() {


}

void Boss_Inuit::Revive()
{

}



bool Boss_Inuit::Bossfinding(float dt, iPoint playerPosP)
{

	iPoint playerPos = app->map->WorldToMap(playerPosP.x, playerPosP.y);
	iPoint enemyPos = app->map->WorldToMap(position.x, position.y);

	//printf("\nBossArea:%f", dist(bossArea, enemyPos));
	//printf("\nEmemyPosicion:%d", enemyPos);

	if (playerInBossArea) {

		app->map->pathfinding->CreatePath(enemyPos, playerPos); // Calcula el camino desde la posicion del enemigo hacia la posicion del jugador
		lastPath = *app->map->pathfinding->GetLastPath();
		if (atackCube != nullptr) {
			app->physics->GetWorld()->DestroyBody(atackCube->body);
			atackCube = nullptr;
		}
		inAtack = false;
	}
	else {
		app->map->pathfinding->CreatePath(enemyPos, originalPosition); // Calcula el camino desde la posicion del enemigo hacia la posicion del jugador
		lastPath = *app->map->pathfinding->GetLastPath();
	}


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
		attackAnim.Reset();

	}
	pbodyFoot->body->SetLinearVelocity(velocity);

	return true;
}

float Boss_Inuit::GetHealth() const {
	return health;
}

void Boss_Inuit::TakeDamage(float damage) {


}


BTPDirection  Boss_Inuit::calculate_direction() {
	iPoint playerPos = app->entityManager->GetPlayer()->position;
	playerPos = app->map->WorldToMap(playerPos.x, playerPos.y);
	iPoint enemyPos = app->map->WorldToMap(position.x, position.y);

	int dx = playerPos.x - enemyPos.x;
	int dy = playerPos.y - enemyPos.y;

	double angle_rad = atan2(dy, dx);

	// 将弧度转换为角度
	double angle_deg = angle_rad * 180 / M_PI;
	if (angle_deg < 0) {
		angle_deg += 360; 
	}
	BTPDirection directions[] = { BTPDirection::RIGHT, BTPDirection::DOWNRIGHT, BTPDirection::UP, BTPDirection::DOWNLEFT, BTPDirection::LEFT, BTPDirection::UPLEFT, BTPDirection::DOWN, BTPDirection::UPRIGHT };
	int index = static_cast<int>(round(angle_deg / 45)) % 8;
	BTPDirection direction = directions[index];

	return direction;
}


std::string Boss_Inuit::directionToString(BTPDirection direction) {
	switch (direction) {
	case BTPDirection::LEFT:
		return "LEFT";
	case BTPDirection::RIGHT:
		return "RIGHT";
	case BTPDirection::UP:
		return "UP";
	case BTPDirection::DOWN:
		return "DOWN";
	case BTPDirection::UPLEFT:
		return "UPLEFT";
	case BTPDirection::UPRIGHT:
		return "UPRIGHT";
	case BTPDirection::DOWNLEFT:
		return "DOWNLEFT";
	case BTPDirection::DOWNRIGHT:
		return "DOWNRIGHT";
	default:
		return "UNKNOWN";
	}
}

void Boss_Inuit::stateMachine(float dt, iPoint playerPos)
{
	//printf("\ncurrentState: %d, desiredState: %d", static_cast<int>(currentState), static_cast<int>(desiredState));
	nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;
	switch (nextState) {
	case EntityState_Boss_Inuit::IDLE:
		DoNothing(dt);
		break;
	case EntityState_Boss_Inuit::RUNNING:
		Chase(dt, playerPos);
		break;
	case EntityState_Boss_Inuit::ATTACKING_BASIC:

		pbodyFoot->body->SetLinearVelocity(b2Vec2_zero); //No se mueve mientras ataca


		if (!inAtack) {
			Attack(dt);
		}

		if (TimerColdDown(1)) {
			if (atackCube != nullptr) {
				app->physics->GetWorld()->DestroyBody(atackCube->body);
				atackCube = nullptr;
			}
		}

		if (TimerColdDown(3)) {
			inAtack = false;

		}

		/*if (atackCube != nullptr) {
			inAtack = false;
			firstAtack = true;
			app->physics->GetWorld()->DestroyBody(atackCube->body);
			atackCube = nullptr;
		}*/


		//if (inAtack) {
		//	/*ataqueTimeClodDown = atackTimeColdDown.CountDown(60);*/
		//	printf("\nataqueTimeClodDown%: %f", ataqueTimeClodDown);
		//}
		//else
		//{
		//	Attack(dt);
		//}



		break;
	case EntityState_Boss_Inuit::ATTACKING_DISTANCE:
		break;
	case EntityState_Boss_Inuit::DEAD:
		Die();
		break;
	case EntityState_Boss_Inuit::DASHI:
		break;

	case EntityState_Boss_Inuit::FASE_CHANGE:
		break;
	case EntityState_Boss_Inuit::NONE:

		desiredState = EntityState_Boss_Inuit::IDLE;
		break;

	default:
		break;
	}
	currentState = nextState;

}

bool Boss_Inuit::TimerColdDown(float time)
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

void Boss_Inuit::ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate) {
	this->poisonDamage = poisonDamage;
	this->poisonDuration = poisonDuration;
	this->poisonTickRate = poisonTickRate;
	this->poisonTimer = 0.0f;
	this->timeSinceLastTick = 0.0f;
	this->poisoned = true;
}

void Boss_Inuit::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::PLAYER:
		if (physA->ctype == ColliderType::BOSSAREA) {
			//printf("\n entraBossArea");
			playerInBossArea = true;
		}
		LOG("Collision PLAYER");
		//restar vida al player
		break;
	case ColliderType::PLAYER_ATTACK:
		LOG("Collision Player_Attack");
		health -= app->entityManager->GetPlayer()->attackDamage;
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}

void Boss_Inuit::OnEndCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::PLAYER:
		if (physA->ctype == ColliderType::BOSSAREA) {
			//printf("Out");
			playerInBossArea = false;
			inAtack = false;
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