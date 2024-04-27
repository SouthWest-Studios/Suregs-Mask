#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene_Testing.h"
#include "Scene_Pueblo.h"
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

	// Inicializa las estadísticas base
	baseStats.maxHealth = 100;
	baseStats.currentHealth = 100;
	baseStats.movementSpeed = 100;
	baseStats.attackSpeed = 100;
	baseStats.attackDamage = 100;

	// Inicializa las estadísticas actuales a las estadísticas base
	currentStats = baseStats;

	// Estadísticas de la máscara 0 a nivel 0
	maskStats[Mask::MASK0][0].maxHealthModifier = 0;
	maskStats[Mask::MASK0][0].movementSpeedModifier = -10;
	maskStats[Mask::MASK0][0].attackDamageModifier = 10;
	maskStats[Mask::MASK0][0].attackSpeedModifier = 10;
	maskStats[Mask::MASK0][0].maskDamage = 50;
	maskStats[Mask::MASK0][0].maskCoolDown = 25000; //En Milisegundos
	maskStats[Mask::MASK0][0].firstTimeUsed = false;
	

	// maskStats[static_cast<int>(Mask::MASK0)].maxHealthModifier = 0;
	// maskStats[static_cast<int>(Mask::MASK0)].movementSpeedModifier = -10;
	// maskStats[static_cast<int>(Mask::MASK0)].attackDamageModifier = 10;
	// maskStats[static_cast<int>(Mask::MASK0)].attackSpeedModifier = 10;
	// maskStats[static_cast<int>(Mask::MASK0)].maskDamage = 50;
	// maskStats[static_cast<int>(Mask::MASK0)].maskCoolDown = 25000; //En Milisegundos
	// maskStats[static_cast<int>(Mask::MASK0)].firstTimeUsed = false;

	//Estadísticas de la máscara 0 a nivel 1

	maskStats[Mask::MASK0][1].maxHealthModifier = 0;
	maskStats[Mask::MASK0][1].movementSpeedModifier = -10;
	maskStats[Mask::MASK0][1].attackDamageModifier = 10;
	maskStats[Mask::MASK0][1].attackSpeedModifier = 10;
	maskStats[Mask::MASK0][1].maskDamage = 80;
	maskStats[Mask::MASK0][1].maskCoolDown = 23000; //En Milisegundos
	maskStats[Mask::MASK0][1].firstTimeUsed = false;

	//Estadísticas de la máscara 0 a nivel 2
	
	maskStats[Mask::MASK0][2].maxHealthModifier = 0;
	maskStats[Mask::MASK0][2].movementSpeedModifier = -10;
	maskStats[Mask::MASK0][2].attackDamageModifier = 10;
	maskStats[Mask::MASK0][2].attackSpeedModifier = 10;
	maskStats[Mask::MASK0][2].maskDamage = 130;
	maskStats[Mask::MASK0][2].maskCoolDown = 20000; //En Milisegundos
	maskStats[Mask::MASK0][2].firstTimeUsed = false;

	//Estadísticas de la máscara 0 a nivel 3
	
	maskStats[Mask::MASK0][3].maxHealthModifier = 0;
	maskStats[Mask::MASK0][3].movementSpeedModifier = -10;
	maskStats[Mask::MASK0][3].attackDamageModifier = 10;
	maskStats[Mask::MASK0][3].attackSpeedModifier = 10;
	maskStats[Mask::MASK0][3].maskDamage = 200;
	maskStats[Mask::MASK0][3].maskCoolDown = 17000; //En Milisegundos
	maskStats[Mask::MASK0][3].firstTimeUsed = false;

	//Estadísticas de la máscara 0 a nivel 4
	
	maskStats[Mask::MASK0][4].maxHealthModifier = 0;
	maskStats[Mask::MASK0][4].movementSpeedModifier = -10;
	maskStats[Mask::MASK0][4].attackDamageModifier = 10;
	maskStats[Mask::MASK0][4].attackSpeedModifier = 10;
	maskStats[Mask::MASK0][4].maskDamage = 300;
	maskStats[Mask::MASK0][4].maskCoolDown = 15000; //En Milisegundos
	maskStats[Mask::MASK0][4].firstTimeUsed = false;


	// Estadísticas de la máscara 1 a nivel 0
	maskStats[Mask::MASK1][0].maxHealthModifier = -10;
	maskStats[Mask::MASK1][0].movementSpeedModifier = 10;
	maskStats[Mask::MASK1][0].attackDamageModifier = 10;
	maskStats[Mask::MASK1][0].attackSpeedModifier = 0;
	maskStats[Mask::MASK1][0].maskDamage = 100;
	maskStats[Mask::MASK1][0].maskCoolDown = 30000; //En Milisegundos
	maskStats[Mask::MASK1][0].firstTimeUsed = false;

	// maskStats[static_cast<int>(Mask::MASK1)].maxHealthModifier = -10;
	// maskStats[static_cast<int>(Mask::MASK1)].movementSpeedModifier = 10;
	// maskStats[static_cast<int>(Mask::MASK1)].attackDamageModifier = 10;
	// maskStats[static_cast<int>(Mask::MASK2)].attackSpeedModifier = 0;
	// maskStats[static_cast<int>(Mask::MASK1)].maskDamage = 100;
	// maskStats[static_cast<int>(Mask::MASK1)].maskCoolDown = 30000; //En Milisegundos
	// maskStats[static_cast<int>(Mask::MASK1)].firstTimeUsed = false;

	// Estadísticas de la máscara 1 a nivel 1

	maskStats[Mask::MASK1][1].maxHealthModifier = -10;
	maskStats[Mask::MASK1][1].movementSpeedModifier = 10;
	maskStats[Mask::MASK1][1].attackDamageModifier = 10;
	maskStats[Mask::MASK1][1].attackSpeedModifier = 0;
	maskStats[Mask::MASK1][1].maskDamage = 120;
	maskStats[Mask::MASK1][1].maskCoolDown = 28000; //En Milisegundos
	maskStats[Mask::MASK1][1].firstTimeUsed = false;

	// Estadísticas de la máscara 1 a nivel 2
	
	maskStats[Mask::MASK1][1].maxHealthModifier = -10;
	maskStats[Mask::MASK1][1].movementSpeedModifier = 10;
	maskStats[Mask::MASK1][1].attackDamageModifier = 10;
	maskStats[Mask::MASK1][1].attackSpeedModifier = 0;
	maskStats[Mask::MASK1][1].maskDamage = 180;
	maskStats[Mask::MASK1][1].maskCoolDown = 25000; //En Milisegundos
	maskStats[Mask::MASK1][1].firstTimeUsed = false;

	// Estadísticas de la máscara 1 a nivel 3
	
	maskStats[Mask::MASK1][1].maxHealthModifier = -10;
	maskStats[Mask::MASK1][1].movementSpeedModifier = 10;
	maskStats[Mask::MASK1][1].attackDamageModifier = 10;
	maskStats[Mask::MASK1][1].attackSpeedModifier = 0;
	maskStats[Mask::MASK1][1].maskDamage = 250;
	maskStats[Mask::MASK1][1].maskCoolDown = 23000; //En Milisegundos
	maskStats[Mask::MASK1][1].firstTimeUsed = false;

	// Estadísticas de la máscara 1 a nivel 4
	
	maskStats[Mask::MASK1][1].maxHealthModifier = -10;
	maskStats[Mask::MASK1][1].movementSpeedModifier = 10;
	maskStats[Mask::MASK1][1].attackDamageModifier = 10;
	maskStats[Mask::MASK1][1].attackSpeedModifier = 0;
	maskStats[Mask::MASK1][1].maskDamage = 400;
	maskStats[Mask::MASK1][1].maskCoolDown = 20000; //En Milisegundos
	maskStats[Mask::MASK1][1].firstTimeUsed = false;


	//Estadísticas de la máscara 2 a nivel 0
	maskStats[Mask::MASK2][0].maxHealthModifier = -20;
	maskStats[Mask::MASK2][0].movementSpeedModifier = 10;
	maskStats[Mask::MASK2][0].attackDamageModifier = 10;
	maskStats[Mask::MASK2][0].attackSpeedModifier = 10;

	// maskStats[static_cast<int>(Mask::MASK2)].maxHealthModifier = -20;
	// maskStats[static_cast<int>(Mask::MASK2)].movementSpeedModifier = 10;
	// maskStats[static_cast<int>(Mask::MASK2)].attackDamageModifier = 10;
	// maskStats[static_cast<int>(Mask::MASK2)].attackSpeedModifier = 10;

	//Estadísticas de la máscara 3 a nivel 0
	maskStats[Mask::MASK3][0].maxHealthModifier = -5;
	maskStats[Mask::MASK3][0].movementSpeedModifier = -5;
	maskStats[Mask::MASK3][0].attackDamageModifier = -5;
	maskStats[Mask::MASK3][0].attackSpeedModifier = -5;

	// maskStats[static_cast<int>(Mask::MASK3)].maxHealthModifier = -5;
	// maskStats[static_cast<int>(Mask::MASK3)].movementSpeedModifier = -5;
	// maskStats[static_cast<int>(Mask::MASK3)].attackDamageModifier = -5;
	// maskStats[static_cast<int>(Mask::MASK3)].attackSpeedModifier = -5;

	//Estadísticas pasivas mascara 0 a nivel 0
	passiveStats[Mask::MASK0][0].damageBoost = 20;
	passiveStats[Mask::MASK0][0].rangeBoost = 0;
	passiveStats[Mask::MASK0][0].dashBoost = 0;

	// passiveStats[static_cast<int>(Mask::MASK0)].damageBoost = 20;
	// passiveStats[static_cast<int>(Mask::MASK0)].rangeBoost = 0;
	// passiveStats[static_cast<int>(Mask::MASK0)].dashBoost = 0;

	//Estadísticas pasivas mascara 1 a nivel 0
	passiveStats[Mask::MASK1][0].damageBoost = 0.33;
	passiveStats[Mask::MASK1][0].rangeBoost = 25;
	passiveStats[Mask::MASK1][0].dashBoost = 0;

	// passiveStats[static_cast<int>(Mask::MASK1)].damageBoost = 0.33;
	// passiveStats[static_cast<int>(Mask::MASK1)].rangeBoost = 25;
	// passiveStats[static_cast<int>(Mask::MASK1)].dashBoost = 0;

	//Estadísticas pasivas mascara 2
	passiveStats[Mask::MASK2][0].damageBoost = 0;
	passiveStats[Mask::MASK2][0].rangeBoost = 0;
	passiveStats[Mask::MASK2][0].dashBoost = 2;

	// passiveStats[static_cast<int>(Mask::MASK2)].damageBoost = 0;
	// passiveStats[static_cast<int>(Mask::MASK2)].rangeBoost = 0;
	// passiveStats[static_cast<int>(Mask::MASK2)].dashBoost = 2;

	//Estadísticas pasivas mascara 3
	passiveStats[Mask::MASK3][0].damageBoost = 20;
	passiveStats[Mask::MASK3][0].rangeBoost = 0;
	passiveStats[Mask::MASK3][0].dashBoost = 0;

	// passiveStats[static_cast<int>(Mask::MASK3)].damageBoost = 20;
	// passiveStats[static_cast<int>(Mask::MASK3)].rangeBoost = 0;
	// passiveStats[static_cast<int>(Mask::MASK3)].dashBoost = 0;

	// Máscaras a NOMASK
	primaryMask = Mask::NOMASK;
	secondaryMask = Mask::NOMASK;


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
	dashiAnim.LoadAnim("player", "dashiAnim", spritePositions);


	texture = app->tex->Load(config.attribute("texturePath").as_string());
	app->entityManager->objectsToDraw.push_back({
		texture,
		position.x, // x
		position.y, // y
		position.y + 236, // anchorY
		300, // width
		250, // height
		NULL, // currentFrame
		isFacingLeft
	});

	pbodyFoot = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
	pbodyFoot->entity = this;
	pbodyFoot->listener = this;
	pbodyFoot->ctype = ColliderType::PLAYER;

	pbodySensor = app->physics->CreateRectangleSensor(position.x, position.y, 20, 60, bodyType::DYNAMIC);
	pbodySensor->entity = this;
	pbodySensor->listener = this;
	pbodySensor->ctype = ColliderType::UNKNOWN;

	//initialize audio effect
	run_fx = app->audio->LoadAudioFx("runAlt_fx");
	dash_fx = app->audio->LoadAudioFx("dash_fx");

	cdTimerDashMS = 500;
	cdTimerAttackMS = 100000 / currentStats.attackSpeed;

	player_Direction = Direction::UNKNOWN;


	EquipPrimaryMask(Mask::NOMASK);
	EquipSecondaryMask(Mask::MASK1);

	return true;
}

