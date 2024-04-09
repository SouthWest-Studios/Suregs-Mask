
#include "EntityManager.h"
#include "InventoryManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene_testing.h"
#include "inventity.h"
#include "SwordInv.h"
#include "ArmaduraInv.h"
#include "ItemInv.h"
#include "espada.h"
#include "Defs.h"
#include "Log.h"
#include "SString.h"

InventoryManager::InventoryManager(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("iventorymanager");
}

// Destructor
InventoryManager::~InventoryManager()
{}

// Called before render is available
bool InventoryManager::Awake(pugi::xml_node config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Inventity*>* item;
	Inventity* pEntity = NULL;

	for (item = inventities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool InventoryManager::Start() {

	InventoryBackground = app->tex->Load("Assets/Textures/inventario.png");
	PointerItemText = app->tex->Load("Assets/Textures/select.png");
	SelectedItemText = app->tex->Load("Assets/Textures/selected.png");
	EquipedItemText = app->tex->Load("Assets/Textures/equiped.png");

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Inventity*>* item;
	Inventity* pEntity = NULL;

	for (item = inventities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool InventoryManager::CleanUp()
{
	bool ret = true;
	ListItem<Inventity*>* item;
	item = inventities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	inventities.Clear();

	return ret;
}
int highestId = -1;
Inventity* InventoryManager::CreateItem(EntityType type, int id, int ataque, int durabilidad, int magia, float peso, int defensa)
{
	Inventity* entity = nullptr;


	//L03: DONE 3a: Instantiate entity according to the type and add the new entity to the list of Entities
	switch (type)
	{
	case EntityType::ITEM:
	{
		int newId = 0;
		for (ListItem<Inventity*>* item = inventities.start; item != nullptr; item = item->next)
		{

			item->data->id = newId;
			newId++;
		}
		if (inventities.Count() > 0)
		{
			highestId = inventities.end->data->id;
		}
		else
		{
			highestId = -1;
		}

		Iteminv* itemm = new Iteminv();
		itemm->id = highestId + 1;
		itemm->type = InventityType::ITEM;
		itemm->icon = app->tex->Load("Assets/Textures/CoinIcon.png");
		itemm->stackable = true;
		entity = itemm;

		break;
	}

	case EntityType::RESOURCE_ESPADA:
	{
		int newId = 0;
		for (ListItem<Inventity*>* item = inventities.start; item != nullptr; item = item->next)
		{

			item->data->id = newId;
			newId++;
		}
		/*for (ListItem<Inventity*>* item = inventities.start; item != NULL; item = item->next) 
		{
			if (item->data->id > highestId)
			{
				highestId = item->data->id;
			}
		}*/
		if (inventities.Count() > 0)
		{
			highestId = inventities.end->data->id;
		}
		else
		{
			highestId = -1;
		}
		

		Swordinv* sword = new Swordinv();
		sword->id = highestId+1;
		sword->type = InventityType::ESPADA;
		sword->damage = ataque;
		sword->durability = durabilidad;
		sword->magic = magia;
		sword->weight = peso;
		sword->icon = app->tex->Load("Assets/Textures/espmadIcon.png");
		entity = sword;
		break;
	}

	case EntityType::RESOURCE_ARMADURA:
	{
		for (ListItem<Inventity*>* item = inventities.start; item != NULL; item = item->next)
		{
			if (item->data->id > highestId)
			{
				highestId = item->data->id;
			}
		}

		ArmaduraInv* armadura = new ArmaduraInv();
		armadura->id = highestId + 1;
		armadura->type = InventityType::ARMADURA;
		armadura->defense = defensa;
		armadura->durability = durabilidad;
		armadura->weight = peso;
		armadura->icon = app->tex->Load("Assets/Textures/armaduraicon.png");
		entity = armadura;
	}
		
	default:
		break;
	}


	
	AddItem(entity);

	return entity;
}

bool InventoryManager::IsFull()
{
	
	// Encontrar el ID más alto actualmente asignado

	
	// Verificar si el siguiente ID disponible es 9
	if (inventities.Count() == 10) {
		return true;
	}
	else {
		return false;
	}
		
}

void InventoryManager::DestroyItem(Inventity* entity)
{
	ListItem<Inventity*>* item;

	for (item = inventities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) inventities.Del(item);
	}
}

void InventoryManager::DestroyItem2(int entityId)
{
	ListItem<Inventity*>* item;

	for (item = inventities.start; item != NULL; item = item->next)
	{

			if (item->data->id == entityId) // Comprueba si el ID coincide
			{
				if (item->data->stackable && item->data->quantity > 1)
				{
					item->data->quantity--;
				}
				else
				{
					inventities.Del(item);
					delete item->data; // Libera la memoria de la espada eliminada
				}
				

				break; // Termina el bucle después de eliminar la espada
			}
		
		
	}
	// Reasignar los IDs después de la eliminación
	int newId = 0;
	for (item = inventities.start; item != nullptr; item = item->next)
	{

		item->data->id = newId;
		newId++;
	}
}

void InventoryManager::UseItemSelected(int id)
{

	ListItem<Inventity*>* item;

	/*for (item = inventities.start; item != NULL; item = item->next)
	{
		item->data->active = false;
	}*/
	bool foundMatchingID = false;

	for (item = inventities.start; item != NULL; item = item->next)
	{

		if (item->data->id == id) // Comprueba si el ID coincide
		{
			 foundMatchingID = true;
			item->data->active = true;
			switch (item->data->type)
			{
			case InventityType::ESPADA:
				{
					//app->scene->GetPlayer()->espadaHierro = false; //ponemos la textura de la correspondiente
					//app->scene->GetPlayer()->espadaMadera = true;
					//app->scene->GetPlayer()->armaduraPoner = false;
					//Swordinv* espada = dynamic_cast<Swordinv*>(item->data); // Convierte a Espada si es posible

					//	app->scene->GetPlayer()->espadaMadera = true;
					//	app->scene->GetPlayer()->espadaHierro = false;
					//	app->scene->GetPlayer()->ataque = espada->damage;
					//	app->scene->GetPlayer()->durabilidadArma = espada->durability;
					//	app->scene->GetPlayer()->magia = espada->magic;
					//	app->scene->GetPlayer()->peso = espada->weight;
					
					

					
					break;
				}
			case InventityType::ESPADA2:
				{
				//app->scene->GetPlayer()->espadaHierro = true; //ponemos la textura de la correspondiente
				//app->scene->GetPlayer()->espadaMadera = false;
				//app->scene->GetPlayer()->armaduraPoner = false;
				//Swordinv* espada = dynamic_cast<Swordinv*>(item->data); // Convierte a Espada si es posible

				//app->scene->GetPlayer()->ataque = espada->damage;
				//app->scene->GetPlayer()->durabilidadArma = espada->durability;
				//app->scene->GetPlayer()->magia = espada->magic;
				//app->scene->GetPlayer()->peso = espada->weight;
					
					
					break;
				}
			case InventityType::ARMADURA:
			{
				//app->scene->GetPlayer()->armaduraPoner = true; //ponemos la textura de la correspondiente
				//app->scene->GetPlayer()->espadaMadera = false;
				//app->scene->GetPlayer()->espadaHierro = false;
				//ArmaduraInv* armadura = dynamic_cast<ArmaduraInv*>(item->data); // Convierte a Espada si es posible

				//app->scene->GetPlayer()->defensa = armadura->defense;
				//app->scene->GetPlayer()->durabilidadArma = armadura->durability;
				//app->scene->GetPlayer()->magia = armadura->magic;
				//app->scene->GetPlayer()->peso = armadura->weight;


				break;
			}
			}

			
		}


	}
	if (!foundMatchingID)
	{
		/*app->scene->GetPlayer()->espadaMadera = false;
		app->scene->GetPlayer()->espadaHierro = false;
		app->scene->GetPlayer()->armaduraPoner = false;*/
	}
}

void InventoryManager::OnMovePointer()
{

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN && PointerPosition.x < 420) {
		PointerPosition.x += 75;
		PointerId += 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN && PointerPosition.x > 125) {
		PointerPosition.x -= 75;
		PointerId -= 1;
	}

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && PointerPosition.y < 1) {
		PointerPosition.y += 76;
		PointerId += 5;
	}
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && PointerPosition.y > -75) {
		PointerPosition.y -= 76;
		PointerId -= 5;
	}
}

