#include "Enemy_Ols.h"
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
#include "Item_Garra.h"
#include "Physics.h"
#include <Optick/include/optick.h>





Enemy_Ols::Enemy_Ols() : Entity(EntityType::ENEMY_OLS), maxHealth(350.0f), health(350.0f), speed(1.0f){
	name.Create("ols");
}

Enemy_Ols::~Enemy_Ols() {

}

bool Enemy_Ols::Awake() {

	return true;
}

bool Enemy_Ols::Start() {

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

	texture = app->tex->Load(config.attribute("texturePath").as_string());

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

	projectileSpeed = config.attribute("projectileSpeed").as_float();
	projectileDamage = config.attribute("projectileDamage").as_float();
	projectileTexture = app->tex->Load(config.attribute("texturePathProjectile").as_string());

	return true;
}

bool Enemy_Ols::Update(float dt)
{
	OPTICK_EVENT();

	iPoint playerPos = app->entityManager->GetPlayer()->position;

	if (playerPos.x < position.x)
	{
		isFacingLeft = true;
	}
	else(isFacingLeft = false);

	float distanceToPlayer = app->map->pathfinding->GetDistance(playerPos, position);
	float length = sqrt((playerPos.x - position.x) * (playerPos.x - position.x) + (playerPos.y - position.y) * (playerPos.y - position.y));
	iPoint direction = iPoint((playerPos.x - position.x) / length, (playerPos.y - position.y) / length);

	if (health <= 0) 
	{
		desiredState = EntityState_Enemy::DEAD;
	}
	// Si el jugador está dentro del rango de ataque
	else if (distanceToPlayer <= attackDistance * 32)
	{
		    if (canAttack) {
				desiredState = EntityState_Enemy::ATTACKING;
			}
			else {
				desiredState = EntityState_Enemy::IDLE;
			}
	}
	// Si el jugador está dentro del rango de visión pero fuera del rango de ataque
	else if (distanceToPlayer <= viewDistance * 32)
	{
		desiredState = EntityState_Enemy::RUNNING;

		// Si el jugador está demasiado cerca, alejarse
		if (distanceToPlayer < attackDistance * 0.75f * 32)
		{
			position.x -= direction.x * speed * dt;
			position.y -= direction.y * speed * dt;
		}
		// Si el jugador está demasiado lejos, acercarse
		else if (distanceToPlayer > attackDistance * 32)
		{
			position.x += direction.x * speed * dt;
			position.y += direction.y * speed * dt;
		}
	}
	// Si el jugador está fuera del rango de visión
	else
	{
		DoNothing(dt);
	}

	UpdateAttackSensor(dt);

	stateMachine(dt, playerPos);

	//VENENO <----------
	CheckPoison();
	//VENENO ---------->

	currentAnimation->Update();

	return true;
}


bool Enemy_Ols::PostUpdate() {

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
		app->render->DrawTexture(texture, position.x - 120, position.y - 60, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 120, position.y - 60, SDL_FLIP_NONE, &rect);
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


bool Enemy_Ols::CleanUp()
{
	app->entityManager->DestroyEntity(pbodyFoot->entity);
	app->physics->DestroyBody(pbodyFoot);
	app->tex->UnLoad(texture);

	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}

void Enemy_Ols::DoNothing(float dt)
{
	currentAnimation = &idleAnim;
	//printf("Osiris idle");
	pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
}

void Enemy_Ols::Chase(float dt, iPoint playerPos)
{
	//printf("Osiris chasing");
	currentAnimation = &runAnim;
	Olsfinding(dt);
}

void Enemy_Ols::Attack(float dt, iPoint playerPos)
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

void Enemy_Ols::Die() {
	app->audio->PlayFx(ols_death_fx, 2);

	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	if (parseResult) {
		configNode = configFile.child("config");
	}

	garra = (Item_Garra*)app->entityManager->CreateEntity(EntityType::ITEM_GARRA);
	garra->config = configNode.child("entities_data").child("item_garra");
	garra->position = iPoint(position.x, position.y);
	garra->Start();

	app->entityManager->DestroyEntity(this);
	app->physics->GetWorld()->DestroyBody(pbodyFoot->body);
	app->tex->UnLoad(texture);
	app->entityManager->GetPlayer()->playerXP += 30;
	printf("Current XP %i \n", app->entityManager->GetPlayer()->playerXP);
}

// L07 DONE 6: Define OnCollision function for the player. 
void Enemy_Ols::OnCollision(PhysBody* physA, PhysBody* physB) {
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

void Enemy_Ols::SetPlayer(Player* player)
{
	this->player = player;
}	

bool Enemy_Ols::Olsfinding(float dt)
{
	iPoint playerPos = app->map->WorldToMap(app->entityManager->GetPlayer()->position.x, app->entityManager->GetPlayer()->position.y);
	iPoint enemyPos = app->map->WorldToMap(position.x, position.y);

	if (abs(playerPos.x - enemyPos.x) < 15 && abs(playerPos.y - enemyPos.y) < 15) {
		app->map->pathfinding->CreatePath(enemyPos, playerPos); // Calcula el camino desde la posicion del enemigo hacia la posicion del jugador
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
				isFacingLeft = true;
			}
			else {
				isFacingLeft = false;
			}

			isAttacking = false;
			attackAnim.Reset();

		}
	}

	
	return true;
}

float Enemy_Ols::GetHealth() const {
	printf("Initial health: %f\n", health);
	return health;

}

void Enemy_Ols::TakeDamage(float damage) {

	health -= damage;
	invulnerabilityTimer.Start();
	timerRecibirDanioColor.Start();
	app->audio->PlayRandomFx(ols_get_damage_fx, ols_get_damageAlt_fx, ols_get_damageAlt2_fx);
	printf("Enemy_Ols has received  %f damage\n", damage);
}

void Enemy_Ols::stateMachine(float dt, iPoint playerPos)
{
	//printf("\ncurrentState: %d, desiredState: %d", static_cast<int>(currentState), static_cast<int>(desiredState));
	nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;
	switch (nextState) {
	case EntityState_Enemy::IDLE:
		DoNothing(dt);
		break;
	case EntityState_Enemy::RUNNING:
		Chase(dt, playerPos);
		break;
	case EntityState_Enemy::ATTACKING:
		Attack(dt, playerPos);
		break;
	case EntityState_Enemy::DEAD:
		Die();
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
void Enemy_Ols::ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate) {
	this->poisonDamage = poisonDamage;
	this->poisonDuration = poisonDuration;
	this->poisonTickRate = poisonTickRate;
	
	this->poisoned = true;
	this->firstTimePoisonRecibed = true;

	poisonTimer.Start();
	poisonTickTimer.Start();


}

void Enemy_Ols::CheckPoison() {
	float epsilon = 0.1f; //Para margen de error

    // Aplicar el primer tick de daño inmediatamente (si no, el primer tick no se aplica en el segundo 0.0)
	if(firstTimePoisonRecibed) {
		if (currentState != EntityState_Enemy::DEAD) {
			health -= poisonDamage;
			invulnerabilityTimer.Start();
			timerRecibirDanioColor.Start();

			printf("Enemy_Osiris has received  %f damage of poison\n", poisonDamage);
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

                printf("Enemy_Osiris has received  %f damage of poison\n", poisonDamage);
            }
        }
    }
}
//VENENO ---------->

void Enemy_Ols::UpdateAttackSensor(float dt)
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