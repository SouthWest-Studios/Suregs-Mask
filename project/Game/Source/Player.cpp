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
#include "NotesManager.h"
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
	maskStats[Mask::MASK0][0].numberLightning = 1;
	maskStats[Mask::MASK0][0].poisonDamage = 0;
	maskStats[Mask::MASK0][0].poisonDuration = 0;
	maskStats[Mask::MASK0][0].poisonTickRate = 0;

	//Estadísticas de la máscara 0 a nivel 1
	maskStats[Mask::MASK0][1].maxHealthModifier = 0;
	maskStats[Mask::MASK0][1].movementSpeedModifier = -10;
	maskStats[Mask::MASK0][1].attackDamageModifier = 10;
	maskStats[Mask::MASK0][1].attackSpeedModifier = 10;
	maskStats[Mask::MASK0][1].maskDamage = 80;
	maskStats[Mask::MASK0][1].maskCoolDown = 23000; //En Milisegundos
	maskStats[Mask::MASK0][1].firstTimeUsed = false;
	maskStats[Mask::MASK0][1].numberLightning = 2;
	maskStats[Mask::MASK0][1].poisonDamage = 10;
	maskStats[Mask::MASK0][1].poisonDuration = 6.0f;
	maskStats[Mask::MASK0][1].poisonTickRate = 1.5f;

	//Estadísticas de la máscara 0 a nivel 2
	maskStats[Mask::MASK0][2].maxHealthModifier = 0;
	maskStats[Mask::MASK0][2].movementSpeedModifier = -10;
	maskStats[Mask::MASK0][2].attackDamageModifier = 10;
	maskStats[Mask::MASK0][2].attackSpeedModifier = 10;
	maskStats[Mask::MASK0][2].maskDamage = 130;
	maskStats[Mask::MASK0][2].maskCoolDown = 20000; //En Milisegundos
	maskStats[Mask::MASK0][2].firstTimeUsed = false;
	maskStats[Mask::MASK0][2].numberLightning = 3;
	maskStats[Mask::MASK0][2].poisonDamage = 20;
	maskStats[Mask::MASK0][2].poisonDuration = 6.0f;
	maskStats[Mask::MASK0][2].poisonTickRate = 1.5f;

	//Estadísticas de la máscara 0 a nivel 3
	maskStats[Mask::MASK0][3].maxHealthModifier = 0;
	maskStats[Mask::MASK0][3].movementSpeedModifier = -10;
	maskStats[Mask::MASK0][3].attackDamageModifier = 10;
	maskStats[Mask::MASK0][3].attackSpeedModifier = 10;
	maskStats[Mask::MASK0][3].maskDamage = 200;
	maskStats[Mask::MASK0][3].maskCoolDown = 17000; //En Milisegundos
	maskStats[Mask::MASK0][3].firstTimeUsed = false;
	maskStats[Mask::MASK0][3].numberLightning = 4;
	maskStats[Mask::MASK0][3].poisonDamage = 20;
	maskStats[Mask::MASK0][3].poisonDuration = 9.0f;
	maskStats[Mask::MASK0][3].poisonTickRate = 1.5f;

	//Estadísticas de la máscara 0 a nivel 4
	maskStats[Mask::MASK0][4].maxHealthModifier = 0;
	maskStats[Mask::MASK0][4].movementSpeedModifier = -10;
	maskStats[Mask::MASK0][4].attackDamageModifier = 10;
	maskStats[Mask::MASK0][4].attackSpeedModifier = 10;
	maskStats[Mask::MASK0][4].maskDamage = 300;
	maskStats[Mask::MASK0][4].maskCoolDown = 15000; //En Milisegundos
	maskStats[Mask::MASK0][4].firstTimeUsed = false;
	maskStats[Mask::MASK0][4].numberLightning = 5;
	maskStats[Mask::MASK0][4].poisonDamage = 30;
	maskStats[Mask::MASK0][4].poisonDuration = 11.0f;
	maskStats[Mask::MASK0][4].poisonTickRate = 1.5f;


	// Estadísticas de la máscara 1 a nivel 0
	maskStats[Mask::MASK1][0].maxHealthModifier = -10;
	maskStats[Mask::MASK1][0].movementSpeedModifier = 10;
	maskStats[Mask::MASK1][0].attackDamageModifier = 10;
	maskStats[Mask::MASK1][0].attackSpeedModifier = 0;
	maskStats[Mask::MASK1][0].rangeBallModifier = 0.0f;
	maskStats[Mask::MASK1][0].maskDamage = 100;
	maskStats[Mask::MASK1][0].maskCoolDown = 30000; //En Milisegundos
	maskStats[Mask::MASK1][0].firstTimeUsed = false;
	maskStats[Mask::MASK1][0].poisonDamage = 0;
	maskStats[Mask::MASK1][0].poisonDuration = 0;
	maskStats[Mask::MASK1][0].poisonTickRate = 0;

	// Estadísticas de la máscara 1 a nivel 1
	maskStats[Mask::MASK1][1].maxHealthModifier = -10;
	maskStats[Mask::MASK1][1].movementSpeedModifier = 10;
	maskStats[Mask::MASK1][1].attackDamageModifier = 10;
	maskStats[Mask::MASK1][1].attackSpeedModifier = 0;
	maskStats[Mask::MASK1][1].rangeBallModifier = 0.05f;
	maskStats[Mask::MASK1][1].maskDamage = 120;
	maskStats[Mask::MASK1][1].maskCoolDown = 28000; //En Milisegundos
	maskStats[Mask::MASK1][1].firstTimeUsed = false;
	maskStats[Mask::MASK1][1].poisonDamage = 5;
	maskStats[Mask::MASK1][1].poisonDuration = 5.0f;
	maskStats[Mask::MASK1][1].poisonTickRate = 1.5f;

	// Estadísticas de la máscara 1 a nivel 2
	maskStats[Mask::MASK1][2].maxHealthModifier = -10;
	maskStats[Mask::MASK1][2].movementSpeedModifier = 10;
	maskStats[Mask::MASK1][2].attackDamageModifier = 10;
	maskStats[Mask::MASK1][2].attackSpeedModifier = 0;
	maskStats[Mask::MASK1][2].rangeBallModifier = 0.10f;
	maskStats[Mask::MASK1][2].maskDamage = 180;
	maskStats[Mask::MASK1][2].maskCoolDown = 25000; //En Milisegundos
	maskStats[Mask::MASK1][2].firstTimeUsed = false;
	maskStats[Mask::MASK1][2].poisonDamage = 10;
	maskStats[Mask::MASK1][2].poisonDuration = 5.0f;
	maskStats[Mask::MASK1][2].poisonTickRate = 1.5f;

	// Estadísticas de la máscara 1 a nivel 3
	maskStats[Mask::MASK1][3].maxHealthModifier = -10;
	maskStats[Mask::MASK1][3].movementSpeedModifier = 10;
	maskStats[Mask::MASK1][3].attackDamageModifier = 10;
	maskStats[Mask::MASK1][3].attackSpeedModifier = 0;
	maskStats[Mask::MASK1][3].rangeBallModifier = 0.15f;
	maskStats[Mask::MASK1][3].maskDamage = 250;
	maskStats[Mask::MASK1][3].maskCoolDown = 23000; //En Milisegundos
	maskStats[Mask::MASK1][3].firstTimeUsed = false;
	maskStats[Mask::MASK1][3].poisonDamage = 10;
	maskStats[Mask::MASK1][3].poisonDuration = 10.0f;
	maskStats[Mask::MASK1][3].poisonTickRate = 1.5f;

	// Estadísticas de la máscara 1 a nivel 4
	maskStats[Mask::MASK1][4].maxHealthModifier = -10;
	maskStats[Mask::MASK1][4].movementSpeedModifier = 10;
	maskStats[Mask::MASK1][4].attackDamageModifier = 10;
	maskStats[Mask::MASK1][4].attackSpeedModifier = 0;
	maskStats[Mask::MASK1][4].rangeBallModifier = 0.20f; // 20% de aumento en el rango
	maskStats[Mask::MASK1][4].maskDamage = 400;
	maskStats[Mask::MASK1][4].maskCoolDown = 20000; //En Milisegundos
	maskStats[Mask::MASK1][4].firstTimeUsed = false;
	maskStats[Mask::MASK1][4].poisonDamage = 15;
	maskStats[Mask::MASK1][4].poisonDuration = 15.0f;
	maskStats[Mask::MASK1][4].poisonTickRate = 1.5f;


	//Estadísticas de la máscara 2 a nivel 0
	maskStats[Mask::MASK2][0].maxHealthModifier = -20;
	maskStats[Mask::MASK2][0].movementSpeedModifier = 10;
	maskStats[Mask::MASK2][0].attackDamageModifier = 10;
	maskStats[Mask::MASK2][0].attackSpeedModifier = 10;
	maskStats[Mask::MASK2][0].distanceDashModifier = 1.30f;
	maskStats[Mask::MASK2][0].maskDamage = 40.0f;
	maskStats[Mask::MASK2][0].maskCoolDown = 20000.0f;
	maskStats[Mask::MASK2][0].invisibilityDuration = 5.0f;

	//Estadísticas de la máscara 2 a nivel 1
	maskStats[Mask::MASK2][1].maxHealthModifier = -20;
	maskStats[Mask::MASK2][1].movementSpeedModifier = 10;
	maskStats[Mask::MASK2][1].attackDamageModifier = 10;
	maskStats[Mask::MASK2][1].attackSpeedModifier = 10;
	maskStats[Mask::MASK2][1].distanceDashModifier = 1.35f;
	maskStats[Mask::MASK2][1].maskDamage = 50.0f;
	maskStats[Mask::MASK2][1].maskCoolDown = 18000.0f;
	maskStats[Mask::MASK2][1].invisibilityDuration = 6.0f;

	//Estadísticas de la máscara 2 a nivel 2
	maskStats[Mask::MASK2][2].maxHealthModifier = -20;
	maskStats[Mask::MASK2][2].movementSpeedModifier = 10;
	maskStats[Mask::MASK2][2].attackDamageModifier = 10;
	maskStats[Mask::MASK2][2].attackSpeedModifier = 10;
	maskStats[Mask::MASK2][2].distanceDashModifier = 1.40f;
	maskStats[Mask::MASK2][2].maskDamage = 70.0f;
	maskStats[Mask::MASK2][2].maskCoolDown = 16000.0f;
	maskStats[Mask::MASK2][2].invisibilityDuration = 7.3f;

	//Estadísticas de la máscara 2 a nivel 3
	maskStats[Mask::MASK2][3].maxHealthModifier = -20;
	maskStats[Mask::MASK2][3].movementSpeedModifier = 10;
	maskStats[Mask::MASK2][3].attackDamageModifier = 10;
	maskStats[Mask::MASK2][3].attackSpeedModifier = 10;
	maskStats[Mask::MASK2][3].distanceDashModifier = 1.45f;
	maskStats[Mask::MASK2][3].maskDamage = 100.0f;
	maskStats[Mask::MASK2][3].maskCoolDown = 18000.0f;
	maskStats[Mask::MASK2][3].invisibilityDuration = 8.6f;

	//Estadísticas de la máscara 2 a nivel 4
	maskStats[Mask::MASK2][4].maxHealthModifier = -20;
	maskStats[Mask::MASK2][4].movementSpeedModifier = 10;
	maskStats[Mask::MASK2][4].attackDamageModifier = 10;
	maskStats[Mask::MASK2][4].attackSpeedModifier = 10;
	maskStats[Mask::MASK2][4].distanceDashModifier = 1.50f;
	maskStats[Mask::MASK2][4].maskDamage = 150.0f;
	maskStats[Mask::MASK2][4].maskCoolDown = 10000.0f;
	maskStats[Mask::MASK2][4].invisibilityDuration = 10.0f;


	//Estadísticas de la máscara 3 a nivel 0
	maskStats[Mask::MASK3][0].maxHealthModifier = -5;
	maskStats[Mask::MASK3][0].movementSpeedModifier = -5;
	maskStats[Mask::MASK3][0].attackDamageModifier = -5;
	maskStats[Mask::MASK3][0].attackSpeedModifier = -5;
	maskStats[Mask::MASK3][0].maskDamage = 0;
	maskStats[Mask::MASK3][0].maskCoolDown = 60000.0f;
	maskStats[Mask::MASK3][0].durationTime = 20000.0f;
	maskStats[Mask::MASK3][0].maxActiveHealthModifier = 0.10f; //10%
	maskStats[Mask::MASK3][0].maxActiveDamageModifier = 0.10f; //10%

	//Estadísticas de la máscara 3 a nivel 1
	maskStats[Mask::MASK3][1].maxHealthModifier = -5;
	maskStats[Mask::MASK3][1].movementSpeedModifier = -5;
	maskStats[Mask::MASK3][1].attackDamageModifier = -5;
	maskStats[Mask::MASK3][1].attackSpeedModifier = -5;
	maskStats[Mask::MASK3][1].maskDamage = 0;
	maskStats[Mask::MASK3][1].maskCoolDown = 58000.0f;
	maskStats[Mask::MASK3][1].durationTime = 22000.0f;
	maskStats[Mask::MASK3][1].maxActiveHealthModifier = 0.15f;
	maskStats[Mask::MASK3][1].maxActiveDamageModifier = 0.15f; 

	//Estadísticas de la máscara 3 a nivel 2
	maskStats[Mask::MASK3][2].maxHealthModifier = -5;
	maskStats[Mask::MASK3][2].movementSpeedModifier = -5;
	maskStats[Mask::MASK3][2].attackDamageModifier = -5;
	maskStats[Mask::MASK3][2].attackSpeedModifier = -5;
	maskStats[Mask::MASK3][2].maskDamage = 0;
	maskStats[Mask::MASK3][2].maskCoolDown = 56000.0f;
	maskStats[Mask::MASK3][2].durationTime = 25000.0f;
	maskStats[Mask::MASK3][2].maxActiveHealthModifier = 0.20f;
	maskStats[Mask::MASK3][2].maxActiveDamageModifier = 0.20f; 

	//Estadísticas de la máscara 3 a nivel 3
	maskStats[Mask::MASK3][3].maxHealthModifier = -5;
	maskStats[Mask::MASK3][3].movementSpeedModifier = -5;
	maskStats[Mask::MASK3][3].attackDamageModifier = -5;
	maskStats[Mask::MASK3][3].attackSpeedModifier = -5;
	maskStats[Mask::MASK3][3].maskDamage = 0;
	maskStats[Mask::MASK3][3].maskCoolDown = 53000.0f;
	maskStats[Mask::MASK3][3].durationTime = 27000.0f;
	maskStats[Mask::MASK3][3].maxActiveHealthModifier = 0.25f;
	maskStats[Mask::MASK3][3].maxActiveDamageModifier = 0.25f; 

	//Estadísticas de la máscara 3 a nivel 4
	maskStats[Mask::MASK3][4].maxHealthModifier = -5;
	maskStats[Mask::MASK3][4].movementSpeedModifier = -5;
	maskStats[Mask::MASK3][4].attackDamageModifier = -5;
	maskStats[Mask::MASK3][4].attackSpeedModifier = -5;
	maskStats[Mask::MASK3][4].maskDamage = 0;
	maskStats[Mask::MASK3][4].maskCoolDown = 50000.0f;
	maskStats[Mask::MASK3][4].durationTime = 30000.0f;
	maskStats[Mask::MASK3][4].maxActiveHealthModifier = 0.35f;
	maskStats[Mask::MASK3][4].maxActiveDamageModifier = 0.35f; 


	//Estadísticas pasivas mascara 0 a nivel 0
	passiveStats[Mask::MASK0][0].damageBoost = 20;
	passiveStats[Mask::MASK0][0].rangeBoost = 0;
	passiveStats[Mask::MASK0][0].dashBoost = 0;

	//Estadísticas pasivas mascara 0 a nivel 1
	passiveStats[Mask::MASK0][1].damageBoost = 20;
	passiveStats[Mask::MASK0][1].rangeBoost = 0;
	passiveStats[Mask::MASK0][1].dashBoost = 0;

	//Estadísticas pasivas mascara 0 a nivel 2
	passiveStats[Mask::MASK0][2].damageBoost = 20;
	passiveStats[Mask::MASK0][2].rangeBoost = 0;
	passiveStats[Mask::MASK0][2].dashBoost = 0;
	//Estadísticas pasivas mascara 0 a nivel 3
	passiveStats[Mask::MASK0][3].damageBoost = 20;
	passiveStats[Mask::MASK0][3].rangeBoost = 0;
	passiveStats[Mask::MASK0][3].dashBoost = 0;
	//Estadísticas pasivas mascara 0 a nivel 4
	passiveStats[Mask::MASK0][4].damageBoost = 20;
	passiveStats[Mask::MASK0][4].rangeBoost = 0;
	passiveStats[Mask::MASK0][4].dashBoost = 0;


	//Estadísticas pasivas mascara 1 a nivel 0
	passiveStats[Mask::MASK1][0].damageBoost = 0.33;
	passiveStats[Mask::MASK1][0].rangeBoost = 25;
	passiveStats[Mask::MASK1][0].dashBoost = 0;

	//Estadísticas pasivas mascara 1 a nivel 1
	passiveStats[Mask::MASK1][1].damageBoost = 0.33;
	passiveStats[Mask::MASK1][1].rangeBoost = 25;
	passiveStats[Mask::MASK1][1].dashBoost = 0;

	//Estadísticas pasivas mascara 1 a nivel 2
	passiveStats[Mask::MASK1][2].damageBoost = 0.33;
	passiveStats[Mask::MASK1][2].rangeBoost = 25;
	passiveStats[Mask::MASK1][2].dashBoost = 0;

	//Estadísticas pasivas mascara 1 a nivel 3
	passiveStats[Mask::MASK1][3].damageBoost = 0.33;
	passiveStats[Mask::MASK1][3].rangeBoost = 25;
	passiveStats[Mask::MASK1][3].dashBoost = 0;

	//Estadísticas pasivas mascara 1 a nivel 4
	passiveStats[Mask::MASK1][4].damageBoost = 0.33;
	passiveStats[Mask::MASK1][4].rangeBoost = 25;
	passiveStats[Mask::MASK1][4].dashBoost = 0;


	//Estadísticas pasivas mascara 2 a nivel 0
	passiveStats[Mask::MASK2][0].damageBoost = 0;
	passiveStats[Mask::MASK2][0].rangeBoost = 0;
	passiveStats[Mask::MASK2][0].dashBoost = 2;
	passiveStats[Mask::MASK2][0].invisibilityDuration = 2000.0f;

	//Estadísticas pasivas mascara 2 a nivel 1
	passiveStats[Mask::MASK2][1].damageBoost = 0;
	passiveStats[Mask::MASK2][1].rangeBoost = 0;
	passiveStats[Mask::MASK2][1].dashBoost = 2;
	passiveStats[Mask::MASK2][1].invisibilityDuration = 2000.0f;

	//Estadísticas pasivas mascara 2 a nivel 2
	passiveStats[Mask::MASK2][2].damageBoost = 0;
	passiveStats[Mask::MASK2][2].rangeBoost = 0;
	passiveStats[Mask::MASK2][2].dashBoost = 2;
	passiveStats[Mask::MASK2][2].invisibilityDuration = 2000.0f;

	//Estadísticas pasivas mascara 2 a nivel 3
	passiveStats[Mask::MASK2][3].damageBoost = 0;
	passiveStats[Mask::MASK2][3].rangeBoost = 0;
	passiveStats[Mask::MASK2][3].dashBoost = 2;
	passiveStats[Mask::MASK2][3].invisibilityDuration = 2000.0f;

	//Estadísticas pasivas mascara 2 a nivel 4
	passiveStats[Mask::MASK2][4].damageBoost = 0;
	passiveStats[Mask::MASK2][4].rangeBoost = 0;
	passiveStats[Mask::MASK2][4].dashBoost = 2;
	passiveStats[Mask::MASK2][4].invisibilityDuration = 2000.0f;


	//Estadísticas pasivas mascara 3 a level 0
	passiveStats[Mask::MASK3][0].damageBoost = 0.20f; //20% de aumento en el daño
	passiveStats[Mask::MASK3][0].velocityBoost = 0.10f; //10% de aumento en la velocidad

	//Estadísticas pasivas mascara 3 a level 1
	passiveStats[Mask::MASK3][1].damageBoost = 0.20f;
	passiveStats[Mask::MASK3][1].velocityBoost = 0.10f;

	//Estadísticas pasivas mascara 3 a level 2
	passiveStats[Mask::MASK3][2].damageBoost = 0.20f;
	passiveStats[Mask::MASK3][2].velocityBoost = 0.10f;

	//Estadísticas pasivas mascara 3 a level 3
	passiveStats[Mask::MASK3][3].damageBoost = 0.20f;
	passiveStats[Mask::MASK3][3].velocityBoost = 0.10f;

	//Estadísticas pasivas mascara 3 a level 4
	passiveStats[Mask::MASK3][4].damageBoost = 0.20f;
	passiveStats[Mask::MASK3][4].velocityBoost = 0.10f;

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
	run_fx = app->audio->LoadAudioFx("run_fx");
	runAlt_fx = app->audio->LoadAudioFx("runAlt_fx");
	runAlt2_fx = app->audio->LoadAudioFx("runAlt2_fx");
	dash_fx = app->audio->LoadAudioFx("dash_fx");
	switch_masks_fx = app->audio->LoadAudioFx("switch_masks_fx");
	basic_combo_attack1_fx = app->audio->LoadAudioFx("basic_combo_attack1_fx");
	player_get_damage_fx = app->audio->LoadAudioFx("player_get_damage_fx");

	cdTimerDashMS = 500;
	cdTimerAttackMS = 100000 / currentStats.attackSpeed;

	player_Direction = Direction::UNKNOWN;


	EquipPrimaryMask(Mask::MASK3);
	EquipSecondaryMask(Mask::NOMASK);

	//PARA TESTEAR
	maskLevels[primaryMask] = 0; 
	maskLevels[secondaryMask] = 0;
	printf("Primary mask: %d, Level: %d\n", static_cast<int>(primaryMask) - 1, maskLevels[primaryMask]);
	printf("Secondary mask: %d, Level: %d\n", static_cast<int>(secondaryMask) - 1, maskLevels[secondaryMask]);
	return true;
}

