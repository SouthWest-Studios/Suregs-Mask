#include "Item_mascara_0.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene_testing.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Item_mascara_0::Item_mascara_0(EntityType type, int id, int ataque, int durabilidad, int magia, float peso)
	: type(type), ataque(ataque), durabilidad(durabilidad), magia(magia), peso(peso), Entity(EntityType::ITEM_MASCARA0)
{
	name = ("item_masacara_0");
}

Item_mascara_0::~Item_mascara_0() {}

bool Item_mascara_0::Awake() {

	
	 

	return true;
}

bool Item_mascara_0::Start() {
	
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
	pbody = app->physics->CreateCircle(position.x, position.y, 30, bodyType::STATIC);
	pbody->ctype = ColliderType::RESOURCE;
	pbody->listener = this;
	pbody->body->GetFixtureList()->SetSensor(true);


	return true;
}

bool Item_mascara_0::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 83;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 43;

	
	
	return true;
}

bool Item_mascara_0::PostUpdate()
{
	app->render->DrawTexture(texture, position.x, position.y, 0.2f);
	return true;
}

bool Item_mascara_0::CleanUp()
{
	app->physics->DestroyBody(pbody); pbody = nullptr;  app->entityManager->DestroyEntity(this);active = false;
	app->tex->UnLoad(texture);
	return true;
}


