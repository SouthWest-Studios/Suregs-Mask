#include "Item_Pocion_Dano.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene_testing.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Item_Pocion_Dano::Item_Pocion_Dano(EntityType type, int id, int ataque, int durabilidad, int magia, float peso)
	: type(type), ataque(ataque), durabilidad(durabilidad), magia(magia), peso(peso), Entity(EntityType::ITEM_POCION_DANO)
{
	name = ("item_pocion_dano");
}

Item_Pocion_Dano::~Item_Pocion_Dano() {}

bool Item_Pocion_Dano::Awake() {

	
	 

	return true;
}

bool Item_Pocion_Dano::Start() {
	
	//initilize textures
	/*position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();*/
	texture = app->tex->Load(config.attribute("texturePath").as_string());
	description = (config.attribute("description").as_string());
	tipo = (config.attribute("type").as_string());
	name = (config.attribute("name").as_string());
	/*texture = app->tex->Load("Assets/Textures/Entidades/Items/item_Garra.png");*/
	// L07 DONE 4: Add a physics to an item - initialize the physics body
	app->tex->GetSize(texture, texW, texH);
	pbody = app->physics->CreateCircle(position.x, position.y, 11, bodyType::STATIC);
	pbody->ctype = ColliderType::RESOURCE;
	pbody->listener = this;
	pbody->body->GetFixtureList()->SetSensor(true);


	return true;
}

bool Item_Pocion_Dano::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 11;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 11;

	
	
	return true;
}

bool Item_Pocion_Dano::PostUpdate()
{
	app->render->DrawTexture(texture, position.x, position.y, 0.5f);
	return true;
}

bool Item_Pocion_Dano::CleanUp()
{
	app->physics->DestroyBody(pbody); pbody = nullptr;  app->entityManager->DestroyEntity(this);active = false;
	app->tex->UnLoad(texture);
	return true;
}


