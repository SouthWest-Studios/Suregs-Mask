#include "Elevator.h"
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
#include "ElevatorMenu.h"
#include "Entity.h"

Elevator::Elevator() : Entity(EntityType::ASCENSOR)
{
	name = "ascensor";
}

Elevator::~Elevator() {}

bool Elevator::Awake() {

	
	 

	return true;
}

bool Elevator::Start() {
	
	//initilize textures
	/*position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();*/
	texture = app->tex->Load(config.attribute("texturePath").as_string());
	app->ascensor->Menutexture = app->tex->Load(config.attribute("menutexturePath").as_string());
	app->ascensor->listTexture = app->tex->Load(config.attribute("textureList").as_string());
	app->ascensor->PointerTexture = app->tex->Load(config.attribute("PointerPath").as_string());
	app->ascensor->textura_black = app->tex->Load(config.attribute("textura_blackPath").as_string());
	

	/*texture = app->tex->Load("Assets/Textures/Entidades/Items/item_Garra.png");*/
	// L07 DONE 4: Add a physics to an item - initialize the physics body
	/*pbody = app->physics->CreateCircle(position.x, position.y, 70, bodyType::STATIC);*/
	pbody = app->physics->CreateRectangleSensor(position.x + 10, position.y, 100, 100, bodyType::KINEMATIC);
	pbody->ctype = ColliderType::ASCENSOR;
	pbody->listener = this;
	pbody->body->GetFixtureList()->SetSensor(true);



	return true;
}

bool Elevator::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 25;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 25;

	
	

	
	
	return true;
}

bool Elevator::PostUpdate()
{
	
		app->render->DrawTexture(texture, position.x, position.y, 0.5f);
	
	
	return true;
}
bool Elevator::CleanUp()
{
    app->physics->GetWorld()->DestroyBody(pbody->body);
    app->tex->UnLoad(texture);
    return true;
}
void Elevator::OnCollision(PhysBody* physA, PhysBody* physB) 
{
    switch (physB->ctype)
    {
      case ColliderType::PLAYER:
      {
          
              if (app->input->GetButton(CONFIRM) == KEY_DOWN)
              {

				 
				  if (app->ascensor->mazmorraActual < app->ascensor->mazmorra)
				  {
					  app->ascensor->final = true;
				  }
				  else
				  {
					  /*app->ascensor->mazmorraActual--;*/
					  app->ascensor->final = final;
				  }
				  app->ascensor->abierto = true;
				  
				  
                      
                 
              }

               
          
      }
    }
}







