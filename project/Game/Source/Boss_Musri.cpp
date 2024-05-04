#include "Boss_Musri.h"

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





Boss_Musri::Boss_Musri() : Entity(EntityType::BOSS_MUSRI) {
	name.Create("boss_musri");

}

Boss_Musri::~Boss_Musri() {

}

bool Boss_Musri::Awake() {

	return true;
}

bool Boss_Musri::Start() {

	OPTICK_EVENT();
	//position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	TSprite = config.attribute("Tsprite").as_int();
	SpriteX = config.attribute("sprite_x").as_int();
	SpriteY = config.attribute("sprite_y").as_int();
	Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

	idleAnim.LoadAnim((char*)name.GetString(), "idleAnim", spritePositions);
	attackEmpujarAnim.LoadAnim((char*)name.GetString(), "attackEmpujarAnim", spritePositions);;
	attackFlechasRapidasAnim.LoadAnim((char*)name.GetString(), "attackFlechasRapidas", spritePositions);;
	attackFlechaCargadaAnim.LoadAnim((char*)name.GetString(), "attackFlechaCargada", spritePositions);;

	texture = app->tex->Load(config.attribute("texturePath").as_string());

	pbodyFoot = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
	pbodyFoot->entity = this;
	pbodyFoot->listener = this;
	pbodyFoot->ctype = ColliderType::ENEMY;

	pbodySensor = app->physics->CreateRectangleSensor(position.x, position.y, 130, 110, bodyType::DYNAMIC);
	pbodySensor->entity = this;
	pbodySensor->listener = this;
	pbodySensor->ctype = ColliderType::UNKNOWN;


	arrowTexture = app->tex->Load(config.attribute("arrowTexturePath").as_string());

	//originalPosition = app->map->WorldToMap(position.x, position.y);

	maxHealth = config.attribute("maxHealth").as_float();
	health = maxHealth;
	speed = config.attribute("speed").as_float() / 10;
	attackDamage = config.attribute("attackDamage").as_float();
	attackDistance = config.attribute("attackDistance").as_float();
	viewDistance = config.attribute("viewDistance").as_float();

	movePosition = GetRandomPosicion(position, 10);

	fase = FASE_Musri::FASE_ONE;

	cambiarPosicionTimer.Start();
	dispararRafagasTimer.Start();
	dispararFlechaRafagaTimer.Start();

	habilidadEmpujeTimer.Start();
	habilidadFlechaCargadaTimer.Start();
	habilidadDashInvisibleTimer.Start();

	numeroRafagasAct = 0;

	return true;
}

bool Boss_Musri::Update(float dt)
{
	OPTICK_EVENT();



	//Pone el sensor del cuerpo en su posicion
	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
	pbodySensor->body->SetTransform(b2Vec2(pbodyPos.p.x, pbodyPos.p.y - 1.8f), 0);

	iPoint playerPos = app->entityManager->GetPlayer()->position;

	//printf("\n %d : %d", playerPos.x, playerPos.y);

	/*if (health <= 0)
	{
		desiredState = EntityState_Boss_Musri::DEAD;
	}
	else if (app->map->pathfinding->GetDistance(playerPos, position) <= attackDistance * 32)
	{
		desiredState = EntityState_Boss_Musri::ATTACKING_BASIC;
	}
	else if (app->map->pathfinding->GetDistance(playerPos, position) <= viewDistance * 32 && currentState != EntityState_Boss_Musri::ATTACKING_BASIC)
	{
		desiredState = EntityState_Boss_Musri::RUNNING;
	}
	else
	{
		desiredState = EntityState_Boss_Musri::RUNNING;
	}*/

	switch (fase)
	{
	case FASE_Musri::FASE_ONE:
		Fase1(dt, playerPos);
		break;
	case FASE_Musri::FASE_CHANGE:
		FaseC(dt, playerPos);
		break;
	case FASE_Musri::FASE_TWO:
		Fase2(dt, playerPos);
		break;
	}





	stateMachine(dt, playerPos);




	currentAnimation->Update();
	return true;
}


