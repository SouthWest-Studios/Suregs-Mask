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
#include "Player.h"
#include "Item_Hueso.h"
#include "Item_Rubi.h"
#include <Optick/include/optick.h>
#include "Utils.cpp"





Boss_Musri::Boss_Musri() : Entity(EntityType::BOSS_MUSRI) {
	name = ("boss_musri");

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

	idleAnim.LoadAnim((char*)name.c_str(), "idleAnim", spritePositions);
	attackEmpujarAnim.LoadAnim((char*)name.c_str(), "attackEmpujarAnim", spritePositions);;
	attackFlechasRapidasAnim.LoadAnim((char*)name.c_str(), "attackFlechasRapidas", spritePositions);;
	attackFlechaCargadaAnim.LoadAnim((char*)name.c_str(), "attackFlechaCargada", spritePositions);;

	texture = app->tex->Load(config.attribute("texturePath").as_string());

	musri_dash_fx = app->audio->LoadAudioFx("musri_dash_fx");

	pbodyFoot = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
	pbodyFoot->entity = this;
	pbodyFoot->listener = this;
	pbodyFoot->ctype = ColliderType::ENEMY;

	pbodySensor = app->physics->CreateRectangleSensor(position.x, position.y, 130, 110, bodyType::DYNAMIC);
	pbodySensor->entity = this;
	pbodySensor->listener = this;
	pbodySensor->ctype = ColliderType::UNKNOWN;


	arrowTexture = app->tex->Load(config.attribute("arrowTexturePath").as_string());
	arrowChargedTexture = app->tex->Load(config.attribute("arrowChargedTexturePath").as_string());
	arrowChargedRastroTexture = app->tex->Load(config.attribute("arrowChargedRastroTexturePath").as_string());

	//originalPosition = app->map->WorldToMap(position.x, position.y);

	maxHealth = config.attribute("maxHealth").as_float();
	health = maxHealth;
	speed = config.attribute("speed").as_float() / 10;
	attackDamage = config.attribute("attackDamage").as_float();
	attackDistance = config.attribute("attackDistance").as_float();
	viewDistance = config.attribute("viewDistance").as_float();

	movePosition = GetRandomPosicion(position, 10);

	fase = FASE_Musri::FASE_ONE;
	//fase = FASE_Musri::FASE_CHANGE;

	cambiarPosicionTimer.Start(30000);
	dispararRafagasTimer.Start(30000);
	dispararFlechaRafagaTimer.Start(30000);

	habilidadEmpujeTimer.Start(30000);
	habilidadFlechaCargadaTimer.Start(30000);
	habilidadDashInvisibleTimer.Start(30000);

	room = GetCurrentRoom();
	
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
	case FASE_Musri::FASE_DYNIG:
		FaseDying(dt, playerPos);
	}





	stateMachine(dt, playerPos);




	currentAnimation->Update();
	return true;
}