void InventoryManager::AddItem(Inventity* entity)
{

	bool encontrado = false;
	if (entity != nullptr) {

		if (entity->type == InventityType::ITEM) {
			for (int i = 0; i < inventities.Count(); i++) {
				if (inventities.At(i)->data->type == InventityType::ITEM) {
					inventities.At(i)->data->quantity += entity->quantity;
					encontrado = true;
					break;
				}
			}
			if (!encontrado)
			{
				inventities.Add(entity);
			}
		
		}
		else {
			inventities.Add(entity);
		}

		ListItem<Inventity*>* item;

		int newId = 0;
		for (item = inventities.start; item != nullptr; item = item->next)
		{

			item->data->id = newId;
			newId++;
		}
		
	
	} 
}

bool InventoryManager::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN) {
		mostrar = !mostrar;
	}
	bool ret = true;



	if (mostrar == true)
	{
		OnMovePointer();

		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
			options = true;
			selected = { PointerPosition.x, PointerPosition.y };
			selectedId = PointerId;
			
		}

		if (options == true)
		{
			if (app->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
			{
				DestroyItem2(selectedId);
				 options = false;
				 selected = { -1000, -1000 };

			}

			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				equiped = { PointerPosition.x, PointerPosition.y };
				equipedId = PointerId;
				UseItemSelected(equipedId);
				 options = false;
				 selected = { -1000, -1000 };

			}
		}

		

	}

	




	return ret;
}

