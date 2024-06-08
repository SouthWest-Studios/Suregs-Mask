#include "Enemy_Ols_Variation.h"
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
#include "Item_Garra.h"
#include "Item_Saliva.h"
#include "BestiarioManager.h"
#include "InventoryManager.h"
#include "Physics.h"
#include <Optick/include/optick.h>





Enemy_Ols_Variation::Enemy_Ols_Variation() : Entity(EntityType::ENEMY_OLS_VARIATION), maxHealth(350.0f), health(350.0f), speed(1.0f){
	name = ("ols");
	state = EntityState_Enemy::IDLE;
	nextState = EntityState_Enemy::IDLE;
	currentState = state;
	desiredState = nextState;
	nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;

}

Enemy_Ols_Variation::~Enemy_Ols_Variation() {

}

bool Enemy_Ols_Variation::Awake() {

	return true;
}

bool Enemy_Ols_Variation::Start() {

	OPTICK_EVENT();
	//position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	TSprite = config.attribute("Tsprite").as_int();
	SpriteX = config.attribute("sprite_x").as_int();
	SpriteY = config.attribute("sprite_y").as_int();
	Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

	idleAnim.LoadAnim("ols", "idleAnim_ols", spritePositions);
	runAnim.LoadAnim("ols", "runAnim_ols", spritePositions);
	attackAnim.LoadAnim("ols", "attackAnim_ols", spritePositions);
	dieAnim.LoadAnim("ols", "dieAnim_ols", spritePositions);

	//texture = app->tex->Load(config.attribute("texturePath").as_string());
	texture = app->entityManager->textureOls;

	ols_get_damage_fx = app->audio->LoadAudioFx("ols_get_damage_fx");
	ols_get_damageAlt_fx = app->audio->LoadAudioFx("ols_get_damageAlt_fx");
	ols_get_damageAlt2_fx = app->audio->LoadAudioFx("ols_get_damageAlt2_fx");
	ols_death_fx = app->audio->LoadAudioFx("ols_death_fx");

	pbodyFoot = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
	pbodyFoot->entity = this;
	pbodyFoot->listener = this;
	pbodyFoot->ctype = ColliderType::ENEMY;

	maxHealth = config.attribute("maxHealth").as_float();
	health = maxHealth;
	speed = config.attribute("speed").as_float();
	attackDamage = config.attribute("attackDamage").as_float();
	attackDistance = config.attribute("attackDistance").as_float();
	viewDistance = config.attribute("viewDistance").as_float();
	minDistance = config.attribute("minDistance").as_float();

	projectileSpeed = config.attribute("projectileSpeed").as_float();
	projectileDamage = config.attribute("projectileDamage").as_float();
	projectileTexture = app->tex->Load(config.attribute("texturePathProjectile").as_string());

	// app->entityManager->objectsToDraw.push_back({
	// 	texture,
	// 	position.x, // x
	// 	position.y, // y
	// 	position.y + 98, // anchorY
	// 	100, // width
	// 	100, // height
	// 	NULL, // currentFrame
	// 	isFacingLeft
	// });

	room = GetCurrentRoom();

	return true;
}

bool Enemy_Ols_Variation::Update(float dt)
{
	OPTICK_EVENT();

	iPoint playerPos = app->entityManager->GetPlayer()->position;

	if (playerPos.x < position.x)
	{
		isFacingLeft = false;
	}
	else(isFacingLeft = true);

	float distanceToPlayer = app->map->pathfinding->GetDistance(playerPos, position);
	float length = sqrt((playerPos.x - position.x) * (playerPos.x - position.x) + (playerPos.y - position.y) * (playerPos.y - position.y));
	iPoint direction = iPoint((playerPos.x - position.x) / length, (playerPos.y - position.y) / length);

	if (health <= 0) 
	{
		desiredState = EntityState_Enemy::DEAD;
	}
	else if (distanceToPlayer <= attackDistance * 32 && canAttack)
	{
		desiredState = EntityState_Enemy::ATTACKING;
	}
	else if (distanceToPlayer <= viewDistance * 32 && distanceToPlayer > attackDistance * 32)
	{
		desiredState = EntityState_Enemy::RUNNING;
	}
	 else if(distanceToPlayer <= viewDistance * 32 && distanceToPlayer <= minDistance * 32)
	 {
	 	desiredState = EntityState_Enemy::RUNNING;
	 }
	else
	{
		desiredState = EntityState_Enemy::IDLE;
	}

	UpdateAttackSensor(dt);

	stateMachine(dt, playerPos);

	//VENENO <----------
	CheckPoison();
	//VENENO ---------->

	if(currentAnimation != nullptr){
		currentAnimation->Update();
	}


	return true;
}