bool Player::Update(float dt)
{

	
	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
	pbodySensor->body->SetTransform(b2Vec2(pbodyPos.p.x, pbodyPos.p.y - 1), 0);

	if (!inAnimation) {
		desiredState = EntityState::IDLE;
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

	if (maskStats[primaryMask][maskLevels[primaryMask]].invisibilityTimer.ReadSec() > maskStats[primaryMask][maskLevels[primaryMask]].invisibilityDuration) {
        SDL_SetTextureAlphaMod(texture, 255);
		isInvisible = false;
		maskStats[primaryMask][maskLevels[primaryMask]].invisibilityTimer.Start();

    }
	if(passiveStats[secondaryMask][maskLevels[secondaryMask]].invisibilityTimer.ReadSec() > passiveStats[secondaryMask][maskLevels[secondaryMask]].invisibilityDuration){
		SDL_SetTextureAlphaMod(texture, 255);
		isInvisible = false;
		passiveStats[secondaryMask][maskLevels[secondaryMask]].invisibilityTimer.Start();

	}
	if(!isInvisible){
		SDL_SetTextureAlphaMod(texture, 255);
	}

	if (mask3Timer.ReadMSec() >= maskStats[Mask::MASK3][maskLevels[Mask::MASK3]].durationTime && mask3Active) {
        // Revertir las estadísticas a sus valores originales
        currentStats.attackDamage = baseStats.attackDamage * (1 + maskStats[Mask::MASK3][maskLevels[Mask::MASK3]].maxActiveDamageModifier / 100); 
        currentStats.maxHealth = baseStats.maxHealth * (1 + maskStats[Mask::MASK3][maskLevels[Mask::MASK3]].maxActiveHealthModifier / 100);
		mask3Active = false;
    }

	if(secondaryMask == Mask::MASK3){
		currentStats.attackDamage = baseStats.attackDamage * (1 + passiveStats[Mask::MASK3][maskLevels[Mask::MASK3]].damageBoost); 
		currentStats.movementSpeed = baseStats.movementSpeed * (1 + passiveStats[Mask::MASK3][maskLevels[Mask::MASK3]].velocityBoost);
	}

	if (playerXP >= XPtoLevelUp) {
		playerXP -= XPtoLevelUp;
		XPtoLevelUp += 20;
		maskPoints++;
		level++;

		printf("Has subido a nivel %i y tu experiencia actual es %i \n", level, playerXP);
	}

	stateMachine(dt);
	currentAnimation->Update();
	return true;
}


bool Player::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	//printf("Posición: (%d, %d)\n", position.x, position.y);

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
}

