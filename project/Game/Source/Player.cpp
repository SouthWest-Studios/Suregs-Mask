#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene_Testing.h"
#include "Scene_Pueblo.h"
#include "Scene_GameOver.h"
#include "Scene_Mazmorra0.h"
#include "Log.h"
#include "InventoryManager.h"
#include "NotesManager.h"
#include "Point.h"
#include "Physics.h"
#include "Window.h"
#include "ModuleFadeToBlack.h"
#include "DebugConsole.h"
#include "TreeManager.h"
#include "DialogManager.h"


#include "Fishing.h"


Player::Player() : Entity(EntityType::PLAYER)
{
	name = ("player");
	state = EntityStatePlayer::IDLE;
	nextState = EntityStatePlayer::IDLE;
	currentState = state;
	desiredState = nextState;
	nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;


	//Sword damage
	//swordLevel = 0;

	attackDamagePerLevel[0] = 100;  // Daño de ataque para el nivel 1
	attackDamagePerLevel[1] = 120;  // Daño de ataque para el nivel 2
	attackDamagePerLevel[2] = 140;  // Daño de ataque para el nivel 3
	attackDamagePerLevel[3] = 170;  // Daño de ataque para el nivel 4
	attackDamagePerLevel[4] = 200;  // Daño de ataque para el nivel 5
	attackDamagePerLevel[5] = 240;  // Daño de ataque para el nivel 6
	attackDamagePerLevel[6] = 280;  // Daño de ataque para el nivel 7
	attackDamagePerLevel[7] = 350;  // Daño de ataque para el nivel 8
	attackDamagePerLevel[8] = 420;  // Daño de ataque para el nivel 9
	attackDamagePerLevel[9] = 500;  // Daño de ataque para el nivel 10

	//Armor
	//armorLevel = 0;

	armorPerLevel[0] = 100;  // Armadura para el nivel 1
	armorPerLevel[1] = 120;  // Armadura para el nivel 2
	armorPerLevel[2] = 150;  // Armadura para el nivel 3
	armorPerLevel[3] = 190;  // Armadura para el nivel 4
	armorPerLevel[4] = 250;  // Armadura para el nivel 5
	armorPerLevel[5] = 280;  // Armadura para el nivel 6
	armorPerLevel[6] = 350;  // Armadura para el nivel 7
	armorPerLevel[7] = 400;  // Armadura para el nivel 8
	armorPerLevel[8] = 480;  // Armadura para el nivel 9
	armorPerLevel[9] = 600;  // Armadura para el nivel 10

	// Inicializa las estadísticas base
	baseStats.maxHealth = 100 + armorPerLevel[app->inventoryManager->armorLevel];
	baseStats.currentHealth = 100 + armorPerLevel[app->inventoryManager->armorLevel];
	baseStats.movementSpeed = 100;
	baseStats.attackSpeed = 100;
	baseStats.attackDamage = 100 + attackDamagePerLevel[app->inventoryManager->swordLevel];

	// Inicializa las estadísticas actuales a las estadísticas base
	currentStats = baseStats;

	// Estadísticas de la máscara 0 a nivel 0
	maskStats[Mask::MASK0][Branches::Modifiers][0].attackDamageModifier = 0;
	maskStats[Mask::MASK0][Branches::Modifiers][0].movementSpeedModifier = -10;
	maskStats[Mask::MASK0][Branches::Modifiers][0].attackDamageModifier = 10;
	maskStats[Mask::MASK0][Branches::Modifiers][0].attackSpeedModifier = 10;

	maskStats[Mask::MASK0][Branches::Rama1][0].maskDamage = 50;

	maskStats[Mask::MASK0][Branches::Rama2][0].maskCoolDown = 25000; //En Milisegundos
	maskStats[Mask::MASK0][Branches::Rama2][0].originalmaskCoolDown = 25000; //En Milisegundos

	maskStats[Mask::MASK0][Branches::Rama3][0].firstTimeUsed = false;
	maskStats[Mask::MASK0][Branches::Rama3][0].numberLightning = 1;

	maskStats[Mask::MASK0][Branches::Rama4][0].poisonDamage = 0;
	maskStats[Mask::MASK0][Branches::Rama4][0].poisonDuration = 0;
	maskStats[Mask::MASK0][Branches::Rama4][0].poisonTickRate = 0;

	//Estadísticas de la máscara 0 a nivel 1
	maskStats[Mask::MASK0][Branches::Modifiers][1].maxHealthModifier = 0;
	maskStats[Mask::MASK0][Branches::Modifiers][1].movementSpeedModifier = -10;
	maskStats[Mask::MASK0][Branches::Modifiers][1].attackDamageModifier = 10;
	maskStats[Mask::MASK0][Branches::Modifiers][1].attackSpeedModifier = 10;

	maskStats[Mask::MASK0][Branches::Rama1][1].maskDamage = 80;

	maskStats[Mask::MASK0][Branches::Rama2][1].maskCoolDown = 23000; //En Milisegundos
	maskStats[Mask::MASK0][Branches::Rama2][1].originalmaskCoolDown = 23000; //En Milisegundos

	maskStats[Mask::MASK0][Branches::Rama3][1].firstTimeUsed = false;
	maskStats[Mask::MASK0][Branches::Rama3][1].numberLightning = 2;

	maskStats[Mask::MASK0][Branches::Rama4][1].poisonDamage = 10;
	maskStats[Mask::MASK0][Branches::Rama4][1].poisonDuration = 6.0f;
	maskStats[Mask::MASK0][Branches::Rama4][1].poisonTickRate = 1.5f;

	//Estadísticas de la máscara 0 a nivel 2
	maskStats[Mask::MASK0][Branches::Modifiers][2].maxHealthModifier = 0;
	maskStats[Mask::MASK0][Branches::Modifiers][2].movementSpeedModifier = -10;
	maskStats[Mask::MASK0][Branches::Modifiers][2].attackDamageModifier = 10;
	maskStats[Mask::MASK0][Branches::Modifiers][2].attackSpeedModifier = 10;

	maskStats[Mask::MASK0][Branches::Rama1][2].maskDamage = 130;

	maskStats[Mask::MASK0][Branches::Rama2][2].maskCoolDown = 20000; //En Milisegundos
	maskStats[Mask::MASK0][Branches::Rama2][2].originalmaskCoolDown = 20000; //En Milisegundos

	maskStats[Mask::MASK0][Branches::Rama3][2].firstTimeUsed = false;
	maskStats[Mask::MASK0][Branches::Rama3][2].numberLightning = 3;

	maskStats[Mask::MASK0][Branches::Rama4][2].poisonDamage = 20;
	maskStats[Mask::MASK0][Branches::Rama4][2].poisonDuration = 6.0f;
	maskStats[Mask::MASK0][Branches::Rama4][2].poisonTickRate = 1.5f;

	//Estadísticas de la máscara 0 a nivel 3
	maskStats[Mask::MASK0][Branches::Modifiers][3].maxHealthModifier = 0;
	maskStats[Mask::MASK0][Branches::Modifiers][3].movementSpeedModifier = -10;
	maskStats[Mask::MASK0][Branches::Modifiers][3].attackDamageModifier = 10;
	maskStats[Mask::MASK0][Branches::Modifiers][3].attackSpeedModifier = 10;

	maskStats[Mask::MASK0][Branches::Rama1][3].maskDamage = 200;

	maskStats[Mask::MASK0][Branches::Rama2][3].maskCoolDown = 17000; //En Milisegundos
	maskStats[Mask::MASK0][Branches::Rama2][3].originalmaskCoolDown = 17000; //En Milisegundos

	maskStats[Mask::MASK0][Branches::Rama3][3].firstTimeUsed = false;
	maskStats[Mask::MASK0][Branches::Rama3][3].numberLightning = 4;

	maskStats[Mask::MASK0][Branches::Rama4][3].poisonDamage = 20;
	maskStats[Mask::MASK0][Branches::Rama4][3].poisonDuration = 9.0f;
	maskStats[Mask::MASK0][Branches::Rama4][3].poisonTickRate = 1.5f;

	//Estadísticas de la máscara 0 a nivel 4
	maskStats[Mask::MASK0][Branches::Modifiers][4].maxHealthModifier = 0;
	maskStats[Mask::MASK0][Branches::Modifiers][4].movementSpeedModifier = -10;
	maskStats[Mask::MASK0][Branches::Modifiers][4].attackDamageModifier = 10;
	maskStats[Mask::MASK0][Branches::Modifiers][4].attackSpeedModifier = 10;

	maskStats[Mask::MASK0][Branches::Rama1][4].maskDamage = 300;

	maskStats[Mask::MASK0][Branches::Rama2][4].maskCoolDown = 15000; //En Milisegundos
	maskStats[Mask::MASK0][Branches::Rama2][4].originalmaskCoolDown = 15000; //En Milisegundos

	maskStats[Mask::MASK0][Branches::Rama3][4].firstTimeUsed = false;
	maskStats[Mask::MASK0][Branches::Rama3][4].numberLightning = 5;

	maskStats[Mask::MASK0][Branches::Rama4][4].poisonDamage = 30;
	maskStats[Mask::MASK0][Branches::Rama4][4].poisonDuration = 11.0f;
	maskStats[Mask::MASK0][Branches::Rama4][4].poisonTickRate = 1.5f;


	// Estadísticas de la máscara 1 a nivel 0
	maskStats[Mask::MASK1][Branches::Modifiers][0].maxHealthModifier = -10;
	maskStats[Mask::MASK1][Branches::Modifiers][0].movementSpeedModifier = 10;
	maskStats[Mask::MASK1][Branches::Modifiers][0].attackDamageModifier = 10;
	maskStats[Mask::MASK1][Branches::Modifiers][0].attackSpeedModifier = 0;

	maskStats[Mask::MASK1][Branches::Rama1][0].maskDamage = 100;

	maskStats[Mask::MASK1][Branches::Rama2][0].maskCoolDown = 30000; //En Milisegundos
	maskStats[Mask::MASK1][Branches::Rama2][0].originalmaskCoolDown = 30000; //En Milisegundos

	maskStats[Mask::MASK1][Branches::Rama3][0].rangeBallModifier = 0.0f;
	maskStats[Mask::MASK1][Branches::Rama3][0].firstTimeUsed = false;

	maskStats[Mask::MASK1][Branches::Rama4][0].poisonDamage = 0;
	maskStats[Mask::MASK1][Branches::Rama4][0].poisonDuration = 0;
	maskStats[Mask::MASK1][Branches::Rama4][0].poisonTickRate = 0;

	// Estadísticas de la máscara 1 a nivel 1
	maskStats[Mask::MASK1][Branches::Modifiers][1].maxHealthModifier = -10;
	maskStats[Mask::MASK1][Branches::Modifiers][1].movementSpeedModifier = 10;
	maskStats[Mask::MASK1][Branches::Modifiers][1].attackDamageModifier = 10;
	maskStats[Mask::MASK1][Branches::Modifiers][1].attackSpeedModifier = 0;

	maskStats[Mask::MASK1][Branches::Rama1][1].maskDamage = 120;

	maskStats[Mask::MASK1][Branches::Rama2][1].maskCoolDown = 28000; //En Milisegundos
	maskStats[Mask::MASK1][Branches::Rama2][1].originalmaskCoolDown = 28000; //En Milisegundos

	maskStats[Mask::MASK1][Branches::Rama3][1].rangeBallModifier = 0.05f;
	maskStats[Mask::MASK1][Branches::Rama3][1].firstTimeUsed = false;

	maskStats[Mask::MASK1][Branches::Rama4][1].poisonDamage = 5;
	maskStats[Mask::MASK1][Branches::Rama4][1].poisonDuration = 5.0f;
	maskStats[Mask::MASK1][Branches::Rama4][1].poisonTickRate = 1.5f;

	// Estadísticas de la máscara 1 a nivel 2
	maskStats[Mask::MASK1][Branches::Modifiers][2].maxHealthModifier = -10;
	maskStats[Mask::MASK1][Branches::Modifiers][2].movementSpeedModifier = 10;
	maskStats[Mask::MASK1][Branches::Modifiers][2].attackDamageModifier = 10;
	maskStats[Mask::MASK1][Branches::Modifiers][2].attackSpeedModifier = 0;

	maskStats[Mask::MASK1][Branches::Rama1][2].maskDamage = 180;

	maskStats[Mask::MASK1][Branches::Rama2][2].maskCoolDown = 25000; //En Milisegundos
	maskStats[Mask::MASK1][Branches::Rama2][2].originalmaskCoolDown = 25000; //En Milisegundos

	maskStats[Mask::MASK1][Branches::Rama3][2].rangeBallModifier = 0.10f;
	maskStats[Mask::MASK1][Branches::Rama3][2].firstTimeUsed = false;

	maskStats[Mask::MASK1][Branches::Rama4][2].poisonDamage = 10;
	maskStats[Mask::MASK1][Branches::Rama4][2].poisonDuration = 5.0f;
	maskStats[Mask::MASK1][Branches::Rama4][2].poisonTickRate = 1.5f;

	// Estadísticas de la máscara 1 a nivel 3
	maskStats[Mask::MASK1][Branches::Modifiers][3].maxHealthModifier = -10;
	maskStats[Mask::MASK1][Branches::Modifiers][3].movementSpeedModifier = 10;
	maskStats[Mask::MASK1][Branches::Modifiers][3].attackDamageModifier = 10;
	maskStats[Mask::MASK1][Branches::Modifiers][3].attackSpeedModifier = 0;

	maskStats[Mask::MASK1][Branches::Rama1][3].maskDamage = 250;

	maskStats[Mask::MASK1][Branches::Rama2][3].maskCoolDown = 23000; //En Milisegundos
	maskStats[Mask::MASK1][Branches::Rama2][3].originalmaskCoolDown = 23000; //En Milisegundos

	maskStats[Mask::MASK1][Branches::Rama3][3].rangeBallModifier = 0.15f;
	maskStats[Mask::MASK1][Branches::Rama3][3].firstTimeUsed = false;

	maskStats[Mask::MASK1][Branches::Rama4][3].poisonDamage = 10;
	maskStats[Mask::MASK1][Branches::Rama4][3].poisonDuration = 10.0f;
	maskStats[Mask::MASK1][Branches::Rama4][3].poisonTickRate = 1.5f;

	// Estadísticas de la máscara 1 a nivel 4
	maskStats[Mask::MASK1][Branches::Modifiers][4].maxHealthModifier = -10;
	maskStats[Mask::MASK1][Branches::Modifiers][4].movementSpeedModifier = 10;
	maskStats[Mask::MASK1][Branches::Modifiers][4].attackDamageModifier = 10;
	maskStats[Mask::MASK1][Branches::Modifiers][4].attackSpeedModifier = 0;

	maskStats[Mask::MASK1][Branches::Rama1][4].maskDamage = 400;

	maskStats[Mask::MASK1][Branches::Rama2][4].maskCoolDown = 20000; //En Milisegundos
	maskStats[Mask::MASK1][Branches::Rama2][4].originalmaskCoolDown = 20000; //En Milisegundos

	maskStats[Mask::MASK1][Branches::Rama3][4].rangeBallModifier = 0.20f; // 20% de aumento en el rango
	maskStats[Mask::MASK1][Branches::Rama3][4].firstTimeUsed = false;

	maskStats[Mask::MASK1][Branches::Rama4][4].poisonDamage = 15;
	maskStats[Mask::MASK1][Branches::Rama4][4].poisonDuration = 15.0f;
	maskStats[Mask::MASK1][Branches::Rama4][4].poisonTickRate = 1.5f;


	//Estadísticas de la máscara 2 a nivel 0
	maskStats[Mask::MASK2][Branches::Modifiers][0].maxHealthModifier = -20;
	maskStats[Mask::MASK2][Branches::Modifiers][0].movementSpeedModifier = 10;
	maskStats[Mask::MASK2][Branches::Modifiers][0].attackDamageModifier = 10;
	maskStats[Mask::MASK2][Branches::Modifiers][0].attackSpeedModifier = 10;

	maskStats[Mask::MASK2][Branches::Rama1][0].maskDamage = 40.0f;

	maskStats[Mask::MASK2][Branches::Rama2][0].maskCoolDown = 20000.0f;
	maskStats[Mask::MASK2][Branches::Rama2][0].originalmaskCoolDown = 20000.0f;

	maskStats[Mask::MASK2][Branches::Rama3][0].distanceDashModifier = 1.30f;

	maskStats[Mask::MASK2][Branches::Rama4][0].invisibilityDuration = 5.0f;

	//Estadísticas de la máscara 2 a nivel 1
	maskStats[Mask::MASK2][Branches::Modifiers][1].maxHealthModifier = -20;
	maskStats[Mask::MASK2][Branches::Modifiers][1].movementSpeedModifier = 10;
	maskStats[Mask::MASK2][Branches::Modifiers][1].attackDamageModifier = 10;
	maskStats[Mask::MASK2][Branches::Modifiers][1].attackSpeedModifier = 10;

	maskStats[Mask::MASK2][Branches::Rama1][1].maskDamage = 50.0f;

	maskStats[Mask::MASK2][Branches::Rama2][1].maskCoolDown = 18000.0f;
	maskStats[Mask::MASK2][Branches::Rama2][1].originalmaskCoolDown = 18000.0f;

	maskStats[Mask::MASK2][Branches::Rama3][1].distanceDashModifier = 1.35f;

	maskStats[Mask::MASK2][Branches::Rama4][1].invisibilityDuration = 6.0f;

	//Estadísticas de la máscara 2 a nivel 2
	maskStats[Mask::MASK2][Branches::Modifiers][2].maxHealthModifier = -20;
	maskStats[Mask::MASK2][Branches::Modifiers][2].movementSpeedModifier = 10;
	maskStats[Mask::MASK2][Branches::Modifiers][2].attackDamageModifier = 10;
	maskStats[Mask::MASK2][Branches::Modifiers][2].attackSpeedModifier = 10;

	maskStats[Mask::MASK2][Branches::Rama1][2].maskDamage = 70.0f;

	maskStats[Mask::MASK2][Branches::Rama2][2].maskCoolDown = 16000.0f;
	maskStats[Mask::MASK2][Branches::Rama2][2].originalmaskCoolDown = 16000.0f;

	maskStats[Mask::MASK2][Branches::Rama3][2].distanceDashModifier = 1.40f;

	maskStats[Mask::MASK2][Branches::Rama4][2].invisibilityDuration = 7.3f;

	//Estadísticas de la máscara 2 a nivel 3
	maskStats[Mask::MASK2][Branches::Modifiers][3].maxHealthModifier = -20;
	maskStats[Mask::MASK2][Branches::Modifiers][3].movementSpeedModifier = 10;
	maskStats[Mask::MASK2][Branches::Modifiers][3].attackDamageModifier = 10;
	maskStats[Mask::MASK2][Branches::Modifiers][3].attackSpeedModifier = 10;

	maskStats[Mask::MASK2][Branches::Rama1][3].maskDamage = 100.0f;

	maskStats[Mask::MASK2][Branches::Rama2][3].maskCoolDown = 18000.0f;
	maskStats[Mask::MASK2][Branches::Rama2][3].originalmaskCoolDown = 18000.0f;

	maskStats[Mask::MASK2][Branches::Rama3][3].distanceDashModifier = 1.45f;

	maskStats[Mask::MASK2][Branches::Rama4][3].invisibilityDuration = 8.6f;

	//Estadísticas de la máscara 2 a nivel 4
	maskStats[Mask::MASK2][Branches::Modifiers][4].maxHealthModifier = -20;
	maskStats[Mask::MASK2][Branches::Modifiers][4].movementSpeedModifier = 10;
	maskStats[Mask::MASK2][Branches::Modifiers][4].attackDamageModifier = 10;
	maskStats[Mask::MASK2][Branches::Modifiers][4].attackSpeedModifier = 10;

	maskStats[Mask::MASK2][Branches::Rama1][4].maskDamage = 150.0f;

	maskStats[Mask::MASK2][Branches::Rama2][4].maskCoolDown = 10000.0f;
	maskStats[Mask::MASK2][Branches::Rama2][4].originalmaskCoolDown = 10000.0f;

	maskStats[Mask::MASK2][Branches::Rama3][4].distanceDashModifier = 1.50f;

	maskStats[Mask::MASK2][Branches::Rama4][4].invisibilityDuration = 10.0f;


	//Estadísticas de la máscara 3 a nivel 0
	maskStats[Mask::MASK3][Branches::Modifiers][0].maxHealthModifier = -5;
	maskStats[Mask::MASK3][Branches::Modifiers][0].movementSpeedModifier = -5;
	maskStats[Mask::MASK3][Branches::Modifiers][0].attackDamageModifier = -5;
	maskStats[Mask::MASK3][Branches::Modifiers][0].attackSpeedModifier = -5;

	maskStats[Mask::MASK3][Branches::Rama1][0].maskDamage = 0;

	maskStats[Mask::MASK3][Branches::Rama2][0].maskCoolDown = 60000.0f;
	maskStats[Mask::MASK3][Branches::Rama2][0].originalmaskCoolDown = 60000.0f;

	maskStats[Mask::MASK3][Branches::Rama3][0].durationTime = 20000.0f;

	maskStats[Mask::MASK3][Branches::Rama4][0].maxActiveHealthModifier = 0.10f; //10%
	maskStats[Mask::MASK3][Branches::Rama4][0].maxActiveDamageModifier = 0.10f; //10%

	//Estadísticas de la máscara 3 a nivel 1
	maskStats[Mask::MASK3][Branches::Modifiers][1].maxHealthModifier = -5;
	maskStats[Mask::MASK3][Branches::Modifiers][1].movementSpeedModifier = -5;
	maskStats[Mask::MASK3][Branches::Modifiers][1].attackDamageModifier = -5;
	maskStats[Mask::MASK3][Branches::Modifiers][1].attackSpeedModifier = -5;

	maskStats[Mask::MASK3][Branches::Rama1][1].maskDamage = 0;

	maskStats[Mask::MASK3][Branches::Rama2][1].maskCoolDown = 58000.0f;
	maskStats[Mask::MASK3][Branches::Rama2][1].originalmaskCoolDown = 58000.0f;

	maskStats[Mask::MASK3][Branches::Rama3][1].durationTime = 22000.0f;

	maskStats[Mask::MASK3][Branches::Rama4][1].maxActiveHealthModifier = 0.15f;
	maskStats[Mask::MASK3][Branches::Rama4][1].maxActiveDamageModifier = 0.15f;

	//Estadísticas de la máscara 3 a nivel 2
	maskStats[Mask::MASK3][Branches::Modifiers][2].maxHealthModifier = -5;
	maskStats[Mask::MASK3][Branches::Modifiers][2].movementSpeedModifier = -5;
	maskStats[Mask::MASK3][Branches::Modifiers][2].attackDamageModifier = -5;
	maskStats[Mask::MASK3][Branches::Modifiers][2].attackSpeedModifier = -5;

	maskStats[Mask::MASK3][Branches::Rama1][2].maskDamage = 0;

	maskStats[Mask::MASK3][Branches::Rama2][2].maskCoolDown = 56000.0f;
	maskStats[Mask::MASK3][Branches::Rama2][2].originalmaskCoolDown = 56000.0f;

	maskStats[Mask::MASK3][Branches::Rama3][2].durationTime = 25000.0f;

	maskStats[Mask::MASK3][Branches::Rama4][2].maxActiveHealthModifier = 0.20f;
	maskStats[Mask::MASK3][Branches::Rama4][2].maxActiveDamageModifier = 0.20f;

	//Estadísticas de la máscara 3 a nivel 3
	maskStats[Mask::MASK3][Branches::Modifiers][3].maxHealthModifier = -5;
	maskStats[Mask::MASK3][Branches::Modifiers][3].movementSpeedModifier = -5;
	maskStats[Mask::MASK3][Branches::Modifiers][3].attackDamageModifier = -5;
	maskStats[Mask::MASK3][Branches::Modifiers][3].attackSpeedModifier = -5;

	maskStats[Mask::MASK3][Branches::Rama1][3].maskDamage = 0;

	maskStats[Mask::MASK3][Branches::Rama2][3].maskCoolDown = 53000.0f;
	maskStats[Mask::MASK3][Branches::Rama2][3].originalmaskCoolDown = 53000.0f;

	maskStats[Mask::MASK3][Branches::Rama3][3].durationTime = 27000.0f;

	maskStats[Mask::MASK3][Branches::Rama4][3].maxActiveHealthModifier = 0.25f;
	maskStats[Mask::MASK3][Branches::Rama4][3].maxActiveDamageModifier = 0.25f;

	//Estadísticas de la máscara 3 a nivel 4
	maskStats[Mask::MASK3][Branches::Modifiers][4].maxHealthModifier = -5;
	maskStats[Mask::MASK3][Branches::Modifiers][4].movementSpeedModifier = -5;
	maskStats[Mask::MASK3][Branches::Modifiers][4].attackDamageModifier = -5;
	maskStats[Mask::MASK3][Branches::Modifiers][4].attackSpeedModifier = -5;

	maskStats[Mask::MASK3][Branches::Rama1][4].maskDamage = 0;

	maskStats[Mask::MASK3][Branches::Rama2][4].maskCoolDown = 50000.0f;
	maskStats[Mask::MASK3][Branches::Rama2][4].originalmaskCoolDown = 50000.0f;

	maskStats[Mask::MASK3][Branches::Rama3][4].durationTime = 30000.0f;

	maskStats[Mask::MASK3][Branches::Rama4][4].maxActiveHealthModifier = 0.35f;
	maskStats[Mask::MASK3][Branches::Rama4][4].maxActiveDamageModifier = 0.35f;


	//Estadísticas pasivas mascara 0 a nivel 0
	passiveStats[Mask::MASK0][Branches::Modifiers][0].damageBoost = 20;
	passiveStats[Mask::MASK0][Branches::Modifiers][0].rangeBoost = 0;
	passiveStats[Mask::MASK0][Branches::Modifiers][0].dashBoost = 0;

	//Estadísticas pasivas mascara 0 a nivel 1
	passiveStats[Mask::MASK0][Branches::Modifiers][1].damageBoost = 20;
	passiveStats[Mask::MASK0][Branches::Modifiers][1].rangeBoost = 0;
	passiveStats[Mask::MASK0][Branches::Modifiers][1].dashBoost = 0;

	//Estadísticas pasivas mascara 0 a nivel 2
	passiveStats[Mask::MASK0][Branches::Modifiers][2].damageBoost = 20;
	passiveStats[Mask::MASK0][Branches::Modifiers][2].rangeBoost = 0;
	passiveStats[Mask::MASK0][Branches::Modifiers][2].dashBoost = 0;
	//Estadísticas pasivas mascara 0 a nivel 3
	passiveStats[Mask::MASK0][Branches::Modifiers][3].damageBoost = 20;
	passiveStats[Mask::MASK0][Branches::Modifiers][3].rangeBoost = 0;
	passiveStats[Mask::MASK0][Branches::Modifiers][3].dashBoost = 0;
	//Estadísticas pasivas mascara 0 a nivel 4
	passiveStats[Mask::MASK0][Branches::Modifiers][4].damageBoost = 20;
	passiveStats[Mask::MASK0][Branches::Modifiers][4].rangeBoost = 0;
	passiveStats[Mask::MASK0][Branches::Modifiers][4].dashBoost = 0;


	//Estadísticas pasivas mascara 1 a nivel 0
	passiveStats[Mask::MASK1][Branches::Modifiers][0].damageBoost = 0.33;
	passiveStats[Mask::MASK1][Branches::Modifiers][0].rangeBoost = 25;
	passiveStats[Mask::MASK1][Branches::Modifiers][0].dashBoost = 0;

	//Estadísticas pasivas mascara 1 a nivel 1
	passiveStats[Mask::MASK1][Branches::Modifiers][1].damageBoost = 0.33;
	passiveStats[Mask::MASK1][Branches::Modifiers][1].rangeBoost = 25;
	passiveStats[Mask::MASK1][Branches::Modifiers][1].dashBoost = 0;

	//Estadísticas pasivas mascara 1 a nivel 2
	passiveStats[Mask::MASK1][Branches::Modifiers][2].damageBoost = 0.33;
	passiveStats[Mask::MASK1][Branches::Modifiers][2].rangeBoost = 25;
	passiveStats[Mask::MASK1][Branches::Modifiers][2].dashBoost = 0;

	//Estadísticas pasivas mascara 1 a nivel 3
	passiveStats[Mask::MASK1][Branches::Modifiers][3].damageBoost = 0.33;
	passiveStats[Mask::MASK1][Branches::Modifiers][3].rangeBoost = 25;
	passiveStats[Mask::MASK1][Branches::Modifiers][3].dashBoost = 0;

	//Estadísticas pasivas mascara 1 a nivel 4
	passiveStats[Mask::MASK1][Branches::Modifiers][4].damageBoost = 0.33;
	passiveStats[Mask::MASK1][Branches::Modifiers][4].rangeBoost = 25;
	passiveStats[Mask::MASK1][Branches::Modifiers][4].dashBoost = 0;


	//Estadísticas pasivas mascara 2 a nivel 0
	passiveStats[Mask::MASK2][Branches::Modifiers][0].damageBoost = 0;
	passiveStats[Mask::MASK2][Branches::Modifiers][0].rangeBoost = 0;
	passiveStats[Mask::MASK2][Branches::Modifiers][0].dashBoost = 2;
	passiveStats[Mask::MASK2][Branches::Modifiers][0].invisibilityDuration = 2000.0f;

	//Estadísticas pasivas mascara 2 a nivel 1
	passiveStats[Mask::MASK2][Branches::Modifiers][1].damageBoost = 0;
	passiveStats[Mask::MASK2][Branches::Modifiers][1].rangeBoost = 0;
	passiveStats[Mask::MASK2][Branches::Modifiers][1].dashBoost = 2;
	passiveStats[Mask::MASK2][Branches::Modifiers][1].invisibilityDuration = 2000.0f;

	//Estadísticas pasivas mascara 2 a nivel 2
	passiveStats[Mask::MASK2][Branches::Modifiers][2].damageBoost = 0;
	passiveStats[Mask::MASK2][Branches::Modifiers][2].rangeBoost = 0;
	passiveStats[Mask::MASK2][Branches::Modifiers][2].dashBoost = 2;
	passiveStats[Mask::MASK2][Branches::Modifiers][2].invisibilityDuration = 2000.0f;

	//Estadísticas pasivas mascara 2 a nivel 3
	passiveStats[Mask::MASK2][Branches::Modifiers][3].damageBoost = 0;
	passiveStats[Mask::MASK2][Branches::Modifiers][3].rangeBoost = 0;
	passiveStats[Mask::MASK2][Branches::Modifiers][3].dashBoost = 2;
	passiveStats[Mask::MASK2][Branches::Modifiers][3].invisibilityDuration = 2000.0f;

	//Estadísticas pasivas mascara 2 a nivel 4
	passiveStats[Mask::MASK2][Branches::Modifiers][4].damageBoost = 0;
	passiveStats[Mask::MASK2][Branches::Modifiers][4].rangeBoost = 0;
	passiveStats[Mask::MASK2][Branches::Modifiers][4].dashBoost = 2;
	passiveStats[Mask::MASK2][Branches::Modifiers][4].invisibilityDuration = 2000.0f;


	//Estadísticas pasivas mascara 3 a level 0
	passiveStats[Mask::MASK3][Branches::Modifiers][0].damageBoost = 0.20f; //20% de aumento en el daño
	passiveStats[Mask::MASK3][Branches::Modifiers][0].velocityBoost = 0.10f; //10% de aumento en la velocidad

	//Estadísticas pasivas mascara 3 a level 1
	passiveStats[Mask::MASK3][Branches::Modifiers][1].damageBoost = 0.20f;
	passiveStats[Mask::MASK3][Branches::Modifiers][1].velocityBoost = 0.10f;

	//Estadísticas pasivas mascara 3 a level 2
	passiveStats[Mask::MASK3][Branches::Modifiers][2].damageBoost = 0.20f;
	passiveStats[Mask::MASK3][Branches::Modifiers][2].velocityBoost = 0.10f;

	//Estadísticas pasivas mascara 3 a level 3
	passiveStats[Mask::MASK3][Branches::Modifiers][3].damageBoost = 0.20f;
	passiveStats[Mask::MASK3][Branches::Modifiers][3].velocityBoost = 0.10f;

	//Estadísticas pasivas mascara 3 a level 4
	passiveStats[Mask::MASK3][Branches::Modifiers][4].damageBoost = 0.20f;
	passiveStats[Mask::MASK3][Branches::Modifiers][4].velocityBoost = 0.10f;

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

	idle_player.LoadAnim("player", "idle_player", spritePositions);
	run_player.LoadAnim("player", "run_player", spritePositions);
	dashi_player.LoadAnim("player", "dashi_player", spritePositions);
	atack1_player.LoadAnim("player", "atack1_player", spritePositions);
	atack2_player.LoadAnim("player", "atack2_player", spritePositions);
	atack3_player.LoadAnim("player", "atack3_player", spritePositions);
	dead_player.LoadAnim("player", "dead_player", spritePositions);
	pocion_player.LoadAnim("player", "pocion_player", spritePositions);
	maskBola_player.LoadAnim("player", "maskBola_player", spritePositions);
	maskRayo_player.LoadAnim("player", "maskRayo_player", spritePositions);
	takeDMG_player.LoadAnim("player", "takeDMG_player", spritePositions);


	texture = app->tex->Load(config.attribute("texturePath").as_string());
	app->entityManager->objectsToDraw.push_back({
		texture,
		position.x, // x
		position.y, // y
		position.y + 236, // anchorY
		300, // width
		250, // height
		NULL, // currentFrame
		isFacingLeft,
		true
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
	basic_combo_attack2_fx = app->audio->LoadAudioFx("basic_combo_attack2_fx");
	basic_combo_attack3_fx = app->audio->LoadAudioFx("basic_combo_attack3_fx");
	basic_combo_attack3Alt_fx = app->audio->LoadAudioFx("basic_combo_attack3Alt_fx");
	basic_combo_attack3Alt2_fx = app->audio->LoadAudioFx("basic_combo_attack3Alt2_fx");
	player_get_damage_fx = app->audio->LoadAudioFx("player_get_damage_fx");
	player_death_fx = app->audio->LoadAudioFx("player_death_fx");
	get_item_fx = app->audio->LoadAudioFx("get_item_fx");

	cdTimerDashMS = 500;
	cdTimerAttackMS = 30000 / currentStats.attackSpeed;

	player_Direction = Direction::UNKNOWN;


	EquipPrimaryMask(Mask::NOMASK);
	EquipSecondaryMask(Mask::NOMASK);

	/*	------------ALEIX------------
	PARA MEJORAR X RAMA DE X MASCARA SERIA ASI

	app->entityManager->GetPlayer()->maskLevels[Mask::MASK0][Branches::Rama1] += 1;

	Siendo [Mask::MASK0] -> El numero de la mascara (0,1,2,3)
	Siendo [Branches::Rama1] -> El numero de la rama (1,2,3,4), según el excel de arriba a abajo (1 = daño, 2 = cooldown, 3 = mejora de habilidad principal, 4 =  veneno/invisible/aumento)

		------------ALEIX------------ */


		//PARA TESTEAR
	currentStats = baseStats;
	printf("Max Health: %f\n", currentStats.maxHealth);
	printf("Current Health: %f\n", currentStats.currentHealth);
	printf("Movement Speed: %f\n", currentStats.movementSpeed);
	printf("Attack Speed: %f\n", currentStats.attackSpeed);
	printf("Attack Damage: %f\n", currentStats.attackDamage);

	maskLevels[primaryMask][Branches::Modifiers] = 0;
	maskLevels[primaryMask][Branches::Rama1] = 0;
	maskLevels[primaryMask][Branches::Rama2] = 0;
	maskLevels[primaryMask][Branches::Rama3] = 0;
	maskLevels[primaryMask][Branches::Rama4] = 0;

	maskLevels[secondaryMask][Branches::Modifiers] = 0;
	maskLevels[secondaryMask][Branches::Rama1] = 0;
	maskLevels[secondaryMask][Branches::Rama2] = 0;
	maskLevels[secondaryMask][Branches::Rama3] = 0;
	maskLevels[secondaryMask][Branches::Rama4] = 0;

	printf("Primary mask: %d, Level Rama1: %d, Level Rama2: %d, Level Rama3: %d, Level Rama4: %d\n",
		static_cast<int>(primaryMask) - 1,
		maskLevels[primaryMask][Branches::Rama1],
		maskLevels[primaryMask][Branches::Rama2],
		maskLevels[primaryMask][Branches::Rama3],
		maskLevels[primaryMask][Branches::Rama4]);
	printf("Secondary mask: %d, Level Rama1: %d, Level Rama2: %d, Level Rama3: %d, Level Rama4: %d\n",
		static_cast<int>(secondaryMask) - 1,
		maskLevels[secondaryMask][Branches::Rama1],
		maskLevels[secondaryMask][Branches::Rama2],
		maskLevels[secondaryMask][Branches::Rama3],
		maskLevels[secondaryMask][Branches::Rama4]);

	baseStats.maxHealth = 100 + armorPerLevel[app->inventoryManager->armorLevel];
	baseStats.currentHealth = 100 + armorPerLevel[app->inventoryManager->armorLevel];
	baseStats.attackDamage = 100 + attackDamagePerLevel[app->inventoryManager->swordLevel];

    if (primaryMask != Mask::NOMASK) {
		currentStats.maxHealth = baseStats.maxHealth * (1 + maskStats[primaryMask][Branches::Modifiers][maskLevels[primaryMask][Branches::Modifiers]].maxHealthModifier / 100);
		currentStats.movementSpeed = baseStats.movementSpeed * (1 + maskStats[primaryMask][Branches::Modifiers][maskLevels[primaryMask][Branches::Modifiers]].movementSpeedModifier / 100);
		currentStats.attackSpeed = baseStats.attackSpeed * (1 + maskStats[primaryMask][Branches::Modifiers][maskLevels[primaryMask][Branches::Modifiers]].attackSpeedModifier / 100);
		currentStats.attackDamage = baseStats.attackDamage * (1 + maskStats[primaryMask][Branches::Modifiers][maskLevels[primaryMask][Branches::Modifiers]].attackDamageModifier / 100);
 	}

    if (secondaryMask == Mask::MASK0) {
		currentStats.attackDamage *= (1 + passiveStats[secondaryMask][Branches::Modifiers][maskLevels[secondaryMask][Branches::Modifiers]].damageBoost / 100);
    }

	currentStats.currentHealth = currentStats.maxHealth;

	app->tex->GetSize(texture, texW, texH);

	return true;
}

bool Player::Update(float dt)
{
	//LOG("Playerpos: x:%d y:%d", position.x, position.y);
	UpdateStats();



	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
	pbodySensor->body->SetTransform(b2Vec2(pbodyPos.p.x, pbodyPos.p.y - 1), 0);

	if (!inAnimation) {
		desiredState = EntityStatePlayer::IDLE;
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godmode = !godmode;
		pbodyFoot->body->GetFixtureList()[0].SetSensor(godmode);
	}
	ResetAnimacion();

	//Animacion
	if (inTakeDMG) {
		//printf("\n PlayerTakeDMG");
		currentAnimation = &takeDMG_player;
	}


	if (atack_Anim) {
		if (atackNum == 1) {
			currentAnimation = &atack1_player;
		}
		else if (atackNum == 2) {
			currentAnimation = &atack2_player;
		}
		else if (atackNum == 3) {
			currentAnimation = &atack3_player;
			atackNum = 0;
		}
	}

	if (inPocionAnim) {
		currentAnimation = &pocion_player;
	}
	//EndAnimacion

	if (godmode) { GodMode(dt); }
	else if (!app->dialogManager->isPlaying) {
		PlayerMovement(dt);
		
	}
	else {
		pbodyFoot->body->SetLinearVelocity(b2Vec2_zero);
	}

	CameraMovement(dt);


	
	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {
		desiredState = EntityStatePlayer::POCION;
	}

	/*printf("\nposx:%d, posy: %d",position.x, position.y);*/

	if (maskStats[primaryMask][Branches::Rama4][maskLevels[primaryMask][Branches::Rama4]].invisibilityTimer.ReadSec() > maskStats[primaryMask][Branches::Rama4][maskLevels[primaryMask][Branches::Rama4]].invisibilityDuration) {
		SDL_SetTextureAlphaMod(texture, 255);
		isInvisible = false;
		maskStats[primaryMask][Branches::Rama4][maskLevels[primaryMask][Branches::Rama4]].invisibilityTimer.Start();

	}
	if (passiveStats[secondaryMask][Branches::Modifiers][maskLevels[secondaryMask][Branches::Modifiers]].invisibilityTimer.ReadSec() > passiveStats[secondaryMask][Branches::Modifiers][maskLevels[secondaryMask][Branches::Modifiers]].invisibilityDuration) {
		SDL_SetTextureAlphaMod(texture, 255);
		isInvisible = false;
		passiveStats[secondaryMask][Branches::Modifiers][maskLevels[secondaryMask][Branches::Modifiers]].invisibilityTimer.Start();

	}
	if (!isInvisible) {
		SDL_SetTextureAlphaMod(texture, 255);
	}

	if (mask3Timer.ReadMSec() >= maskStats[Mask::MASK3][Branches::Rama3][maskLevels[Mask::MASK3][Branches::Rama3]].durationTime && mask3Active) {
		// Revertir las estadísticas a sus valores originales
		currentStats.attackDamage = baseStats.attackDamage * (1 + maskStats[Mask::MASK3][Branches::Rama4][maskLevels[Mask::MASK3][Branches::Rama4]].maxActiveDamageModifier / 100);
		currentStats.maxHealth = baseStats.maxHealth * (1 + maskStats[Mask::MASK3][Branches::Rama4][maskLevels[Mask::MASK3][Branches::Rama4]].maxActiveHealthModifier / 100);
		mask3Active = false;
	}

	if (secondaryMask == Mask::MASK3) {
		currentStats.attackDamage = baseStats.attackDamage * (1 + passiveStats[Mask::MASK3][Branches::Modifiers][maskLevels[Mask::MASK3][Branches::Modifiers]].damageBoost);
		currentStats.movementSpeed = baseStats.movementSpeed * (1 + passiveStats[Mask::MASK3][Branches::Modifiers][maskLevels[Mask::MASK3][Branches::Modifiers]].velocityBoost);
	}

	//Mask XP System

	//MASK 0
	if (maskZeroXP >= XPtoLevelUpZero && maskZeroLevel < maxMaskLevel) {
		maskZeroXP -= XPtoLevelUpZero;
		maskZeroPoints++;
		maskZeroLevel++;
		XPtoLevelUpZero += matrizLevelXP[maskZeroLevel];

		printf("Has subido la mask 0 a nivel %i y su experiencia actual es %i \n", maskZeroLevel, maskZeroXP);
	}

	//MASK 1
	if (maskOneXP >= XPtoLevelUpOne && maskOneLevel < maxMaskLevel) {
		maskOneXP -= XPtoLevelUpOne;
		maskOnePoints++;
		maskOneLevel++;
		XPtoLevelUpOne += matrizLevelXP[maskOneLevel];

		printf("Has subido la mask 1 a nivel %i y su experiencia actual es %i \n", maskOneLevel, maskOneXP);
	}

	//MASK 2
	if (maskTwoXP >= XPtoLevelUpTwo && maskTwoLevel < maxMaskLevel) {
		maskTwoXP -= XPtoLevelUpTwo;
		maskTwoPoints++;
		maskTwoLevel++;
		XPtoLevelUpTwo += matrizLevelXP[maskTwoLevel];

		printf("Has subido la mask 2 a nivel %i y su experiencia actual es %i \n", maskTwoLevel, maskTwoXP);
	}

	//MASK 3
	if (maskThreeXP >= XPtoLevelUpThree && maskThreeLevel < maxMaskLevel) {
		maskThreeXP -= XPtoLevelUpThree;
		maskThreePoints++;
		maskThreeLevel++;
		XPtoLevelUpThree += matrizLevelXP[maskThreeLevel];

		printf("Has subido la mask 3 a nivel %i y su experiencia actual es %i \n", maskThreeLevel, maskThreeXP);
	}

	//if (currentState == EntityStatePlayer::DEAD && !particulaBlood) {
	//	fPoint pos((float)position.x, (float)position.y);
	//	blood = app->psystem->AddEmiter(pos, EMITTER_TYPE_SPARK);
	//	particulaBlood = true;
	//}
	EfectoPociones(dt);

	stateMachine(dt);
	currentAnimation->Update();

	return true;
}


bool Player::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idle_player; }
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
	app->entityManager->DestroyEntity(this);
	app->entityManager->SetPlayer(nullptr);
	/*app->physics->GetWorld()->DestroyBody(pbodyFoot->body);
	app->physics->GetWorld()->DestroyBody(pbodySensor->body);*/
	app->physics->DestroyBody(pbodyFoot);
	app->physics->DestroyBody(pbodySensor);
	/*app->tex->UnLoad(texture);*/
	app->tex->UnLoad(texture);
	DeadTP = false;
	dead_player.Reset();
	blood = nullptr;
	app->psystem->RemoveAllEmitters();
	particulaBlood = false;

	RELEASE(spritePositions);
	delete spritePositions;

	return true;
}