bool Enemy_Ols_Variation::PostUpdate() {

    SDL_SetTextureColorMod(texture, 198, 115, 255);
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
		app->render->DrawTexture(texture, position.x - 40, position.y - 70, 0.5f, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 70, position.y - 70, 0.5f, SDL_FLIP_NONE, &rect);
	}

    if (attackSensor != nullptr)
    {
        // Actualiza la posición del proyectil
        b2Vec2 projectilePos = attackSensor->body->GetPosition();
        projectilePosition.x = METERS_TO_PIXELS(projectilePos.x);
        projectilePosition.y = METERS_TO_PIXELS(projectilePos.y);

        // Dibuja el proyectil en su posición actualizada
        app->render->DrawTexture(projectileTexture, projectilePosition.x-12, projectilePosition.y-12);
    }

	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 16;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 16;
	return true;
}


bool Enemy_Ols_Variation::CleanUp()
{
	app->entityManager->DestroyEntity(pbodyFoot->entity);
	app->physics->DestroyBody(pbodyFoot);
	pbodyFoot = nullptr;
	//app->tex->UnLoad(texture);

	app->physics->DestroyBody(attackSensor); 
	attackSensor = nullptr;
	app->tex->UnLoad(projectileTexture);

	blood = nullptr;

	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}

void Enemy_Ols_Variation::DoNothing(float dt)
{
	if((currentAnimation->HasFinished() && currentAnimation == &attackAnim) || currentAnimation == &runAnim){
		currentAnimation = &idleAnim;
	}
	////printf("Osiris idle");
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
}

void Enemy_Ols_Variation::Chase(float dt, iPoint playerPos)
{
	float distanceToPlayer = app->map->pathfinding->GetDistance(playerPos, position);
	if (distanceToPlayer <= viewDistance * 32 && distanceToPlayer > attackDistance * 32 && distanceToPlayer > minDistance * 32)
	{
		Olsfinding(dt);
	}
	else
	{
		Flee(dt);
	}
	if(currentAnimation != nullptr){
		if(currentAnimation->HasFinished() && currentAnimation == &attackAnim){
			currentAnimation = &runAnim;
		}
		else if(currentAnimation == &idleAnim){
			currentAnimation = &runAnim;
		}
	}
}

void Enemy_Ols_Variation::Attack(float dt, iPoint playerPos)
{
    if (canAttack)
    {
        attackSensor = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
        attackSensor->ctype = ColliderType::PROJECTILE;
        attackSensor->listener = this;
        attackSensor->body->GetFixtureList()->SetSensor(true);
		projectilePosition = position;

        b2Vec2 direction = b2Vec2(playerPos.x - position.x, playerPos.y-position.y);  
        direction.Normalize();
		direction *= projectileSpeed;

        attackSensor->body->SetLinearVelocity(direction); 

        attackCooldownTimer.Start();
		

        canAttack = false;

        currentAnimation = &attackAnim;
        //sonido ataque
    }
}

void Enemy_Ols_Variation::Die() {
	app->audio->PlayFx(ols_death_fx, 2);

	fPoint pos((float)position.x, (float)position.y);
	blood = app->psystem->AddEmiter(pos, EMITTER_TYPE_ENEMY_BLOOD);

	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	if (parseResult) {
		configNode = configFile.child("config");
	}

	float randomValue = (float(std::rand() % 101) / 100);

	// Determina si el item debe crearse basado en un 30% de probabilidad
	if (randomValue <= 0.30f) {
		Item_Saliva* saliva = (Item_Saliva*)app->entityManager->CreateEntity(EntityType::ITEM_SALIVA);
		saliva->config = configNode.child("entities_data").child("item_saliva");
		saliva->position = iPoint(position.x, position.y);
		saliva->Start();
	}
	app->bestiarioManager->CreateItem("olsV");

	//app->entityManager->DestroyEntity(this);
	//app->physics->GetWorld()->DestroyBody(pbodyFoot->body);LIN
	//app->physics->DestroyBody(pbodyFoot);
	//app->tex->UnLoad(texture);

	if(currentAnimation == &dieAnim && currentAnimation->HasFinished()){
		CleanUp();
	}

	//Mask 0
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK0)
	{
		app->inventoryManager->maskZeroXP += 80;
		////printf("Current Mask 0 XP %i \n", app->inventoryManager->maskZeroXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK0)
	{
		app->inventoryManager->maskZeroXP += 80;
		////printf("Current Mask 0 XP %i \n", app->inventoryManager->maskZeroXP);
	}

	//Mask 1
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK1)
	{
		app->inventoryManager->maskOneXP += 80;
		////printf("Current Mask 1 XP %i \n", app->inventoryManager->maskOneXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK1)
	{
		app->inventoryManager->maskOneXP += 80;
		////printf("Current Mask 1 XP %i \n", app->inventoryManager->maskOneXP);
	}

	//Mask 2
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK2)
	{
		app->inventoryManager->maskTwoXP += 80;
		////printf("Current Mask 2 XP %i \n", app->inventoryManager->maskTwoXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK2)
	{
		app->inventoryManager->maskTwoXP += 80;
		////printf("Current Mask 2 XP %i \n", app->inventoryManager->maskTwoXP);
	}

	//Mask 3
	if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK3)
	{
		app->inventoryManager->maskThreeXP += 80;
		////printf("Current Mask 3 XP %i \n", app->inventoryManager->maskThreeXP);
	}

	if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK3)
	{
		app->inventoryManager->maskThreeXP += 80;
		////printf("Current Mask 3 XP %i \n", app->inventoryManager->maskThreeXP);
	}

}

