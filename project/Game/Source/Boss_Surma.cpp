#include "Boss_Surma.h"

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
#include "Player.h"
#include "Item_Hueso.h"
#include <Optick/include/optick.h>
#include "Utils.cpp"





Boss_Surma::Boss_Surma() : Entity(EntityType::BOSS_MUSRI) {
	name = ("boss_surma");

}

Boss_Surma::~Boss_Surma() {

}

bool Boss_Surma::Awake() {

	return true;
}

bool Boss_Surma::Start() {

	OPTICK_EVENT();
	//position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	TSprite = config.attribute("Tsprite").as_int();
	SpriteX = config.attribute("sprite_x").as_int();
	SpriteY = config.attribute("sprite_y").as_int();
	Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

	idleAnim.LoadAnim((char*)name.c_str(), "idleAnim", spritePositions);
	runAnim.LoadAnim((char*)name.c_str(), "runAnim", spritePositions);
	combo1Anim.LoadAnim((char*)name.c_str(), "combo1Anim", spritePositions);
	combo2Anim.LoadAnim((char*)name.c_str(), "combo2Anim", spritePositions);
	combo3Anim.LoadAnim((char*)name.c_str(), "combo3Anim", spritePositions);
	ataqueCargadoAnim.LoadAnim((char*)name.c_str(), "ataqueCargadoAnim", spritePositions);
	ataqueCargadoEjecutarAnim.LoadAnim((char*)name.c_str(), "ataqueCargadoEjecutarAnim", spritePositions);
	ataqueCargadoExplosionAnim.LoadAnim((char*)name.c_str(), "ataqueCargadoExplosionAnim", spritePositions);
	cansadoAnim.LoadAnim((char*)name.c_str(), "cansadoAnim", spritePositions);
	ataqueRapidoAnim.LoadAnim((char*)name.c_str(), "ataqueRapidoAnim", spritePositions);
	muerteAnim.LoadAnim((char*)name.c_str(), "muerteAnim", spritePositions);
	cambioFaseAnim.LoadAnim((char*)name.c_str(), "cambioFaseAnim", spritePositions);



	texture = app->tex->Load(config.attribute("texturePath").as_string());

	Surma_dash_fx = app->audio->LoadAudioFx("surma_dash_fx");

	pbodyFoot = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
	pbodyFoot->entity = this;
	pbodyFoot->listener = this;
	pbodyFoot->ctype = ColliderType::ENEMY;

	pbodySensor = app->physics->CreateRectangleSensor(position.x, position.y, 150, 140, bodyType::DYNAMIC);
	pbodySensor->entity = this;
	pbodySensor->listener = this;
	pbodySensor->ctype = ColliderType::UNKNOWN;




	//originalPosition = app->map->WorldToMap(position.x, position.y);

	maxHealth = config.attribute("maxHealth").as_float();
	health = maxHealth;
	speed = (config.attribute("speed").as_float() / 10) * 0.4;
	attackDamage = config.attribute("attackDamage").as_float();
	attackDistance = config.attribute("attackDistance").as_float();
	viewDistance = config.attribute("viewDistance").as_float();



	fase = FASE_Surma::FASE_ONE;
	//fase = FASE_Surma::FASE_CHANGE;


	return true;
}

bool Boss_Surma::Update(float dt)
{
	OPTICK_EVENT();



	//Pone el sensor del cuerpo en su posicion
	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
	pbodySensor->body->SetTransform(b2Vec2(pbodyPos.p.x, pbodyPos.p.y - 1.8f), 0);

	iPoint playerPos = app->entityManager->GetPlayer()->position;

	//printf("\n %d : %d", playerPos.x, playerPos.y);

	/*if (health <= 0)
	{
		desiredState = EntityState_Boss_Surma::DEAD;
	}
	else if (app->map->pathfinding->GetDistance(playerPos, position) <= attackDistance * 32)
	{
		desiredState = EntityState_Boss_Surma::ATTACKING_BASIC;
	}
	else if (app->map->pathfinding->GetDistance(playerPos, position) <= viewDistance * 32 && currentState != EntityState_Boss_Surma::ATTACKING_BASIC)
	{
		desiredState = EntityState_Boss_Surma::RUNNING;
	}
	else
	{
		desiredState = EntityState_Boss_Surma::RUNNING;
	}*/

	switch (fase)
	{
	case FASE_Surma::FASE_ONE:
		Fase1(dt, playerPos);
		break;
	case FASE_Surma::FASE_CHANGE:
		FaseC(dt, playerPos);
		break;
	case FASE_Surma::FASE_TWO:
		Fase2(dt, playerPos);
		break;
	case FASE_Surma::FASE_DYNIG:
		FaseDying(dt, playerPos);
	}





	//stateMachine(dt, playerPos);




	currentAnimation->Update();
	return true;
}


