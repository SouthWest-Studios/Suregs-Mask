#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene_Testing.h"
#include "Scene_Mazmorra0.h"
#include "Log.h"
#include "InventoryManager.h"
#include "Point.h"
#include "Physics.h"
#include "Window.h"
#include "ModuleFadeToBlack.h"


#include "Fishing.h"


Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("player");

}

Player::~Player() {

}

bool Player::Awake() {



	return true;
}

bool Player::Start() {


	//position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	TSprite = config.attribute("Tsprite").as_int();
	SpriteX = config.attribute("sprite_x").as_int();
	SpriteY = config.attribute("sprite_y").as_int();
	Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

	idleAnim.LoadAnim("player", "idleAnim", spritePositions);
	runAnim.LoadAnim("player", "runAnim", spritePositions);


	texture = app->tex->Load(config.attribute("texturePath").as_string());

	pbody = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
	pbody->entity = this;
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	attackDamage = 50;

	//initialize audio effect
	pickCoinFxId = app->audio->LoadFx(config.attribute("coinfxpath").as_string());

	cdTimerDashMS = 500;

	player_Direction = Direction::UNKNOWN;
	return true;
}

bool Player::Update(float dt)
{
	nextState = EntityState::IDLE;

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godmode = !godmode;
		pbody->body->GetFixtureList()[0].SetSensor(godmode);
	}

	if (godmode) { GodMode(dt); }
	else PlayerMovement(dt);

	CameraMovement(dt);




	//printf("\nposx:%d, posy: %d",position.x, position.y);

	switch (nextState) {
	case EntityState::RUNNING:
		Run(dt);
		break;
	case EntityState::ATTACKING:
		Attack(dt);
		break;
	case EntityState::IDLE:
		DoNothing(dt);
		break;
	case EntityState::MASK_ATTACK:
		MaskAttack(dt);
		break;
	default:
		break;
	}
	//switch (state)
	//{
	//case EntityState::IDLE:
	//	currentAnimation = &idleAnim;
	//	break;
	//case EntityState::RUNNING:
	//	currentAnimation = &runAnim;
	//	break;
	//case EntityState::ATTACKING:
	//	//currentAnimation = &attackAnim;
	//	break;
	//default:
	//	//currentAnimation = &idleAnim;
	//	break;
	//}

	currentAnimation->Update();
	return true;
}


bool Player::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();


	if (isFacingLeft) {
		app->render->DrawTexture(texture, position.x - 50, position.y - 200, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 50, position.y - 200, SDL_FLIP_NONE, &rect);
	}
	return true;
}


bool Player::CleanUp()
{
	app->physics->GetWorld()->DestroyBody(pbody->body);
	SDL_DestroyTexture(texture);
	return true;
}

void Player::DoNothing(float dt)
{
	currentAnimation = &idleAnim;
	//printf("idle");
}

void Player::Run(float dt)
{
	//printf("runn");

	currentAnimation = &runAnim;
}

void Player::Attack(float dt)
{
 	//printf("attack");

    int attackWidth = 50; 
    int attackHeight = 50; 

    // Dirección del ataque
    int attackX = position.x + lastMovementDirection.x * attackWidth;
    int attackY = position.y + lastMovementDirection.y * attackHeight;

    //Sensor
    attackSensor = app->physics->CreateRectangleSensor(attackX, attackY, attackWidth, attackHeight, DYNAMIC);
    attackSensor->ctype = ColliderType::PLAYER_ATTACK;
    attackSensor->listener = this;
}

void Player::ChangeMask() {
    if (currentMask == Mask::MASK0) {
        currentMask = Mask::MASK1;
    } else {
        currentMask = Mask::MASK0;
    }
	printf("Mask changed to %d\n", currentMask);
}
void Player::MaskAttack(float dt)
{
	switch (currentMask) {
        case Mask::MASK0:
            CastLightning();
            break;
        case Mask::MASK1:
            AreaAttack(dt);
            break;
    }
}

//Rango ataque mascara 0
Entity* Player::GetEnemyWithHighestHealthWithinRadius(iPoint position, int radius) {
	Entity* highestHealthEnemy = nullptr;
	int highestHealth = 0;
	for (Entity* enemy : app->entityManager->GetEnemies()) {
		int dx = position.x - enemy->position.x;
		int dy = position.y - enemy->position.y;
		if (dx * dx + dy * dy <= radius * radius) {
			printf("Considering enemy at (%d, %d) with health %f\n", enemy->position.x, enemy->position.y, enemy->GetHealth());
			if (enemy->GetHealth() > highestHealth) {
				printf("Selected as highest health enemy so far\n");
				highestHealthEnemy = enemy;
				highestHealth = enemy->GetHealth();
			}
		}
	}
	return highestHealthEnemy;
}