// L07 DONE 6: Define OnCollision function for the player. 
void Enemy_Ols_Variation::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		if(physA->ctype == ColliderType::PROJECTILE) {
			app->entityManager->GetPlayer()->TakeDamage(projectileDamage);
		}
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

void Enemy_Ols_Variation::SetPlayer(Player* player)
{
	this->player = player;
}	

bool Enemy_Ols_Variation::Olsfinding(float dt)
{
	iPoint playerPos = app->map->WorldToMap(app->entityManager->GetPlayer()->position.x, app->entityManager->GetPlayer()->position.y);
	iPoint enemyPos = app->map->WorldToMap(position.x, position.y);

	int pathLength = app->map->pathfinding->CreatePath(enemyPos, playerPos);

	if (abs(playerPos.x - enemyPos.x) < viewDistance && abs(playerPos.y - enemyPos.y) < viewDistance && pathLength < viewDistance) {
		lastPath = app->map->pathfinding->GetLastPath();

		//Get the latest calculated path and draw
		for (uint i = 0; i < lastPath->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(lastPath->At(i)->x, lastPath->At(i)->y);
			if (app->physics->debug == true) {
				app->render->DrawTexture(app->map->pathfinding->mouseTileTex, pos.x, pos.y, SDL_FLIP_NONE);
			}
		}

		if (lastPath->Count() > 1) { // Asegate de que haya al menos una posicion en el camino

			// Toma la primera posicion del camino como el objetivo al que el enemigo debe dirigirse
			iPoint targetPos = app->map->MapToWorld(lastPath->At(1)->x, lastPath->At(1)->y);

			// Calcula la direccion hacia el objetivo
			b2Vec2 direction(targetPos.x - position.x, targetPos.y - position.y);
			direction.Normalize();

			// Calcula la velocidad del movimiento
			b2Vec2 velocity(direction.x * speed, direction.y * speed);

			// Aplica la velocidad al cuerpo del enemigo
			pbodyFoot->body->SetLinearVelocity(velocity);

			// Determina si el enemigo est?mirando hacia la izquierda o hacia la derecha
			if (direction.x < 0) {
				isFacingLeft = false;
			}
			else {
				isFacingLeft = true;
			}

			isAttacking = false;
			attackAnim.Reset();

		}
	}

	
	return true;
}

float Enemy_Ols_Variation::GetHealth() const {
	//printf("Initial health: %f\n", health);
	return health;

}

void Enemy_Ols_Variation::TakeDamage(float damage) {

	if (currentState != EntityState_Enemy::DEAD && invulnerabilityTimer.ReadMSec() >= 500) {
		health -= damage;
		invulnerabilityTimer.Start();
		timerRecibirDanioColor.Start();

 		app->audio->PlayRandomFx(ols_get_damage_fx, ols_get_damageAlt_fx, ols_get_damageAlt2_fx);

		//printf("Enemy_Ols_Variation has received  %f damage\n", damage);
		
	}
}

