#include "Enemy_Khurt_Variation.h"
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
#include "ParticleSystem.h"
#include "Map.h"
#include "Item_Hueso.h"
#include "Item_Garra.h"
#include <Optick/include/optick.h>
#include "Utils.cpp"

Enemy_Khurt_Variation::Enemy_Khurt_Variation() : Entity(EntityType::ENEMY_KHURT) {
	name = ("khurt_variation");
	state = EntityState_Enemy::IDLE;
	nextState = EntityState_Enemy::IDLE;
	currentState = state;
	desiredState = nextState;
	nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;
}

Enemy_Khurt_Variation::~Enemy_Khurt_Variation() {}

bool Enemy_Khurt_Variation::Awake() {
	return true;
}

bool Enemy_Khurt_Variation::Start() {
	OPTICK_EVENT();
	// Configuración inicial...

	TSprite = config.attribute("Tsprite").as_int();
	SpriteX = config.attribute("sprite_x").as_int();
	SpriteY = config.attribute("sprite_y").as_int();
	Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

	idleAnim.LoadAnim("khurt_variation", "idleAnim", spritePositions);
	underAnim_start.LoadAnim("khurt_variation", "underAnim_start", spritePositions);
	underAnim_process.LoadAnim("khurt_variation", "underAnim_process", spritePositions);
	underAnim_finish.LoadAnim("khurt_variation", "underAnim_finish", spritePositions);
	stunAnim.LoadAnim("khurt_variation", "stunAnim", spritePositions);
	attack.LoadAnim("khurt_variation", "attack", spritePositions);
	dieAnim.LoadAnim("khurt_variation", "dieAnim", spritePositions);

	texture = app->tex->Load(config.attribute("texturePath").as_string());

	khurt_get_damage_fx = app->audio->LoadAudioFx("khurt_get_damage_fx");
	khurt_get_damageAlt_fx = app->audio->LoadAudioFx("khurt_get_damageAlt_fx");

	digTimer.Start();

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

	room = GetCurrentRoom();

	return true;
}

bool Enemy_Khurt_Variation::Update(float dt) {
	OPTICK_EVENT();
	// Actualizar sensor de posición
	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
	pbodySensor->body->SetTransform(b2Vec2(pbodyPos.p.x, pbodyPos.p.y - 1), 0);

	iPoint playerPos = app->entityManager->GetPlayer()->position;


	if (health <= 0) {
		nextState = EntityState_Enemy::DEAD;
		//printf("DEAD\n");
	}
	else if (app->map->pathfinding->GetDistance(playerPos, position) <= attackDistance * 32 && !digging && !stunned) {
		nextState = EntityState_Enemy::ATTACKING;
		/*printf("ATTACKING\n")*/;
	}
	else if (app->map->pathfinding->GetDistance(playerPos, position) <= viewDistance * 32 && app->map->pathfinding->GetDistance(playerPos, position) >= attackDistance * 32 && !charging && !stunned) {
		nextState = EntityState_Enemy::RUNNING;
		//printf("RUNNING\n");
	}
	else if (!stunned && !digging && !charging) {
		nextState = EntityState_Enemy::IDLE;
		//printf("IDLE\n");
	}



	switch (nextState) {
	case EntityState_Enemy::RUNNING:
		Chase(dt, playerPos);
		break;
	case EntityState_Enemy::ATTACKING:
		Attack(dt, playerPos);
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

	if (stunned)
	{
		Stunned(dt);

	}

	for (auto it = magicTrails.begin(); it != magicTrails.end();) {
		if (it->durationTimer.ReadSec() >= 2.0) {
			app->physics->DestroyBody(it->sensor);
			it = magicTrails.erase(it);
		}
		else {
			++it;
		}
	}

	// VENENO <----------
	CheckPoison();
	// VENENO ---------->

	currentState = nextState;
	currentAnimation->Update();
	return true;
}


bool Enemy_Khurt_Variation::PostUpdate() {
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
		app->render->DrawTexture(texture, position.x - 25, position.y - 65, 5, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 40, position.y - 65, 5, SDL_FLIP_NONE, &rect);
	}

	for (uint i = 0; i < lastPath.Count(); ++i) {
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

bool Enemy_Khurt_Variation::CleanUp() {
	app->physics->GetWorld()->DestroyBody(pbodyFoot->body);
	app->physics->GetWorld()->DestroyBody(pbodySensor->body);
	app->tex->UnLoad(texture);
	lastPath.Clear();

	blood = nullptr;

	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}

void Enemy_Khurt_Variation::DoNothing(float dt) {
	currentAnimation = &idleAnim;
	/*printf("Khurt idle\n");*/
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
}

void Enemy_Khurt_Variation::Chase(float dt, iPoint playerPos) {
	//printf("Khurt chasing\n");
	currentAnimation = &runAnim;
	stunned = false;
	chargetimer = false;
	digtimer = false;

	if (!digging && !stunned)
	{
		digTimer.Start();
	}
	HandleDigging(dt, playerPos);
	digging = true;

}

void Enemy_Khurt_Variation::Attack(float dt, iPoint playerPos) {

	//printf("Khurt attacking\n");
	currentAnimation = &attack;

	if (!chargetimer)
	{
		/*printf("WWWWWWW");*/
		chargeTimer.Start();
	}
	chargetimer = true;
	Antposition = position;
	if (!stunned)
	{
		/*printf("NNNNNNNN");*/
		charging = true;
		Charge(dt, playerPos);
	}
}

void Enemy_Khurt_Variation::Die() {
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
	currentAnimation = &dieAnim;

	fPoint pos((float)position.x, (float)position.y);
	blood = app->psystem->AddEmiter(pos, EMITTER_TYPE_ENEMY_BLOOD);

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

	// Mask XP

	// Mask 0
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK0) {
		app->entityManager->GetPlayer()->maskZeroXP += 80;
		// printf("Current Mask 0 XP %i \n", app->entityManager->GetPlayer()->maskZeroXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK0) {
		app->entityManager->GetPlayer()->maskZeroXP += 80;
		// printf("Current Mask 0 XP %i \n", app->entityManager->GetPlayer()->maskZeroXP);
	}

	// Mask 1
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK1) {
		app->entityManager->GetPlayer()->maskOneXP += 80;
		// printf("Current Mask 1 XP %i \n", app->entityManager->GetPlayer()->maskOneXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK1) {
		app->entityManager->GetPlayer()->maskOneXP += 80;
		// printf("Current Mask 1 XP %i \n", app->entityManager->GetPlayer()->maskOneXP);
	}

	// Mask 2
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK2) {
		app->entityManager->GetPlayer()->maskTwoXP += 80;
		// printf("Current Mask 2 XP %i \n", app->entityManager->GetPlayer()->maskTwoXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK2) {
		app->entityManager->GetPlayer()->maskTwoXP += 80;
		// printf("Current Mask 2 XP %i \n", app->entityManager->GetPlayer()->maskTwoXP);
	}

	// Mask 3
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK3) {
		app->entityManager->GetPlayer()->maskThreeXP += 80;
		// printf("Current Mask 3 XP %i \n", app->entityManager->GetPlayer()->maskThreeXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK3) {
		app->entityManager->GetPlayer()->maskThreeXP += 80;
		// printf("Current Mask 3 XP %i \n", app->entityManager->GetPlayer()->maskThreeXP);
	}
}

