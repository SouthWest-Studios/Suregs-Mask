#include "Estatua.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene_testing.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Item_Diamante.h"
#include "Item_Rubi.h"
#include "Item_Amatista.h"
#include "Item_Carbon.h"
#include "Item_Pocion_Dano.h"
#include "Item_Pocion_Regeneracion.h"
#include "Item_Pocion_Velocidad.h"
#include "Item_Pocion_Vida_1.h"
#include "Item_Pocion_Vida_2.h"
#include "Item_Pocion_Vida_3.h"
#include "Item_Pocion_Vida_Max.h"
#include "Estatua.h"
#include "Entity.h"
#include "Hud.h"
#include "Scene_Pueblo.h"

Estatua::Estatua() : Entity(EntityType::ESTATUA)
{
	name = "estatua";
}

Estatua::~Estatua() {}

bool Estatua::Awake() {

	
	 

	return true;
}

bool Estatua::Start() {
	
	//initilize textures
	/*position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();*/
	texture = app->tex->Load(config.attribute("texturePath").as_string());
	
	/*texture = app->tex->Load("Assets/Textures/Entidades/Items/item_Garra.png");*/
	// L07 DONE 4: Add a physics to an item - initialize the physics body
	/*pbody = app->physics->CreateCircle(position.x, position.y, 70, bodyType::STATIC);*/
	pbody = app->physics->CreateRectangleSensor(position.x + 10, position.y, 100, 100, bodyType::KINEMATIC);
	pbody->ctype = ColliderType::ESTATUA;
	pbody->listener = this;
	pbody->body->GetFixtureList()->SetSensor(true);



	return true;
}

bool Estatua::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 25;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 25;

	
	/*if (mostrar)
	{
		app->hud->estatua = true;
	}
	else if (mostrar == false)
	{
		app->hud->estatua = false;
	}*/
	
	
	return true;
}

bool Estatua::PostUpdate()
{
	if (mostrar)
	{
		/*app->render->DrawTexture(texture, 400, 100, SDL_FLIP_NONE, 0, 0);*/
	}
		
	
	
	return true;
}
bool Estatua::CleanUp()
{
	app->entityManager->DestroyEntity(this);
	app->physics->DestroyBody(pbodyFoot);
	app->physics->DestroyBody(pbodySensor);
	//app->tex->UnLoad(texture);

    return true;
}
void Estatua::OnCollision(PhysBody* physA, PhysBody* physB) 
{
    switch (physB->ctype)
    {
      case ColliderType::PLAYER:
      {
          
              if (app->input->GetButton(CONFIRM) == KEY_DOWN && !app->scene_pueblo->GetRod()->fishing.rodReady)
              {
				  app->hud->estatua = true;
				  app->hud->EstatuaTexture = texture;
              }

               
          
      }
    }
}