void Player::ResetAnimacion()
{
	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "dashi_player") {
		inAnimation = false;
		dashi_player.Reset();
	}

	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "atack1_player") {
		inAnimation = false;
		atack_Anim = false;
		desiredState = EntityStatePlayer::IDLE;
		atack1_player.Reset();
	}
	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "atack2_player") {
		inAnimation = false;
		atack_Anim = false;
		desiredState = EntityStatePlayer::IDLE;
		atack2_player.Reset();
	}
	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "atack3_player") {
		inAnimation = false;
		atack_Anim = false;
		desiredState = EntityStatePlayer::IDLE;
		atack3_player.Reset();
	}

	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "dead_player") {
		if (!DeadTP) {
			TimerColdDown.Start();
		}
		DeadTP = true;
	}

	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "takeDMG_player") {
		inAnimation = false;
		inTakeDMG = false;
		//desiredState = EntityStatePlayer::IDLE;
		takeDMG_player.Reset();
	}
	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "maskBola_player") {
		printf("\maskBola_player");
		inAnimation = false;
		//desiredState = EntityStatePlayer::IDLE;
		takeDMG_player.Reset();
		desiredState = EntityStatePlayer::IDLE;


		if (mask1AttackSensor != nullptr) {
			mask1AttackSensor->body->SetLinearVelocity(b2Vec2(0, 0));
			mask1AttackSensor->body->GetWorld()->DestroyBody(mask1AttackSensor->body);
			//app->physics->GetWorld()->DestroyBody(mask1AttackSensor->body);
			mask1AttackSensor = nullptr;
			printf("delete");
		}

	}

	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "maskRayo_player") {
		printf("\maskRayo_player");
		inAnimation = false;
		maskRayo_player.Reset();
		desiredState = EntityStatePlayer::IDLE;
	}

	if (currentAnimation->HasFinished() && currentAnimation->getNameAnimation() == "pocion_player") {
		printf("\pocion_player");
		inAnimation = false;
		inPocionAnim = false;
		pocion_player.Reset();
		desiredState = EntityStatePlayer::IDLE;
	}
	
}