bool InventoryManager::PostUpdate()
{
	bool ret = true;
	app->tex->GetSize(InventoryBackground, texW, texH);

	




	if (mostrar == true)
	{
		ListItem<Inventity*>* item;
		Inventity* pEntity = NULL;
		app->render->DrawTexture(InventoryBackground, texW / 8, texH / 8 - 200);
		
		
		

		app->render->DrawTexture(EquipedItemText, equiped.x, equiped.y);

		app->render->DrawTexture(PointerItemText, PointerPosition.x, PointerPosition.y);
		app->render->DrawTexture(SelectedItemText, selected.x, selected.y);

		for (item = inventities.start; item != nullptr; item = item->next)
		{
			pEntity = item->data;

			if((pEntity->stackable))
			{
				std::string quantityStr = std::to_string(pEntity->quantity);
				for (ListItem<Inventity*>* itam = inventities.start; itam != NULL && ret == true; itam = itam->next)
				{

					if (itam->data->type == InventityType::ITEM)
					{
						Iteminv* moneda = dynamic_cast<Iteminv*>(itam->data);

							if (pEntity->quantity > 1)
							{
								
								
								if (itam->data->id < 5)
								{
									app->render->DrawText(quantityStr.c_str(), 485 + itam->data->id * 75, 340, 20, 20);
								}
								else
								{
									app->render->DrawText(quantityStr.c_str(), 485 + (itam->data->id-5) * 75, 420, 20, 20);
								}
							}
							{
								if (itam->data->id < 5)
								{
									app->render->DrawTexture(pEntity->icon, 435 + pEntity->id * 75, 300);
								}
								else
								{
									app->render->DrawTexture(pEntity->icon, 445 + ((pEntity->id - 5) * 75), 380);
								}
								
							}

						
					}
				}
			}
			else
			{
				if (pEntity->id < 5) //if(inventities.cout() < 5)
				{
					app->render->DrawTexture(pEntity->icon, 445 + pEntity->id * 75, 300);
				}
				else
				{

					app->render->DrawTexture(pEntity->icon, 445 + ((pEntity->id - 5) * 75), 380);
				}
				
				
			}
			
			
			
			
			
			

		}

		
	}



	return ret;
}