bool Boss_Musri::PostUpdate() {

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
		app->render->DrawTexture(texture, position.x - 70, position.y - 150, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 70, position.y - 150, SDL_FLIP_NONE, &rect);
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

	
	//Flechas
	for (int i = 0; i < flechasLanzadas.size(); i++) {

		b2Vec2 arrowPos = flechasLanzadas.at(i).pbody->body->GetTransform().p;
		app->render->DrawTexture(arrowTexture, METERS_TO_PIXELS(arrowPos.x), METERS_TO_PIXELS(arrowPos.y), 1, SDL_FLIP_NONE, 0, 1, GetAngleFromDirection(flechasLanzadas.at(i).direction) + 180, 0, 0);
	}


	/*LIMITES SALA*/
	//app->render->DrawRectangle(limitesSala, 255,255,255,200,true,true);



	return true;
}


bool Boss_Musri::CleanUp()
{
	app->physics->GetWorld()->DestroyBody(pbodyFoot->body);
	app->physics->GetWorld()->DestroyBody(pbodySensor->body);
	app->tex->UnLoad(texture);
	lastPath.Clear();

	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}

void Boss_Musri::DoNothing(float dt)
{
	//currentAnimation = &idleAnim;
	////printf("Osiris idle");
	//pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);

}

void Boss_Musri::Chase(float dt, iPoint playerPos)
{
	////printf("Osiris chasing");
	//currentAnimation = &runAnim;
	Bossfinding(dt, playerPos);

}

void Boss_Musri::Attack(float dt)
{
	////printf("Osiris attacking");
	//currentAnimation = &attackAnim;
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero); //No se mueve mientras ataca

}

void Boss_Musri::Die() {


}

void Boss_Musri::Revive()
{

}


// L07 DONE 6: Define OnCollision function for the player. 
void Boss_Musri::OnCollision(PhysBody* physA, PhysBody* physB) {


	if (physA->ctype == ColliderType::BOSS_MUSRI_ARROW) {
		//Colisiones flecha
		switch (physB->ctype) {
		case ColliderType::PLAYER:
		case ColliderType::PLAYER_ATTACK:
		case ColliderType::PLATFORM:
			for (int i = 0; i < flechasLanzadas.size(); i++) {
				if (flechasLanzadas.at(i).pbody->body->GetTransform().p == physA->body->GetTransform().p) {
					app->physics->GetWorld()->DestroyBody(flechasLanzadas.at(i).pbody->body);
					flechasLanzadas.erase(flechasLanzadas.begin() + i);
					break;
				}
			}
			break;

		}
	}
	else {
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
			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
		default:
			break;
		}
	}

	
}