//Ataque mascara 0
void Player::CastLightning() {
    Entity* target = GetEnemyWithHighestHealthWithinRadius(position, 500);
    if (target != nullptr) {
        printf("Enemy hit: %p at position (%d, %d)\n", (void*)target, target->position.x, target->position.y);
        target->TakeDamage(attackDamage);
    } else {
        printf("No enemy alive in range to attack\n");
    }
}

//Ataque mascara 1

void Player::AreaAttack(float dt) {
    mask1AttackSensor = app->physics->CreateRectangleSensor(this->position.x, this->position.y, attackMask1Width, attackMask1Height, DYNAMIC);
	mask1AttackSensor->ctype = ColliderType::MASK1_ATTACK;
	mask1AttackSensor->listener = this;
}

// L07 DONE 6: Define OnCollision function for the player. 
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::ENEMY:
		if (physA == attackSensor) {
			LOG("Collision ENEMY");
			if (physB->entity != nullptr) {
				physB->entity->TakeDamage(attackDamage);
			}
		}
		if (physA == mask1AttackSensor) {
			LOG("Collision ENEMY");
			if (physB->entity != nullptr) {
				physB->entity->TakeDamage(attackDamage);
			}
		}
	break;
	case ColliderType::RESOURCE_ESPADA:
		LOG("Collision ESPADA");
		if (physB->listener->active) {
			app->inventoryManager->CreateItem(physB->listener->type, 0, 0, 0, 0, 0, 0);
			physB->listener->active = false;
			app->entityManager->DestroyEntity(physB->listener);
			physB->body->SetActive(false);
		}

		break;
	case ColliderType::RESOURCE_DIAMANTE:
		LOG("Collision DIAMANTE");
		if (physB->listener->active) {
			app->inventoryManager->CreateItem(physB->listener->type, 0, 0, 0, 0, 0, 0);
			physB->listener->active = false;
			app->entityManager->DestroyEntity(physB->listener);
			physB->body->SetActive(false);
		}

		break;

	case ColliderType::ENTRADA_MAZMORRA0:
		LOG("Collision ENTRADA_MAZMORRA0");
		app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra0);
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}

void Player::CameraMovement(float dt)
{
	uint windowH;
	uint windowW;
	app->win->GetWindowSize(windowW, windowH);

	int targetPosX = (-position.x * app->win->GetScale() + (windowW / 2) - 10);
	int targetPosY = (-position.y * app->win->GetScale() + (windowH / 2) - 10) + 100;
	
	if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		camaralibre = !camaralibre; // Esto alternar?el valor de camaralibre cada vez que se presione la tecla C en modo debug
	}
	if (camaralibre == true)
	{
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			app->render->camera.y += (int)ceil(1 * dt);

		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			app->render->camera.y -= (int)ceil(1 * dt);

		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			app->render->camera.x += (int)ceil(1 * dt);

		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			app->render->camera.x -= (int)ceil(1 * dt);
	}
	else
	{
		if (app->fadeToBlack->currentStep != 0) {
			app->render->camera.x = lerp(app->render->camera.x, targetPosX, 1);
			app->render->camera.y = lerp(app->render->camera.y, targetPosY, 1);
		}
		else {
			app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
			app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
		}
		
	}
	

}

void Player::GodMode(float dt)
{
	float speedFast;
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		speedFast = speed * 2;
	}
	else {
		speedFast = speed;
	}


	b2Vec2 velocity = b2Vec2(0, 0);
	pbody->body->SetLinearVelocity(velocity);

	//Moverse a la izquierda
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		velocity.y += -speedFast * dt;
		nextState = EntityState::RUNNING;
		isFacingLeft = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		velocity.y += speedFast * dt;
		nextState = EntityState::RUNNING;
		isFacingLeft = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = -speedFast * dt;
		nextState = EntityState::RUNNING;
		isFacingLeft = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = speedFast * dt;
		nextState = EntityState::RUNNING;
		isFacingLeft = false;
	}
	pbody->body->SetLinearVelocity(velocity);
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;

}

