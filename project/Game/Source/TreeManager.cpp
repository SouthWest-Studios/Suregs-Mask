
#include "EntityManager.h"
#include "TreeManager.h"
#include "NotesManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene_testing.h"
#include "Tree.h"
#include "SwordInv.h"
#include "GarraInv.h"
#include "ArmaduraInv.h"
#include "DiamanteInv.h"
#include "ItemInv.h"
#include "OjoInv.h"
#include "Defs.h"
#include "Log.h"
#include "SString.h"

TreeManager::TreeManager(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name = ("treemanager");
}

// Destructor
TreeManager::~TreeManager()
{}

// Called before render is available
bool TreeManager::Awake(pugi::xml_node config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Tree*>* item;
	Tree* pEntity = NULL;

	for (item = arboles.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	iconoTreeTexturePath = ((char*)config.child("arbol").attribute("texturePath").as_string());
	listTexturePath = ((char*)config.child("arbol").attribute("list_texture").as_string());
	listTexturePathNot = ((char*)config.child("arbol").attribute("list_texture").as_string());
	PointerPath = ((char*)config.child("arbol").attribute("pointer").as_string());
	CloseUpPath = ((char*)config.child("arbol").attribute("closeUp").as_string());
	BackGroundPath = ((char*)config.child("arbol").attribute("BackGround").as_string());


	return ret;

}

bool TreeManager::Start() {


	BackGroundTexture = app->tex->Load("Assets/Textures/Interfaz/Arbol/ArbolFondo.png");
	CloseUp = app->tex->Load(CloseUpPath);
	listTexture = app->tex->Load("Assets/Textures/Interfaz/Arbol/textura_lista.png");
	PointerItemText = app->tex->Load("Assets/Textures/Interfaz/Arbol/noteselect.png");
	listTextureNot = app->tex->Load("Assets/Textures/Interfaz/Arbol/textura_lista_not.png");
	PointerId = 0;

	bool ret = true;

	//Iterates over the entities and calls Start
	ListItem<Tree*>* item;
	Tree* pEntity = NULL;

	for (item = arboles.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	scrollY = 0;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (i == 0 && j == 0)
			{
				CreateItem(TreeType::MASK0, i, j, true);
			}
			else if (i == 1 && j == 0)
			{
				CreateItem(TreeType::MASK1, i, j, true);
			}
			else if (i == 2 && j == 0)
			{
				CreateItem(TreeType::MASK2, i, j, true);
			}
			else if (i == 3 && j == 0)
			{
				CreateItem(TreeType::MASK3, i, j, true);
			}
			else
			{
				CreateItem(TreeType::BUTTON, i, j, false);
			}
			
		}
	}
		

	return ret;
}

