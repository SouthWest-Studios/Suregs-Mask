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




Enemy_Ols::Enemy_Ols() : Entity(EntityType::ENEMY_OLS), maxHealth(350.0f), health(350.0f), speed(1.0f), attackDamage(50.0f){
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

	pbody = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
	pbody->entity = this;
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;

	attackDamage = 10;


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

	switch (nextState) {
	case EntityState::DEAD:
		Die(dt);
		break;
	case EntityState::ATTACKING:
		Attack(dt);
		break;
	case EntityState::RUNNING:
		Chase(dt);
		break;
	case EntityState::IDLE:
		DoNothing(dt);
		break;
	default:
		break;
	}

	if (health <= 0) {
		nextState = EntityState::DEAD;
		
	}
	else if (app->map->pathfinding->GetDistance(app->entityManager->GetPlayer()->position, position) <= 20 /*Cambiar*/) {
		nextState = EntityState::ATTACKING;
	}
	else if (app->map->pathfinding->GetDistance(app->entityManager->GetPlayer()->position, position) <= 400/*Cambiar*/) {
		nextState = EntityState::RUNNING;
	}
	else {
		nextState = EntityState::IDLE;
	}


	currentAnimation->Update();

	return true;
}


bool Enemy_Ols::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();



	if (isFacingLeft) {
		app->render->DrawTexture(texture, position.x - 120, position.y - 60, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 120, position.y - 60, SDL_FLIP_NONE, &rect);
	}

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 16;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 16;
	return true;
}


bool Enemy_Ols::CleanUp()
{
	app->entityManager->DestroyEntity(pbody->entity);
	app->physics->DestroyBody(pbody);
	app->tex->UnLoad(texture);

	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}

void Enemy_Ols::DoNothing(float dt)
{
	currentAnimation = &idleAnim;
	//printf("Osiris idle");
	pbody->body->SetLinearVelocity(b2Vec2_zero);
}

void Enemy_Ols::Chase(float dt)
{
	//printf("Osiris chasing");
	currentAnimation = &runAnim;
	Olsfinding(dt);
}

void Enemy_Ols::Attack(float dt)
{
	//printf("Osiris attacking");
	currentAnimation = &attackAnim;
	//sonido ataque
}

void Enemy_Ols::Die(float dt) {

	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	if (parseResult) {
		configNode = configFile.child("config");
	}

	garra = (Item_Garra*)app->entityManager->CreateEntity(EntityType::ITEM_GARRA);
	garra->config = configNode.child("entities_data").child("item_garra");
	garra->position = iPoint(position.x, position.y);
	garra->Start();

	app->entityManager->DestroyEntity(this);
	app->physics->GetWorld()->DestroyBody(pbody->body);
	app->tex->UnLoad(texture);
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
			pbody->body->SetLinearVelocity(velocity);

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
	printf("Enemy_Ols has received  %f damage\n", damage);
}


