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
#include "Item_Diamante.h"
#include "Item_Rubi.h"
#include "Item_Amatista.h"
#include "Item_Carbon.h"
#include "Entity.h"

Cofre::Cofre() : Entity(EntityType::COFRE)
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
	/*pbody = app->physics->CreateCircle(position.x, position.y, 70, bodyType::STATIC);*/
	pbody = app->physics->CreateRectangleSensor(position.x + 10, position.y, 100, 100, bodyType::KINEMATIC);
	pbody->ctype = ColliderType::COFRE;
	pbody->listener = this;
	pbody->body->GetFixtureList()->SetSensor(true);


	return true;
}

bool Cofre::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 25;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 25;

	
	
	return true;
}

bool Cofre::PostUpdate()
{
	if (abierto)
	{
		app->render->DrawTexture(openTexture, position.x, position.y, 0.5f);
	}
	else
	{
		app->render->DrawTexture(texture, position.x, position.y, 0.5f);
	}
	
	return true;
}
void Cofre::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
        if (abierto == false)
        {
            if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
            {
                abierto = true;

                pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
                if (parseResult) {
                    configNode = configFile.child("config");
                }

                // Lista de items posibles a soltar
                std::vector<EntityType> possibleItems = {
                    EntityType::ITEM_DIAMANTE,
                    EntityType::ITEM_RUBI,
                    EntityType::ITEM_AMATISTA,
                    EntityType::ITEM_CARBON
                };

                // Suelta al menos 2 items
                for (int i = 0; i < 2; ++i) {
                    int randomIndex = rand() % possibleItems.size(); // Elige un índice aleatorio de la lista
                    EntityType itemType = possibleItems[randomIndex]; // Obtiene el tipo de item correspondiente al índice
                    Entity* item = nullptr;
                    std::string nombreItem = "";
                    // Crea la entidad correspondiente al tipo de item elegido y configúrala
                    switch (itemType) {
                    case EntityType::ITEM_DIAMANTE:
                    {
                        Item_Diamante* diamante = (Item_Diamante*)app->entityManager->CreateEntity(EntityType::ITEM_DIAMANTE);
                        diamante->config = configNode.child("entities_data").child("item_diamante");
                        diamante->position = iPoint(position.x, position.y);
                        diamante->Start();
                        nombreItem = "item_diamante";
                        break;
                    }
                    case EntityType::ITEM_RUBI:
                    {
                        Item_Rubi* rubi = (Item_Rubi*)app->entityManager->CreateEntity(EntityType::ITEM_RUBI);
                        rubi->config = configNode.child("entities_data").child("item_rubi");
                        rubi->position = iPoint(position.x + 30, position.y);
                        rubi->Start();
                        break;
                    }
                    case EntityType::ITEM_AMATISTA:
                    {
                        Item_Amatista* amatista = (Item_Amatista*)app->entityManager->CreateEntity(EntityType::ITEM_AMATISTA);
                        amatista->config = configNode.child("entities_data").child("item_amatista");
                        amatista->position = iPoint(position.x + 30, position.y + 30);
                        amatista->Start();
                        break;
                    }
                    case EntityType::ITEM_CARBON:
                        Item_Carbon* carbon = (Item_Carbon*)app->entityManager->CreateEntity(EntityType::ITEM_CARBON);
                        carbon->config = configNode.child("entities_data").child("item_carbon");
                        carbon->position = iPoint(position.x, position.y + 30);
                        carbon->Start();
                        break;
                    }


                }

                // Con probabilidad del 15%, suelta otro item
                if (rand() % 100 < 15) {
                    int randomIndex = rand() % possibleItems.size();
                    EntityType itemType = possibleItems[randomIndex];
                    Entity* item = nullptr;
                    std::string nombreItem = "";

                    switch (itemType) {
                    case EntityType::ITEM_DIAMANTE:
                    {
                        Item_Diamante* diamante = (Item_Diamante*)app->entityManager->CreateEntity(EntityType::ITEM_DIAMANTE);
                        diamante->config = configNode.child("entities_data").child("item_diamante");
                        diamante->position = iPoint(position.x + 10, position.y);
                        diamante->Start();
                        nombreItem = "item_diamante";
                        break;
                    }
                        
                    case EntityType::ITEM_RUBI:
                    {
                        Item_Rubi* rubi = (Item_Rubi*)app->entityManager->CreateEntity(EntityType::ITEM_RUBI);
                        rubi->config = configNode.child("entities_data").child("item_rubi");
                        rubi->position = iPoint(position.x + 40, position.y);
                        rubi->Start();
                        break;
                    }
                        
                    case EntityType::ITEM_AMATISTA:
                    {
                        Item_Amatista* amatista = (Item_Amatista*)app->entityManager->CreateEntity(EntityType::ITEM_AMATISTA);
                        amatista->config = configNode.child("entities_data").child("item_amatista");
                        amatista->position = iPoint(position.x + 40, position.y + 40);
                        amatista->Start();
                        break;
                    }

                    case EntityType::ITEM_CARBON:
                        Item_Carbon* carbon = (Item_Carbon*)app->entityManager->CreateEntity(EntityType::ITEM_CARBON);
                        carbon->config = configNode.child("entities_data").child("item_carbon");
                        carbon->position = iPoint(position.x, position.y + 40);
                        carbon->Start();
                        break;
                    }
                }
            }
        }
        
		break;
	}

}

bool Cofre::CleanUp()
{
	app->physics->GetWorld()->DestroyBody(pbody->body);
	app->tex->UnLoad(texture);
	return true;
}