// Called before quitting
bool TreeManager::CleanUp()
{
	bool ret = true;
	ListItem<Tree*>* item;
	item = arboles.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	arboles.Clear();
	app->tex->UnLoad(PointerItemText);
	app->tex->UnLoad(SelectedItemText);
	app->tex->UnLoad(EquipedItemText);
	app->tex->UnLoad(listTexture);
	return ret;
}
int highesttttId = -1;
Tree* TreeManager::CreateItem(TreeType type, int nivelArbol, int nivelMejora, bool usable)
{
	Tree* entity = nullptr;


	highesttttId = (arboles.Count() > 0) ? arboles.end->data->id : -1;
	entity = new Tree();
	entity->id = highesttttId + 1;
	entity->closeUpTrees = CloseUp;
	
	entity->icon = app->tex->Load(iconoTreeTexturePath);
	entity->type = type;
	entity->nivelArbol = nivelArbol;
	entity->nivelMejora = nivelMejora;
	entity->usable1 = usable;
	entity->usable2 = usable;
	entity->usable3 = usable;
	entity->usable4 = usable;
	

	switch (nivelArbol)
	{
		
	case 0:
	{
		
		switch (nivelMejora)
		{

		case 0:
		{
			entity->name = "mask 1";
			entity->desc = "a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a a";
			entity->desc1 = "b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b";
			entity->desc2 = "c c c c c c c c c c c c c c c c c c c c c c c c c c c c c c c c c c c c c c c c c c c c c c c c c";
			entity->desc3 = "d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d d";


			break;
		}
		case 1:
		{
			entity->name = "Dano 1";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";
			entity->usable1 = true;
			entity->usable2 = true;
			entity->usable3 = true;
			entity->usable4 = true;
			break;
		}
		case 2:
		{
			entity->name = "Dano 2";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";

			break;
		}
		case 3:
		{
			entity->name = "Dano 3";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";

			break;
		}
		case 4:
		{
			entity->name = "Dano 4";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";

			break;
		}
		}

		break;
	}
	case 1:
	{

		switch (nivelMejora)
		{

		case 0:
		{
			entity->name = "mask 2";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";

			break;
		}
		case 1:
		{
			entity->name = "CoolDown 1";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";
			entity->usable1 = true;
			entity->usable2 = true;
			entity->usable3 = true;
			entity->usable4 = true;

			break;
		}
		case 2:
		{
			entity->name = "CoolDown 2";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";

			break;
		}
		case 3:
		{
			entity->name = "CoolDown 3";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";

			break;
		}
		case 4:
		{
			entity->name = "CoolDown 4";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";

			break;
		}
		}

		break;
	}
	case 2:
	{
		switch (nivelMejora)
		{

		case 0:
		{
			entity->name = "mask3";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";

			break;
		}
		case 1:
		{
			entity->name = "Rayos 1";
			entity->name2 = "Alcance 1";
			entity->name3 = "Distancia 1";
			entity->name4 = "Duracion 1";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";
			entity->usable1 = true;
			entity->usable2 = true;
			entity->usable3 = true;
			entity->usable4 = true;

			break;
		}
		case 2:
		{
			entity->name = "Rayos 2";
			entity->name2 = "Alcance 2";
			entity->name3 = "Distancia 2";
			entity->name4 = "Duracion 2";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";

			break;
		}
		case 3:
		{
			entity->name = "Rayos 3";
			entity->name2 = "Alcance 3";
			entity->name3 = "Distancia 3";
			entity->name4 = "Duracion 3";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";

			break;
		}
		case 4:
		{
			entity->name = "Rayos 4";
			entity->name2 = "Alcance 4";
			entity->name3 = "Distancia 4";
			entity->name4 = "Duracion 4";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";

			break;
		}
		}

		break;
	}
	case 3:
	{
		switch (nivelMejora)
		{

		case 0:
		{
			entity->name = "mask4";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";

			break;
		}
		case 1:
		{
			entity->name = "Veneno 1";
			entity->name2 = "Invisible 1";
			entity->name3 = "Aumento 1";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";
			entity->usable1 = true;
			entity->usable2 = true;
			entity->usable3 = true;
			entity->usable4 = true;

			break;
		}
		case 2:
		{
			entity->name = "Veneno 2";
			entity->name2 = "Invisible 2";
			entity->name3 = "Aumento 2";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";

			break;
		}
		case 3:
		{
			entity->name = "Veneno 3";
			entity->name2 = "Invisible 3";
			entity->name3 = "Aumento 3";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";

			break;
		}
		case 4:
		{
			entity->name = "Veneno 4";
			entity->name2 = "Invisible 4";
			entity->name3 = "Aumento 4";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";

			break;
		}
		}

		break;
	}
	}

	AddTree(entity);

	return entity;
}

bool TreeManager::IsFull()
{

	// Encontrar el ID más alto actualmente asignado


	// Verificar si el siguiente ID disponible es 9
	/*if (arboles.Count() == 8) {
		return true;
	}
	else {
		return false;
	}*/
	return false;
}

bool TreeManager::LoadState(pugi::xml_node node)
{
	bool ret = true;

	ListItem<Tree*>* item;
	item = arboles.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	arboles.Clear();

	pugi::xml_document configFile;
	pugi::xml_node entitiesDataNode;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	entitiesDataNode = configFile.child("config").child("entities_data");

	//Tree* TreeItem = app->TreeManager->CreateItem(, 0, 0, 0, 0, 0, 0);
	//for (pugi::xml_node itemNode = node.child("Tree").child("Tree"); itemNode; itemNode = itemNode.next_sibling("Tree"))
	//{

	//	Tree* itemLoaded = nullptr;
	//	pugi::xml_node itemConfigurationNode;
	//	switch ((TreeType)itemNode.attribute("type").as_int())
	//	{
	//	case TreeType::BEST:
	//		/*itemConfigurationNode = entitiesDataNode.child("item_garra");
	//		itemLoaded = app->bestiarioManager->CreateItem(EntityType::ITEM_NOTA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string());*/
	//		break;

	//	case TreeType::UNKNOWN:
	//		break;
	//	default:
	//		break;
	//	}


	//}




	return ret;
}