void Player::DoNothing(float dt)
{
	currentAnimation = &idle_player;
	app->audio->StopFx(1);
}

float Player::GetRealMovementSpeed() const {
	return 0.2f * (currentStats.movementSpeed / 100.0f);
}

void Player::Run(float dt)
{
	currentAnimation = &run_player;
	app->audio->PlayRunFx(run_fx, runAlt_fx, runAlt2_fx);
}

void Player::Dashi(float dt)
{

	inAnimation = true;
	currentAnimation = &dashi_player;

}

void Player::EfectoPociones(float dt)
{
	Regenpocion(dt);
	Damagepocion(dt);
	Speedpocion(dt);
}

void Player::Regenpocion(float dt)
{
	if (currentStats.currentHealth <= currentStats.maxHealth)
	{
		if (regenAmount > 0)
		{
			if (regenTimer.ReadSec() >= 0.2f)
			{
				currentStats.currentHealth += regenAmount;
				totalRegen += regenAmount;

				if (totalRegen >= 50)
				{
					regenAmount = 0;
					totalRegen = 0;
				}
				regenTimer.Start();
			}
		}
	}
}

void Player::Damagepocion(float dt)
{
	if (damagePotionActive)
	{
		if (potiondamageTimer.ReadSec() < 10.0f) {}
		else
		{
			attackDamage = originalDamage;
			damagePotionActive = false;
		}
	}
}

