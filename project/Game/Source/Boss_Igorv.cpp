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
#include "DialogManager.h"
#include "ModuleFadeToBlack.h"
#include "Scene_menu.h"


#include "Item_Mascara_3.h"

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
	dash_idle_boss_Igory.LoadAnim("boss_igory", "dash_idle_boss_Igory", spritePositions);;
	dash_inDashi_boss_Igory.LoadAnim("boss_igory", "dash_inDashi_boss_Igory", spritePositions);;
	dash_DashiAtq_boss_Igory.LoadAnim("boss_igory", "dash_DashiAtq_boss_Igory", spritePositions);;
	atq1_boss_Igory.LoadAnim("boss_igory", "atq1_boss_Igory", spritePositions);
	geneSure_boss_Igory.LoadAnim("boss_igory", "geneSure_boss_Igory", spritePositions);
	idle_boss_Igory.LoadAnim("boss_igory", "idle_boss_Igory", spritePositions);
	atq2_boss_Igory.LoadAnim("boss_igory", "atq2_boss_Igory", spritePositions);
	atq3_boss_Igory.LoadAnim("boss_igory", "atq3_boss_Igory", spritePositions);
	dead_boss_Igory.LoadAnim("boss_igory", "dead_boss_Igory", spritePositions);
	reviver_boss_Igory.LoadAnim("boss_igory", "reviver_boss_Igory", spritePositions);
	hit_boss_Igory.LoadAnim("boss_igory", "hit_boss_Igory", spritePositions);


	texture = app->tex->Load(config.attribute("texturePath").as_string());

	father_get_damage_fx = app->audio->LoadAudioFx("father_get_damage_fx");
	father_get_damageAlt_fx = app->audio->LoadAudioFx("father_get_damageAlt_fx");
	father_get_damageAlt2_fx = app->audio->LoadAudioFx("father_get_damageAlt2_fx");
	father_melee_attack_fx = app->audio->LoadAudioFx("father_melee_attack_fx");
	father_melee_attackAlt_fx = app->audio->LoadAudioFx("father_melee_attackAlt_fx");
	father_generate_sureg_fx = app->audio->LoadAudioFx("father_generate_sureg_fx");


	pbodyFoot = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
	pbodyFoot->entity = this;
	pbodyFoot->listener = this;
	pbodyFoot->ctype = ColliderType::BOSS_INUIT;

	pbodySensor = app->physics->CreateRectangleSensor(position.x, position.y, 40, 60, bodyType::DYNAMIC);
	pbodySensor->entity = this;
	pbodySensor->listener = this;
	pbodySensor->ctype = ColliderType::UNKNOWN;

	app->entityManager->SetIgory(this);

	originalPosition = app->map->WorldToMap(position.x, position.y);

	maxHealth = config.attribute("maxHealth").as_float();
	health = maxHealth;
	speed = (config.attribute("speed").as_float() / 10) * 0.4;
	attackDamage = config.attribute("attackDamage").as_float();
	attackDistance = config.attribute("attackDistance").as_float();
	viewDistance = config.attribute("viewDistance").as_float();
	//FASE_Igory
	fase = FASE_Igory::FASE_ONE;
	lifeLow80 = maxHealth * 0.6;
	lifeLow40 = maxHealth * 0.3;

	room = GetCurrentRoom();

	generaTimeColdDown.Start();
	curaTimer.Start();


	pugi::xml_parse_result  parseResult = dialogoFile.load_file("dialogs.xml");
	pugi::xml_node dialogoPadre = dialogoFile.child("dialogues");
	dialogNode = dialogoPadre.find_child_by_attribute("dialog", "id", "2002").child("sentences").child("sentence");
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
	else if (playerInFight && health <= lifeLow80 && !faseTwo && !isDead && !stun) {

		desiredState = EntityState_Boss_Igory::FASE_CHANGE;
	}
	else if (playerInFight && health <= lifeLow40 && !faseThree && !isDead && !stun) {
		desiredState = EntityState_Boss_Igory::FASE_CHANGE;
	}
	else if (playerInFight && stun && !isDead && !inCurar && !inAtack && !inAtqDashi)
	{
		if (stunTimer.ReadMSec() >= 2000) {
			stun = false;
		}
		desiredState = EntityState_Boss_Igory::IDLE;
	}
	else if (playerInFight && goCurar && !inCurar && !inAtack && nextState != EntityState_Boss_Igory::FASE_CHANGE && fase == FASE_Igory::FASE_THREE)
	{
		desiredState = EntityState_Boss_Igory::HEAL;
	}
	else if (playerInFight && inTakeHit && !isDead && !inCurar)
	{
		desiredState = EntityState_Boss_Igory::TAKEHIT;
	}
	else if (playerInFight && goColdDown && !inAtqDashi && !isDead && !inCurar && !inAtack) {
		if (GeneraColdDown(5)) {
			generaTimeColdDown.Start();
			goColdDown = false;
			inSuregAni = false;
		}
		else {
			desiredState = EntityState_Boss_Igory::IDLE;
		}
	}
	else if (playerInFight && inSuregAni && !inAtqDashi && !isDead && !inCurar && !inAtack && !stun)
	{
		desiredState = EntityState_Boss_Igory::GENERATESUREG;
	}
	else if (playerInFight && atqDashQuali >= 3 && app->map->pathfinding->GetDistance(playerPos, position) >= attackDistance * 64 && fase != FASE_Igory::FASE_ONE && !isDead && !inCurar && !inAtack && !stun) {

		desiredState = EntityState_Boss_Igory::ATTACKING_DASHI;
	}
	else if (playerInFight && app->map->pathfinding->GetDistance(playerPos, position) <= attackDistance * 32 && ataqColdDown && !inTakeHit && !isDead && !inCurar && !stun)
	{
		desiredState = EntityState_Boss_Igory::IDLE;
	}
	else if (playerInFight && app->map->pathfinding->GetDistance(playerPos, position) <= attackDistance * 32 && ataqColdDown == false && !inTakeHit && !inSuregAni && !inAtqDashi && !goColdDown && !isDead && !inCurar && !stun)
	{
		desiredState = EntityState_Boss_Igory::ATTACKING_BASIC;
	}
	else if (playerInFight && currentState != EntityState_Boss_Igory::ATTACKING_BASIC && !isDead && !inCurar && !stun)
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
		generaTimeColdDown.Start();
		curaTimer.Start();
		desiredState = EntityState_Boss_Igory::IDLE;
		pbodyFoot->body->SetType(b2_staticBody);
	}
	else
	{
		desiredState = EntityState_Boss_Igory::IDLE;
	}

	if (playerInFight && !isDead) {
		pbodyFoot->body->SetType(b2_dynamicBody);
	}


	stateMachine(dt, playerPos);

	if (checkColdDown) {
		AtqColdDown();
	}


	//Cura
	if (shieldBroken) {
		inCurar = false;
		goCurar = false;
		shieldBroken = false;
		shield = 1500;
		stun = true;
		curaTimer.Start();
		stunTimer.Start();
		cura_boss_Igory.Reset();
	}

	if (shieldTimer.ReadMSec() >= 10000 && inCurar) {
		inCurar = false;
		goCurar = false;
		shieldBroken = false;
		shield = 1500;
		health += 1000;
		if (health >= maxHealth) {
			health = maxHealth;
		}
		//printf("\nhealth: %f", health);
		curaTimer.Start();
		cura_boss_Igory.Reset();
	}

	if (curaTimer.ReadMSec() >= 25000) {
		goCurar = true;
	}
	if (inCurar) {
		if (healTimer.ReadMSec() >= 2000) {
			health += 200;
			if (health >= maxHealth) {
				health = maxHealth;
			}
			//printf("\nhealth: %f", health);
			healTimer.Start();
		}

	}
	//EndCura

	if (GeneraColdDown(10)) {
		inSuregAni = true;
	}


	if (empujaPlayer) {
		if (habilidadEmpujeTimer.ReadMSec() < 400) {
			fPoint dirToPlayer = getDirectionVector(position, playerPos);
			app->entityManager->GetPlayer()->pbodyFoot->body->ApplyForceToCenter(b2Vec2(dirToPlayer.x * fuerzaHabilidadEmpuje, dirToPlayer.y * fuerzaHabilidadEmpuje), true);
		}
		else
		{
			empujaPlayer = false;
		}
	}
	switch (fase)
	{
	case FASE_Igory::FASE_ONE:
		app->map->maxEnemies = 6;
		break;
	case FASE_Igory::FASE_TWO:
		atq1_boss_Igory.speed = 0.15;
		atq2_boss_Igory.speed = 0.15;
		atq3_boss_Igory.speed = 0.15;
		app->map->maxEnemies = 8;
		speed = (120 / 10) * 0.4;
		attackDamage = 280;
		break;
	case FASE_Igory::FASE_THREE:
		atq1_boss_Igory.speed = 0.25;
		atq2_boss_Igory.speed = 0.25;
		atq3_boss_Igory.speed = 0.25;
		app->map->maxEnemies = 10;
		speed = (150 / 10) * 0.4;
		attackDamage = 300;
		break;
	}

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		health -= 1000;
	}


	showAnimation();
	resetAnimation();
	currentAnimation->Update();
	return true;
}


