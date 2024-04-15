
#include "EntityManager.h"
#include "InventoryManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene_testing.h"
#include "inventity.h"
#include "SwordInv.h"
#include "CuernoInv.h"
#include "ArmaduraInv.h"
#include "DiamanteInv.h"
#include "ItemInv.h"
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

	PointerItemText = app->tex->Load("Assets/Textures/Interfaz/select.png");
	SelectedItemText = app->tex->Load("Assets/Textures/Interfaz/selected.png");
	EquipedItemText = app->tex->Load("Assets/Textures/Interfaz/equiped.png");

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

	case EntityType::ITEM_CUERNO:
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
		

		CuernoInv* cuerno = new CuernoInv();
		cuerno->id = highestId+1;
		cuerno->type = InventityType::CUERNO;
		cuerno->stackable = true;
		/*sword->damage = ataque;
		sword->durability = durabilidad;
		sword->magic = magia;
		sword->weight = peso;*/
		cuerno->icon = app->tex->Load("Assets/Textures/Interfaz/cuernoInv.png");
		entity = cuerno;
		break;
	}
	case EntityType::ITEM_DIAMANTE:
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


		DiamanteInv* diamante = new DiamanteInv();
		diamante->id = highestId + 1;
		diamante->type = InventityType::DIAMANTE;
		diamante->stackable = true;
		/*sword->damage = ataque;
		sword->durability = durabilidad;
		sword->magic = magia;
		sword->weight = peso;*/
		diamante->icon = app->tex->Load("Assets/Textures/Interfaz/diamante_inv.png");
		entity = diamante;
		break;
	}
	case EntityType::ITEM_ARMADURA:
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
	
	// Encontrar el ID m�s alto actualmente asignado

	
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
				

				break; // Termina el bucle despu�s de eliminar la espada
			}
		
		
	}
	// Reasignar los IDs despu�s de la eliminaci�n
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
			//case InventityType::CU:
			//	{
			//		//app->scene->GetPlayer()->espadaHierro = false; //ponemos la textura de la correspondiente
			//		//app->scene->GetPlayer()->espadaMadera = true;
			//		//app->scene->GetPlayer()->armaduraPoner = false;
			//		//Swordinv* espada = dynamic_cast<Swordinv*>(item->data); // Convierte a Espada si es posible

			//		//	app->scene->GetPlayer()->espadaMadera = true;
			//		//	app->scene->GetPlayer()->espadaHierro = false;
			//		//	app->scene->GetPlayer()->ataque = espada->damage;
			//		//	app->scene->GetPlayer()->durabilidadArma = espada->durability;
			//		//	app->scene->GetPlayer()->magia = espada->magic;
			//		//	app->scene->GetPlayer()->peso = espada->weight;
			//		
			//		

			//		
			//		break;
			//	}
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

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN && PointerPosition.x < 143) {
		PointerPosition.x += 100;
		PointerId += 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && PointerPosition.x > -43) {
		PointerPosition.x -= 100;
		PointerId -= 1;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && PointerPosition.y < 1) {
		PointerPosition.y += 76;
		PointerId += 5;
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && PointerPosition.y > -160) {
		PointerPosition.y -= 76;
		PointerId -= 5;
	}
}

void InventoryManager::AddItem(Inventity* entity)
{

	bool CuernoEncontrado = false;

	bool DiamanteEncontrado = false;
	if (entity != nullptr) {

		if (entity->stackable)
		{
			if (entity->type == InventityType::CUERNO)
			{


				for (int i = 0; i < inventities.Count(); i++) {
					if (inventities.At(i)->data->type == InventityType::CUERNO) {
						inventities.At(i)->data->quantity += entity->quantity;
						CuernoEncontrado = true;
						break;
					}
					
				}
				if (!CuernoEncontrado)
				{
					inventities.Add(entity);
				}
			}
			else if (entity->type == InventityType::DIAMANTE)
			{
				for (int i = 0; i < inventities.Count(); i++) {
					
					if (inventities.At(i)->data->type == InventityType::DIAMANTE) {
						inventities.At(i)->data->quantity += entity->quantity;
						DiamanteEncontrado = true;
						break;
					}
				}
				if (!DiamanteEncontrado)
				{
					inventities.Add(entity);
				}
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
	
	bool ret = true;



	if (mostrar == true)
	{
		OnMovePointer();

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
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
		
		app->render->DrawTexture(EquipedItemText, equiped.x, equiped.y, SDL_FLIP_NONE, 0, 0);

		app->render->DrawTexture(PointerItemText, PointerPosition.x, PointerPosition.y,SDL_FLIP_NONE, 0, 0);
		app->render->DrawTexture(SelectedItemText, selected.x, selected.y, SDL_FLIP_NONE, 0, 0);

		for (item = inventities.start; item != nullptr; item = item->next)
		{
			pEntity = item->data;

			if((pEntity->stackable))
			{
				std::string quantityStr = std::to_string(pEntity->quantity);
				
					if (pEntity->type == InventityType::CUERNO)
					{

						if (pEntity->quantity > 1)
						{


							if (item->data->id < 5)
							{
								app->render->DrawText(quantityStr.c_str(), 320 + item->data->id * 100, 260, 20, 20);
							}
							else
							{
								app->render->DrawText(quantityStr.c_str(), 485 + (item->data->id - 5) * 100, 420, 20, 20);
							}
						}
						{
							if (item->data->id < 5)
							{
								app->render->DrawTexture(pEntity->icon, 290 + pEntity->id * 100, 230, SDL_FLIP_NONE, 0, 0);
							}
							else
							{
								app->render->DrawTexture(pEntity->icon, 445 + ((pEntity->id - 5) * 100), 380, SDL_FLIP_NONE, 0, 0);
							}

						}

					}
					else if (pEntity->type == InventityType::DIAMANTE)
					{

						if (pEntity->quantity > 1)
						{


							if (item->data->id < 5)
							{
								app->render->DrawText(quantityStr.c_str(), 320 + item->data->id * 100, 260, 20, 20);
							}
							else
							{
								app->render->DrawText(quantityStr.c_str(), 485 + (item->data->id - 5) * 100, 420, 20, 20);
							}
						}
						{
							if (item->data->id < 5)
							{
								app->render->DrawTexture(pEntity->icon, 290 + pEntity->id * 100, 230, SDL_FLIP_NONE, 0, 0);
							}
							else
							{
								app->render->DrawTexture(pEntity->icon, 445 + ((pEntity->id - 5) * 100), 380, SDL_FLIP_NONE, 0, 0);
							}

						}

					}
				
			}
			else
			{
				if (pEntity->id < 5) //if(inventities.cout() < 5)
				{
					app->render->DrawTexture(pEntity->icon, 445 + pEntity->id * 100, 300, SDL_FLIP_NONE, 0, 0);
				}
				else
				{

					app->render->DrawTexture(pEntity->icon, 445 + ((pEntity->id - 5) * 100), 380, SDL_FLIP_NONE, 0, 0);
				}
				
				
			}
			
			
			
			
			
			

		}

		
	}



	return ret;
}