bool TreeManager::SaveState(pugi::xml_node node)
{
	bool ret = true;
	/*pugi::xml_node TreeNode = node.append_child("Tree");

	for (int i = 0; i < arboles.Count(); i++) {

		Tree* TreeItem = arboles.At(i)->data;

		pugi::xml_node TreeItemNode = TreeNode.append_child("Tree");
		TreeItemNode.append_attribute("type").set_value((int)TreeItem->type);
		TreeItemNode.append_attribute("quantity").set_value(TreeItem->quantity);
	}*/




	return ret;
}


void TreeManager::UseTreeSelected(int id)
{

	ListItem<Tree*>* item;

	ListItem<Tree*>* itum;
	/*for (item = bestiario.start; item != NULL; item = item->next)
	{
		item->data->active = false;
	}*/
	
	for (item = arboles.start; item != NULL; item = item->next)
	{
		if (item->data->usable1 && mask ==1 || item->data->usable2 && mask == 2 || item->data->usable3 && mask == 3 || item->data->usable4 && mask == 4)
		{
			if (item->data->id == id) // Comprueba si el ID coincide
			{

				item->data->active = true;
				switch (item->data->type)
				{

				case TreeType::MASK0: 
				{
					mask = 1;

					break;
				}
				case TreeType::MASK1:
				{
					mask = 2;

					break;
				}
				case TreeType::MASK2:
				{
					mask = 3;

					break;
				}
				case TreeType::MASK3:
				{
					mask = 4;

					break;
				}
				case TreeType::BUTTON:
				{
					switch (item->data->nivelArbol)
					{
						
					case 0:
					{ 
						bool siguiente = false;
						if (mask == 1 /*&& app->entityManager->GetPlayer()->maskZeroXP*/ )
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK0][Branches::Rama1] += 1;
							item->data->usable1 = false;
							siguiente = true;
						}
						else if (mask == 2)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK1][Branches::Rama1] += 1;
							item->data->usable2 = false;
							siguiente = true;
						}
						else if (mask == 3)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK2][Branches::Rama1] += 1;
							item->data->usable3 = false;
							siguiente = true;
						}
						else if (mask == 4)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK3][Branches::Rama1] += 1;
							item->data->usable4 = false;
							siguiente = true;
						}
						if (siguiente == true)
						{
							for (itum = arboles.start; itum != NULL; itum = itum->next)
							{
								if (itum->data->nivelArbol == item->data->nivelArbol && itum->data->nivelMejora == item->data->nivelMejora + 1)
								{
									if (mask == 1)
									{
										
										itum->data->usable1 = true;
										
									}
									else if (mask == 2)
									{
										
										itum->data->usable2 = true;
										
									}
									else if (mask == 3)
									{
										
										itum->data->usable3 = true;
										
									}
									else if (mask == 4)
									{
										
										itum->data->usable4 = true;
										
									}
								}


							}
							numMejoras += 1;
						}
						break;
					}
					case 1:
					{
						bool siguiente = false;
						if (mask == 1)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK0][Branches::Rama2] += 1;
							item->data->usable1 = false;
							siguiente = true;
						}
						else if (mask == 2)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK1][Branches::Rama2] += 1;
							item->data->usable2 = false;
							siguiente = true;
						}
						else if (mask == 3)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK2][Branches::Rama2] += 1;
							item->data->usable3 = false;
							siguiente = true;
						}
						else if (mask == 4)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK3][Branches::Rama2] += 1;
							item->data->usable4 = false;
							siguiente = true;
						}
						if (siguiente == true)
						{
							for (itum = arboles.start; itum != NULL; itum = itum->next)
							{
								if (itum->data->nivelArbol == item->data->nivelArbol && itum->data->nivelMejora == item->data->nivelMejora + 1)
								{
									if (mask == 1)
									{

										itum->data->usable1 = true;

									}
									else if (mask == 2)
									{

										itum->data->usable2 = true;

									}
									else if (mask == 3)
									{

										itum->data->usable3 = true;

									}
									else if (mask == 4)
									{

										itum->data->usable4 = true;

									}
								}


							}
							numMejoras += 1;
						}
						break;
					}
					case 2:
					{
						bool siguiente = false;
						if (mask == 1)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK0][Branches::Rama3] += 1;
							item->data->usable1 = false;
							siguiente = true;
						}
						else if (mask == 2)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK1][Branches::Rama3] += 1;
							item->data->usable2 = false;
							siguiente = true;
						}
						else if (mask == 3)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK2][Branches::Rama3] += 1;
							item->data->usable3 = false;
							siguiente = true;
						}
						else if (mask == 4)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK3][Branches::Rama3] += 1;
							item->data->usable4 = false;
							siguiente = true;
						}
						if (siguiente == true)
						{
							for (itum = arboles.start; itum != NULL; itum = itum->next)
							{
								if (itum->data->nivelArbol == item->data->nivelArbol && itum->data->nivelMejora == item->data->nivelMejora + 1)
								{
									if (mask == 1)
									{

										itum->data->usable1 = true;

									}
									else if (mask == 2)
									{

										itum->data->usable2 = true;

									}
									else if (mask == 3)
									{

										itum->data->usable3 = true;

									}
									else if (mask == 4)
									{

										itum->data->usable4 = true;

									}
								}


							}
							numMejoras += 1;
						}
						break;
					}
					case 3:
					{
						bool siguiente = false;
						if (mask == 1)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK0][Branches::Rama4] += 1;
							item->data->usable1 = false;
							siguiente = true;
						}
						else if (mask == 2)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK1][Branches::Rama4] += 1;
							item->data->usable2 = false;
							siguiente = true;
						}
						else if (mask == 3)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK2][Branches::Rama4] += 1;
							item->data->usable3 = false;
							siguiente = true;
						}
						else if (mask == 4)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK3][Branches::Rama4] += 1;
							item->data->usable4 = false;
							siguiente = true;
						}
						if (siguiente == true)
						{
							for (itum = arboles.start; itum != NULL; itum = itum->next)
							{
								if (itum->data->nivelArbol == item->data->nivelArbol && itum->data->nivelMejora == item->data->nivelMejora + 1)
								{
									if (mask == 1)
									{

										itum->data->usable1 = true;

									}
									else if (mask == 2)
									{

										itum->data->usable2 = true;

									}
									else if (mask == 3)
									{

										itum->data->usable3 = true;

									}
									else if (mask == 4)
									{

										itum->data->usable4 = true;

									}
								}


							}
							numMejoras += 1;
						}
						break;
					}
					}

				}
				}
		}
		


		}


	}

	

}