float Player::GetRealMovementSpeed() const {
	return 0.2f * (currentStats.movementSpeed / 100.0f);
}

void Player::Run(float dt)
{	
	currentAnimation = &runAnim;
	app->audio->PlayRunFx(run_fx, runAlt_fx, runAlt2_fx);
}

void Player::Dashi(float dt)
{
	inAnimation = true;
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

	pbodyFoot->body->ApplyForceToCenter(b2Vec2(lastMovementDirection.x * attackMovement, lastMovementDirection.y * attackMovement), true);

	//Onda expansiva ataque pasivo mascara 1
	if (secondaryMask == Mask::MASK1) {
		mask1PassiveSensor = app->physics->CreateRectangleSensor(attackX, attackY, 100, 100, DYNAMIC);
		mask1PassiveSensor->ctype = ColliderType::MASK0_PASSIVE_ATTACK;
		mask1PassiveSensor->listener = this;
	}

	app->audio->PlayFx(basic_combo_attack1_fx); // <--- No funciona
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

		app->audio->PlayFx(switch_masks_fx);  // <--- No funciona

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
		CastMultipleLightnings();
		break;
	case Mask::MASK1:
		AreaAttack(dt);
		break;
	case Mask::MASK2:
		DashAttack(dt);
		SetInvisible();
		break;
	case Mask::MASK3:
		Mask3Statistics();
		break;
	}
}
void Player::ApplyPoison(Entity* entity) {
	int poisonDamage = maskStats[Mask::MASK0][maskLevels[Mask::MASK0]].poisonDamage;
	float poisonDuration = maskStats[Mask::MASK0][maskLevels[Mask::MASK0]].poisonDuration / 1000.0f; // Convertir a segundos
	float poisonTickRate = maskStats[Mask::MASK0][maskLevels[Mask::MASK0]].poisonTickRate / 1000.0f; // Convertir a segundos

	// Aquí asumimos que Entity tiene una función ApplyPoison que toma estos parámetros.
	// Si no es el caso, necesitarás ajustar este código.
	entity->ApplyPoison(poisonDamage, poisonDuration, poisonTickRate);
}