void Player::Speedpocion(float dt)
{
	if (speedPotionActive)
	{
		if (potionspeedTimer.ReadSec() < 20.0f) {}
		else
		{
			currentStats.movementSpeed = originalSpeed;
			speedPotionActive = false;
		}
	}
}

void Player::Attack(float dt)
{
	//printf("attack"); 
	atack_Anim = true;
	inAnimation = true;

	// Dirección del ataque
	int attackX = position.x + lastMovementDirection.x * attackWidth;
	int attackY = position.y + lastMovementDirection.y * attackHeight;

	//Sensor
	if (!attackSensor && !hasAttacked) {
		attackSensor = app->physics->CreateRectangleSensor(attackX, attackY, attackWidth, attackHeight, DYNAMIC);
		attackSensor->ctype = ColliderType::PLAYER_ATTACK;
		attackSensor->listener = this;
		//printf("CREATE SENSOR\n");
		hasAttacked = true;
		atackNum++;
	}
	else if (attackSensor && hasAttacked) {
		// Si el sensor de ataque ya existe, actualizamos su posición
		attackSensor->body->SetTransform(b2Vec2(PIXEL_TO_METERS(attackX), PIXEL_TO_METERS(attackY)), 0);
		//printf("TRANSFORM SENSOR\n");
	}
	pbodyFoot->body->ApplyForceToCenter(b2Vec2(lastMovementDirection.x * attackMovement, lastMovementDirection.y * attackMovement), true);

	//Onda expansiva ataque pasivo mascara 1
	if (secondaryMask == Mask::MASK1) {
		if (!mask1PassiveSensor) {
			mask1PassiveSensor = app->physics->CreateRectangleSensor(attackX, attackY, 100, 100, DYNAMIC);
			mask1PassiveSensor->ctype = ColliderType::MASK0_PASSIVE_ATTACK;
			mask1PassiveSensor->listener = this;
		}
		else if (mask1PassiveSensor) {
			mask1PassiveSensor->body->SetTransform(b2Vec2(PIXEL_TO_METERS(attackX), PIXEL_TO_METERS(attackY)), 0);
		}
	}

	if (atackNum == 1) {
		app->audio->PlayTimedFx(basic_combo_attack1_fx, 473);
	}
	else if (atackNum == 2) {
		app->audio->PlayTimedFx(basic_combo_attack2_fx, 473);
	}
	else if (atackNum == 3) {
		app->audio->PlayRandomTimedFx(basic_combo_attack3_fx, basic_combo_attack3Alt_fx, basic_combo_attack3Alt2_fx, 473);
	}
}