void TreeManager::ReembolsarTreeSelected(int id)
{
	ListItem<Tree*>* item;

	ListItem<Tree*>* itum;


	for (item = arboles.start; item != NULL; item = item->next)
	{
		if (item->data->id == id)
		{
			for (itum = arboles.start; itum != NULL; itum = itum->next)
			{
				if ((itum->data->nivelArbol == item->data->nivelArbol && itum->data->nivelMejora == item->data->nivelMejora + 1) || item->data->nivelMejora + 1 == 5 && itum->data->type == TreeType::BUTTON)
				{
					if ((mask == 1 && item->data->usable1 == false && itum->data->usable1 == true) || (mask == 1 && item->data->usable1 == false && item->data->nivelMejora + 1 == 5))
					{
						if (item->data->nivelArbol == 0)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK0][Branches::Rama1] -= 1;
						}
						if (item->data->nivelArbol == 1)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK0][Branches::Rama2] -= 1;
						}
						if (item->data->nivelArbol == 2)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK0][Branches::Rama3] -= 1;
						}
						if (item->data->nivelArbol == 3)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK0][Branches::Rama4] -= 1;
						}
						
						item->data->usable1 = true;
						if (item->data->usable1 + 1 < 5)
						{
							itum->data->usable1 = false;
						}
						numMejoras -= 1;
						puntsRembolso += 1;
					}
					else if ((mask == 2 && item->data->usable2 == false && itum->data->usable2 == true) || (mask == 2 && item->data->usable2 == false && item->data->nivelMejora + 1 == 5))
					{
						if (item->data->nivelArbol == 0)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK1][Branches::Rama1] -= 1;
						}
						if (item->data->nivelArbol == 1)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK1][Branches::Rama2] -= 1;
						}
						if (item->data->nivelArbol == 2)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK1][Branches::Rama3] -= 1;
						}
						if (item->data->nivelArbol == 3)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK1][Branches::Rama4] -= 1;
						}
						item->data->usable2 = true;
						if (item->data->usable2 + 1 < 5)
						{
							itum->data->usable2 = false;
						}
						numMejoras -= 1;
						puntsRembolso += 1;
					}
					else if ((mask == 3 && item->data->usable3 == false && itum->data->usable3 == true) || (mask == 3 && item->data->usable3 == false && item->data->nivelMejora + 1 == 5))
					{
						if (item->data->nivelArbol == 0)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK2][Branches::Rama1] -= 1;
						}
						if (item->data->nivelArbol == 1)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK2][Branches::Rama2] -= 1;
						}
						if (item->data->nivelArbol == 2)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK2][Branches::Rama3] -= 1;
						}
						if (item->data->nivelArbol == 3)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK2][Branches::Rama4] -= 1;
						}
						item->data->usable3 = true;
						if (item->data->usable3 + 1 < 5)
						{
							itum->data->usable3 = false;
						}
						numMejoras -= 1;
						puntsRembolso += 1;
					}
					else if ((mask == 4 && item->data->usable4 == false && itum->data->usable4 == true) || (mask == 4 && item->data->usable4 == false && item->data->nivelMejora + 1 == 5))
					{
						if (item->data->nivelArbol == 0)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK3][Branches::Rama1] -= 1;
						}
						if (item->data->nivelArbol == 1)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK3][Branches::Rama2] -= 1;
						}
						if (item->data->nivelArbol == 2)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK3][Branches::Rama3] -= 1;
						}
						if (item->data->nivelArbol == 3)
						{
							app->entityManager->GetPlayer()->maskLevels[Mask::MASK3][Branches::Rama4] -= 1;
						}
						item->data->usable4 = true;
						if (item->data->usable4 + 1 < 5)
						{
							itum->data->usable4 = false;
						}
						numMejoras -= 1;
						puntsRembolso += 1;
					}
				}


			}
		}
		/*if (item->data->type == TreeType::MASK0)
		{
			item->data->usable1 = true;
		}*/

	}
}