bool Boss_Igory::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idle_boss_Igory; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	if (!killPadre) {
		if (timerRecibirDanioColor.ReadMSec() <= 100 && !isDead) {
			float alpha = (100 - timerRecibirDanioColor.ReadMSec()) / 100;
			SDL_SetTextureAlphaMod(texture, static_cast<Uint8>(255 * alpha)); // Ajusta la opacidad

		}
		else {
			SDL_SetTextureAlphaMod(texture, 255);
		}
	}
	else
	{
		SDL_SetTextureAlphaMod(texture, padreTranparente);
		padreTranparente -= 2;
		if (padreTranparente <= 0 && !deletePadre) {
			CleanUp();
			deletePadre = true;
			closeFinalSelecion = false;
		}
	}

	if (isFacingLeft) {
		if (!inCurar) {
			app->render->DrawTexture(texture, position.x - 110, position.y - 160, 0.5, SDL_FLIP_HORIZONTAL, &rect);
		}
		else
		{
			app->render->DrawTexture(texture, position.x - 130, position.y - 160, 0.5, SDL_FLIP_HORIZONTAL, &rect);
		}

	}
	else {
		if (!inCurar) {
			app->render->DrawTexture(texture, position.x - 160, position.y - 160, 0.5, SDL_FLIP_NONE, &rect);
		}
		else
		{
			app->render->DrawTexture(texture, position.x - 140, position.y - 160, 0.5, SDL_FLIP_NONE, &rect);
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

	/*if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
		app->map->generaSureg(fase, position);
	}*/

	if (father_get_hit == true && getHitTimer.ReadMSec() >= 900) father_get_hit = false;

	return true;
}


bool Boss_Igory::CleanUp()
{
	//printf("\n CleanUp");
	if (pbodyFoot != nullptr) {
		app->physics->GetWorld()->DestroyBody(pbodyFoot->body);
		pbodyFoot = nullptr;
	}
	if (pbodySensor != nullptr) {
		app->physics->GetWorld()->DestroyBody(pbodySensor->body);
		pbodySensor = nullptr;
	}

	if (texture != nullptr) {
		app->tex->UnLoad(texture);
		texture = nullptr;
	}
	lastPath.Clear();

	app->entityManager->canShowFinal = false;
	killPadre = false;
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
		hit_boss_Igory.Reset();
		if (atackCube != nullptr) {
			app->physics->GetWorld()->DestroyBody(atackCube->body);
			atackCube = nullptr;
		}
		inAtack = false;
		/*startColdDown = true;
		checkColdDown = true;*/
	}

	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "geneSure_boss_Igory") {
		if (atackCube != nullptr) {
			app->physics->GetWorld()->DestroyBody(atackCube->body);
			atackCube = nullptr;
		}
		geneSure_boss_Igory.Reset();
		generaTimeColdDown.Start();
		showSuregAni = false;
		goColdDown = true;
		atqDashQuali = 0;
		inSuregAni = false;
		app->map->generaSureg(fase, position);
	}

	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "atqCargado_fase2_boss_Igory") {
		atqCargado_fase1_boss_Igory.Reset();
		atqCargado_fase2_boss_Igory.Reset();
		generaTimeColdDown.Start();
		faseAni = false;
		if (fase == FASE_Igory::FASE_TWO) {
			//printf("\nFase3");
			fase = FASE_Igory::FASE_THREE;
			faseThree = true;
			atqDashQuali = 0;
			curaTimer.Start();
		}

		if (fase == FASE_Igory::FASE_ONE) {
			//printf("\nFase2");
			fase = FASE_Igory::FASE_TWO;
			faseTwo = true;
			atqDashQuali = 0;
		}
		goColdDown = false;
		inSuregAni = false;
	}

	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "atqCargado_fase1_boss_Igory") {
		faseAni = true;
	}


	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "dash_idle_boss_Igory") {
		if (atackCube != nullptr) {
			app->physics->GetWorld()->DestroyBody(atackCube->body);
			atackCube = nullptr;
		}
		if (!inDashDashi) {
			inDashiTime.Start();
			getPlayerPosition = app->entityManager->GetPlayer()->position;
		}
		inDashDashi = true;

	}
	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "dash_inDashi_boss_Igory") {
		inDashDashi = false;
		inAtaqueDashi = true;
		if (isFacingLeft) {
			atackCube = app->physics->CreateRectangleSensor(position.x + 100, position.y - 60, 180, 180, STATIC);
		}
		else
		{
			atackCube = app->physics->CreateRectangleSensor(position.x - 80, position.y - 60, 180, 180, STATIC);
		}
	}
	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "dash_DashiAtq_boss_Igory") {
		if (atackCube != nullptr) {
			app->physics->GetWorld()->DestroyBody(atackCube->body);
			atackCube = nullptr;
		}
		dash_idle_boss_Igory.Reset();
		dash_inDashi_boss_Igory.Reset();
		dash_DashiAtq_boss_Igory.Reset();
		inAtqDashi = false;
		inIdleDashi = false;
		inDashDashi = false;
		inAtaqueDashi = false;
		atqDashQuali = 0;
		//desiredState = EntityState_Boss_Igory::RUNNING;
	}
	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "reviver_boss_Igory") {
		if (!deletePadre) {
			CleanUp();
			closeFinalSelecion = false;
			deletePadre = true;

		}
	}


}

