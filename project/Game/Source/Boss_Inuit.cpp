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
#include "Item_Zafiro.h"
#include "BestiarioManager.h"
#include "Item_Mascara_1.h"
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

	atk2_boss_inuit.LoadAnim("boss_inuit", "atk2_boss_inuit", spritePositions);
	atk1_boss_inuit.LoadAnim("boss_inuit", "atk1_boss_inuit", spritePositions);
	wave_boss_inuit.LoadAnim("boss_inuit", "wave_boss_inuit", spritePositions);
	idleAnim_boss_inuit.LoadAnim("boss_inuit", "idleAnim_boss_inuit", spritePositions);
	boomerang_boss_inuit.LoadAnim("boss_inuit", "boomerang_boss_inuit", spritePositions);
	changeFase_boss_inuit.LoadAnim("boss_inuit", "changeFase_boss_inuit", spritePositions);
	move_inuit.LoadAnim("boss_inuit", "move_inuit", spritePositions);
	/*runAnim.LoadAnim("osiris", "runAnim", spritePositions);
	attackAnim.LoadAnim("osiris", "attackAnim", spritePositions);
	dieAnim.LoadAnim("osiris", "dieAnim", spritePositions);*/

	texture = app->tex->Load(config.attribute("texturePath").as_string());

	inuit_get_damage_fx = app->audio->LoadAudioFx("inuit_get_damage_fx");
	inuit_melee_attack_fx = app->audio->LoadAudioFx("inuit_melee_attack_fx");
	inuit_melee_attackAlt_fx = app->audio->LoadAudioFx("inuit_melee_attackAlt_fx");
	inuit_melee_attackAlt2_fx = app->audio->LoadAudioFx("inuit_melee_attackAlt2_fx");
	inuit_ranged_attack_fx = app->audio->LoadAudioFx("inuit_ranged_attack_fx");

	pbodyFoot = app->physics->CreateCircle(position.x, position.y, 60, bodyType::DYNAMIC);
	pbodyFoot->entity = this;
	pbodyFoot->listener = this;
	pbodyFoot->ctype = ColliderType::BOSS_INUIT;

	pbodySensor = app->physics->CreateRectangleSensor(position.x, position.y, 120, 300, bodyType::DYNAMIC);
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
	speed = (config.attribute("speed").as_float() / 10) * 0.4;
	attackDamage = config.attribute("attackDamage").as_float();
	attackDistance = config.attribute("attackDistance").as_float();
	viewDistance = config.attribute("viewDistance").as_float();
	bmrSpeed = 80;
	//fase
	fase = FASE::FASE_ONE;
	lifeLow40 = maxHealth * 0.4;
	lifeLow5 = maxHealth * 0.05;

	desiredState = EntityState_Boss_Inuit::IDLE;

	room = GetCurrentRoom();

	pugi::xml_parse_result parseResult2 = configFile.load_file("config.xml");
	if (parseResult2) {
		configNode = configFile.child("config");
	}

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
	else if (enemyMove && currentState != EntityState_Boss_Inuit::ATTACKING_BASIC)
	{
		desiredState = EntityState_Boss_Inuit::RUNNING;
	}
	else
	{
		desiredState = EntityState_Boss_Inuit::IDLE;
	}
	stateMachine(dt, playerPos);

	//Si jugador fuera de area, eliminar cubo de ataque
	if (atackCube != nullptr && playerInBossArea == false) {
		//app->physics->GetWorld()->DestroyBody(atackCube->body);
		app->physics->DestroyBody(atackCube);
		atackCube = nullptr;
	}


	if (health <= lifeLow40 && !faseTwo) {
		fase = FASE::FASE_CHANGE;
	}

	if (health <= lifeLow5 && !useUlt) {
		useUlt = true;
		goUseUlt = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		health -= 1000;
	}

	switch (fase)
	{
	case FASE::FASE_ONE:

		break;
	case FASE::FASE_CHANGE:
		//printf("\n FaseCAHNEG");
		currentAnimation = &changeFase_boss_inuit;
		break;
	case FASE::FASE_TWO:
		//desiredState = EntityState_Boss_Inuit::IDLE;
		atk2_boss_inuit.speed = 0.25;
		atk1_boss_inuit.speed = 0.25;
		BMRatkF = 400;
		BMRatkSpeed = 1;
		if (!goUseWave) {
			waveTime.Start();
		}
		goUseWave = true;
		speed = (120 / 10) * 0.4;
		attackDamage = 240;
		break;
	}

	currentAnimation->Update();
	return true;
}