bool Boss_Musri::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();


	if (timerRecibirDanioColor.ReadMSec() <= 100) {
		float alpha = (100 - timerRecibirDanioColor.ReadMSec()) / 100;
		SDL_SetTextureAlphaMod(texture, static_cast<Uint8>(alphaTexture * alpha)); // Ajusta la opacidad

	}
	else {
		SDL_SetTextureAlphaMod(texture, alphaTexture);
	}



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

	for (int i = 0; i < flechasCargadas.size(); i++) {
		FlechaCargadaMusri flechaC = flechasCargadas.at(i);
		b2Vec2 arrowPos = flechaC.pbody->body->GetTransform().p;
		app->render->DrawTexture(arrowChargedTexture, METERS_TO_PIXELS(arrowPos.x), METERS_TO_PIXELS(arrowPos.y), 1, SDL_FLIP_NONE, 0, 1, GetAngleFromDirection(flechaC.direction) + 180, 0, 0);

		for (int j = 0; j < flechaC.rastroGenerado.size(); j++) {
			RastroFlechaCargadaMusri rastro = flechaC.rastroGenerado.at(j);
			app->render->DrawTexture(arrowChargedRastroTexture, METERS_TO_PIXELS(rastro.position.x), METERS_TO_PIXELS(rastro.position.y), 1, SDL_FLIP_NONE, 0, 1, GetAngleFromDirection(flechaC.direction) + 180);
		}
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

	Item_Rubi* rubi = (Item_Rubi*)app->entityManager->CreateEntity(EntityType::ITEM_RUBI);
	rubi->config = configNode.child("entities_data").child("item_rubi");
	rubi->position = iPoint(position.x, position.y);
	rubi->Start();

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

void Boss_Musri::Revive()
{

}


// L07 DONE 6: Define OnCollision function for the player. 
void Boss_Musri::OnCollision(PhysBody* physA, PhysBody* physB) {


	if (physA->ctype == ColliderType::BOSS_MUSRI_ARROW) {
		//Colisiones flecha
		switch (physB->ctype) {
		
		case ColliderType::PLAYER:
			app->entityManager->GetPlayer()->TakeDamage(flechaRafagaDamage);
		case ColliderType::PLAYER_ATTACK:
		case ColliderType::PLATFORM:
			for (int i = 0; i < flechasLanzadas.size(); i++) {
				if (flechasLanzadas.at(i).pbody->body->GetTransform().p == physA->body->GetTransform().p) {
					app->physics->DestroyBody(flechasLanzadas.at(i).pbody);
					flechasLanzadas.erase(flechasLanzadas.begin() + i);
					break;
				}
			}
			break;

		}
	}if (physA->ctype == ColliderType::BOSS_MUSRI_CHARGED_ARROW) {
		//Colisiones flecha
		switch (physB->ctype) {

		case ColliderType::PLAYER:
			app->entityManager->GetPlayer()->TakeDamage(flechaCargadaDamage);
		case ColliderType::PLAYER_ATTACK:
		case ColliderType::PLATFORM:
			for (int i = 0; i < flechasCargadas.size(); i++) {
				if (flechasCargadas.at(i).pbody->body->GetTransform().p == physA->body->GetTransform().p) {
					app->physics->DestroyBody(flechasCargadas.at(i).pbody);
					flechasCargadas.erase(flechasCargadas.begin() + i);
					break;
				}
			}
			break;

		}
	}if (physA->ctype == ColliderType::BOSS_MUSRI_CHARGED_RASTRO_ARROW) {
		//Colisiones flecha
		switch (physB->ctype) {

		case ColliderType::PLAYER:
			app->entityManager->GetPlayer()->TakeDamage(flechaCargadaRastroDamage);
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

	if (invulnerabilityTimer.ReadMSec() >= 100) {
		health -= damage;
		LOG("Musri health: %f / %f", health, maxHealth);
		invulnerabilityTimer.Start();
	}



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
			
			fPoint dirToPlayer = getDirectionVector(position, playerPos);
			pbodyFoot->body->ApplyForceToCenter(b2Vec2(-dirToPlayer.x * fuerzaHabilidadEmpuje, -dirToPlayer.y * fuerzaHabilidadEmpuje), true);
			
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

	if (habilidadEmpujeTimer.ReadMSec() >= 400 && cambiarPosicionTimer.ReadMSec() < cambiarPosicionTime) {
		pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
	}
	else {
		if (habilidadEmpujeTimer.ReadMSec() < 400) {
			fPoint dirToPlayer = getDirectionVector(position, playerPos);
			app->entityManager->GetPlayer()->pbodyFoot->body->ApplyForceToCenter(b2Vec2(dirToPlayer.x * fuerzaHabilidadEmpuje, dirToPlayer.y * fuerzaHabilidadEmpuje), true);
		}
	}

	if (health <= 4200) {
		fase = FASE_Musri::FASE_CHANGE;
	}

	
	/*LOG("POSDX: %d, POSDY: %d, HA LLEGADO: %d, ", movePosition.x, movePosition.y, haLlegado);
	LOG("Timer cambiarPosicion: %f", cambiarPosicionTimer.ReadMSec());*/





}
void Boss_Musri::FaseC(float dt, iPoint playerPos)
{

	cambiarPosicionTime = 10000;
	numeroRafagas = 5;
	habilidadRafagasCD = 800;

	movePosition = iPoint(limitesSala.x + (limitesSala.w/2), limitesSala.y + (limitesSala.h/2));
	if (Bossfinding(dt, movePosition)) {
		fase = FASE_Musri::FASE_TWO;
	}

}
void Boss_Musri::Fase2(float dt, iPoint playerPos)
{
	//Fase 2
	bool haLlegado = false;
	if (cambiarPosicionTimer.ReadMSec() > cambiarPosicionTime) {

		//Si tiene la habilidad de dashear invisiblem dashea 3 veces (poca distancia) cada vez mas invisible; Al terminar el dash, la invisibilidad dura 4s
		if (habilidadDashInvisibleTimer.ReadMSec() >= habilidadDashInvisibleCD) {
			if (numeroDashesAct <= numeroDashes) {
				haLlegado = Bossfinding(dt, movePosition);
				if (haLlegado) {
					movePosition = GetRandomPosicion(movePosition, 3, 7); //Para la proxima
					numeroDashesAct++;
					alphaTexture -= opacidadQuitadaDashes;
				}
			}
			else {
				habilidadDashInvisibleTimer.Start();
				cambiarPosicionTimer.Start();
				numeroDashesAct = 0;
			}
		}
		else {
			haLlegado = Bossfinding(dt, movePosition);
			if (haLlegado) {
				cambiarPosicionTimer.Start();
				movePosition = GetRandomPosicion(movePosition, 5, 10); //Para la proxima
			}
		}


		
	}
	else {
		//AtaqueFlechas + lo otro
		if (dist(playerPos, position) < meleeAttackDistance * 32 && habilidadEmpujeTimer.ReadMSec() >= habilidadEmpujeCD) {
			//Realizar ataque empuje
			habilidadEmpujeTimer.Start();

			fPoint dirToPlayer = getDirectionVector(position, playerPos);
			pbodyFoot->body->ApplyForceToCenter(b2Vec2(-dirToPlayer.x * fuerzaHabilidadEmpuje, -dirToPlayer.y * fuerzaHabilidadEmpuje), true);

		}
		else if (habilidadFlechaCargadaTimer.ReadMSec() >= habilidadCargadaCD) {
			//Habilidad flecha cargada
			if (cargaFlechaRafagaTimer.ReadMSec() > 2000) { //2S de carga
				if (cargaFlechaRafagaTimer.ReadMSec() < 3000) {
					//Disparar la flecha
					fPoint dirToPlayer = getDirectionVector(position, playerPos);
					FlechaCargadaMusri flechaCargada = { dirToPlayer, app->physics->CreateCircle(position.x, position.y, 10, bodyType::DYNAMIC) };
					flechaCargada.pbody->listener = this;
					flechaCargada.pbody->ctype = ColliderType::BOSS_MUSRI_ARROW;
					flechaCargada.pbody->body->GetFixtureList()->SetSensor(true);
					flechaCargada.pbody->body->ApplyForceToCenter(b2Vec2(flechaCargada.direction.x * velocidadFlechas * 1.2f, flechaCargada.direction.y * velocidadFlechas * 1.2f), true);
					flechaCargada.dejarRastroTimer.Start();
					flechasCargadas.push_back(flechaCargada);
					//flechasCargadas
					habilidadFlechaCargadaTimer.Start();
					dispararRafagasTimer.Start(-5000);
				}
				else {
					cargaFlechaRafagaTimer.Start();
				}

			}
			
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

						flecha.pbody->body->ApplyForceToCenter(b2Vec2(flecha.direction.x * velocidadFlechas, flecha.direction.y * velocidadFlechas), true);
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

	if (habilidadEmpujeTimer.ReadMSec() >= 400 && cambiarPosicionTimer.ReadMSec() < cambiarPosicionTime) {
		pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
	}
	else {
		if (habilidadEmpujeTimer.ReadMSec() < 400) {
			fPoint dirToPlayer = getDirectionVector(position, playerPos);
			app->entityManager->GetPlayer()->pbodyFoot->body->ApplyForceToCenter(b2Vec2(dirToPlayer.x * fuerzaHabilidadEmpuje, dirToPlayer.y * fuerzaHabilidadEmpuje), true);
		}
	}

	//Volver a ser visible
	

	if (habilidadDashInvisibleTimer.ReadMSec() <= 4000) {
		alphaTexture = (255-(numeroDashes* opacidadQuitadaDashes)) + ((255 - (255 - (numeroDashes * opacidadQuitadaDashes))) * habilidadDashInvisibleTimer.ReadMSec() / 4000);
	}
	else {
		if (habilidadDashInvisibleTimer.ReadMSec() <= 4500) {
			alphaTexture = 255;
		}
	}




	for (int i = 0; i < flechasCargadas.size(); i++) {
		FlechaCargadaMusri* flechaC = &flechasCargadas.at(i);
		if (flechaC->dejarRastroTimer.ReadMSec() > 50 &&  !flechaC->flechaRastroTerminado) {
			//Spawn rastro
			b2Vec2 flechaPos = flechaC->pbody->body->GetTransform().p;
			RastroFlechaCargadaMusri rastro = { iPoint(flechaPos.x, flechaPos.y) };
			rastro.pbody = app->physics->CreateCircle(METERS_TO_PIXELS(flechaPos.x), METERS_TO_PIXELS(flechaPos.y), 20, bodyType::DYNAMIC);
					
			rastro.pbody->listener = this;
			rastro.pbody->ctype = ColliderType::BOSS_MUSRI_ARROW;
			rastro.pbody->body->GetFixtureList()->SetSensor(true);
			rastro.lifeTimer.Start();
			flechaC->rastroGenerado.push_back(rastro);
			flechaC->dejarRastroTimer.Start();
		}

		if (flechaC->rastroGenerado.size() >= flechaC->maxRastro) {
			flechaC->flechaRastroTerminado = true;
		}

		for (int j = 0; j < flechaC->rastroGenerado.size(); j++) {

			RastroFlechaCargadaMusri* rastro = &flechaC->rastroGenerado.at(j);

			if (rastro->lifeTimer.ReadMSec() > 3000) {
				app->physics->GetWorld()->DestroyBody(rastro->pbody->body);
				flechaC->rastroGenerado.erase(flechaC->rastroGenerado.begin() + j);

				if (flechaC->rastroGenerado.size() == 0) {
					app->physics->GetWorld()->DestroyBody(flechaC->pbody->body);
					flechasCargadas.clear();
				}

				break;
			}
		}
	}

	if (health <= 0) {
		fase = FASE_Musri::FASE_DYNIG;
	}

}

void Boss_Musri::FaseDying(float dt, iPoint playerPos)
{
	movePosition = iPoint(limitesSala.x + (limitesSala.w / 2), limitesSala.y + (limitesSala.h / 2));
	if (Bossfinding(dt, movePosition)) {
		//C muere
		if (muriendoseTimer.ReadMSec() >= 3000) {
			app->physics->GetWorld()->DestroyBody(pbodyFoot->body);
			app->physics->GetWorld()->DestroyBody(pbodySensor->body);
			app->entityManager->DestroyEntity(this);
		}
	}
	else {
		muriendoseTimer.Start();
	}

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

MapObject* Boss_Musri::GetCurrentRoom()
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