bool Player::Update(float dt)
{
	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
	pbodySensor->body->SetTransform(b2Vec2(pbodyPos.p.x, pbodyPos.p.y - 1), 0);



	if (!inAnimation) {
		nextState = EntityState::IDLE;
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godmode = !godmode;
		pbodyFoot->body->GetFixtureList()[0].SetSensor(godmode);
	}

	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "dashiAnim") {
		inAnimation = false;
		dashiAnim.Reset();
	}

	if (godmode) { GodMode(dt); }
	else PlayerMovement(dt);

	CameraMovement(dt);

	//printf("\nposx:%d, posy: %d",position.x, position.y);

	



	//if (isDashing) {
	//	//printf("dashi");
	//	printf("\nAnimationName: %s:", currentAnimation->getNameAnimation());
	//	currentAnimation = &dashiAnim;
	//}

	switch (nextState) {
	case EntityState::RUNNING:
		Run(dt);
		app->audio->PlayFx(run_fx);
		break;
	case EntityState::ATTACKING:
		Attack(dt);
		break;
	case EntityState::IDLE:
		DoNothing(dt);
		app->audio->StopFx(-1);
		break;
	case EntityState::DASHI:
		if (isDashing) {
			Dashi(dt);
		}
		break;
	case EntityState::MASK_ATTACK:
		MaskAttack(dt);
		break;
	default:
		break;
	}


	currentAnimation->Update();
	return true;
}