bool Boss_Inuit::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim_boss_inuit; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();


	if (timerRecibirDanioColor.ReadMSec() <= 100) {
		float alpha = (100 - timerRecibirDanioColor.ReadMSec()) / 100;
		SDL_SetTextureAlphaMod(texture, static_cast<Uint8>(255 * alpha)); // Ajusta la opacidad

	}
	else {
		SDL_SetTextureAlphaMod(texture, 255);
	}


	/*if (isInCenter) {
		isFacingLeft = true;
	}*/
	////printf("\ninCenter: %d", isInCenter);
	if (!Dead) {
		if (isFacingLeft) {
			app->render->DrawTexture(texture, position.x - 410, position.y - 300, SDL_FLIP_HORIZONTAL, &rect);
		}
		else {
			app->render->DrawTexture(texture, position.x - 410, position.y - 300, SDL_FLIP_NONE, &rect);
		}
	}


	//BMR ATAQUE
	/*if (startBMR) {

	}*/
	if (TimerColdDown(BMRatkSpeed) && inbmrAtack) {
		////printf("\n bmrBack");
		bmrBack = true;
		startBMR = false;
	}
	if (checkAtackBMR) {
		if (bmrBack) {
			//BMRfinding(BMRposition);
			/*fPoint returnBMR = getDirectionVector(BMRposition,position);
			atackBMR->body->ApplyForceToCenter(b2Vec2(returnBMR.x * 80, returnBMR.y * 80), true);
			//printf("\n %d", bmrBack);*/
			bmrSpeed = -BMRatkF;
		}
		else
		{
			bmrSpeed = BMRatkF;

		}
		atackBoomerang(playerDireccion);
	}
	else
	{
		if (atackBMR != nullptr) {
			atackBMR->body->SetLinearVelocity(b2Vec2(0, 0));
			atackBMR->body->GetWorld()->DestroyBody(atackBMR->body);
			atackBMR = nullptr;
			//printf("delete");
			inuit_ranged_attack = false;
		}
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

	if (saveOriginPos) {
		originalWavePosition.x = METERS_TO_PIXELS(pbodyPos.p.x) - 16;
		originalWavePosition.y = METERS_TO_PIXELS(pbodyPos.p.y) - 16;
		saveOriginPos = false;

	}

	////printf("\n Heal: %f", health);

	if (atkAnimation && fase != FASE::FASE_CHANGE) {
		if (attackTime == 1) {
			//atk1_boss_inuit.speed = 1;
			currentAnimation = &atk1_boss_inuit;
		}
		if (attackTime == 2) {
			currentAnimation = &atk2_boss_inuit;
		}
		if (attackTime == 0) {

			currentAnimation = &idleAnim_boss_inuit;
		}
	}
	resetAnimation();

	/*if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
		health -= 1000;
		//printf("\nHealth: %f", health);
	}*/

	if (goUseUlt) {
		ultDef = true;
		if (!inWave) {
			waveTime.Start();
			waveTimeStart = true;
			goUseUlt = false;
		}
	}
	if (ultDef && !inWave && !waveTimeStart) {
		//printf("2");
		waveTime.Start();
		waveTimeStart = true;
	}
	//Ulti
	if (ultDef && waveTimeStart) {
		ulti_Atack();
	}
	//Wave
	if (waveTimerColdDown(10) && !waveTimeStart && goUseWave && !Dead) {
		////printf("\ndelete-3");
		shock_wave(originalWavePosition.x, originalWavePosition.y, 5, 520, 0);
	}
	return true;
}