bool Boss_Musri::Bossfinding(float dt, iPoint targetPosP)
{
	bool haLlegadoDestino = false;

	iPoint targetPos = app->map->WorldToMap(targetPosP.x, targetPosP.y);
	iPoint enemyPos = app->map->WorldToMap(position.x, position.y);


	if (dist(targetPos, enemyPos) < viewDistance * 100000) {
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
		isFacingLeft = (direction.x >= 0);


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

float Boss_Musri::GetHealth() const {
	return health;
}

void Boss_Musri::TakeDamage(float damage) {


}

void Boss_Musri::stateMachine(float dt, iPoint playerPos)
{
	//printf("\ncurrentState: %d, desiredState: %d", static_cast<int>(currentState), static_cast<int>(desiredState));
	nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;
	switch (nextState) {
	case EntityState_Boss_Musri::IDLE:
		DoNothing(dt);
		break;
	case EntityState_Boss_Musri::RUNNING:
		Chase(dt, playerPos);
		break;
	case EntityState_Boss_Musri::ATTACKING_BASIC:
		Attack(dt);
		break;
	case EntityState_Boss_Musri::ATTACKING_DISTANCE:
		break;
	case EntityState_Boss_Musri::DEAD:
		Die();
		break;
	case EntityState_Boss_Musri::DASHI:
		break;

	case EntityState_Boss_Musri::FASE_CHANGE:
		break;
	case EntityState_Boss_Musri::NONE:

		desiredState = EntityState_Boss_Musri::IDLE;
		break;

	default:
		break;
	}
	currentState = nextState;

}

void Boss_Musri::ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate) {
	this->poisonDamage = poisonDamage;
	this->poisonDuration = poisonDuration;
	this->poisonTickRate = poisonTickRate;
	this->poisonTimer = 0.0f;
	this->timeSinceLastTick = 0.0f;
	this->poisoned = true;
}

void Boss_Musri::Fase1(float dt, iPoint playerPos)
{
	bool haLlegado = false;
	if (cambiarPosicionTimer.ReadMSec() > cambiarPosicionTime) {
		haLlegado = Bossfinding(dt, movePosition);
		if (haLlegado) {
			cambiarPosicionTimer.Start();
			movePosition = GetRandomPosicion(movePosition, 10); //Para la proxima
		}
	}
	else {
		//AtaqueFlechas + lo otro
		if (dist(playerPos, position) < meleeAttackDistance * 32 && habilidadEmpujeTimer.ReadMSec() >= habilidadEmpujeCD) {
			//Realizar ataque empuje
			habilidadEmpujeTimer.Start();
		}
		else {

			if (dispararRafagasTimer.ReadMSec() >= habilidadRafagasCD) {
				
				//Disparar rafagas
				//flechasLanzadas
				if (numeroRafagasAct < numeroRafagas) {
					if (dispararFlechaRafagaTimer.ReadMSec() >= 100) {

						dispararFlechaRafagaTimer.Start();
						FlechaMusri flecha = { getDirectionVector(position, playerPos), app->physics->CreateCircle(position.x, position.y, 10, bodyType::DYNAMIC) };
						flecha.pbody->listener = this;
						flecha.pbody->ctype = ColliderType::BOSS_MUSRI_ARROW;
						flecha.pbody->body->GetFixtureList()->SetSensor(true);
						flecha.lifeTimer.Start();
						flechasLanzadas.push_back(flecha);

						flecha.pbody->body->ApplyForceToCenter(b2Vec2(flecha.direction.x* velocidadFlechas, flecha.direction.y * velocidadFlechas), true);
						numeroRafagasAct++;
					}
				}
				else {
					dispararRafagasTimer.Start(); //Reset el timer de las 3 flechas
					numeroRafagasAct = 0;
				}
				



			}


		}




	}

	//Gestionar las flechas
	/*for (int i = 0; i < flechasLanzadas.size(); i++) {
		if (flechasLanzadas.at(i).lifeTimer.ReadMSec() >= 300) {
			app->physics->GetWorld()->DestroyBody(flechasLanzadas.at(i).pbody->body);
			flechasLanzadas.erase(flechasLanzadas.begin() + i);
			break;
		}
	}*/



	/*LOG("POSDX: %d, POSDY: %d, HA LLEGADO: %d, ", movePosition.x, movePosition.y, haLlegado);
	LOG("Timer cambiarPosicion: %f", cambiarPosicionTimer.ReadMSec());*/





}
void Boss_Musri::FaseC(float dt, iPoint playerPos)
{
}
void Boss_Musri::Fase2(float dt, iPoint playerPos)
{
}

iPoint Boss_Musri::GetRandomPosicion(iPoint actualPosition, int distanceLimitInf, int distanceLimitSup)
{
	iPoint finalTarget;
	int distT;

	do {
		finalTarget.x = limitesSala.x + rand() % limitesSala.w;
		finalTarget.y = limitesSala.y + rand() % limitesSala.h;
		distT = (dist(finalTarget, actualPosition) / 32);
	} while (distT < distanceLimitInf && distT > distanceLimitSup);

	return finalTarget;
}