void Player::Dead()
{
	
	if (app->audio->playingMusic == true)
	{
		app->audio->StopMusic(0.0f);
		app->audio->playingMusic = false;
	}
	
	if (app->audio->playingDeathFx == false)
	{
		app->audio->PlayFx(player_death_fx);
		app->audio->playingDeathFx = true;
	}

	if (!particulaBlood) {
		fPoint pos((float)position.x, (float)position.y);
		blood = app->psystem->AddEmiter(pos, EMITTER_TYPE_SPARK);
		particulaBlood = true;
	}
	

	 //printf("dead");
	currentAnimation = &dead_player;
	if (DeadTP && PlayerTimerColdDown(3)) {
		app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_gameover);
		app->audio->playingDeathFx = false;
		app->audio->StopFx(-1);
	}

	
}

void Player::UnequipMasks() {
	currentStats = baseStats;
}

void Player::EquipPrimaryMask(Mask mask) {
	primaryMask = mask;

	// Si la máscara no es NOMASK, aplica las modificaciones de estadísticas
	// if (mask != Mask::NOMASK) {
	// 	currentStats.maxHealth = baseStats.maxHealth * (1 + maskStats[mask][Branches::Modifiers][maskLevels[mask][Branches::Modifiers]].maxHealthModifier / 100);
	// 	currentStats.movementSpeed = baseStats.movementSpeed * (1 + maskStats[mask][Branches::Modifiers][maskLevels[mask][Branches::Modifiers]].movementSpeedModifier / 100);
	// 	currentStats.attackSpeed = baseStats.attackSpeed * (1 + maskStats[mask][Branches::Modifiers][maskLevels[mask][Branches::Modifiers]].attackSpeedModifier / 100);
	// 	currentStats.attackDamage = baseStats.attackDamage * (1 + maskStats[mask][Branches::Modifiers][maskLevels[mask][Branches::Modifiers]].attackDamageModifier / 100);
	// }
}

