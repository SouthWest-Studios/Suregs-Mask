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
#include "Item_Diamante.h"
#include "BestiarioManager.h"
#include "InventoryManager.h"
#include "Item_Mascara_3.h"
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
	combo2Anim.Reverse();
	combo3Anim.LoadAnim((char*)name.c_str(), "combo3Anim", spritePositions);
	ataqueCargadoAnim.LoadAnim((char*)name.c_str(), "ataqueCargadoAnim", spritePositions);
	ataqueCargadoEjecutarAnim.LoadAnim((char*)name.c_str(), "ataqueCargadoEjecutarAnim", spritePositions);
	ataqueCargadoExplosionAnim.LoadAnim((char*)name.c_str(), "ataqueCargadoExplosionAnim", spritePositions);
	cansadoAnim.LoadAnim((char*)name.c_str(), "cansadoAnim", spritePositions);
	ataqueRapidoAnim.LoadAnim((char*)name.c_str(), "ataqueRapidoAnim", spritePositions);
	muerteAnim.LoadAnim((char*)name.c_str(), "muerteAnim", spritePositions);
	cambioFaseAnim.LoadAnim((char*)name.c_str(), "cambioFaseAnim", spritePositions);


	spritePositionsExplosion = SPositionExplosion.SpritesPos(10, 863, 600, 6904);
	explosionAnim.LoadAnim((char*)name.c_str(), "explosionAnim", spritePositionsExplosion);


	surma_get_damage_fx = app->audio->LoadAudioFx("surma_get_damage_fx");

	pbodyFoot = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
	pbodyFoot->entity = this;
	pbodyFoot->listener = this;
	pbodyFoot->ctype = ColliderType::ENEMY;

	pbodySensor = app->physics->CreateRectangleSensor(position.x, position.y, 150, 140, bodyType::DYNAMIC);
	pbodySensor->entity = this;
	pbodySensor->listener = this;
	pbodySensor->ctype = ColliderType::UNKNOWN;
	texture = app->tex->Load(config.attribute("texturePath").as_string());
	textureExplosion = app->tex->Load(config.attribute("texturePathExplosion").as_string());


	//originalPosition = app->map->WorldToMap(position.x, position.y);

	maxHealth = config.attribute("maxHealth").as_float();
	health = maxHealth;
	speed = (config.attribute("speed").as_float() / 10) * 0.4;
	speedSecondFase = (config.attribute("speedSecondFase").as_float() / 10) * 0.4;
	attackDamage = config.attribute("attackDamage").as_float();
	attackDistance = config.attribute("attackDistance").as_float();
	viewDistance = config.attribute("viewDistance").as_float();

	room = GetCurrentRoom();

	fase = FASE_Surma::FASE_ONE;
	//fase = FASE_Surma::FASE_CHANGE;

	pugi::xml_parse_result parseResult2 = configFile.load_file("config.xml");
	if (parseResult2) {
		configNode = configFile.child("config");
	}


	return true;
}

bool Boss_Surma::Update(float dt)
{
	OPTICK_EVENT();



	//Pone el sensor del cuerpo en su posicion
	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
	pbodySensor->body->SetTransform(b2Vec2(pbodyPos.p.x, pbodyPos.p.y - 1.8f), 0);

	iPoint playerPos = app->entityManager->GetPlayer()->position;

	

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



	if (explosionActual != nullptr) {

		tamañoExplosionActual += dt * 0.8f;

		app->physics->DestroyBody(explosionActual->pbody);

		explosionActual->pbody = app->physics->CreateCircle(position.x, position.y + 50, tamañoExplosionActual, bodyType::DYNAMIC);
		explosionActual->pbody->body->GetFixtureList()->SetSensor(true);
		explosionActual->pbody->ctype = ColliderType::BOSS_SURMA_EXPLOSION;
		explosionActual->pbody->listener = this;
		explosionActual->currentAnimation->Update();

		if (explosionActual->lifeTime.ReadMSec() > 300) {
			app->physics->DestroyBody(explosionActual->pbody);
			delete explosionActual;
			explosionActual = nullptr;
		}


	}

	//stateMachine(dt, playerPos);




	currentAnimation->Update();
	return true;
}


