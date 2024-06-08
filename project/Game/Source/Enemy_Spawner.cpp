#include "Enemy_Spawner.h"
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
#include "Item_Hueso.h"
#include "Item_Viscera.h"
#include "Item_Diente.h"
#include "BestiarioManager.h"
#include <Optick/include/optick.h>
#include "Utils.cpp"
#include "Enemy_Ols.h"
#include "Enemy_Khurt.h"
#include "Enemy_Muur.h"
#include "Enemy_Boorok.h"
#include "ParticleSystem.h"


Enemy_Spawner::Enemy_Spawner() : Entity(EntityType::ENEMY_SPAWNER) {
	name = ("spawner");


}

Enemy_Spawner::~Enemy_Spawner() {

}

bool Enemy_Spawner::Awake() {

	return true;
}

bool Enemy_Spawner::Start() {

	OPTICK_EVENT();
	texture = app->tex->Load(config.attribute("texturePath").as_string());

	contadorSpawnerTimer.Start(contadorSpawnerMS + 1);

	return true;
}

bool Enemy_Spawner::Update(float dt)
{
	OPTICK_EVENT();


	//Pone el sensor del cuerpo en su posicion
	iPoint playerPos = app->entityManager->GetPlayer()->position;
	if (dist(playerPos, position) <= 12 * 32) {
		if (contadorSpawnerTimer.ReadMSec() > contadorSpawnerMS) {
			//Spawn enemigo
			int numEntities = rand() % 3 + 1;

			pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
			if (parseResult) {
				configEntities = configFile.child("config").child("entities_data");
			}

			for (int i = 0; i < numEntities; i++) {
				int entityRandom = rand() % 5;

				switch (entityRandom)
				{
					case 1: {
						Enemy_Osiris* osiris = (Enemy_Osiris*)app->entityManager->CreateEntity(EntityType::ENEMY_OSIRIS);
						osiris->config = configEntities.child("osiris");
						osiris->position = iPoint(position.x + 16, position.y + 16);
						fPoint pos((float)osiris->position.x, (float)osiris->position.y);
						app->psystem->AddEmiter(pos, EMITTER_TYPE_SMOKE);
						osiris->isSpawner = true;
						osiris->Start();
						break;}
					case 2: {
						Enemy_Ols* ols = (Enemy_Ols*)app->entityManager->CreateEntity(EntityType::ENEMY_OLS);
						ols->config = configEntities.child("ols");
						ols->position = iPoint(position.x + 16, position.y + 16);
						fPoint pos((float)ols->position.x, (float)ols->position.y);
						app->psystem->AddEmiter(pos, EMITTER_TYPE_SMOKE);
						ols->isSpawner = true;
						ols->Start();
						break;}
					case 3: {
						Enemy_Khurt* khurt = (Enemy_Khurt*)app->entityManager->CreateEntity(EntityType::ENEMY_KHURT);
						khurt->config = configEntities.child("khurt");
						khurt->position = iPoint(position.x + 16, position.y + 16);
						fPoint pos((float)khurt->position.x, (float)khurt->position.y);
						app->psystem->AddEmiter(pos, EMITTER_TYPE_SMOKE);
						khurt->isSpawner = true;
						khurt->Start();
						break;
					}
					case 0:
					case 4: {
						Enemy_Muur* muur = (Enemy_Muur*)app->entityManager->CreateEntity(EntityType::ENEMY_MUUR);
						muur->config = configEntities.child("muur");
						muur->position = iPoint(position.x + 16, position.y + 16);
						fPoint pos((float)muur->position.x, (float)muur->position.y);
						app->psystem->AddEmiter(pos, EMITTER_TYPE_SMOKE);
						muur->isSpawner = true;
						muur->Start();
						break;
					}
					case 5: {
						Enemy_Boorok* boorok = (Enemy_Boorok*)app->entityManager->CreateEntity(EntityType::ENEMY_BOOROK);
						boorok->config = configEntities.child("boorok");
						boorok->position = iPoint(position.x + 16, position.y + 16);
						fPoint pos((float)boorok->position.x, (float)boorok->position.y);
						app->psystem->AddEmiter(pos, EMITTER_TYPE_SMOKE);
						boorok->isSpawner = true;
						boorok->Start();
						break;
					}
					default: {
						Enemy_Muur* muur = (Enemy_Muur*)app->entityManager->CreateEntity(EntityType::ENEMY_MUUR);
						muur->config = configEntities.child("muur");
						muur->position = iPoint(position.x + 16, position.y + 16);
						fPoint pos((float)muur->position.x, (float)muur->position.y);
						app->psystem->AddEmiter(pos, EMITTER_TYPE_SMOKE);
						muur->isSpawner = true;
						muur->Start();
						break;
					}
				
				}


			}


			contadorSpawnerTimer.Start();
		}
	}


	return true;
}


bool Enemy_Spawner::PostUpdate() {


	app->render->DrawTexture(texture, position.x - 100, position.y - 100);

	return true;
}

bool Enemy_Spawner::CleanUp()
{

	return true;
}



// L07 DONE 6: Define OnCollision function for the player. 
void Enemy_Spawner::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
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