void TreeManager::OnMovePointer()
{

		if (app->input->GetButton(RIGHT) == KEY_DOWN) {
			if (horitzontalPointerId + 1 == 5)
			{
				horitzontalPointerId = 0;
				PointerPosition.x = 350;
				PointerId -= 4;
			}
			else
			{
				if (PointerId != -1)
				{
					horitzontalPointerId += 1;
					PointerPosition.x += 120;
				}
				PointerId += 1;
			}

		}
		if (app->input->GetButton(LEFT) == KEY_DOWN) {
			if (horitzontalPointerId - 1 == -1)
			{

				horitzontalPointerId += 4;
				PointerPosition.x = 350 + 4 * 120;
				PointerId += 4;
			}
			else
			{
				horitzontalPointerId -= 1;
				PointerPosition.x -= 120;
				PointerId -= 1;
			}
		}

		if (app->input->GetButton(DOWN) == KEY_DOWN) {
			if (verticalPointerId < 3)
			{
				PointerId += 5;
				verticalPointerId += 1;
				PointerPosition.y += 100;
			}
			else
			{
				verticalPointerId = 0;
				PointerId -= 15;
				PointerPosition.y = 130;
			}



		}
		if (app->input->GetButton(UP) == KEY_DOWN) {
			if (verticalPointerId - 1 > -1)
			{
				PointerPosition.y -= 100;
				PointerId -= 5;
				verticalPointerId -= 1;
			}
			else
			{
				verticalPointerId = 3;
				PointerId += 15;
				PointerPosition.y = 130 + 100 * (3);
			}


		}

}

void TreeManager::AddTree(Tree* entity)
{

	if (entity != nullptr) {

		arboles.Add(entity);

	}

}

bool TreeManager::Update(float dt)
{

	bool ret = true;

	if (mostrar == true)
	{
		OnMovePointer();
		app->entityManager->active = false;
		app->physics->active = false;
		if (app->input->GetButton(CONFIRM) == KEY_DOWN) {
			/*options = true;
			selected = { PointerPosition.x, PointerPosition.y };
			selectedId = PointerId;*/
			UseTreeSelected(PointerId);

		}
		if (app->input->GetButton(BACK) == KEY_DOWN)
		{
			ReembolsarTreeSelected(PointerId);

		}
		
		if (app->input->GetButton(APP_EXIT) == KEY_DOWN) {
			mostrar = false;
			app->entityManager->active = true;
			app->physics->active = true;
		}
		if (numMejoras > 8) {
			numMejoras = 8; // Limitar a 8 si es mayor
		}
		if (puntsRembolso > 8) {
			puntsRembolso = 8; // Limitar a 8 si es mayor
		}
		int num;

		num = numberRows + 1;


		if (arboles.Count() > 3 * num)
		{
			numberRows += 1;
		}

	}






	return ret;
}