bool Player::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	printf("Posición: (%d, %d)\n", position.x, position.y);

	if (isFacingLeft) {
		//app->render->DrawTexture(texture, position.x - 50, position.y - 200, SDL_FLIP_HORIZONTAL, &rect);
		//app->render->DrawTexture(texture, position.x - 75, position.y - 100, 0.5f, SDL_FLIP_NONE, &rect);
		
	}
	else {
		//app->render->DrawTexture(texture, position.x - 75, position.y - 100, 0.5f, SDL_FLIP_HORIZONTAL, &rect);
		
	}
	return true;
}


bool Player::CleanUp()
{
	app->entityManager->DestroyEntity(pbodyFoot->entity);
	app->physics->GetWorld()->DestroyBody(pbodyFoot->body);
	app->physics->GetWorld()->DestroyBody(pbodySensor->body);
	/*app->tex->UnLoad(texture);*/
	app->tex->UnLoad(texture);

	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}

void Player::DoNothing(float dt)
{
	currentAnimation = &idleAnim;
	//printf("idle");
}

float Player::GetRealMovementSpeed() const {
	return 0.2f * (currentStats.movementSpeed / 100.0f);
}

void Player::Run(float dt)
{
	//printf("runn");

	currentAnimation = &runAnim;
}

void Player::Dashi(float dt)
{
	//printf("runn");
	currentAnimation = &dashiAnim;
}
void Player::Attack(float dt)
{
	//printf("attack"); 

	// Dirección del ataque
	int attackX = position.x + lastMovementDirection.x * attackWidth;
	int attackY = position.y + lastMovementDirection.y * attackHeight;

	//Sensor
	attackSensor = app->physics->CreateRectangleSensor(attackX, attackY, attackWidth, attackHeight, DYNAMIC);
	attackSensor->ctype = ColliderType::PLAYER_ATTACK;
	attackSensor->listener = this;

	//Onda expansiva ataque pasivo mascara 1
	if (secondaryMask == Mask::MASK1) {
		mask1PassiveSensor = app->physics->CreateRectangleSensor(attackX, attackY, 100, 100, DYNAMIC);
		mask1PassiveSensor->ctype = ColliderType::MASK0_PASSIVE_ATTACK;
		mask1PassiveSensor->listener = this;
	}
}