bool Boss_Inuit::CleanUp()
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
	if (atackBMR != nullptr) {
		app->physics->GetWorld()->DestroyBody(atackBMR->body);
	}

	if (!shockWaves.empty()) {
		for (auto& pair : shockWaves) {
			deleteCollision(pair.second);
		}
		shockWaves.clear();
	}
	app->tex->UnLoad(texture);
	lastPath.Clear();

	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}



void Boss_Inuit::stateMachine(float dt, iPoint playerPos)
{
	////printf("\ncurrentState: %d, desiredState: %d", static_cast<int>(currentState), static_cast<int>(desiredState));
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

		/*if (TimerColdDown(1)) {
			if (atackCube != nullptr) {
				app->physics->GetWorld()->DestroyBody(atackCube->body);
				atackCube = nullptr;
			}
		}


		if (TimerColdDown(2) && inbmrAtack == false) {
			inAtack = false;
		}*/


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



void Boss_Inuit::shock_wave(int posX, int posY, float shockSpeed, float maxSize, int tag)
{

	static std::unordered_map<int, float> shockSizes;

	auto it = shockWaves.find(tag);
	if (it != shockWaves.end()) {
		deleteCollision(it->second);
		shockWaves.erase(it);
	}

	float& shockSize = shockSizes[tag];

	if (shockSize < maxSize) {
		shockSize += shockSpeed;
	}
	else {
		waveIsMax = true;
	}

	if (!waveIsMax) {
		PhysBody* newShockWave = app->physics->CreateCircle(posX, posY, shockSize, DYNAMIC, true);
		newShockWave->entity = this;
		newShockWave->listener = this;
		newShockWave->ctype = ColliderType::WAVE;
		shockWaves[tag] = newShockWave;
		inWave = true;
	}
	else {
		//waveFinishi = true;
		inWave = false;
		if (tag == 0) {
			if (!ultDef) {
				//printf("3");
				waveTime.Start();
			}
		}

		if (tag == 1 || tag == 2 || tag == 3 || tag == 4) {
			wave0Finishing = true;
		}
		if (tag == 5 || tag == 6 || tag == 7 || tag == 9) {
			wave1Finishing = true;
		}
		if (tag == 9 || tag == 10) {
			wave2Finishing = true;
		}
		shockSize = 0;
		waveIsMax = false;
	}
}


void Boss_Inuit::ulti_Atack()
{
	//TimerColdDown(10);
	//TimerColdDown(5);

	if (!wave0Finishing) {
		shock_wave(originalWavePosition.x - 1000, originalWavePosition.y - 250, 3, 400, 1);
		shock_wave(originalWavePosition.x - 1000, originalWavePosition.y + 250, 3, 400, 2);
		shock_wave(originalWavePosition.x + 1000, originalWavePosition.y - 250, 3, 400, 3);
		shock_wave(originalWavePosition.x + 1000, originalWavePosition.y + 250, 3, 400, 4);
	}

	if (waveTimerColdDown(1) && !wave1Finishing) {
		shock_wave(originalWavePosition.x - 500, originalWavePosition.y - 250, 3, 400, 5);
		shock_wave(originalWavePosition.x - 500, originalWavePosition.y + 250, 3, 400, 6);
		shock_wave(originalWavePosition.x + 500, originalWavePosition.y - 250, 3, 400, 7);
		shock_wave(originalWavePosition.x + 500, originalWavePosition.y + 250, 3, 400, 8);
	}

	if (waveTimerColdDown(2) && !wave2Finishing) {
		shock_wave(originalWavePosition.x, originalWavePosition.y - 250, 3, 400, 9);
		shock_wave(originalWavePosition.x, originalWavePosition.y + 250, 3, 400, 10);
	}

	if (wave0Finishing && wave1Finishing && wave2Finishing) {
		//printf("\nUltiFishing");
		wave0Finishing = false;
		wave1Finishing = false;
		wave2Finishing = false;
		waveTimeStart = false;
		waveTime.Start();
		ultDef = false;

	}

}

void Boss_Inuit::resetAnimation()
{
	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "atk1_boss_inuit") {
		if (atackCube != nullptr) {
			app->physics->GetWorld()->DestroyBody(atackCube->body);
			atackCube = nullptr;
		}
		changeNextAtk = true;
		inAtack = false;
	}
	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "atk2_boss_inuit") {
		if (atackCube != nullptr) {
			app->physics->GetWorld()->DestroyBody(atackCube->body);
			atackCube = nullptr;
		}
		changeNextAtk = true;
		inAtack = false;
	}
	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "changeFase_boss_inuit") {
		faseCount++;
		if (faseCount == 5) {
			faseTwo = true;
			fase = FASE::FASE_TWO;
		}
		changeFase_boss_inuit.Reset();
	}
}




