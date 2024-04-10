#include "espada.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene_testing.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Espada::Espada(EntityType type, int id, int ataque, int durabilidad, int magia, float peso)
	: type(type), ataque(ataque), durabilidad(durabilidad), magia(magia), peso(peso), Entity(EntityType::RESOURCE_ESPADA)
{
	name.Create("espada");
}

Espada::~Espada() {}

bool Espada::Awake() {

	
	 

	return true;
}

bool Espada::Start() {

	//initilize textures
	/*position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();*/
	texture = app->tex->Load(config.attribute("texturePath").as_string());
	
	// L07 DONE 4: Add a physics to an item - initialize the physics body
	app->tex->GetSize(texture, texW, texH);
	pbody = app->physics->CreateCircle(position.x, position.y, 11, bodyType::STATIC);
	pbody->ctype = ColliderType::RESOURCE_ESPADA;
	pbody->listener = this;


	return true;
}

bool Espada::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 11;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 11;

	
	
	return true;
}

bool Espada::PostUpdate()
{
	app->render->DrawTexture(texture, position.x, position.y);
	return true;
}

bool Espada::CleanUp()
{
	app->physics->GetWorld()->DestroyBody(pbody->body);
	SDL_DestroyTexture(texture);
	return true;
}