void Player::UnequipMasks() {
	currentStats = baseStats;
}

void Player::EquipPrimaryMask(Mask mask) {
	primaryMask = mask;

	// Si la máscara no es NOMASK, aplica las modificaciones de estadísticas
	 if (mask != Mask::NOMASK) {
        currentStats.maxHealth = baseStats.maxHealth * (1 + maskStats[mask][maskLevels[mask]].maxHealthModifier / 100);
        currentStats.movementSpeed = baseStats.movementSpeed * (1 + maskStats[mask][maskLevels[mask]].movementSpeedModifier / 100);
        currentStats.attackSpeed = baseStats.attackSpeed * (1 + maskStats[mask][maskLevels[mask]].attackSpeedModifier / 100);
        currentStats.attackDamage = baseStats.attackDamage * (1 + maskStats[mask][maskLevels[mask]].attackDamageModifier / 100);
    }
}

void Player::EquipSecondaryMask(Mask mask) {
	secondaryMask = mask;

	// Si la máscara no es NOMASK, aplica las modificaciones de estadísticas
    if (mask != Mask::NOMASK) {
        currentStats.attackDamage = baseStats.attackDamage * (1 + passiveStats[mask][maskLevels[mask]].damageBoost / 100);
    }
}



Mask* Player::GetPrimaryMask()
{
	return &primaryMask;
}