void Boss_Inuit::DoNothing(float dt)
{
	currentAnimation = &idleAnim_boss_inuit;
	//pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);

}

void Boss_Inuit::Chase(float dt, iPoint playerPos)
{
	//////printf("Osiris chasing");
	currentAnimation = &move_inuit;
	Bossfinding(dt, playerPos);

}

void Boss_Inuit::Attack(float dt)
{
	//////printf("Osiris attacking");
	//currentAnimation = &attackAnim;

	if (changeNextAtk) {
		attackTime++;
		changeNextAtk = false;
	}

	atkAnimation = true;
	switch (attackTime)
	{
	case 1:
		app->audio->PlayFx(inuit_melee_attack_fx);

		inAtack = true;
		//printf("\nataque1");
		bmrBack = false;
		if (isFacingLeft) {
			atkDistancia = -150;
		}
		else
		{
			atkDistancia = 150;
		}
		atackCube = app->physics->CreateRectangleSensor(position.x - atkDistancia, position.y, 200, 300, STATIC);
		atackCube->entity = this;
		atackCube->listener = this;
		atackCube->ctype = ColliderType::ATACK_INUIT;
		atkTimeReset = false;
		break;
	case 2:
		app->audio->PlayFx(inuit_melee_attack_fx); // No funciona (por algun motivo extraño)
		atk1_boss_inuit.Reset();
		inAtack = true;
		//printf("\nataque2");
		if (isFacingLeft) {
			atkDistancia = -150;
		}
		else
		{
			atkDistancia = 150;
		}
		atackCube = app->physics->CreateRectangleSensor(position.x - atkDistancia, position.y, 200, 300, STATIC);

		atackCube->entity = this;
		atackCube->listener = this;
		atackCube->ctype = ColliderType::ATACK_INUIT;
		atkTimeReset = false;
		
		break;
	case 3:
		atk2_boss_inuit.Reset();
		inAtack = true;
		//printf("\nataque4");
		playerDireccion = calculate_direction();
		printplayerDireccion = directionToString(playerDireccion);
		//printf("\n PlayerDireccion %s", printplayerDireccion.c_str());
		atackBMR = app->physics->CreateCircle(position.x, position.y, 160, DYNAMIC, true);
		atackBMR->entity = this;
		atackBMR->listener = this;
		atackBMR->ctype = ColliderType::ATACKBMR;
		checkAtackBMR = true;
		inbmrAtack = true;
		attackTime = 0;
		atkTimeReset = false;
		atackTimeColdDown.Start();
		break;
		/*case 4:
			inAtack = true;
			//printf("\nataque3");
			atackCube = app->physics->CreateRectangleSensor(position.x, position.y, 60, 120, STATIC);
			break;

			break;*/

	default:
		break;
	}

	atackTimeColdDown.Start();

}