void Boss_Igory::showAnimation()
{
	if (inAtack && !inSuregAni) {
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
		////printf("genrasureg");
		currentAnimation = &geneSure_boss_Igory;
		app->audio->PlayFx(father_generate_sureg_fx);
	}

	if (faseAni) {
		currentAnimation = &atqCargado_fase2_boss_Igory;
	}

	if (inAtqDashi && inIdleDashi) {

		currentAnimation = &dash_idle_boss_Igory;

	}

	if (inDashiTime.ReadMSec() >= 200) {

		if (inAtqDashi && inDashDashi) {

			float timeLerp = 0.1f;
			b2Vec2 velocity((getPlayerPosition.x - position.x) * timeLerp, (getPlayerPosition.y - position.y) * timeLerp);
			pbodyFoot->body->SetLinearVelocity(velocity);
			position.x = position.x * (1 - timeLerp) + getPlayerPosition.x * timeLerp;
			position.y = position.y * (1 - timeLerp) + getPlayerPosition.y * timeLerp;
			currentAnimation = &dash_inDashi_boss_Igory;
		}
	}

	if (inAtqDashi && inAtaqueDashi) {
		currentAnimation = &dash_DashiAtq_boss_Igory;
	}

	if (inCurar) {
		currentAnimation = &cura_boss_Igory;
	}

	/*if (killPadre) {

		SDL_SetTextureAlphaMod(texture, padreTranparente);
		padreTranparente--;
		if (padreTranparente <= 0) {
			CleanUp();
		}

	}*/
}