bool Boss_Surma::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();


	if (isFacingLeft) {
		app->render->DrawTexture(texture, position.x - 220, position.y - 200, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 100, position.y - 200, SDL_FLIP_NONE, &rect);
	}

	if (app->physics->debug == true) {
		for (uint i = 0; i < lastPath.Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(lastPath.At(i)->x, lastPath.At(i)->y);

			app->render->DrawTexture(app->map->pathfinding->mouseTileTex, pos.x, pos.y, SDL_FLIP_NONE);

		}
	}

	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 16;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 16;








	return true;
}


bool Boss_Surma::CleanUp()
{
	app->physics->GetWorld()->DestroyBody(pbodyFoot->body);
	app->physics->GetWorld()->DestroyBody(pbodySensor->body);
	app->tex->UnLoad(texture);
	lastPath.Clear();

	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}

void Boss_Surma::DoNothing(float dt)
{
	//currentAnimation = &idleAnim;
	////printf("Osiris idle");
	//pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);

}

void Boss_Surma::Chase(float dt, iPoint playerPos)
{
	////printf("Osiris chasing");
	//currentAnimation = &runAnim;
	Bossfinding(dt, playerPos);

}

void Boss_Surma::Attack(float dt)
{
	////printf("Osiris attacking");
	//currentAnimation = &attackAnim;
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero); //No se mueve mientras ataca

}

void Boss_Surma::Die() {

	//Mask XP

//Mask 0
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK0)
	{
		app->entityManager->GetPlayer()->maskZeroXP += 3500;
		//printf("Current Mask 0 XP %i \n", app->entityManager->GetPlayer()->maskZeroXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK0)
	{
		app->entityManager->GetPlayer()->maskZeroXP += 3500;
		//printf("Current Mask 0 XP %i \n", app->entityManager->GetPlayer()->maskZeroXP);
	}

	//Mask 1
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK1)
	{
		app->entityManager->GetPlayer()->maskOneXP += 3500;
		//printf("Current Mask 1 XP %i \n", app->entityManager->GetPlayer()->maskOneXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK1)
	{
		app->entityManager->GetPlayer()->maskOneXP += 3500;
		//printf("Current Mask 1 XP %i \n", app->entityManager->GetPlayer()->maskOneXP);
	}

	//Mask 2
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK2)
	{
		app->entityManager->GetPlayer()->maskTwoXP += 3500;
		//printf("Current Mask 2 XP %i \n", app->entityManager->GetPlayer()->maskTwoXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK2)
	{
		app->entityManager->GetPlayer()->maskTwoXP += 3500;
		//printf("Current Mask 2 XP %i \n", app->entityManager->GetPlayer()->maskTwoXP);
	}

	//Mask 3
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK3)
	{
		app->entityManager->GetPlayer()->maskThreeXP += 3500;
		//printf("Current Mask 3 XP %i \n", app->entityManager->GetPlayer()->maskThreeXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK3)
	{
		app->entityManager->GetPlayer()->maskThreeXP += 3500;
		//printf("Current Mask 3 XP %i \n", app->entityManager->GetPlayer()->maskThreeXP);
	}

}

void Boss_Surma::Revive()
{

}


// L07 DONE 6: Define OnCollision function for the player. 
void Boss_Surma::OnCollision(PhysBody* physA, PhysBody* physB) {



	//Colisiones al boss
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
		timerRecibirDanioColor.Start();
		TakeDamage(app->entityManager->GetPlayer()->currentStats.attackDamage);
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}



}

bool Boss_Surma::Bossfinding(float dt, iPoint targetPosP)
{
	bool haLlegadoDestino = false;

	iPoint targetPos = app->map->WorldToMap(targetPosP.x, targetPosP.y);
	iPoint enemyPos = app->map->WorldToMap(position.x, position.y);


	if (dist(targetPos, enemyPos) < viewDistance ) {
		app->map->pathfinding->CreatePath(enemyPos, targetPos); // Calcula el camino desde la posicion del enemigo hacia la posicion del jugador
		lastPath = *app->map->pathfinding->GetLastPath();
	}
	else {
		//app->map->pathfinding->CreatePath(enemyPos, originalPosition); // Calcula el camino desde la posicion del enemigo hacia la posicion del jugador
		//lastPath = *app->map->pathfinding->GetLastPath();
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
		isFacingLeft = (direction.x < 0);


		isAttacking = false;
		//attackAnim.Reset();
		haLlegadoDestino = false;


	}
	else {
		haLlegadoDestino = true;
	}
	pbodyFoot->body->SetLinearVelocity(velocity);

	return haLlegadoDestino;
}

