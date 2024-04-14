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
#include "Physics.h"




Enemy_Ols::Enemy_Ols() : Entity(EntityType::ENEMY_OLS)
{
	name.Create("ols");

}

Enemy_Ols::~Enemy_Ols() {

}

bool Enemy_Ols::Awake() {

	return true;
}

bool Enemy_Ols::Start() {


	//position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	TSprite = config.attribute("Tsprite").as_int();
	SpriteX = config.attribute("sprite_x").as_int();
	SpriteY = config.attribute("sprite_y").as_int();
	Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

	idleAnim.LoadAnim("ols", "idleAnim", spritePositions);
	runAnim.LoadAnim("ols", "runAnim", spritePositions);
	attackAnim.LoadAnim("ols", "attackAnim", spritePositions);
	dieAnim.LoadAnim("ols", "dieAnim", spritePositions);

	texture = app->tex->Load(config.attribute("texturePath").as_string());

	pbody = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
	pbody->entity = this;
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;


	//Estadisticas
	health = 100.0f;
	maxHealth = 100.0f;
	speed = 0.2f;
	attackDamage = 50;

	return true;
}

bool Enemy_Ols::Update(float dt)
{
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


	if (nextState == EntityState::DEAD)
	{
		return true;
	}

	if (health <= 0) {
		nextState = EntityState::DEAD;
	}
	else if (app->map->pathfinding->GetDistance(app->entityManager->GetPlayer()->position, position) <= 20 /*Cambiar*/) {
		nextState = EntityState::ATTACKING;
	}
	else if (app->map->pathfinding->GetDistance(app->entityManager->GetPlayer()->position, position) <= 120/*Cambiar*/) {
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
	SDL_DestroyTexture(texture);
	return true;
}

void Enemy_Ols::DoNothing(float dt)
{
	currentAnimation = &idleAnim;
	//printf("Osiris idle");
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
	app->entityManager->DestroyEntity(this);
	app->physics->GetWorld()->DestroyBody(pbody->body);
	SDL_DestroyTexture(texture);
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
		nextState = EntityState::DEAD;
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
	if (app->map->pathfinding->GetDistance(app->entityManager->GetPlayer()->position, position) <= 120) {

		iPoint playerPos = app->map->WorldToMap(app->entityManager->GetPlayer()->position.x, app->entityManager->GetPlayer()->position.y);
		playerPos.x += 1;
		playerPos.y += 1;
		iPoint enemyPos = app->map->WorldToMap(position.x, position.y);
		enemyPos.y += 1;

		app->map->pathfinding->CreatePath(playerPos, enemyPos);
		lastPath = *app->map->pathfinding->GetLastPath();

		//Get the latest calculated path and draw
		for (uint i = 0; i < lastPath.Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(lastPath.At(i)->x, lastPath.At(i)->y);
			if (app->physics->debug == true) {
				app->render->DrawTexture(app->map->pathfinding->mouseTileTex, pos.x, pos.y, SDL_FLIP_NONE);
			}
		}

		if (lastPath.Count() > 2) {
			if (lastPath.At(lastPath.Count() - 2)->x < enemyPos.x) {
				vel.x -= speed * dt;
				isFacingLeft = true;
			}

			if (lastPath.At(lastPath.Count() - 2)->x > enemyPos.x) {
				vel.x += speed * dt;
				isFacingLeft = false;
			}

			isAttacking = false;
			attackAnim.Reset();
			pbody->body->SetLinearVelocity(vel);
		}

		if (app->map->pathfinding->GetDistance(app->entityManager->GetPlayer()->position, position) <= 66) {

			if (isFacingLeft) {
				vel.x -= speed * dt;
			}
			else {
				vel.x += speed * dt;

			}
			isAttacking = true;
			pbody->body->SetLinearVelocity(vel);
		}

	}
	return true;
}



