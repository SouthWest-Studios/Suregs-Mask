#include "Cofre.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene_testing.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Cofre::Cofre() : Entity(EntityType::PLAYER)
{
	name.Create("cofre");
}

Cofre::~Cofre() {}

bool Cofre::Awake() {

	
	 

	return true;
}

bool Cofre::Start() {
	
	//initilize textures
	/*position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();*/
	texture = app->tex->Load(config.attribute("texturePath").as_string());
	openTexture = app->tex->Load(config.attribute("opentexturePath").as_string());
	/*texture = app->tex->Load("Assets/Textures/Entidades/Items/item_Garra.png");*/
	// L07 DONE 4: Add a physics to an item - initialize the physics body
	app->tex->GetSize(texture, texW, texH);
	pbody = app->physics->CreateCircle(position.x, position.y, 30, bodyType::STATIC);
	pbody->ctype = ColliderType::RESOURCE;
	pbody->listener = this;
	pbody->body->GetFixtureList()->SetSensor(true);


	return true;
}

bool Cofre::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 30;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 30;

	
	
	return true;
}

bool Cofre::PostUpdate()
{
	app->render->DrawTexture(texture, position.x, position.y, 0.5f);
	return true;
}

bool Cofre::CleanUp()
{
	app->physics->GetWorld()->DestroyBody(pbody->body);
	app->tex->UnLoad(texture);
	return true;
}