bool Boss_Igory::AtqColdDown()
{
	if (startColdDown) {
		////printf("\nStart");
		atackTimeColdDown.Start();
		startColdDown = false;
	}

	if (atackTimeColdDown.ReadMSec() >= 500) {
		////printf("\nEnd");
		ataqColdDown = false;
		checkColdDown = false;
		startColdDown = true;
		atqGoNext = true;
		return true;
	}
	else
	{
		////printf("\nColdDown");
		ataqColdDown = true;
		return false;
	}

	atackTimeColdDown.Start();

}

void Boss_Igory::takeHit()
{
	currentAnimation = &hit_boss_Igory;

	if (father_get_hit == false)
	{
		app->audio->PlayRandomFx(father_get_damage_fx, father_get_damageAlt_fx, father_get_damageAlt2_fx);
		father_get_hit = true;
		getHitTimer.Start();
	}
}



void Boss_Igory::stateMachine(float dt, iPoint playerPos)
{
	////printf("\ncurrentState: %d, desiredState: %d", static_cast<int>(currentState), static_cast<int>(desiredState));
	nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;
	switch (nextState) {
	case EntityState_Boss_Igory::IDLE:
		DoNothing(dt);
		break;
	case EntityState_Boss_Igory::RUNNING:
		Chase(dt, playerPos);
		break;
	case EntityState_Boss_Igory::ATTACKING_BASIC:
		////printf("\nAtaqueBasic");
		pbodyFoot->body->SetLinearVelocity(b2Vec2_zero); //No se mueve mientras ataca

		if (!inAtack) {
			Attack(dt);
		}

		break;
	case EntityState_Boss_Igory::ATTACKING_CHARGE:
		break;
	case EntityState_Boss_Igory::ATTACKING_DASHI:
		inAtqDashi = true;
		if (!inDashDashi || !inAtaqueDashi) {
			inIdleDashi = true;
		}
		break;
	case EntityState_Boss_Igory::DEAD:
		//pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
		Die();
		break;
	case EntityState_Boss_Igory::FASE_CHANGE:
		pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
		currentAnimation = &atqCargado_fase1_boss_Igory;
		break;
	case EntityState_Boss_Igory::TAKEHIT:
		pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
		takeHit();
		break;
	case EntityState_Boss_Igory::GENERATESUREG:

		pbodyFoot->body->SetLinearVelocity(b2Vec2_zero); //No se mueve mientras ataca
		////printf("\ngeneraSureg");
		showSuregAni = true;
		break;
	case EntityState_Boss_Igory::HEAL:
		pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
		shieldTimer.Start();
		healTimer.Start();
		inCurar = true;
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
	////printf("Osiris idle");
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);

}