bool Boss_Surma::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();


	//Lo de recibir daño
	if (timerRecibirDanioColor.ReadMSec() <= 100) {
		float alpha = (100 - timerRecibirDanioColor.ReadMSec()) / 100;
		SDL_SetTextureAlphaMod(texture, static_cast<Uint8>(255 * alpha)); // Ajusta la opacidad

	}
	else {
		SDL_SetTextureAlphaMod(texture, 255);
	}

	SDL_SetTextureColorMod(texture, actualColorTint.r, actualColorTint.g, actualColorTint.b);






	if (isFacingLeft) {

		app->render->DrawTexture(texture, position.x + sprieOffsetL.x, position.y + sprieOffsetL.y, actualScale, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x + sprieOffsetR.x, position.y + sprieOffsetR.y, actualScale, SDL_FLIP_NONE, &rect);
	}

	if (app->physics->debug == true) {
		for (uint i = 0; i < lastPath.Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(lastPath.At(i)->x, lastPath.At(i)->y);

			app->render->DrawTexture(app->map->pathfinding->mouseTileTex, pos.x, pos.y, SDL_FLIP_NONE);

		}
	}


	if (explosionActual != nullptr) {
		SDL_Rect rectExp = explosionActual->currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(textureExplosion, position.x - 350, position.y - 250, 1, SDL_FLIP_HORIZONTAL, &rectExp);
	}


	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 16;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 16;








	return true;
}


bool Boss_Surma::CleanUp()
{
	app->entityManager->DestroyEntity(this);
	app->physics->DestroyBody(pbodyFoot);
	app->physics->DestroyBody(pbodySensor);
	app->tex->UnLoad(texture);
	app->tex->UnLoad(textureExplosion);
	app->tex->UnLoad(textureExplosion);
	lastPath.Clear();


	RELEASE(spritePositions);
	delete spritePositions;

	RELEASE(spritePositionsExplosion);
	delete spritePositionsExplosion;

	RELEASE(explosionActual);
	delete explosionActual;

	return true;
}

void Boss_Surma::DoNothing(float dt)
{
	//currentAnimation = &idleAnim;
	//////printf("Osiris idle");
	//pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);

}

void Boss_Surma::Chase(float dt, iPoint playerPos)
{
	//////printf("Osiris chasing");
	//currentAnimation = &runAnim;
	Bossfinding(dt, playerPos);

}

void Boss_Surma::Attack(float dt)
{
	//////printf("Osiris attacking");
	//currentAnimation = &attackAnim;
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero); //No se mueve mientras ataca

}

void Boss_Surma::Die() {
	app->map->boss3_defeated = true;

	Item_Diamante* diamante = (Item_Diamante*)app->entityManager->CreateEntity(EntityType::ITEM_DIAMANTE);
	diamante->config = configNode.child("entities_data").child("item_diamante");
	diamante->position = iPoint(position.x, position.y);
	diamante->Start();

	app->bestiarioManager->CreateItem("surma");

	Item_mascara_3* mascara3 = (Item_mascara_3*)app->entityManager->CreateEntity(EntityType::ITEM_MASCARA3);
	mascara3->config = configNode.child("entities_data").child("item_mascara_3");
	mascara3->position = iPoint(position.x, position.y);
	mascara3->Start();
	//Mask XP

	//Mask 0
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK0)
	{
		app->inventoryManager->maskZeroXP += 3500;
		////printf("Current Mask 0 XP %i \n", app->inventoryManager->maskZeroXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK0)
	{
		app->inventoryManager->maskZeroXP += 3500;
		////printf("Current Mask 0 XP %i \n", app->inventoryManager->maskZeroXP);
	}

	//Mask 1
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK1)
	{
		app->inventoryManager->maskOneXP += 3500;
		////printf("Current Mask 1 XP %i \n", app->inventoryManager->maskOneXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK1)
	{
		app->inventoryManager->maskOneXP += 3500;
		////printf("Current Mask 1 XP %i \n", app->inventoryManager->maskOneXP);
	}

	//Mask 2
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK2)
	{
		app->inventoryManager->maskTwoXP += 3500;
		////printf("Current Mask 2 XP %i \n", app->inventoryManager->maskTwoXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK2)
	{
		app->inventoryManager->maskTwoXP += 3500;
		////printf("Current Mask 2 XP %i \n", app->inventoryManager->maskTwoXP);
	}

	//Mask 3
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK3)
	{
		app->inventoryManager->maskThreeXP += 3500;
		////printf("Current Mask 3 XP %i \n", app->inventoryManager->maskThreeXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK3)
	{
		app->inventoryManager->maskThreeXP += 3500;
		////printf("Current Mask 3 XP %i \n", app->inventoryManager->maskThreeXP);
	}

}