// L07 DONE 6: Define OnCollision function for the player.
void Enemy_Khurt_Variation::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype) {
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		// restar vida al player
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

float Enemy_Khurt_Variation::GetHealth() const {
	return health;
}

void Enemy_Khurt_Variation::TakeDamage(float damage) {
	if (currentState != EntityState_Enemy::DEAD && invulnerabilityTimer.ReadMSec() >= 500) {
		health -= damage;
		invulnerabilityTimer.Start();
		timerRecibirDanioColor.Start();
		app->audio->PlayRandomFx(khurt_get_damage_fx, khurt_get_damageAlt_fx, NULL);
	}
}

void Enemy_Khurt_Variation::Charge(float dt, iPoint playerPos) {
	//printf("Charge\n");
	currentAnimation = &attack;
	if (!chargeimpulse)
	{
		b2Vec2 direction(playerPos.x - position.x, playerPos.y - position.y);
		direction.Normalize();

		b2Vec2 impulse = b2Vec2(direction.x * speed * 3, direction.y * speed * 3);
		pbodyFoot->body->ApplyLinearImpulse(impulse, pbodyFoot->body->GetWorldCenter(), true);
		/*printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");*/
	}
	chargeimpulse = true;
	if (charging && chargeTimer.ReadSec() >= 0.7 || app->map->pathfinding->GetDistance(playerPos, position) > 500) {
		charging = false;
		/*printf("Charge ended, transitioning to STUNNED\n");*/
		Stunned(dt);
	}
}

void Enemy_Khurt_Variation::Stunned(float dt) {
	if (!stunnedtimer)
	{
		stunTimer.Start();
	}
	stunnedtimer = true;
	if (stunTimer.ReadSec() <= 1.5) {
		stunned = true;
		/*printf("Stunned\n");*/
		currentAnimation = &stunAnim;
		pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
	}
	else {
		stunned = false;
		digtimer = false;
		digging = false;
		chargeimpulse = false;
		charging = false;
		chargetimer = false;
		stunnedtimer = false;
		nextState = EntityState_Enemy::RUNNING;
		/*printf("Transition to %d after stun\n", nextState);*/
	}
}

void Enemy_Khurt_Variation::HandleDigging(float dt, iPoint playerPos) {
	float elapsedTime = digTimer.ReadSec();
	if (!digtimer)
	{
		if (elapsedTime < 0.2f) {
			/*printf("Digging Phase 1\n");*/
			// Fase de inicio de la dig
			currentAnimation = &underAnim_start;
		}
		else if (elapsedTime < 2.0f) {
			/*printf("Digging Phase 2\n");*/
			// Fase de desplazamiento bajo tierra
			b2Vec2 direction(playerPos.x - position.x, playerPos.y - position.y);
			direction.Normalize();
			pbodyFoot->body->SetLinearVelocity(b2Vec2(direction.x * speed * 2, direction.y * speed * 2));

			currentAnimation = &underAnim_process;
			CreateAroMagica();
		}
		else {
			/*printf("Digging Phase 3\n");*/
			// Fase de emergencia
			digging = false;
			digTimer.Start();
			digtimer = true;
			currentAnimation = &underAnim_finish;
			if (app->map->pathfinding->GetDistance(playerPos, position) <= attackDistance * 32) {
				nextState = EntityState_Enemy::ATTACKING;
				/*printf("ATTACKING despues de DIGGING fase 3\n");*/
			}
			else {
				nextState = EntityState_Enemy::IDLE;
				/*printf("IDLE despues de DIGGING fase 3\n");*/
			}
			//pbodyFoot->body->SetLinearVelocity(b2Vec2_zero); // Detener el movimiento

		}
	}
}

void Enemy_Khurt_Variation::CreateAroMagica() {
	if (stunned) {
		ClearMagicTrails();
		return;
	}

	iPoint AroPosition = position;

	for (const auto& trail : magicTrails) {
		if (trail.sensor != nullptr && trail.sensor->Contains(AroPosition.x, AroPosition.y)) {
			return;
		}
	}

	PhysBody* newMagicTrail = CreateMagicTrailSensor(AroPosition.x, AroPosition.y, 30, 30);
	MagicTrail trail = { newMagicTrail };
	trail.durationTimer.Start();
	magicTrails.push_back(trail);

	if (magicTrails.size() >= 20) {
		app->physics->DestroyBody(magicTrails.front().sensor);
		magicTrails.erase(magicTrails.begin());
	}
}

PhysBody* Enemy_Khurt_Variation::CreateMagicTrailSensor(int x, int y, int width, int height) {
	PhysBody* newMagicTrail = app->physics->CreateRectangleSensor(x, y, width, height, STATIC);
	newMagicTrail->ctype = ColliderType::AROMAGICA;
	newMagicTrail->listener = this;
	return newMagicTrail;
}

void Enemy_Khurt_Variation::ClearMagicTrails() {
	for (const auto& trail : magicTrails) {
		app->physics->DestroyBody(trail.sensor);
	}
	magicTrails.clear();
}


void Enemy_Khurt_Variation::ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate) {
	this->poisonDamage = poisonDamage;
	this->poisonDuration = poisonDuration;
	this->poisonTickRate = poisonTickRate;

	this->poisoned = true;
	this->firstTimePoisonRecibed = true;

	poisonTimer.Start();
	poisonTickTimer.Start();
}