void Enemy_Ols_Variation::stateMachine(float dt, iPoint playerPos)
{
    ////printf("\ncurrentState: %d, desiredState: %d", static_cast<int>(currentState), static_cast<int>(desiredState));
    nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;
    switch (nextState) {
    case EntityState_Enemy::IDLE:
        DoNothing(dt);
        // if (currentAnimation != &idleAnim) {
        //     currentAnimation = &idleAnim;
        // }
        break;
    case EntityState_Enemy::RUNNING:
        Chase(dt, playerPos);
        // if (currentAnimation != &runAnim) {
        //     currentAnimation = &runAnim;
        // }
        break;
    case EntityState_Enemy::ATTACKING:
        Attack(dt, playerPos);
        // if (currentAnimation != &attackAnim) {
        //     currentAnimation = &attackAnim;
        // }
        break;
    case EntityState_Enemy::DEAD:
        Die();
        // if (currentAnimation != &dieAnim) {
        //     currentAnimation = &dieAnim;
        // }
        break;
    case EntityState_Enemy::DASHI:
        break;
    case EntityState_Enemy::NONE:
        desiredState = EntityState_Enemy::IDLE;
        break;
    default:
        break;
    }
    currentState = nextState;
}

//VENENO <----------
void Enemy_Ols_Variation::ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate) {
	this->poisonDamage = poisonDamage;
	this->poisonDuration = poisonDuration;
	this->poisonTickRate = poisonTickRate;
	
	this->poisoned = true;
	this->firstTimePoisonRecibed = true;

	poisonTimer.Start();
	poisonTickTimer.Start();


}

void Enemy_Ols_Variation::CheckPoison() {
	float epsilon = 0.1f; //Para margen de error

    // Aplicar el primer tick de daño inmediatamente (si no, el primer tick no se aplica en el segundo 0.0)
	if(firstTimePoisonRecibed) {
		if (currentState != EntityState_Enemy::DEAD) {
			health -= poisonDamage;
			invulnerabilityTimer.Start();
			timerRecibirDanioColor.Start();

			//printf("Enemy_Osiris has received  %f damage of poison\n", poisonDamage);
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

                //printf("Enemy_Osiris has received  %f damage of poison\n", poisonDamage);
            }
        }
    }
}
//VENENO ---------->

void Enemy_Ols_Variation::UpdateAttackSensor(float dt)
{
	if (!canAttack && attackCooldownTimer.ReadSec() >= 5.0f)
    {
        canAttack = true;
    }
	
    if (attackSensor != nullptr && canAttack)
    {
		canAttack = false;
		attackCooldownTimer.Start();

        if (attackSensorTimer.ReadSec() >= 2.0f)
        {
            app->physics->DestroyBody(attackSensor);
            attackSensor = nullptr;
        }
    }
}

bool Enemy_Ols_Variation::Flee(float dt) // Huye del jugador
{
	iPoint playerPos = app->map->WorldToMap(app->entityManager->GetPlayer()->position.x, app->entityManager->GetPlayer()->position.y);
	iPoint enemyPos = app->map->WorldToMap(position.x, position.y);

	int pathLength = app->map->pathfinding->CreatePath(playerPos, enemyPos);

	if (abs(playerPos.x - enemyPos.x) < 15 && abs(playerPos.y - enemyPos.y) < 15 && pathLength > 0 && pathLength < viewDistance) {
		lastPath = app->map->pathfinding->GetLastPath();

		//Get the latest calculated path and draw
		for (uint i = 0; i < lastPath->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(lastPath->At(i)->x, lastPath->At(i)->y);
			if (app->physics->debug == true) {
				app->render->DrawTexture(app->map->pathfinding->mouseTileTex, pos.x, pos.y, SDL_FLIP_NONE);
			}
		}

		if (lastPath->Count() > 1) { // Asegate de que haya al menos una posicion en el camino

			// Toma la primera posicion del camino como el objetivo al que el enemigo debe dirigirse
			iPoint targetPos = app->map->MapToWorld(lastPath->At(1)->x, lastPath->At(1)->y);

			// Calcula la direccion hacia el objetivo
			b2Vec2 direction(targetPos.x - position.x, targetPos.y - position.y);
			direction.Normalize();

			// Invierte la dirección para alejarse del jugador
			direction.x *= -1;
			direction.y *= -1;

			// Calcula la velocidad del movimiento
			b2Vec2 velocity(direction.x * speed, direction.y * speed);

			// Aplica la velocidad al cuerpo del enemigo
			pbodyFoot->body->SetLinearVelocity(velocity);

			// Determina si el enemigo está mirando hacia la izquierda o hacia la derecha
			if (direction.x < 0) {
				isFacingLeft = false;
			}
			else {
				isFacingLeft = true;
			}
		}
	}

	return true;
}

MapObject* Enemy_Ols_Variation::GetCurrentRoom()
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

void Enemy_Ols_Variation::DestroyProjectile()
{
	if(attackSensor != nullptr){
		app->physics->DestroyBody(attackSensor);
		attackSensor = nullptr;
	}
}