void Player::stateMachine(float dt)
{
	//printf("\ncurrentState: %d, desiredState: %d", static_cast<int>(currentState), static_cast<int>(desiredState));
	nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;
	switch (nextState) {
	case EntityState::IDLE:
		DoNothing(dt);
		app->audio->StopFx(-1);
		break;
	case EntityState::RUNNING:
		Run(dt);
		break;
	case EntityState::ATTACKING:
		app->audio->PlayFx(basic_combo_attack1_fx);
		Attack(dt);
		break;
	case EntityState::DEAD:
	
		break;
	case EntityState::REVIVING:
		break;
	case EntityState::MASK_ATTACK:
		MaskAttack(dt);
		break;
	case EntityState::DASHI:
		if (isDashing) {
			Dashi(dt);
		}
		break;
	case EntityState::NONE:

		desiredState = EntityState::IDLE;
		break;
	
	default:
		break;
	}
	currentState = nextState;
}

//Rango ataque mascara 0
Entity* Player::GetEnemyWithHighestHealthWithinRadius(iPoint position, int radius) {
	Entity* highestHealthEnemy = nullptr;
	int highestHealth = 0;
	for (Entity* enemy : app->entityManager->GetEnemies()) {
		int dx = position.x - enemy->position.x;
		int dy = position.y - enemy->position.y;
		if (dx * dx + dy * dy <= radius * radius) {
			//printf("Considering enemy at (%d, %d) with health %f\n", enemy->position.x, enemy->position.y, enemy->GetHealth());
			if (enemy->GetHealth() > highestHealth) {
				//printf("Selected as highest health enemy so far\n");
				highestHealthEnemy = enemy;
				highestHealth = enemy->GetHealth();
			}
		}
	}
	return highestHealthEnemy;
}