void Boss_Inuit::atackBoomerang(BTPDirection direccion)
{
	b2Vec2 force(0.0f, 0.0f);

	switch (direccion) {
	case BTPDirection::LEFT:
		force.x = -bmrSpeed;
		break;
	case BTPDirection::RIGHT:
		force.x = bmrSpeed;
		break;
	case BTPDirection::UP:
		force.y = -bmrSpeed;
		break;
	case BTPDirection::DOWN:
		force.y = bmrSpeed;
		break;
	case BTPDirection::UPLEFT:
		force.y = -bmrSpeed;
		force.x = -bmrSpeed;
		break;
	case BTPDirection::UPRIGHT:
		force.y = -bmrSpeed;
		force.x = bmrSpeed;
		break;
	case BTPDirection::DOWNLEFT:
		force.y = bmrSpeed;
		force.x = -bmrSpeed;
		break;
	case BTPDirection::DOWNRIGHT:
		force.y = bmrSpeed;
		force.x = bmrSpeed;
		break;
	default:
		force.y = bmrSpeed;
	}
	if (atackBMR != nullptr) {
		atackBMR->body->ApplyForceToCenter(force, true);
	}

	if (inuit_ranged_attack == false) 
	{
		app->audio->PlayFx(inuit_ranged_attack_fx);
		inuit_ranged_attack = true;
	}

	b2Transform pbodyPosBMR = atackBMR->body->GetTransform();

	BMRposition.x = METERS_TO_PIXELS(pbodyPosBMR.p.x) - 16;
	BMRposition.y = METERS_TO_PIXELS(pbodyPosBMR.p.y) - 16;
	currentAnimation1 = &boomerang_boss_inuit;
	SDL_Rect rect = currentAnimation1->GetCurrentFrame();
	app->render->DrawTexture(texture, BMRposition.x - 410, BMRposition.y - 300, SDL_FLIP_HORIZONTAL, &rect);
	currentAnimation1->Update();


}

void Boss_Inuit::Die() {

	//Mask XP
	Dead = true;
	if (!saveXml) {
		app->map->boss1_defeated = true;
		app->map->SaveBoss();
		saveXml = true;
	}

	if (count == 0)
	{
		Item_Zafiro* zafiro = (Item_Zafiro*)app->entityManager->CreateEntity(EntityType::ITEM_ZAFIRO);
		zafiro->config = configNode.child("entities_data").child("item_zafiro");
		zafiro->position = iPoint(position.x, position.y);
		zafiro->Start();

		app->bestiarioManager->CreateItem("inuit");


		Item_mascara_1* mascara1 = (Item_mascara_1*)app->entityManager->CreateEntity(EntityType::ITEM_MASCARA1);
		mascara1->config = configNode.child("entities_data").child("item_mascara_1");
		mascara1->position = iPoint(position.x + 50, position.y);
		mascara1->Start();

		count++;
	}
	
	/*if (pbodyFoot != nullptr) {
		pbodyFoot->body->GetWorld()->DestroyBody(pbodyFoot->body);
	}
	if (pbodySensor != nullptr) {
		pbodySensor->body->GetWorld()->DestroyBody(pbodySensor->body);
	}*/
	//phy->body->GetWorld()->DestroyBody(phy->body);


	/*if (atackBMR != nullptr) {
		app->physics->GetWorld()->DestroyBody(atackBMR->body);
	}*/
	/*
	if (areaSensor != nullptr) {
		app->physics->GetWorld()->DestroyBody(areaSensor->body);
	}
	*/

	//Mask 0
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK0)
	{
		app->entityManager->GetPlayer()->maskZeroXP += 1000;
		////printf("Current Mask 0 XP %i \n", app->entityManager->GetPlayer()->maskZeroXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK0)
	{
		app->entityManager->GetPlayer()->maskZeroXP += 1000;
		////printf("Current Mask 0 XP %i \n", app->entityManager->GetPlayer()->maskZeroXP);
	}

	//Mask 1
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK1)
	{
		app->entityManager->GetPlayer()->maskOneXP += 1000;
		////printf("Current Mask 1 XP %i \n", app->entityManager->GetPlayer()->maskOneXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK1)
	{
		app->entityManager->GetPlayer()->maskOneXP += 1000;
		////printf("Current Mask 1 XP %i \n", app->entityManager->GetPlayer()->maskOneXP);
	}

	//Mask 2
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK2)
	{
		app->entityManager->GetPlayer()->maskTwoXP += 1000;
		////printf("Current Mask 2 XP %i \n", app->entityManager->GetPlayer()->maskTwoXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK2)
	{
		app->entityManager->GetPlayer()->maskTwoXP += 1000;
		////printf("Current Mask 2 XP %i \n", app->entityManager->GetPlayer()->maskTwoXP);
	}

	//Mask 3
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK3)
	{
		app->entityManager->GetPlayer()->maskThreeXP += 1000;
		////printf("Current Mask 3 XP %i \n", app->entityManager->GetPlayer()->maskThreeXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK3)
	{
		app->entityManager->GetPlayer()->maskThreeXP += 1000;
		////printf("Current Mask 3 XP %i \n", app->entityManager->GetPlayer()->maskThreeXP);
	}

}