void Boss_Surma::Revive()
{

}


// L07 DONE 6: Define OnCollision function for the player.
void Boss_Surma::OnCollision(PhysBody* physA, PhysBody* physB) {

	//Colision explosion
	if (explosionActual != nullptr && physA == explosionActual->pbody) {
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:

			if (!explosionAlcanzada) {
				physB->listener->TakeDamage(ataqueExplosionDamage);
				((Player*)physB->listener)->SlowDown(0.4);
				explosionAlcanzada = true;
			}
			break;
		}
	}

	if ((attackSensor != nullptr && physA == attackSensor) || (attackSensor2 != nullptr && physA == attackSensor2)) {
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:

			physB->listener->TakeDamage(ataqueNormalDamage);

			break;
		}
	}

	if ((attackBigSensor != nullptr && physA == attackBigSensor)) {
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:

			physB->listener->TakeDamage(ataqueCargadoDamage);

			break;
		}
	}


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


	if (dist(targetPos, enemyPos) < viewDistance) {
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
		app->audio->PlayFx(surma_get_damage_fx);
	}



}

void Boss_Surma::stateMachine(float dt, iPoint playerPos)
{
	////printf("\ncurrentState: %d, desiredState: %d", static_cast<int>(currentState), static_cast<int>(desiredState));
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
			if (attackSensor == nullptr && combo1Anim.GetCurretFrameNumber() >= 3) {
				if (isFacingLeft) {
					attackSensor = app->physics->CreateRectangleSensor(position.x - 90, position.y - 25, 60, 150, bodyType::KINEMATIC);
				}
				else {
					attackSensor = app->physics->CreateRectangleSensor(position.x + 120, position.y - 25, 60, 150, bodyType::KINEMATIC);
				}
				attackSensor->listener = this;
			}
			
			
			
		}
		else if (!combo2Anim.HasFinished()) {
			currentAnimation = &combo2Anim;
			//PARTICULA ATAQUE FLOJO
			if (attackSensor != nullptr) {
				app->physics->DestroyBody(attackSensor);
				attackSensor = nullptr;
			}
			
			if (attackSensor2 == nullptr && combo2Anim.GetCurretFrameNumber() >= 3) {
				if (isFacingLeft) {
					attackSensor2 = app->physics->CreateRectangleSensor(position.x + 120, position.y - 25, 60, 150, bodyType::KINEMATIC);
				}
				else {
					attackSensor2 = app->physics->CreateRectangleSensor(position.x - 90, position.y - 25, 60, 150, bodyType::KINEMATIC);
				}
				attackSensor2->listener = this;
			}
			
			
			
		}
		else if (!combo3Anim.HasFinished()) {
			currentAnimation = &combo3Anim;
			//PARTICULA ATAQUE FLOJO

			if (attackSensor2 != nullptr) {
				app->physics->DestroyBody(attackSensor2);
				attackSensor2 = nullptr;
			}


			if (attackSensor == nullptr && combo3Anim.GetCurretFrameNumber() >= 3) {
				if (isFacingLeft) {
					attackSensor = app->physics->CreateRectangleSensor(position.x - 90, position.y - 25, 60, 150, bodyType::KINEMATIC);
				}
				else {
					attackSensor = app->physics->CreateRectangleSensor(position.x + 120, position.y - 25, 60, 150, bodyType::KINEMATIC);
				}
				attackSensor->listener = this;
			}
			

			cargaAtaqueTimer.Start();
		}
		else {
			if (attackSensor != nullptr) {
				app->physics->DestroyBody(attackSensor);
				attackSensor = nullptr;
			}
			if (jugadorCerca || currentAnimation == &cansadoAnim) {
				if (cargaAtaqueTimer.ReadMSec() <= cargaAtaqueMS) {
					currentAnimation = &ataqueCargadoAnim;
				}
				else {
					currentAnimation = &ataqueCargadoEjecutarAnim;
					if (attackBigSensor == nullptr && ataqueCargadoEjecutarAnim.GetCurretFrameNumber() >= 2 && !ataqueCargadoEjecutarAnim.HasFinished()) {
						attackBigSensor = app->physics->CreateRectangleSensor(position.x + 16, position.y + 50, 180, 100, bodyType::KINEMATIC);
						attackBigSensor->listener = this;
					}

					if (ataqueCargadoEjecutarAnim.HasFinished()) {
						if (attackBigSensor != nullptr) {
							app->physics->DestroyBody(attackBigSensor);
							attackBigSensor = nullptr;
						}
						currentAnimation = &cansadoAnim;
						if (cansadoTimer.ReadMSec() > cansadoMS) {
							realizandoCombo = false;
							combo1Anim.Reset();
							combo2Anim.Reset();
							combo3Anim.Reset();
							ataqueCargadoEjecutarAnim.Reset();
							ataqueCargadoAnim.Reset();
							//Reset todas las anims
						}
						else {
							if (attackBigSensor != nullptr) {
								app->physics->DestroyBody(attackBigSensor);
								attackBigSensor = nullptr;
							}
						}
					}
					else {
						cansadoTimer.Start();
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

	if (health <= 7000) {
		fase = FASE_Surma::FASE_CHANGE;
		cambioFaseTimer.Start();
	}



}
void Boss_Surma::FaseC(float dt, iPoint playerPos)
{
	//Cambia color y se hace mas grandecito
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
	currentAnimation = &cambioFaseAnim;
	speed = speedSecondFase;



	if (cambioFaseTimer.ReadMSec() < cambioFaseMS) {
		actualColorTint.r = lerp(actualColorTint.r, 200, 0.01);
		actualColorTint.g = lerp(actualColorTint.g, 0, 0.01);
		actualColorTint.b = lerp(actualColorTint.b, 200, 0.01);
		actualScale = lerp(actualScale, 0.9, 0.01);
		sprieOffsetR.x = lerp(sprieOffsetR.x, -340, 0.01);
		sprieOffsetR.y = lerp(sprieOffsetR.y, -520, 0.01);
		sprieOffsetL.x = lerp(sprieOffsetL.x, -340, 0.01);
		sprieOffsetL.y = lerp(sprieOffsetL.y, -520, 0.01);

		/*sprieOffsetR.x = -120;
		sprieOffsetR.y = -230;
		sprieOffsetL.x = -250;
		sprieOffsetL.y = -230;*/
	}
	else {

		app->physics->DestroyBody(pbodySensor);
		pbodySensor = app->physics->CreateRectangleSensor(position.x, position.y, 175, 160, bodyType::DYNAMIC);
		pbodySensor->entity = this;
		pbodySensor->listener = this;
		pbodySensor->ctype = ColliderType::UNKNOWN;

		fase = FASE_Surma::FASE_TWO;
	}




}
void Boss_Surma::Fase2(float dt, iPoint playerPos)
{
	//Fase 2


	jugadorCerca = (dist(playerPos, position) < meleeAttackDistance * 32);

	if (!jugadorCerca && !realizandoCombo) {
		Bossfinding(dt, playerPos);
		currentAnimation = &runAnim;
		cargaAtaqueTimer.Start();
	}
	else {
		realizandoCombo = true;
		pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);

		if (cantidadCombosRealizados >= 3) {
			//ATAQUE CARGADO EXPLOSIVO
			if (cargaAtaqueTimer.ReadMSec() <= cargaAtaqueMS) {
				currentAnimation = &ataqueCargadoAnim;
				explosionEspadaTimer.Start();
			}
			else {
				currentAnimation = &ataqueCargadoEjecutarAnim;
				if (attackBigSensor == nullptr && ataqueCargadoEjecutarAnim.GetCurretFrameNumber() >= 2 && !ataqueCargadoEjecutarAnim.HasFinished()) {
					attackBigSensor = app->physics->CreateRectangleSensor(position.x + 16, position.y + 50, 180, 100, bodyType::KINEMATIC);
					attackBigSensor->listener = this;
				}
				if (explosionEspadaTimer.ReadMSec() >= 2000) {
					if (attackBigSensor != nullptr) {
						app->physics->DestroyBody(attackBigSensor);
						attackBigSensor = nullptr;
					}
					//EXPLOSION
					if (explosionActual == nullptr && !explosionRealizada) {
						tamañoExplosionActual = 1;
						explosionActual = new ExplosionAtaque{ app->physics->CreateCircle(position.x, position.y, tamañoExplosionActual, bodyType::DYNAMIC) };
						explosionActual->pbody->body->GetFixtureList()->SetSensor(true);
						explosionActual->lifeTime.Start();
						explosionActual->pbody->ctype = ColliderType::BOSS_SURMA_EXPLOSION;
						explosionActual->pbody->listener = this;
						explosionRealizada = true;
						explosionAlcanzada = false;
						explosionAnim.Reset();
						explosionActual->currentAnimation = &explosionAnim;
					}

					//Cansado
					currentAnimation = &cansadoAnim;
					if (cansadoTimer.ReadMSec() > cansadoMS) {
						realizandoCombo = false;
						explosionRealizada = false;
						combo1Anim.Reset();
						combo2Anim.Reset();
						combo3Anim.Reset();
						ataqueCargadoEjecutarAnim.Reset();
						ataqueCargadoAnim.Reset();
						cantidadCombosRealizados = 0;
						//Reset todas las anims
					}
					else {
						if (attackBigSensor != nullptr) {
							app->physics->DestroyBody(attackBigSensor);
							attackBigSensor = nullptr;
						}
					}
				}
				else {
					cansadoTimer.Start();
				}
				
			}

		
		}
		else {
			
			if (!combo1Anim.HasFinished()) {
				currentAnimation = &combo1Anim;
				//PARTICULA ATAQUE FLOJO
				if (attackSensor == nullptr && combo1Anim.GetCurretFrameNumber() >= 3) {
					if (isFacingLeft) {
						attackSensor = app->physics->CreateRectangleSensor(position.x - 90, position.y - 25, 60, 150, bodyType::KINEMATIC);
					}
					else {
						attackSensor = app->physics->CreateRectangleSensor(position.x + 120, position.y - 25, 60, 150, bodyType::KINEMATIC);
					}
					attackSensor->listener = this;
				}
			}
			else if (!combo2Anim.HasFinished()) {
				currentAnimation = &combo2Anim;
				//PARTICULA ATAQUE FLOJO
				if (attackSensor != nullptr) {
					app->physics->DestroyBody(attackSensor);
					attackSensor = nullptr;
				}

				if (attackSensor2 == nullptr && combo2Anim.GetCurretFrameNumber() >= 3) {
					if (isFacingLeft) {
						attackSensor2 = app->physics->CreateRectangleSensor(position.x + 120, position.y - 25, 60, 150, bodyType::KINEMATIC);
					}
					else {
						attackSensor2 = app->physics->CreateRectangleSensor(position.x - 90, position.y - 25, 60, 150, bodyType::KINEMATIC);
					}
					attackSensor2->listener = this;
				}
			}
			else if (!combo3Anim.HasFinished()) {
				currentAnimation = &combo3Anim;
				//PARTICULA ATAQUE FLOJO
				if (attackSensor2 != nullptr) {
					app->physics->DestroyBody(attackSensor2);
					attackSensor2 = nullptr;
				}


				if (attackSensor == nullptr && combo3Anim.GetCurretFrameNumber() >= 3) {
					if (isFacingLeft) {
						attackSensor = app->physics->CreateRectangleSensor(position.x - 90, position.y - 25, 60, 150, bodyType::KINEMATIC);
					}
					else {
						attackSensor = app->physics->CreateRectangleSensor(position.x + 120, position.y - 25, 60, 150, bodyType::KINEMATIC);
					}
					attackSensor->listener = this;
				}
			}
			else if (!ataqueRapidoAnim.HasFinished()) {
				currentAnimation = &ataqueRapidoAnim;
				//PARTICULA ATAQUE RAPIDO
				if (attackSensor != nullptr) {
					app->physics->DestroyBody(attackSensor);
					attackSensor = nullptr;
				}


				if (attackSensor2 == nullptr && combo2Anim.GetCurretFrameNumber() >= 3) {
					if (isFacingLeft) {
						attackSensor2 = app->physics->CreateRectangleSensor(position.x - 90, position.y - 25, 160, 50, bodyType::KINEMATIC);
					}
					else {
						attackSensor2 = app->physics->CreateRectangleSensor(position.x +120, position.y - 25, 160, 50, bodyType::KINEMATIC);
					}
					attackSensor2->listener = this;
				}
			}
			else {
				if (attackSensor2 != nullptr) {
					app->physics->DestroyBody(attackSensor2);
					attackSensor2 = nullptr;
				}
				cargaAtaqueTimer.Start();
				cantidadCombosRealizados++;
				realizandoCombo = false;
				combo1Anim.Reset();
				combo2Anim.Reset();
				combo3Anim.Reset();
				ataqueRapidoAnim.Reset();
			}

		}

	}


	if (health <= 0) {

		fase = FASE_Surma::FASE_DYNIG;
		
		Die();
		CleanUp();
	}

}

void Boss_Surma::FaseDying(float dt, iPoint playerPos)
{
	currentAnimation = &muerteAnim;

}

MapObject* Boss_Surma::GetCurrentRoom()
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