void Player::CastMultipleLightnings() {
	int numLightnings = maskLevels[Mask::MASK0]; // Obtiene el número de rayos basado en el nivel de la máscara

	for (int i = 0; i <= numLightnings; i++) {
		CastLightning();
	}
}

//Ataque mascara 0
void Player::CastLightning() {
	Entity* target = GetEnemyWithHighestHealthWithinRadius(position, 500);
	if (target != nullptr) {
		printf("Enemy hit: %p at position (%d, %d)\n", (void*)target, target->position.x, target->position.y);
        target->TakeDamage(maskStats[primaryMask][maskLevels[primaryMask]].maskDamage);
		target->ApplyPoison(maskStats[primaryMask][maskLevels[primaryMask]].poisonDamage, maskStats[primaryMask][maskLevels[primaryMask]].poisonDuration, maskStats[primaryMask][maskLevels[primaryMask]].poisonTickRate);
		//ApplyPoison(target);
	}
	else {
		printf("No enemy alive in range to attack\n");
	}
}

//Ataque mascara 1

void Player::AreaAttack(float dt) {
	mask1AttackSensor = app->physics->CreateRectangleSensor(this->position.x, this->position.y, attackMask1Width + attackMask1Width * maskStats[primaryMask][maskLevels[primaryMask]].rangeBallModifier, attackMask1Height + attackMask1Height * maskStats[primaryMask][maskLevels[primaryMask]].rangeBallModifier, DYNAMIC);
	mask1AttackSensor->ctype = ColliderType::MASK1_ATTACK;
	mask1AttackSensor->listener = this;
}

//Ataque mascara 2

void Player::DashAttack(float dt) {
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

		isDashing = true;
		timerDash.Start();
		
		dashCollision = app->physics->CreateRectangleSensor(position.x, position.y, 150, 150, DYNAMIC);
		dashCollision->ctype = ColliderType::MASK2_ATTACK;
		dashCollision->listener = this;
		
		pbodyFoot->body->ApplyForce(b2Vec2(velocity.x * 100 * maskStats[primaryMask][maskLevels[primaryMask]].distanceDashModifier, velocity.y * 100 * maskStats[primaryMask][maskLevels[primaryMask]].distanceDashModifier), pbodyFoot->body->GetWorldCenter(), false);
	}	
}

void Player::SetInvisible() {
    SDL_SetTextureAlphaMod(texture, 100);
    maskStats[primaryMask][maskLevels[primaryMask]].invisibilityTimer.Start();
	isInvisible = true;
}

void Player::SetPassiveInvisible() {
    SDL_SetTextureAlphaMod(texture, 100);
	isInvisible = true;
    passiveStats[secondaryMask][maskLevels[secondaryMask]].invisibilityTimer.Start();
}