void Boss_Igory::Chase(float dt, iPoint playerPos)
{
	//////printf("Osiris chasing");
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
		app->audio->PlayFx(father_melee_attack_fx);
		atq3_boss_Igory.Reset();
		inAtack = true;
		//printf("\nataque1");
		if (isFacingLeft) {
			atackCube = app->physics->CreateRectangleSensor(position.x + 100, position.y - 60, 180, 180, STATIC);
		}
		else
		{
			atackCube = app->physics->CreateRectangleSensor(position.x - 80, position.y - 60, 180, 180, STATIC);
		}
		break;
	case 2:
		app->audio->PlayFx(father_melee_attackAlt_fx);
		atq1_boss_Igory.Reset();
		inAtack = true;
		//printf("\nataque2");
		if (isFacingLeft) {
			atackCube = app->physics->CreateRectangleSensor(position.x + 60, position.y - 50, 180, 100, STATIC);
		}
		else
		{
			atackCube = app->physics->CreateRectangleSensor(position.x - 40, position.y - 50, 180, 100, STATIC);
		}
		break;
	case 3:
		app->audio->PlayFx(father_melee_attack_fx);
		atq2_boss_Igory.Reset();
		inAtack = true;
		//printf("\nataque3");
		if (isFacingLeft) {
			atackCube = app->physics->CreateRectangleSensor(position.x + 60, position.y - 40, 180, 180, STATIC);
		}
		else
		{
			atackCube = app->physics->CreateRectangleSensor(position.x - 40, position.y - 40, 180, 180, STATIC);
		}
		atqDashQuali++;
		attackTime = 0;
		break;

	default:
		break;
	}
	if (atackCube != nullptr) {
		atackCube->entity = this;
		atackCube->listener = this;
		atackCube->ctype = ColliderType::ATACK_IGORY;
	}

}