void Player::PlayerMovement(float dt)
{


	b2Vec2 velocity = b2Vec2(0, 0);

	// Obtener teclado
	pressingUp = app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT;
	pressingDown = app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT;
	pressingLeft = app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT;
	pressingRight = app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT;

	// Calcular la velocidad horizontal y vertical
	int horizontalMovement = pressingRight - pressingLeft;
	int verticalMovement = pressingDown - pressingUp;

	//printf("%d", pressingUp);
	// Actualizar velocidad
	if (!isDashing) {
		velocity.x = horizontalMovement * 0.2 * dt;
		velocity.y = verticalMovement * 0.2 * dt;

		// Si hay entrada de movimiento, actualizar estado y direcci��n.

		if (horizontalMovement != 0 || verticalMovement != 0) {
			nextState = EntityState::RUNNING;
			isFacingLeft = (horizontalMovement < 0);
			lastMovementDirection = fPoint(horizontalMovement, verticalMovement);
		}
	}

	
	FishingDirecction(verticalMovement, horizontalMovement);

	//Si pulsas espacio
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && timerDash.ReadMSec() > cdTimerDashMS) {

		isDashing = true;
		timerDash.Start();
		pbody->body->ApplyForce(b2Vec2(velocity.x * 100, velocity.y * 100), pbody->body->GetWorldCenter(), false);
	}

	if (!(timerDash.ReadMSec() < speed * 1000 && isDashing)) {
		isDashing = false;
		pbody->body->SetLinearVelocity(velocity);
	}

	//Si pulsas J para atacar
	if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN && timerAttack.ReadMSec() > cdTimerAttackMS + 100){
		isAttacking = true;
		timerAttack.Start();
		nextState = EntityState::ATTACKING;
	}

	if (!(timerAttack.ReadMSec() < cdTimerAttackMS && isAttacking)) {
		isAttacking = false;
		if (attackSensor) {
			app->physics->DestroyBody(attackSensor);
			attackSensor = nullptr;
		}
	}

	//Si pulsas K para mascara principal

	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN && timerMaskAttack.ReadMSec() > cdTimerMaskAttackMS){
		isAttackingMask = true;
		timerMaskAttack.Start();
		nextState = EntityState::MASK_ATTACK;
	}

	if (!(timerMaskAttack.ReadMSec() < cdTimerMaskAttackMS && isAttackingMask)) {
		isAttackingMask = false;
		if(mask1AttackSensor){
			app->physics->DestroyBody(mask1AttackSensor);
			mask1AttackSensor = nullptr;
		}
	}

	//Si pulsas Q para cambiar de mascara
	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {
		ChangeMask();
	}

	
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;
}
void Player::FishingDirecction(bool verticalMovement, bool horizontalMovement)
{
	

	// Si jugador Mueve
	if (pressingRight) {
		// Derecha
		player_Direction = Direction::RIGHT;
		if (app->scene_testing->GetRod()->fishing.isFishing) {
			playermove = true;
			app->scene_testing->GetRod()->fishing.startFishing = false;
		}
		else {
			playermove = false;
		}
	}
	else if (pressingLeft) {
		// izquierda
		player_Direction = Direction::LEFT;
		if (app->scene_testing->GetRod()->fishing.isFishing) {
			playermove = true;
			app->scene_testing->GetRod()->fishing.startFishing = false;
		}
		else {
			playermove = false;
		}
	}
	else if (pressingDown) {
		// abajo
		player_Direction = Direction::DOWN;
		if (app->scene_testing->GetRod()->fishing.isFishing) {
			playermove = true;
			app->scene_testing->GetRod()->fishing.startFishing = false;
		}
		else {
			playermove = false;
		}
	}
	else if (pressingUp) {
		// arriba
		player_Direction = Direction::UP;
		if (app->scene_testing->GetRod()->fishing.isFishing) {
			playermove = true;
			app->scene_testing->GetRod()->fishing.startFishing = false;
		}
		else {
			playermove = false;
		}
	}

}
//
//void Player::Rodar(float dt)
//{
//	b2Vec2 velocity = b2Vec2(0, 0);
//
//
//	rodar_PlayerPosition = lerp(currentPosX, rodar_PotisionX, 0.1f);
//	printf("\nlerp: %d", rodar_PlayerPosition);
//	velocity.x = rodar_PlayerPosition;
//
//	pbody->body->SetLinearVelocity(velocity);
//	b2Transform pbodyPos = pbody->body->GetTransform();
//	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
//	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;
//}