//Mascara 3
void Player::Mask3Statistics(){
	currentStats.attackDamage = baseStats.attackDamage * (1 + maskStats[Mask::MASK3][maskLevels[Mask::MASK3]].maxActiveDamageModifier);
	currentStats.maxHealth = baseStats.maxHealth * (1 + maskStats[Mask::MASK3][maskLevels[Mask::MASK3]].maxActiveHealthModifier);   
	printf("current stats attack damage: %f\n", currentStats.attackDamage);
	mask3Active = true;
	mask3Timer.Start();
}

// L07 DONE 6: Define OnCollision function for the player. 
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	//Colision general
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
			if (physB->entity != nullptr) {
				physB->entity->TakeDamage(currentStats.attackDamage);
			}
			//collisionAttackTimer.Start();
		}
		if (physA == mask1PassiveSensor) {
			LOG("Collision ENEMY");
			if (physB->entity != nullptr) {
				physB->entity->TakeDamage(currentStats.attackDamage * passiveStats[secondaryMask][maskLevels[secondaryMask]].damageBoost);
			}
		}
		if (physA == mask1AttackSensor) {
			LOG("Collision ENEMY");
			if (physB->entity != nullptr) {
				physB->entity->TakeDamage(maskStats[primaryMask][maskLevels[primaryMask]].maskDamage);
				physB->entity->ApplyPoison(maskStats[primaryMask][maskLevels[primaryMask]].poisonDamage, maskStats[primaryMask][maskLevels[primaryMask]].poisonDuration, maskStats[primaryMask][maskLevels[primaryMask]].poisonTickRate);
			}
			//collisionMask1Timer.Start();
		}
		if(physA == dashCollision)
		{
			if (physB->entity != nullptr) {
				physB->entity->TakeDamage(maskStats[Mask::MASK2][maskLevels[Mask::MASK2]].maskDamage);
			}
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
		app->audio->PlayFx(player_get_damage_fx); // <--- Hay que arreglarlo
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
					physB->body->SetActive(false);
				}
				
			}
		}
		break;
	case ColliderType::NOTA:
		LOG("Collision NOTA");
		if (physA != attackSensor && physA != mask1PassiveSensor && physA != mask1AttackSensor)
		{
			if (physB->listener->active) {
				if (app->notesManager->IsFull() == false)
				{
					app->notesManager->CreateItem(physB->listener->type, physB->listener->CloseTexture);
					physB->listener->active = false;
					physB->body->SetActive(false);
				}

			}
		}
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}

	//Colisiones de sool el pie
	if (physA == pbodyFoot) {
		switch (physB->ctype) {
		case ColliderType::ENTRADA_MAZMORRA0:
			LOG("Collision ENTRADA_MAZMORRA0");
			app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra0);
			break;
		case ColliderType::SALIDA_MAZMORRA0:
			LOG("Collision SALIDA_MAZMORRA0");
			app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_pueblo);
			break;
		}
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
		else
		{
			MapObject* currentRoom = GetCurrentRoom();
			if (currentRoom != nullptr)
			{
				// Comprueba si la sala actual es pequeña
				if (app->map->smallRoomsList.Find(currentRoom) != -1)
				{
					int targetPosX = (-(int)(currentRoom->x + currentRoom->width / 2) * app->win->GetScale() + (windowW / 2) - 10);
					int targetPosY = (-(int)(currentRoom->y + currentRoom->height / 2) * app->win->GetScale() + (windowH / 2) - 10);

					app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
					app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
				}
				// Comprueba si la sala actual es grande
				else if (app->map->largeRoomsList.Find(currentRoom) != -1)
				{
					DetermineRoomType(currentRoom);

					switch (roomType) {
					case ROOM_SQUARE:
						//X
						if (position.x < currentRoom->x + app->render->camera.w / 2) //izq
						{
							int targetPosX = (-(int)(currentRoom->x + currentRoom->width / 2) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else if (position.x > currentRoom->x + currentRoom->width - app->render->camera.w / 2) //der
						{
							int targetPosX = (-(int)(currentRoom->x + currentRoom->width - app->render->camera.w / 2) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else
						{
							int targetPosX = (-(int)(position.x) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}

						//Y
						if (position.y < currentRoom->y + app->render->camera.h / 2) //arriba
						{
							int targetPosY = (-(int)(currentRoom->y + app->render->camera.h / 2) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else if (position.y > currentRoom->y + currentRoom->height - app->render->camera.h / 2) //abajo
						{
							int targetPosY = (-(int)(currentRoom->y + currentRoom->height - app->render->camera.h / 2) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else
						{
							int targetPosY = (-(int)(position.y) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						break;

					case ROOM_RECTANGLE_H:

						//X
						if (position.x < currentRoom->x + app->render->camera.w / 2) //izq
						{
							int targetPosX = (-(int)(currentRoom->x + currentRoom->width / 2) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else if (position.x > currentRoom->x + currentRoom->width - app->render->camera.w / 2) //der
						{
							int targetPosX = (-(int)(currentRoom->x + currentRoom->width - app->render->camera.w / 2) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else
						{
							int targetPosX = (-(int)(position.x) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}

						//Y
						if (position.y < currentRoom->y + app->render->camera.h / 2) //arriba
						{
							int targetPosY = (-(int)(currentRoom->y + app->render->camera.h / 2) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else if (position.y > currentRoom->y + currentRoom->height - app->render->camera.h / 2) //abajo
						{
							int targetPosY = (-(int)(currentRoom->y + currentRoom->height - app->render->camera.h / 2) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else
						{
							int targetPosY = (-(int)(position.y) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						break;

					case ROOM_RECTANGLE_W:

						//X
						if (position.x < currentRoom->x + app->render->camera.w / 2) //izq
						{
							int targetPosX = (-(int)(currentRoom->x + app->render->camera.w / 2) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else if (position.x > currentRoom->x + currentRoom->width - app->render->camera.w / 2) //der
						{
							int targetPosX = (-(int)(currentRoom->x + currentRoom->width - app->render->camera.w / 2) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else
						{
							int targetPosX = (-(int)(position.x) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}

						//Y
						if (position.y < currentRoom->y + app->render->camera.h / 2) //arriba
						{
							int targetPosY = (-(int)(currentRoom->y + app->render->camera.h / 2) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else if (position.y > currentRoom->y + currentRoom->height - app->render->camera.h / 2) //abajo
						{
							int targetPosY = (-(int)(currentRoom->y + currentRoom->height - app->render->camera.h / 2) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else
						{
							int targetPosY = (-(int)(position.y) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						break;

					default:
						printf("isLargeRoom not found");
						break;
					}
				}
				// Comprueba si la sala actual es l
				else if (app->map->LRoomsList.Find(currentRoom) != -1)
				{
					DetermineRoomTypel(currentRoom);

					switch (roomType)
					{
					case ROOM_L_DL: //tiene salas abajo y izquierda
						printf("Abajo y Izquerda \n");
						//X
						if (position.x < currentRoom->x + app->render->camera.w / 2) //izq
						{
							int targetPosX = (-(int)(position.x) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else if (position.x > currentRoom->x + currentRoom->width - app->render->camera.w / 2) //der
						{
							int targetPosX = (-(int)(currentRoom->x + currentRoom->width - app->render->camera.w / 2) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else
						{
							int targetPosX = (-(int)(position.x) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}

						//Y
						if (position.y < currentRoom->y + app->render->camera.h / 2) //arriba
						{
							int targetPosY = (-(int)(currentRoom->y + app->render->camera.h / 2) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else if (position.y > currentRoom->y + currentRoom->height - app->render->camera.h / 2) //abajo
						{
							int targetPosY = (-(int)(position.y) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else
						{
							int targetPosY = (-(int)(position.y) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						break;

					case ROOM_L_DR: //tiene salas abajo y derecha
						printf("Abajo y Derecha \n");
						//X
						if (position.x < currentRoom->x + app->render->camera.w / 2) //izq
						{
							int targetPosX = (-(int)(currentRoom->x + currentRoom->width / 2) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else if (position.x > currentRoom->x + currentRoom->width - app->render->camera.w / 2) //der
						{
							int targetPosX = (-(int)(position.x) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else
						{
							int targetPosX = (-(int)(position.x) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}

						//Y
						if (position.y < currentRoom->y + app->render->camera.h / 2) //arriba
						{
							int targetPosY = (-(int)(currentRoom->y + app->render->camera.h / 2) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else if (position.y > currentRoom->y + currentRoom->height - app->render->camera.h / 2) //abajo
						{
							int targetPosY = (-(int)(position.y) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else
						{
							int targetPosY = (-(int)(position.y) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						break;

					case ROOM_L_UL: //tiene salas arriba y izquierda
						printf("Arriba y Izquerda \n");
						//X
						if (position.x < currentRoom->x + app->render->camera.w / 2) //izq
						{
							int targetPosX = (-(int)(position.x) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else if (position.x > currentRoom->x + currentRoom->width - app->render->camera.w / 2) //der
						{
							int targetPosX = (-(int)(currentRoom->x + currentRoom->width - app->render->camera.w / 2) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else
						{
							int targetPosX = (-(int)(position.x) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}

						//Y
						if (position.y < currentRoom->y + app->render->camera.h / 2) //arriba
						{
							int targetPosY = (-(int)(position.y) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else if (position.y > currentRoom->y + currentRoom->height - app->render->camera.h / 2) //abajo
						{
							int targetPosY = (-(int)(currentRoom->y + currentRoom->height - app->render->camera.h / 2) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else
						{
							int targetPosY = (-(int)(position.y) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						break;


					case ROOM_L_UR: //tiene salas arriba y derecha
						printf("Arriba y Derecha \n");
						//X
						if (position.x < currentRoom->x + app->render->camera.w / 2) //izq
						{
							int targetPosX = (-(int)(currentRoom->x + currentRoom->width / 2) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else if (position.x > currentRoom->x + currentRoom->width - app->render->camera.w / 2) //der
						{
							int targetPosX = (-(int)(position.x) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else
						{
							int targetPosX = (-(int)(position.x) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}

						//Y
						if (position.y < currentRoom->y + app->render->camera.h / 2) //arriba
						{
							int targetPosY = (-(int)(position.y) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else if (position.y > currentRoom->y + currentRoom->height - app->render->camera.h / 2) //abajo
						{
							int targetPosY = (-(int)(currentRoom->y + currentRoom->height - app->render->camera.h / 2) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else
						{
							int targetPosY = (-(int)(position.y) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						break;

					case ROOM_L_D: //sala abajo
						printf("Abajo \n");
						//X
						if (position.x < currentRoom->x + app->render->camera.w / 2) //izq
						{
							int targetPosX = (-(int)(currentRoom->x + currentRoom->width / 2) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else if (position.x > currentRoom->x + currentRoom->width - app->render->camera.w / 2) //der
						{
							int targetPosX = (-(int)(currentRoom->x + currentRoom->width - app->render->camera.w / 2) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else
						{
							int targetPosX = (-(int)(position.x) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}

						//Y
						if (position.y < currentRoom->y + app->render->camera.h / 2) //arriba
						{
							int targetPosY = (-(int)(position.y) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else if (position.y > currentRoom->y + currentRoom->height - app->render->camera.h / 2) //abajo
						{
							int targetPosY = (-(int)(currentRoom->y + currentRoom->height - app->render->camera.h / 2) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else
						{
							int targetPosY = (-(int)(position.y) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						break;

					case ROOM_L_U: //sala arriba
						printf("Arriba \n");
						//X
						if (position.x < currentRoom->x + app->render->camera.w / 2) //izq
						{
							int targetPosX = (-(int)(currentRoom->x + currentRoom->width / 2) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else if (position.x > currentRoom->x + currentRoom->width - app->render->camera.w / 2) //der
						{
							int targetPosX = (-(int)(currentRoom->x + currentRoom->width - app->render->camera.w / 2) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else
						{
							int targetPosX = (-(int)(position.x) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}

						//Y
						if (position.y < currentRoom->y + app->render->camera.h / 2) //arriba
						{
							int targetPosY = (-(int)(currentRoom->y + app->render->camera.h / 2) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else if (position.y > currentRoom->y + currentRoom->height - app->render->camera.h / 2) //abajo
						{
							int targetPosY = (-(int)(position.y) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else
						{
							int targetPosY = (-(int)(position.y) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}

						break;

					case ROOM_L_L: //sala izquierda
						printf("Izquierda \n");
						//X
						if (position.x < currentRoom->x + app->render->camera.w / 2) //izq
						{
							int targetPosX = (-(int)(currentRoom->x + currentRoom->width / 2) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else if (position.x > currentRoom->x + currentRoom->width - app->render->camera.w / 2) //der
						{
							int targetPosX = (-(int)(position.x) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else
						{
							int targetPosX = (-(int)(position.x) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}

						//Y
						if (position.y < currentRoom->y + app->render->camera.h / 2) //arriba
						{
							int targetPosY = (-(int)(currentRoom->y + app->render->camera.h / 2) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else if (position.y > currentRoom->y + currentRoom->height - app->render->camera.h / 2) //abajo
						{
							int targetPosY = (-(int)(currentRoom->y + currentRoom->height - app->render->camera.h / 2) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else
						{
							int targetPosY = (-(int)(position.y) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}

						break;

					case ROOM_L_R: //sala derecha
						printf("Derecha \n");
						//X
						if (position.x < currentRoom->x + app->render->camera.w / 2) //izq
						{
							int targetPosX = (-(int)(position.x) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else if (position.x > currentRoom->x + currentRoom->width - app->render->camera.w / 2) //der
						{
							int targetPosX = (-(int)(currentRoom->x + currentRoom->width - app->render->camera.w / 2) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}
						else
						{
							int targetPosX = (-(int)(position.x) * app->win->GetScale() + (windowW / 2) - 10);
							app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
						}

						//Y
						if (position.y < currentRoom->y + app->render->camera.h / 2) //arriba
						{
							int targetPosY = (-(int)(currentRoom->y + app->render->camera.h / 2) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else if (position.y > currentRoom->y + currentRoom->height - app->render->camera.h / 2) //abajo
						{
							int targetPosY = (-(int)(currentRoom->y + currentRoom->height - app->render->camera.h / 2) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}
						else
						{
							int targetPosY = (-(int)(position.y) * app->win->GetScale() + (windowH / 2) - 10);
							app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
						}

						break;

					default:
						printf("isLRoom not found");
						break;
					}
}

			}
			else
			{
				app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
				app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
			}
		}
	}
}

MapObject* Player::GetCurrentRoom()
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

void Player::DetermineRoomType(MapObject* room) {

	float width = room->width;
	float height = room->height;

	if (width > height) {
		roomType = ROOM_RECTANGLE_W;
	}
	else if (height > width) {
		roomType = ROOM_RECTANGLE_H;
	}
	else if (height == width) {

		roomType = ROOM_SQUARE;
	}
	else
	{
		roomType = ROOM_UNKNOWN;
	}
}

void Player::DetermineRoomTypel(MapObject* room) {
	printf("Llamada a DetermineRoomTypel\n");
	if (room->properties.GetProperty("lType") != NULL)
	{
		std::string lType = room->properties.GetProperty("lType")->value;
		printf("El objeto tiene el atributo lType\n");

		if (lType == "dl")
		{
			printf("dl");
			roomType = ROOM_L_DL;
		}
		else if (lType == "dr")
		{
			printf("dr");
			roomType = ROOM_L_DR;
		}
		else if (lType == "ul")
		{
			printf("ul");
			roomType = ROOM_L_UL;
		}
		else if (lType == "ur")
		{
			printf("ur");
			roomType = ROOM_L_UR;
		}
		else if (lType == "d")
		{
			printf("d");
			roomType = ROOM_L_D;
		}
		else if (lType == "u")
		{
			printf("u");
			roomType = ROOM_L_U;
		}
		else if (lType == "l")
		{
			printf("l");
			roomType = ROOM_L_L;
		}
		else if (lType == "r")
		{
			printf("r");
			roomType = ROOM_L_R;
		}
		else
		{
			roomType = ROOM_UNKNOWN;
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


	velocity = b2Vec2(0, 0);
	pbodyFoot->body->SetLinearVelocity(velocity);

	//Moverse a la izquierda
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		velocity.y += -speedFast * dt;
		desiredState = EntityState::RUNNING;
		isFacingLeft = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		velocity.y += speedFast * dt;
		desiredState = EntityState::RUNNING;
		isFacingLeft = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = -speedFast * dt;
		desiredState = EntityState::RUNNING;
		isFacingLeft = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = speedFast * dt;
		desiredState = EntityState::RUNNING;
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
				desiredState = EntityState::RUNNING;
			}
			isFacingLeft = (horizontalMovement < 0);
			lastMovementDirection = fPoint(horizontalMovement, verticalMovement);
		}
	}


	FishingDirecction(verticalMovement, horizontalMovement);

	//Si pulsas espacio
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && timerDash.ReadMSec() > cdTimerDashMS) {

		isDashing = true;
		timerDash.Start();
		desiredState = EntityState::DASHI;
		pbodyFoot->body->ApplyForce(b2Vec2(velocity.x * 100, velocity.y * 100), pbodyFoot->body->GetWorldCenter(), false);

		if(secondaryMask == Mask::MASK2)
		{
			SetPassiveInvisible();
			passiveStats[secondaryMask][maskLevels[secondaryMask]].invisibilityTimer.Start();
		}

		app->audio->StopFx(-1);
		app->audio->PlayFx(dash_fx);
	}

	if (isDashing && dashCollision != nullptr) {
    	dashCollision->body->SetTransform(pbodyFoot->body->GetPosition(), 0);
	}

	if (!(timerDash.ReadMSec() < speed * 1000 && isDashing)) {
		isDashing = false;
		pbodyFoot->body->SetLinearVelocity(velocity);
	}

	if (!(timerDash.ReadMSec() < speed * 1000 && isDashing) && dashCollision != nullptr) {
		isDashing = false;
		pbodyFoot->body->SetLinearVelocity(velocity);
		app->physics->DestroyBody(dashCollision);
		dashCollision = nullptr;
	}

	//Si pulsas J para atacar
	if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN && timerAttack.ReadMSec() > cdTimerAttackMS + 100) {
		isAttacking = true;
		timerAttack.Start();
		desiredState = EntityState::ATTACKING;
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
		desiredState = EntityState::MASK_ATTACK;
	}

	if (!(timerMaskAttack.ReadMSec() < maskStats[primaryMask][maskLevels[primaryMask]].maskCoolDown && isAttackingMask)) {
		isAttackingMask = false;
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
    if (damageTimer.ReadSec() >= damageCooldown) {
        if(dashCollision == nullptr){
            currentStats.currentHealth -= damage;
            isInvisible = false;
            printf("Player has received  %f damage\n", damage);
            damageTimer.Start();
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
//	pbodyFoot->body->SetLinearVelocity(velocity);
//	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
//	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
//	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;
//}


