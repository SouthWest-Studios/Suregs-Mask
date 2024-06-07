#include "Arbol.h"
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
#include "Item_Nota.h"
#include "TreeManager.h"
#include "menu.h"
#include "Scene_Pueblo.h"
#include "Scene_Pueblo_Tutorial.h"
#include "Entity.h"

Arbol::Arbol() : Entity(EntityType::COFRE)
{
	name = "arbol";
}

Arbol::~Arbol() {}

bool Arbol::Awake() {

	
	 

	return true;
}

bool Arbol::Start() {
	
	//initilize textures
	/*position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();*/

    /*chest_fx = app->audio->LoadAudioFx("chest_fx");*/

	/*texture = app->tex->Load("Assets/Textures/Entidades/Items/item_Garra.png");*/
	// L07 DONE 4: Add a physics to an item - initialize the physics body
	/*pbody = app->physics->CreateCircle(position.x, position.y, 70, bodyType::STATIC);*/

	pbody = app->physics->CreateRectangleSensor(position.x + 10, position.y, 300, 300, bodyType::KINEMATIC);
	pbody->ctype = ColliderType::ARBOL;
	pbody->listener = this;
	pbody->body->GetFixtureList()->SetSensor(true);



	return true;
}

bool Arbol::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 25;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 25;

	
	
	return true;
}

bool Arbol::PostUpdate()
{
  
        
    
	return true;
}
bool Arbol::CleanUp()
{
	app->physics->DestroyBody(pbody);
    app->tex->UnLoad(texture);
    return true;
}
void Arbol::OnCollision(PhysBody* physA, PhysBody* physB) 
{
    switch (physB->ctype)
    {
      case ColliderType::PLAYER:
      {
          if (abierto == false)
          {
              if (app->input->GetButton(CONFIRM) == KEY_DOWN && !app->scene_pueblo->GetRod()->fishing.rodReady  && !app->scene_pueblo_tutorial->GetRod()->fishing.rodReady)
              {
                  app->treeManager->mostrar = true;
                  app->menu->active = false;
                  
 
              }

               
          }
      }
    }
}