void Boss_Inuit::Revive()
{

}


bool Boss_Inuit::BMRfinding(iPoint BMR)
{

	iPoint BMRPos = app->map->WorldToMap(BMR.x, BMR.y);
	iPoint enemyPos = app->map->WorldToMap(position.x, position.y);
	app->map->pathfinding->CreatePath(BMRPos, enemyPos);
	lastPath = *app->map->pathfinding->GetLastPath();

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


bool Boss_Inuit::Bossfinding(float dt, iPoint playerPosP)
{

	iPoint playerPos = app->map->WorldToMap(playerPosP.x, playerPosP.y);
	iPoint enemyPos = app->map->WorldToMap(position.x, position.y);

	////printf("\nBossArea:%f", dist(bossArea, enemyPos));
	////printf("\nEmemyPosicion:%d", enemyPos);


	if (!inbmrAtack) {
		dontMove = false;
	}
	else
	{
		dontMove = true;
	}

	if (fase == FASE::FASE_CHANGE) {
		dontMove = true;
	}

	if (!dontMove) {
		if (playerInBossArea) {
			app->map->pathfinding->CreatePath(enemyPos, playerPos); // Calcula el camino desde la posicion del enemigo hacia la posicion del jugador
			lastPath = *app->map->pathfinding->GetLastPath();
			if (atackCube != nullptr) {
				app->physics->GetWorld()->DestroyBody(atackCube->body);
				atackCube = nullptr;
			}
			if (inbmrAtack == false) {
				inAtack = false;
			}
		}
		else {
			app->map->pathfinding->CreatePath(enemyPos, originalPosition); // Calcula el camino desde la posicion del enemigo hacia la posicion del jugador
			lastPath = *app->map->pathfinding->GetLastPath();
			iPoint inCenter = enemyPos - originalPosition;
			if (inCenter.IsZero() == true) {
				enemyMove = false;
			}
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

		}
		pbodyFoot->body->SetLinearVelocity(velocity);
	}

	return true;
}

void Boss_Inuit::deleteCollision(PhysBody* phy)
{

	if (phy != nullptr) {
		phy->body->SetLinearVelocity(b2Vec2(0, 0));
		//phy->body->GetWorld()->DestroyBody(phy->body);
		app->physics->GetWorld()->DestroyBody(phy->body);
		phy = nullptr;
		////printf("delete");
	}
	////printf("\ndelete0");
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

	double angle_deg = angle_rad * 180 / M_PI;
	if (angle_deg < 0) {
		angle_deg += 360;
	}
	BTPDirection directions[] = { BTPDirection::RIGHT, BTPDirection::DOWNRIGHT, BTPDirection::DOWN, BTPDirection::DOWNLEFT, BTPDirection::LEFT, BTPDirection::UPLEFT, BTPDirection::UP, BTPDirection::UPRIGHT };
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
	}
}