float Boss_Surma::GetHealth() const {
	return health;
}

void Boss_Surma::TakeDamage(float damage) {

	if (invulnerabilityTimer.ReadMSec() >= 100) {
		health -= damage;
		LOG("Surma health: %f / %f", health, maxHealth);
		invulnerabilityTimer.Start();
	}



}

void Boss_Surma::stateMachine(float dt, iPoint playerPos)
{
	//printf("\ncurrentState: %d, desiredState: %d", static_cast<int>(currentState), static_cast<int>(desiredState));
	nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;
	switch (nextState) {
	case EntityState_Boss_Surma::IDLE:
		DoNothing(dt);
		break;
	case EntityState_Boss_Surma::RUNNING:
		Chase(dt, playerPos);
		break;
	case EntityState_Boss_Surma::ATTACKING_BASIC:
		Attack(dt);
		break;
	case EntityState_Boss_Surma::ATTACKING_DISTANCE:
		break;
	case EntityState_Boss_Surma::DEAD:
		Die();
		break;
	case EntityState_Boss_Surma::DASHI:
		break;

	case EntityState_Boss_Surma::FASE_CHANGE:
		break;
	case EntityState_Boss_Surma::NONE:

		desiredState = EntityState_Boss_Surma::IDLE;
		break;

	default:
		break;
	}
	currentState = nextState;

}

void Boss_Surma::ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate) {
	this->poisonDamage = poisonDamage;
	this->poisonDuration = poisonDuration;
	this->poisonTickRate = poisonTickRate;
	this->poisonTimer = 0.0f;
	this->timeSinceLastTick = 0.0f;
	this->poisoned = true;
}

void Boss_Surma::Fase1(float dt, iPoint playerPos)
{

	jugadorCerca = (dist(playerPos, position) < meleeAttackDistance * 32);

	if (!jugadorCerca && !realizandoCombo) {
		Bossfinding(dt, playerPos);
		currentAnimation = &runAnim;
	}
	else {
		realizandoCombo = true;
		pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);

		if (!combo1Anim.HasFinished()) {
			currentAnimation = &combo1Anim;
			//PARTICULA ATAQUE FLOJO
		}
		else if (!combo2Anim.HasFinished()) {
			currentAnimation = &combo2Anim;
			//PARTICULA ATAQUE FLOJO
		}
		else if (!combo3Anim.HasFinished()) {
			currentAnimation = &combo3Anim;
			//PARTICULA ATAQUE FLOJO
			cargaAtaqueTimer.Start();
		}
		else {
			if (jugadorCerca || currentAnimation == &cansadoAnim) {
				if (cargaAtaqueTimer.ReadMSec() <= cargaAtaqueMS) {
					currentAnimation = &ataqueCargadoAnim;
				}
				else {
					currentAnimation = &ataqueCargadoEjecutarAnim;
					

					if (ataqueCargadoEjecutarAnim.HasFinished()) {
						
						currentAnimation = &cansadoAnim;
						LOG("CANSADO TIMER: %f", cansadoTimer.ReadMSec());
						if (cansadoTimer.ReadMSec() > cansadoMS) {
							realizandoCombo = false;
							combo1Anim.Reset();
							combo2Anim.Reset();
							combo3Anim.Reset();
							ataqueCargadoEjecutarAnim.Reset();
							ataqueCargadoAnim.Reset();
							//Reset todas las anims
						}
					}
					else {
						cansadoTimer.Start();
						//PARTICULA ATAQUE FUERTE
					}


				}
			}
			else {
				realizandoCombo = false;
				//Reset todas las anims
				combo1Anim.Reset();
				combo2Anim.Reset();
				combo3Anim.Reset();
				ataqueCargadoEjecutarAnim.Reset();
				ataqueCargadoAnim.Reset();
			}
			
		}


	}
	


}
void Boss_Surma::FaseC(float dt, iPoint playerPos)
{

	
}
void Boss_Surma::Fase2(float dt, iPoint playerPos)
{
	//Fase 2
	

	if (health <= 0) {
		fase = FASE_Surma::FASE_DYNIG;
	}

}

void Boss_Surma::FaseDying(float dt, iPoint playerPos)
{
	

}