void Player::EquipSecondaryMask(Mask mask) {
	secondaryMask = mask;

	// Si la máscara no es NOMASK, aplica las modificaciones de estadísticas
	// if (mask != Mask::NOMASK) {
	// 	currentStats.attackDamage = baseStats.attackDamage * (1 + passiveStats[mask][Branches::Modifiers][maskLevels[mask][Branches::Modifiers]].damageBoost / 100);
	// }
}

Mask Player::GetPrimaryMask()
{
	return primaryMask;
}

Mask Player::GetSecondaryMask()
{
	return secondaryMask;
}

MaskStats* Player::GetMaskStats(Mask mask) {
	return &maskStats[mask][Branches::Modifiers][maskLevels[mask][Branches::Modifiers]];
}

void Player::ChangeMask() {
	if (timerChangeMask.ReadMSec() > changeMaskCooldown) {

		Mask temp = primaryMask;
		primaryMask = secondaryMask;
		secondaryMask = temp;

		app->audio->PlayFx(switch_masks_fx);

		//UnequipMasks();

		EquipPrimaryMask(primaryMask);
		EquipSecondaryMask(secondaryMask);

        float oldMaxHealth = currentStats.maxHealth;

		currentStats.maxHealth = baseStats.maxHealth * (1 + maskStats[primaryMask][Branches::Modifiers][maskLevels[primaryMask][Branches::Modifiers]].maxHealthModifier / 100);

		currentStats.currentHealth = (currentStats.currentHealth / oldMaxHealth) * currentStats.maxHealth;


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
		inAnimation = true;
		currentAnimation = &maskRayo_player;
		CastMultipleLightnings();
		break;
	case Mask::MASK1:
		inAnimation = true;
		currentAnimation = &maskBola_player;
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
	int poisonDamage = maskStats[Mask::MASK0][Branches::Rama4][maskLevels[primaryMask][Branches::Rama4]].poisonDamage;
	float poisonDuration = maskStats[Mask::MASK0][Branches::Rama4][maskLevels[primaryMask][Branches::Rama4]].poisonDuration / 1000.0f; // Convertir a segundos
	float poisonTickRate = maskStats[Mask::MASK0][Branches::Rama4][maskLevels[primaryMask][Branches::Rama4]].poisonTickRate / 1000.0f; // Convertir a segundos

	// Aquí asumimos que Entity tiene una función ApplyPoison que toma estos parámetros.
	// Si no es el caso, necesitarás ajustar este código.
	entity->ApplyPoison(poisonDamage, poisonDuration, poisonTickRate);
}

void Player::stateMachine(float dt)
{
	//printf("\ncurrentState: %d, desiredState: %d", static_cast<int>(currentState), static_cast<int>(desiredState));
	nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;
	switch (nextState) {
	case EntityStatePlayer::IDLE:
		if (!inTakeDMG) {
			DoNothing(dt);
		}
		break;
	case EntityStatePlayer::RUNNING:
		if (!inTakeDMG) {
			Run(dt);
		}
		break;
	case EntityStatePlayer::ATTACKING:
		if (!inTakeDMG) {
			Attack(dt);
		}
		break;
	case EntityStatePlayer::DEAD:
		/*if (!particulaBlood) {
			fPoint pos((float)position.x, (float)position.y);
			blood = app->psystem->AddEmiter(pos, EMITTER_TYPE_SPARK);
			particulaBlood = true;
		}*/
		Dead();
		break;
	case EntityStatePlayer::REVIVING:
		break;
	case EntityStatePlayer::MASK_ATTACK:
		if (!inTakeDMG && !hasMaskAttacked) {
			hasMaskAttacked = true;
			MaskAttack(dt);
		}
		break;
	case EntityStatePlayer::DASHI:
		if (!inTakeDMG) {
			if (isDashing) {
				Dashi(dt);
			}
		}
		break;

	case EntityStatePlayer::POCION:
		if (!inTakeDMG) {
			printf("Pocion");
			inAnimation = true;
			inPocionAnim = true;
		}
		break;
	case EntityStatePlayer::NONE:

		desiredState = EntityStatePlayer::IDLE;
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
	int numLightnings = maskStats[primaryMask][Branches::Rama3][maskLevels[primaryMask][Branches::Rama3]].numberLightning;
	printf("numLightnings: %d\n", numLightnings);
	for (int i = 0; i < numLightnings; i++) {
		CastLightning();
	}
}

//Ataque mascara 0
void Player::CastLightning() {
	Entity* target = GetEnemyWithHighestHealthWithinRadius(position, 500);
	if (target != nullptr) {
		printf("Enemy hit: %p at position (%d, %d)\n", (void*)target, target->position.x, target->position.y);
		target->TakeDamage(maskStats[primaryMask][Branches::Rama1][maskLevels[primaryMask][Branches::Rama1]].maskDamage);
		target->ApplyPoison(maskStats[primaryMask][Branches::Rama4][maskLevels[primaryMask][Branches::Rama4]].poisonDamage, maskStats[primaryMask][Branches::Rama4][maskLevels[primaryMask][Branches::Rama4]].poisonDuration, maskStats[primaryMask][Branches::Rama4][maskLevels[primaryMask][Branches::Rama4]].poisonTickRate);
		//ApplyPoison(target);
	}
	else {
		printf("No enemy alive in range to attack\n");
	}
}

//Ataque mascara 1

void Player::AreaAttack(float dt) {

	inMaskAtack = true;
	if (mask1AttackSensor == nullptr) {
	mask1AttackSensor = app->physics->CreateRectangleSensor(this->position.x, this->position.y, attackMask1Width + attackMask1Width * maskStats[primaryMask][Branches::Rama3][maskLevels[primaryMask][Branches::Rama3]].rangeBallModifier, attackMask1Height + attackMask1Height * maskStats[primaryMask][Branches::Rama3][maskLevels[primaryMask][Branches::Rama3]].rangeBallModifier, DYNAMIC);
	mask1AttackSensor->ctype = ColliderType::MASK1_ATTACK;
	mask1AttackSensor->listener = this;
	}
}

//Ataque mascara 2

void Player::DashAttack(float dt) {
	b2Vec2 velocity = b2Vec2(0, 0);

	// Obtener teclado
	/*pressingUp = app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT;
	pressingDown = app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT;
	pressingLeft = app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT;
	pressingRight = app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT;*/

	fPoint joystick = app->input->GetAxis(MOVE_HORIZONTAL, MOVE_VERTICAL);
	float horizontalMovement = joystick.x;
	float verticalMovement = joystick.y;

	// Actualizar velocidad
	if (!isDashing) {
		velocity.x = horizontalMovement * GetRealMovementSpeed() * speed * 10 * dt;
		velocity.y = verticalMovement * GetRealMovementSpeed() * speed * 10 * dt;

		isDashing = true;
		timerDash.Start();

		dashCollision = app->physics->CreateRectangleSensor(position.x, position.y, 150, 150, DYNAMIC);
		dashCollision->ctype = ColliderType::MASK2_ATTACK;
		dashCollision->listener = this;

		pbodyFoot->body->ApplyForce(b2Vec2(velocity.x * 100 * maskStats[primaryMask][Branches::Rama3][maskLevels[primaryMask][Branches::Rama3]].distanceDashModifier, velocity.y * 100 * maskStats[primaryMask][Branches::Rama3][maskLevels[primaryMask][Branches::Rama3]].distanceDashModifier), pbodyFoot->body->GetWorldCenter(), false);
	}
}

void Player::SetInvisible() {
	SDL_SetTextureAlphaMod(texture, 100);
	maskStats[primaryMask][Branches::Rama4][maskLevels[primaryMask][Branches::Rama4]].invisibilityTimer.Start();
	isInvisible = true;
}

void Player::SetPassiveInvisible() {
	SDL_SetTextureAlphaMod(texture, 100);
	isInvisible = true;
	passiveStats[secondaryMask][Branches::Modifiers][maskLevels[secondaryMask][Branches::Modifiers]].invisibilityTimer.Start();
}

//Mascara 3
void Player::Mask3Statistics() {
	currentStats.attackDamage = baseStats.attackDamage * (1 + maskStats[Mask::MASK3][Branches::Rama4][maskLevels[Mask::MASK3][Branches::Rama4]].maxActiveDamageModifier);
	currentStats.maxHealth = baseStats.maxHealth * (1 + maskStats[Mask::MASK3][Branches::Rama4][maskLevels[Mask::MASK3][Branches::Rama4]].maxActiveHealthModifier);
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
				physB->entity->TakeDamage(currentStats.attackDamage * passiveStats[secondaryMask][Branches::Modifiers][maskLevels[secondaryMask][Branches::Modifiers]].damageBoost);
			}
		}
		if (physA == mask1AttackSensor) {
			LOG("Collision ENEMY");
			if (physB->entity != nullptr) {
				physB->entity->TakeDamage(maskStats[primaryMask][Branches::Rama1][maskLevels[primaryMask][Branches::Rama1]].maskDamage);
				physB->entity->ApplyPoison(maskStats[primaryMask][Branches::Rama4][maskLevels[primaryMask][Branches::Rama4]].poisonDamage, maskStats[primaryMask][Branches::Rama4][maskLevels[primaryMask][Branches::Rama4]].poisonDuration, maskStats[primaryMask][Branches::Rama4][maskLevels[primaryMask][Branches::Rama4]].poisonTickRate);
			}
			//collisionMask1Timer.Start();
		}
		if (physA == dashCollision)
		{
			if (physB->entity != nullptr) {
				physB->entity->TakeDamage(maskStats[Mask::MASK2][Branches::Rama1][maskLevels[Mask::MASK2][Branches::Rama1]].maskDamage);
			}
		}
		if (physA == pbodyFoot) {
			//TakeDamage(physB->entity->attackDamage);
			if (!godmode) {
				if (physB->entity->type != EntityType::ENEMY_OLS) {
					TakeDamage(physB->listener->attackDamage);
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
					app->inventoryManager->CreateItem(physB->listener->type, physB->listener->description, physB->listener->tipo, physB->listener->name.c_str());

					/*physB->listener->active = false;
					physB->body->SetActive(false);*/
					physB->listener->CleanUp();

				}
				else
				{
					app->inventoryManager->AddQuantity(physB->listener);
				}



			}
		}
		app->audio->PlayTimedFx(get_item_fx, 201);
		break;
	case ColliderType::NOTA:
		LOG("Collision NOTA");
		if (physA != attackSensor && physA != mask1PassiveSensor && physA != mask1AttackSensor)
		{
			if (physB->listener->active) {
				if (app->notesManager->IsFull() == false)
				{
					app->notesManager->CreateItem(physB->listener->type, physB->listener->CloseTexture, physB->listener->description, physB->listener->titulo, physB->listener->id);
					/*physB->listener->active = false;
					physB->body->SetActive(false);*/
					physB->listener->CleanUp();
				}

			}
		}
		break;
	case ColliderType::ARBOL:
		LOG("Collision ARBOL");
		if (app->input->GetButton(CONFIRM) == KEY_DOWN)
		{
			app->treeManager->mostrar = true;

		}
		break;
	case ColliderType::AROMAGICA:
		TakeDamage(20);
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
						//printf("Abajo y Izquerda \n");
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
						/*printf("Abajo y Derecha \n");*/
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
						/*printf("Arriba y Izquerda \n");*/
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
						/*printf("Arriba y Derecha \n");*/
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
						/*printf("Abajo \n");*/
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
						/*printf("Arriba \n");*/
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
						/*printf("Izquierda \n");*/
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
						/*printf("Derecha \n");*/
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
	//printf("Llamada a DetermineRoomTypel\n");
	if (room->properties.GetProperty("lType") != NULL)
	{
		std::string lType = room->properties.GetProperty("lType")->value;
		/*printf("El objeto tiene el atributo lType\n");*/

		if (lType == "dl")
		{
			/*printf("dl");*/
			roomType = ROOM_L_DL;
		}
		else if (lType == "dr")
		{
			//printf("dr");
			roomType = ROOM_L_DR;
		}
		else if (lType == "ul")
		{
			//printf("ul");
			roomType = ROOM_L_UL;
		}
		else if (lType == "ur")
		{
			//printf("ur");
			roomType = ROOM_L_UR;
		}
		else if (lType == "d")
		{
			//printf("d");
			roomType = ROOM_L_D;
		}
		else if (lType == "u")
		{
			//printf("u");
			roomType = ROOM_L_U;
		}
		else if (lType == "l")
		{
			//printf("l");
			roomType = ROOM_L_L;
		}
		else if (lType == "r")
		{
			//printf("r");
			roomType = ROOM_L_R;
		}
		else
		{
			printf("UNKNOWN L ROOM");
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

	/*fPoint joystick = app->input->GetAxis(MOVE_HORIZONTAL, MOVE_VERTICAL);
	KeyState sprint = app->input->GetButton(BACK);
	float speed = (sprint == KEY_REPEAT) ? 0.5f : 0.2f;

	position.x += speed * joystick.x * dt;
	position.y += speed * joystick.y * dt;*/

	//Moverse a la izquierda
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		velocity.y += -speedFast * dt;
		desiredState = EntityStatePlayer::RUNNING;
		isFacingLeft = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		velocity.y += speedFast * dt;
		desiredState = EntityStatePlayer::RUNNING;
		isFacingLeft = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = -speedFast * dt;
		desiredState = EntityStatePlayer::RUNNING;
		isFacingLeft = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = speedFast * dt;
		desiredState = EntityStatePlayer::RUNNING;
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





	//printf("\n%f",joystick.x);

	//Controles antiguos

	//pressingUp = app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT;
	//pressingDown = app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT;
	//pressingLeft = app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT;
	//pressingRight = app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT;


	// Calcular la velocidad horizontal y vertical

	fPoint joystick = app->input->GetAxis(MOVE_HORIZONTAL, MOVE_VERTICAL);
	float horizontalMovement = joystick.x;
	float verticalMovement = joystick.y;

	ResetSpeed();

	// Actualizar velocidad
	if (!isDashing) {
		velocity.x = horizontalMovement * GetRealMovementSpeed() * speed * 10 * dt;
		velocity.y = verticalMovement * GetRealMovementSpeed() * speed * 10 * dt;

		// Si hay entrada de movimiento, actualizar estado y direcci��n.

		if (horizontalMovement != 0 || verticalMovement != 0) {
			if (!inAnimation) {
				desiredState = EntityStatePlayer::RUNNING;
			}
			isFacingLeft = (horizontalMovement < 0);
			lastMovementDirection = fPoint(horizontalMovement, verticalMovement);
		}
	}

	FishingDirecction(verticalMovement, horizontalMovement);

	//Si pulsas espacio
	if (app->input->GetButton(DASH) == KEY_DOWN && timerDash.ReadMSec() > cdTimerDashMS) {

		velocityNormalized = velocity;
		velocityNormalized.Normalize();

		isDashing = true;
		timerDash.Start();
		desiredState = EntityStatePlayer::DASHI;
		pbodyFoot->body->ApplyForce(b2Vec2(velocityNormalized.x * 500, velocityNormalized.y * 500), pbodyFoot->body->GetWorldCenter(), false);

		if (secondaryMask == Mask::MASK2)
		{
			SetPassiveInvisible();
			passiveStats[secondaryMask][Branches::Modifiers][maskLevels[secondaryMask][Branches::Modifiers]].invisibilityTimer.Start();
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
	if (app->input->GetButton(ATAQUE) == KEY_DOWN && !isAttacking) {
		hasAttacked = false;
		isAttacking = true;
		timerAttack.Start();
		collisionAttackTimer.Start();
		desiredState = EntityStatePlayer::ATTACKING;
	}

	//Checkea según velocidad de ataque si puede atacar
	if (timerAttack.ReadMSec() > cdTimerAttackMS) {
		isAttacking = false;
	}
	//printf("%f\r",collisionAttackTimer.ReadMSec());
	//Borra colisión pasados 50 milisegundos
	if (collisionAttackTimer.ReadMSec() > 50) {
		if (attackSensor) {
			hasAttacked = true;
			app->physics->DestroyBody(attackSensor);
			attackSensor = nullptr;
		}
		if (mask1PassiveSensor) {
			app->physics->DestroyBody(mask1PassiveSensor);
			mask1PassiveSensor = nullptr;
		}
	}


	//Si pulsas K para mascara principal

	if ((app->input->GetButton(ATAQUE_HABILIDAD) == KEY_DOWN || app->input->GetAxis(ATAQUE_HABILIDAD) != 0) &&
		(timerMaskAttack.ReadMSec() > maskStats[primaryMask][Branches::Rama2][maskLevels[primaryMask][Branches::Rama2]].maskCoolDown ||
			!maskStats[primaryMask][Branches::Rama3][maskLevels[primaryMask][Branches::Rama3]].firstTimeUsed)) {
		maskStats[primaryMask][Branches::Rama3][maskLevels[primaryMask][Branches::Rama3]].firstTimeUsed = true;
		isAttackingMask = true;
		timerMaskAttack.Start(); 
		desiredState = EntityStatePlayer::MASK_ATTACK;

		// Restablece el cooldown de la máscara a su valor original después de usar la habilidad
		for (int i = 0; i < maskStats[primaryMask][Branches::Rama2].size(); i++) {
			maskStats[primaryMask][Branches::Rama2][i].maskCoolDown = maskStats[primaryMask][Branches::Rama2][i].originalmaskCoolDown;
		}
	}

	if (!(timerMaskAttack.ReadMSec() < maskStats[primaryMask][Branches::Rama2][maskLevels[primaryMask][Branches::Rama2]].maskCoolDown && isAttackingMask)) {
		isAttackingMask = false;
		hasMaskAttacked = false;
		if (mask1AttackSensor) {
			app->physics->DestroyBody(mask1AttackSensor);
			mask1AttackSensor = nullptr;
		}
	}

	if (collisionMask1Timer.ReadSec() > 0.25) {
		app->physics->DestroyBody(mask1AttackSensor);
		mask1AttackSensor = nullptr;
	}

	//Si pulsas L para cambiar de mascara
	if (app->input->GetButton(INTERCAMBIAR_MASCARAS) == KEY_DOWN && timerChangeMask.ReadMSec() > changeMaskCooldown) {
		ChangeMask();
	}

	b2Transform pbodyPos = pbodyFoot->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x);
	position.y = METERS_TO_PIXELS(pbodyPos.p.y);
}
void Player::FishingDirecction(float verticalMovement, float horizontalMovement)
{


	// Si jugador Mueve
	if (horizontalMovement == 1) {
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
	else if (horizontalMovement == -1) {
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
	else if (verticalMovement == 1) {
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
	else if (verticalMovement == -1) {
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
		if (dashCollision == nullptr && !isDashing) {
			currentStats.currentHealth -= damage;
			isInvisible = false;
			inAnimation = true;
			inTakeDMG = true;
			app->audio->PlayTimedFx(player_get_damage_fx, 575);
			printf("Player has received  %f damage\n", damage);
			damageTimer.Start();

			if (currentStats.currentHealth <= 0) {
				//printf("Dead");
				inAnimation = true;
				desiredState = EntityStatePlayer::DEAD;
			}
		}
	}
}

void Player::UpdateStats() {
	baseStats.maxHealth = 100 + armorPerLevel[app->inventoryManager->armorLevel];
	baseStats.currentHealth = 100 + armorPerLevel[app->inventoryManager->armorLevel];
	baseStats.attackDamage = 100 + attackDamagePerLevel[app->inventoryManager->swordLevel];

    if (primaryMask != Mask::NOMASK) {
		currentStats.maxHealth = baseStats.maxHealth * (1 + maskStats[primaryMask][Branches::Modifiers][maskLevels[primaryMask][Branches::Modifiers]].maxHealthModifier / 100);
		currentStats.movementSpeed = baseStats.movementSpeed * (1 + maskStats[primaryMask][Branches::Modifiers][maskLevels[primaryMask][Branches::Modifiers]].movementSpeedModifier / 100);
		currentStats.attackSpeed = baseStats.attackSpeed * (1 + maskStats[primaryMask][Branches::Modifiers][maskLevels[primaryMask][Branches::Modifiers]].attackSpeedModifier / 100);
		currentStats.attackDamage = baseStats.attackDamage * (1 + maskStats[primaryMask][Branches::Modifiers][maskLevels[primaryMask][Branches::Modifiers]].attackDamageModifier / 100);
 	}

    if (secondaryMask == Mask::MASK0) {
		currentStats.attackDamage *= (1 + passiveStats[secondaryMask][Branches::Modifiers][maskLevels[secondaryMask][Branches::Modifiers]].damageBoost / 100);
    }
}


bool Player::PlayerTimerColdDown(float time)
{
	//printf("\nataqueTimeClodDown%: %f", ataqueTimeClodDown);
	TimeClodDown = TimerColdDown.CountDown(time);
	if ((float)TimeClodDown == 0) {
		return true;
	}
	else
	{
		return false;
	}
}

void Player::SlowDown(float slowFactor){
    speed *= slowFactor;
    slowSpeedTimer.Start();
    isSlowed = true;
}

void Player::ResetSpeed(){
    if (isSlowed && slowSpeedTimer.ReadSec() >= 4.5) {
        speed  = mainSpeed;
        isSlowed = false;
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