void Enemy_Khurt_Variation::CheckPoison() {
	float epsilon = 0.1f; // Para margen de error

	// Aplicar el primer tick de daño inmediatamente (si no, el primer tick no se aplica en el segundo 0.0)
	if (firstTimePoisonRecibed) {
		if (currentState != EntityState_Enemy::DEAD) {
			health -= poisonDamage;
			invulnerabilityTimer.Start();
			timerRecibirDanioColor.Start();

			/*printf("Enemy_Khurt has received %f damage of poison\n", poisonDamage);*/
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

				/*printf("Enemy_Khurt has received %f damage of poison\n", poisonDamage);*/
			}
		}
	}
}

// VENENO ---------->

MapObject* Enemy_Khurt_Variation::GetCurrentRoom() {
	// salas pequeñas
	for (ListItem<MapObject*>* item = app->map->smallRoomsList.start; item != nullptr; item = item->next) {
		MapObject* room = item->data;

		// el jugador está dentro de la sala
		if (position.x >= room->x && position.x <= room->x + room->width &&
			position.y >= room->y && position.y <= room->y + room->height) {
			return room;
		}
	}

	// salas grandes
	for (ListItem<MapObject*>* item = app->map->largeRoomsList.start; item != nullptr; item = item->next) {
		MapObject* room = item->data;

		// el jugador está dentro de la sala
		if (position.x >= room->x && position.x <= room->x + room->width &&
			position.y >= room->y && position.y <= room->y + room->height) {
			return room;
		}
	}

	// salas l
	for (ListItem<MapObject*>* item = app->map->LRoomsList.start; item != nullptr; item = item->next) {
		MapObject* room = item->data;

		// el jugador está dentro de la sala
		if (position.x >= room->x && position.x <= room->x + room->width &&
			position.y >= room->y && position.y <= room->y + room->height) {
			return room;
		}
	}

	return nullptr;
}