void Boss_Igory::Die() {

	isDead = true;
	currentAnimation = &dead_boss_Igory;
	pbodyFoot->body->SetType(b2_staticBody);
	app->map->boss4_defeated = true;

	if (!unirPadre) {
		currentAnimation = &dead_boss_Igory;
	}
	else
	{
		currentAnimation = &reviver_boss_Igory;
	}

	if (startSelecion && seleccionFinalPersonaje == 1 && !closeFinalSelecion) {
		////printf("Matar padre");
		//CleanUp();
		cankillPadre = true;
		closeFinalSelecion = true;
	}

	if (startSelecion && seleccionFinalPersonaje == 2 && !closeFinalSelecion) {
		////printf("Unir padre");
		closeFinalSelecion = true;
		unirPadre = true;
	}
	////Mask XP
	//Item_mascara_3* mascara3 = (Item_mascara_3*)app->entityManager->CreateEntity(EntityType::ITEM_MASCARA3);
	//mascara3->config = configNode.child("entities_data").child("item_mascara_3");
	//mascara3->position = iPoint(position.x, position.y);
	//mascara3->Start();

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

void Boss_Igory::Revive()
{

}



bool Boss_Igory::Bossfinding(float dt, iPoint playerPosP)
{

	iPoint playerPos = app->map->WorldToMap(playerPosP.x, playerPosP.y);
	iPoint enemyPos = app->map->WorldToMap(position.x, position.y);




	if (playerInFight && !inSuregAni && nextState != EntityState_Boss_Igory::FASE_CHANGE) {
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
		////printf("delete");
	}
	////printf("\ndelete0");
}

float Boss_Igory::GetHealth() const {
	return health;
}

void Boss_Igory::TakeDamage(float damage) {


}




bool Boss_Igory::TimerColdDown(float time)
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

bool Boss_Igory::GeneraColdDown(float time)
{
	float fgenraTimeColdDown = generaTimeColdDown.CountDown(time);
	////printf("\nataqueTimeClodDown%: %f", fgenraTimeColdDown);
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

		if (dialogoMostrado && !app->dialogManager->isPlaying && startDialogo) {
			startSelecion = true;
		}

		if (!app->dialogManager->isPlaying && (app->input->GetButton(CONFIRM) == KEY_DOWN) && startDialogo) {
			if (!dialogoMostrado) {
				int num = 0;
				for (pugi::xml_node itemNode = dialogNode; itemNode; itemNode = itemNode.next_sibling("sentence"))
				{
					app->dialogManager->AddDialog(app->dialogManager->CreateDialog(itemNode, itemNode.attribute("name").as_string(), itemNode.attribute("facetexturepath").as_string()));
					num++;
				}
				//printf("\nnum: %d", num);
				dialogoMostrado = true;
			}
		}
		if (physA->ctype == ColliderType::ATACK_IGORY) {
			app->entityManager->GetPlayer()->TakeDamage(attackDamage);
			////printf("JEFEATAKE");
		}

		if (inAtqDashi) {
			app->entityManager->GetPlayer()->TakeDamage(220);
			empujaPlayer = true;
			habilidadEmpujeTimer.Start();
			//app->entityManager->GetPlayer()->pbodyFoot
		}
		break;
	case ColliderType::PLAYER_ATTACK:
		LOG("Collision Player_Attack");
		if (playerInFight && nextState != EntityState_Boss_Igory::FASE_CHANGE && app->entityManager->GetPlayer()->checkAtk == true) {
			if (!inCurar) {
				inTakeHit = true;
				health -= app->entityManager->GetPlayer()->currentStats.attackDamage;
				timerRecibirDanioColor.Start();
				//printf("\n BossHeal %f", health);
				app->entityManager->GetPlayer()->checkAtk = false;
			}
			else
			{
				inTakeHit = true;
				shield -= app->entityManager->GetPlayer()->currentStats.attackDamage;
				timerRecibirDanioColor.Start();
				app->entityManager->GetPlayer()->checkAtk = false;
				if (shield <= 0) {
					shieldBroken = true;
					shield = 0;
					//printf("\n shieldBroken %f");
				}
				//printf("\n shield %f", shield);
			}

		}
		if (cankillPadre) {
			killPadre = true;
		}
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