bool Boss_Inuit::TimerColdDown(float time)
{
	////printf("\nataqueTimeClodDown%: %f", ataqueTimeClodDown);
	ataqueTimeClodDown = atackTimeColdDown.CountDown(time);
	if ((float)ataqueTimeClodDown == 0) {
		return true;
	}
	else
	{
		return false;
	}
}

bool Boss_Inuit::waveTimerColdDown(float time)
{
	waveTimeClodDown = waveTime.CountDown(time);
	////printf("\n waveTimeClodDown%: %f", waveTimeClodDown);
	if ((float)waveTimeClodDown == 0) {

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

		//if (physA->ctype == ColliderType::ATACKBMR) {
		//	//checkAtackBMR = false;
		//	//atackBMR->body->SetLinearVelocity(b2Vec2(0, 0));
		//	//printf("\nONcolibmrBack");
		//	startBMR = true;
		//	if (!tocaPared) {
		//		tocaPared = true;
		//		atackTimeColdDown.Start();
		//	}
		//	
		//}

		break;
	case ColliderType::BOSS_INUIT:
		if (physA->ctype == ColliderType::ATACKBMR && bmrBack == true) {
			//printf("\n Enemyy");
			atackTimeColdDown.Start();
			checkAtackBMR = false;
			inbmrAtack = false;
			inAtack = false;
			tocaPared = false;
			changeNextAtk = true;
		}
		break;
	case ColliderType::PLAYER:
		if (physA->ctype == ColliderType::ATACK_INUIT) {
			app->entityManager->GetPlayer()->TakeDamage(attackDamage);
		}
		if (physA->ctype == ColliderType::WAVE) {
			app->entityManager->GetPlayer()->TakeDamage(200);
		}

		if (physA->ctype == ColliderType::ATACKBMR) {

			app->entityManager->GetPlayer()->TakeDamage(50);
		}

		if (physA->ctype == ColliderType::BOSSAREA) {
			////printf("\n entraBossArea");
			playerInBossArea = true;
			enemyMove = true;
		}
		LOG("Collision PLAYER");
		//restar vida al player
		break;
	case ColliderType::PLAYER_ATTACK:
		LOG("Collision Player_Attack");
		if (fase != FASE::FASE_CHANGE && app->entityManager->GetPlayer()->checkAtk == true) {
			health -= app->entityManager->GetPlayer()->currentStats.attackDamage;
			timerRecibirDanioColor.Start();
			//printf("\n BossHeal %f", health);
			app->entityManager->GetPlayer()->checkAtk = false;
			app->audio->PlayFx(inuit_get_damage_fx);
		}
		//app->entityManager->GetPlayer()->TakeDamage(flechaCargadaRastroDamage);
		break;

	case ColliderType::ATACK_INUIT:
		LOG("Collision Player_Attack");
		
		
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
		//if (physA->ctype == ColliderType::ATACKBMR) {
		//	//checkAtackBMR = false;
		//	//atackBMR->body->SetLinearVelocity(b2Vec2(0, 0));
		//	////printf("\nENDcolibmrBack");
		//	startBMR = true;
		//	if (!tocaPared) {
		//		tocaPared = true;
		//		atackTimeColdDown.Start();
		//	}
		//}
		break;
	case ColliderType::PLAYER:
		if (physA->ctype == ColliderType::BOSSAREA) {
			////printf("Out");
			playerInBossArea = false;
			if (inbmrAtack == false) {
				inAtack = false;
			}
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

MapObject* Boss_Inuit::GetCurrentRoom()
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