bool TreeManager::PostUpdate()
{
	bool ret = true;

	uint tradeSpacing = 83;

	SDL_Rect viewport = { 620, 200, 400, 350 };


	if (mostrar == true)
	{
		app->render->DrawTexture(BackGroundTexture,100, 100, SDL_FLIP_NONE, 0, 0);
		ListItem<Tree*>* item;
		Tree* pEntity = NULL;






		for (item = arboles.start; item != nullptr; item = item->next)
		{





			pEntity = item->data;
			int rowIndex = item->data->id / maxItemsPerRow; // Calcula el índice de la fila
			int columnIndex = item->data->id % maxItemsPerRow; // Calcula el índice de la columna
			int horizontalPosition = 350 + columnIndex * 120; // Calcula la posición horizontal
			int verticalPosition = 130 + rowIndex * 100; // Calcula la posición vertical

			
				
					if (zoomIn == false && app->notesManager->zoomIn == false)
					{
						app->render->DrawTexture(listTexture, horizontalPosition, verticalPosition, 0.7, SDL_FLIP_NONE, 0, 0);
						switch (mask)
						{
						case 1:
						{
							
							if (pEntity->usable1 == false)
							{
								app->render->DrawTexture(listTextureNot, horizontalPosition, verticalPosition, 0.7, SDL_FLIP_NONE, 0, 0);
							}

								app->render->DrawTextBound(pEntity->name.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });

							

							break;
						}
						case 2:
						{
							if (pEntity->usable2 == false)
							{
								app->render->DrawTexture(listTextureNot, horizontalPosition, verticalPosition, 0.7, SDL_FLIP_NONE, 0, 0);
							}
							if (pEntity->name2 == "")
							{
								app->render->DrawTextBound(pEntity->name.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });
							}
							else
							{
								app->render->DrawTextBound(pEntity->name2.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });
							
							}


							break;
						}
						case 3:
						{
							

								if (pEntity->usable3 == false)
								{
									app->render->DrawTexture(listTextureNot, horizontalPosition, verticalPosition, 0.7, SDL_FLIP_NONE, 0, 0);
								}

							if (pEntity->name3 == "")
							{
								app->render->DrawTextBound(pEntity->name.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });
							}
							else
							{
								app->render->DrawTextBound(pEntity->name3.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });

							}


							break;
						}
						case 4:
						{
							if (pEntity->usable4 == false)
							{
								app->render->DrawTexture(listTextureNot, horizontalPosition, verticalPosition, 0.7, SDL_FLIP_NONE, 0, 0);
							}
							if (pEntity->name4 == "")
							{
								app->render->DrawTextBound(pEntity->name.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });
							}
							else
							{
								app->render->DrawTextBound(pEntity->name4.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });

							}

							break;
						}
						default:
							break;
						}
						
						/*app->render->DrawTexture(pEntity->icon, horizontalPosition, verticalPosition, 0.8, SDL_FLIP_NONE, 0, 0);*/
					}

				
			app->render->DrawTexture(PointerItemText, PointerPosition.x, PointerPosition.y,0.7, SDL_FLIP_NONE, 0, 0);
			int a = 0;
		}


	}
	ret = true;


	if (mostrar == true)
	{

		ListItem<Tree*>* itum;
		for (itum = arboles.start; itum != nullptr; itum = itum->next)
		{


				if (PointerId == itum->data->id)
				{
					if (mask == 1)
					{

						app->render->DrawTextBound(itum->data->desc.c_str(), 1000, 120, 100, { 0,0,0 });

					}
					else if (mask == 2)
					{

						app->render->DrawTextBound(itum->data->desc1.c_str(), 1000, 120, 100, { 0,0,0 });

					}
					else if (mask == 3)
					{

						app->render->DrawTextBound(itum->data->desc2.c_str(), 1000, 120, 100, { 0,0,0 });

					}
					else if (mask == 4)
					{

						app->render->DrawTextBound(itum->data->desc3.c_str(), 1000, 120, 100, { 0,0,0 });

					}

					
					
				}
			
		}
		std::string texto =  std::to_string(numMejoras) + "/8 mejoras max";
		app->render->DrawTextBound(texto.c_str(), 645, 400, 370, { 0,0,0 });

		std::string texto1 = std::to_string(puntsRembolso) + "/8 Rembolso max";
		app->render->DrawTextBound(texto1.c_str(), 645, 450, 370, { 0,0,0 });

	}

	return ret;
}