Mask* Player::GetSecondaryMask()
{
	return &secondaryMask;
}

MaskStats* Player::GetMaskStats(Mask mask) {
    return &maskStats[mask][maskLevels[mask]];
}

void Player::ChangeMask() {
	if (timerChangeMask.ReadMSec() > changeMaskCooldown) {

		Mask temp = primaryMask;
		primaryMask = secondaryMask;
		secondaryMask = temp;

		UnequipMasks();

		EquipPrimaryMask(primaryMask);
		EquipSecondaryMask(secondaryMask);

		cdTimerAttackMS = 100000 / currentStats.attackSpeed;

		timerChangeMask.Start();

		printf("Player primary mask after mask change: %d\n", (static_cast<int>(primaryMask) - 1));
		printf("Player secondary mask after mask change: %d\n", (static_cast<int>(secondaryMask) - 1));
		printf("Player stats after mask change:\n");
		printf("Max Health: %f\n", currentStats.maxHealth);
		printf("Movement Speed: %f\n", currentStats.movementSpeed);
		printf("Attack Speed: %f\n", currentStats.attackSpeed);
		printf("Attack Damage: %f\n", currentStats.attackDamage);
	}
}

void Player::MaskAttack(float dt)
{
	switch (primaryMask) {
	case Mask::NOMASK:
		//No hace nada
		break;
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
        target->TakeDamage(maskStats[primaryMask][maskLevels[primaryMask]].maskDamage);
	}
	else {
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
		break;
	case ColliderType::ENEMY:
		if (physA == attackSensor) {
			LOG("Collision ENEMY");
			if (physB->entity != nullptr && basicAttackDealed == false) {
				physB->entity->TakeDamage(currentStats.attackDamage);
				basicAttackDealed = true;
			}
			//collisionAttackTimer.Start();
		}
		if (physA == mask1PassiveSensor) {
			LOG("Collision ENEMY");
			if (physB->entity != nullptr && mask1PassiveAttackDealed == false) {
				physB->entity->TakeDamage(currentStats.attackDamage * passiveStats[secondaryMask][maskLevels[secondaryMask]].damageBoost);
				mask1PassiveAttackDealed = true;
			}
		}
		if (physA == mask1AttackSensor) {
			LOG("Collision ENEMY");
			if (physB->entity != nullptr && mask1PassiveAttackDealed == false) {
				physB->entity->TakeDamage(maskStats[primaryMask][maskLevels[primaryMask]].maskDamage);
				mask1PassiveAttackDealed = true;
			}
			//collisionMask1Timer.Start();
		}
		if (physA == pbodyFoot) {
			//TakeDamage(physB->entity->attackDamage);
			if (!godmode) {
				TakeDamage(10);
				if (currentStats.currentHealth <= 0) {
					app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_pueblo);
				}
			}
		}
		break;
	case ColliderType::RESOURCE:
		LOG("Collision RESOURCE");
		if (physA != attackSensor && physA != mask1PassiveSensor && physA != mask1AttackSensor)
		{
			if (physB->listener->active) {
				if (app->inventoryManager->IsFull() == false)
				{
					app->inventoryManager->CreateItem(physB->listener->type, physB->listener->description, physB->listener->tipo);
					physB->listener->active = false;
					app->entityManager->DestroyEntity(physB->listener);
					physB->body->SetActive(false);
				}
				
			}
		}
		break;
	

	case ColliderType::ENTRADA_MAZMORRA0:
		LOG("Collision ENTRADA_MAZMORRA0");
		app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra0);
		break;
	case ColliderType::SALIDA_MAZMORRA0:
		LOG("Collision SALIDA_MAZMORRA0");
		app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_pueblo);
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
	int targetPosY = (-position.y * app->win->GetScale() + (windowH / 2) - 10) + 50;

	
	
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
	pbodyFoot->body->SetLinearVelocity(velocity);

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
	pbodyFoot->body->SetLinearVelocity(velocity);
	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
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
		velocity.x = horizontalMovement * GetRealMovementSpeed() * speed * 10 * dt;
		velocity.y = verticalMovement * GetRealMovementSpeed() * speed * 10 * dt;

		// Si hay entrada de movimiento, actualizar estado y direcci��n.

		if (horizontalMovement != 0 || verticalMovement != 0) {
			if (!inAnimation) {
				nextState = EntityState::RUNNING;
			}
			isFacingLeft = (horizontalMovement < 0);
			lastMovementDirection = fPoint(horizontalMovement, verticalMovement);
		}
	}


	FishingDirecction(verticalMovement, horizontalMovement);

	//Si pulsas espacio
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && timerDash.ReadMSec() > cdTimerDashMS) {

		isDashing = true;
		inAnimation = true;
		timerDash.Start();
		nextState = EntityState::DASHI;
		pbodyFoot->body->ApplyForce(b2Vec2(velocity.x * 100, velocity.y * 100), pbodyFoot->body->GetWorldCenter(), false);


		app->audio->StopFx(-1);
		app->audio->PlayFx(dash_fx);
	}

	if (!(timerDash.ReadMSec() < speed * 1000 && isDashing)) {
		isDashing = false;
		pbodyFoot->body->SetLinearVelocity(velocity);
	}

	//Si pulsas J para atacar
	if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN && timerAttack.ReadMSec() > cdTimerAttackMS + 100) {
		isAttacking = true;
		timerAttack.Start();
		nextState = EntityState::ATTACKING;
	}

	if (!(timerAttack.ReadMSec() < cdTimerAttackMS && isAttacking)) {
		isAttacking = false;
		basicAttackDealed = false;
		if (attackSensor) {
			app->physics->DestroyBody(attackSensor);
			attackSensor = nullptr;
		}
		if (mask1PassiveSensor) {
			app->physics->DestroyBody(mask1PassiveSensor);
			mask1PassiveSensor = nullptr;
		}
	}

	if (collisionAttackTimer.ReadSec() > 0.25) {
		app->physics->DestroyBody(attackSensor);
		attackSensor = nullptr;
		if (mask1PassiveSensor) {
			app->physics->DestroyBody(mask1PassiveSensor);
			mask1PassiveSensor = nullptr;
		}
	}


	//Si pulsas K para mascara principal

	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN &&
		(timerMaskAttack.ReadMSec() > maskStats[primaryMask][maskLevels[primaryMask]].maskCoolDown ||
			!maskStats[primaryMask][maskLevels[primaryMask]].firstTimeUsed)) {
		maskStats[primaryMask][maskLevels[primaryMask]].firstTimeUsed = true;
		isAttackingMask = true;
		timerMaskAttack.Start();
		nextState = EntityState::MASK_ATTACK;
	}

	if (!(timerMaskAttack.ReadMSec() < maskStats[primaryMask][maskLevels[primaryMask]].maskCoolDown && isAttackingMask)) {
		isAttackingMask = false;
		mask1PassiveAttackDealed = false;
		if (mask1AttackSensor) {
			app->physics->DestroyBody(mask1AttackSensor);
			mask1AttackSensor = nullptr;
		}
	}

	if (collisionMask1Timer.ReadSec() > 0.25) {
		app->physics->DestroyBody(mask1AttackSensor);
		mask1AttackSensor = nullptr;
	}

	//Si pulsas Q para cambiar de mascara
	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN && timerChangeMask.ReadMSec() > changeMaskCooldown) {
		ChangeMask();
	}


	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;
}
void Player::FishingDirecction(bool verticalMovement, bool horizontalMovement)
{


	// Si jugador Mueve
	if (pressingRight) {
		// Derecha
		player_Direction = Direction::RIGHT;
		if (app->scene_pueblo->GetRod()->fishing.isFishing) {
			playermove = true;
			app->scene_pueblo->GetRod()->fishing.startFishing = false;
		}
		else {
			playermove = false;
		}
	}
	else if (pressingLeft) {
		// izquierda
		player_Direction = Direction::LEFT;
		if (app->scene_pueblo->GetRod()->fishing.isFishing) {
			playermove = true;
			app->scene_pueblo->GetRod()->fishing.startFishing = false;
		}
		else {
			playermove = false;
		}
	}
	else if (pressingDown) {
		// abajo
		player_Direction = Direction::DOWN;
		if (app->scene_pueblo->GetRod()->fishing.isFishing) {
			playermove = true;
			app->scene_pueblo->GetRod()->fishing.startFishing = false;
		}
		else {
			playermove = false;
		}
	}
	else if (pressingUp) {
		// arriba
		player_Direction = Direction::UP;
		if (app->scene_pueblo->GetRod()->fishing.isFishing) {
			playermove = true;
			app->scene_pueblo->GetRod()->fishing.startFishing = false;
		}
		else {
			playermove = false;
		}
	}

}

void Player::TakeDamage(float damage) {

	currentStats.currentHealth -= damage;
	printf("Player has received  %f damage\n", damage);
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
//	pbodyFoot->body->SetLinearVelocity(velocity);
//	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
//	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
//	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;
//}


