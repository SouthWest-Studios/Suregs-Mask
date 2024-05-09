
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

	iconoTreeTexturePath = ((char*)config.child("bestiario").attribute("texturePath").as_string());
	listTexturePath = ((char*)config.child("bestiario").attribute("list_texture").as_string());
	sliderTexturePath = ((char*)config.child("bestiario").attribute("slider_texture").as_string());
	knobTexturePath = ((char*)config.child("bestiario").attribute("knob_texture").as_string());
	PointerPath = ((char*)config.child("bestiario").attribute("pointer").as_string());
	CloseUpPath = ((char*)config.child("bestiario").attribute("closeUp").as_string());
	name = ((char*)config.child("bestiario").attribute("name").as_string());
	description = ((char*)config.child("bestiario").attribute("description").as_string());


	return ret;

}

bool TreeManager::Start() {



	CloseUp = app->tex->Load(CloseUpPath);
	listTexture = app->tex->Load(listTexturePath);
	sliderTexture = app->tex->Load(sliderTexturePath);
	knobTexture = app->tex->Load(knobTexturePath);
	PointerItemText = app->tex->Load(PointerPath);
	PointerId = -2;
	PointerItemText = nullptr;

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
		for (int j = 0; j < 4; j++)
		{
			if (i == 0 && j == 0)
			{
				CreateItem(name, description, TreeType::MASK0, i, j);
			}
			else if (i == 0 && j == 1)
			{
				CreateItem(name, description, TreeType::MASK1, i, j);
			}
			else if (i == 0 && j == 2)
			{
				CreateItem(name, description, TreeType::MASK2, i, j);
			}
			else if (i == 0 && j == 3)
			{
				CreateItem(name, description, TreeType::MASK3, i, j);
			}
			else
			{
				CreateItem(name, description, TreeType::BUTTON, i, j);
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
Tree* TreeManager::CreateItem(char* name, char* description, TreeType type, int nivelArbol, int nivelMejora)
{
	Tree* entity = nullptr;


	highesttttId = (arboles.Count() > 0) ? arboles.end->data->id : -1;
	entity = new Tree();
	entity->id = highesttttId + 1;
	entity->closeUpTrees = CloseUp;
	entity->name = name;
	entity->icon = app->tex->Load(iconoTreeTexturePath);
	entity->type = type;
	entity->nivelArbol = nivelArbol;
	entity->nivelMejora = nivelMejora;

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

	/*for (item = bestiario.start; item != NULL; item = item->next)
	{
		item->data->active = false;
	}*/

	for (item = arboles.start; item != NULL; item = item->next)
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
					switch (item->data->nivelMejora)
					{

					case 0:
					{


						break;
					}
					case 1:
					{


						break;
					}
					case 2:
					{


						break;
					}
					case 3:
					{


						break;
					}
					}
				case 0:
				{
					switch (item->data->nivelMejora)
					{

					case 0:
					{


						break;
					}
					case 1:
					{


						break;
					}
					case 2:
					{


						break;
					}
					case 3:
					{


						break;
					}
					}

					break;
				}
				case 1:
				{
					switch (item->data->nivelMejora)
					{

					case 0:
					{


						break;
					}
					case 1:
					{


						break;
					}
					case 2:
					{


						break;
					}
					case 3:
					{


						break;
					}
					}

					break;
				}
				case 2:
				{
					switch (item->data->nivelMejora)
					{

					case 0:
					{


						break;
					}
					case 1:
					{


						break;
					}
					case 2:
					{


						break;
					}
					case 3:
					{


						break;
					}
					}

					break;
				}
				case 3:
				{
					switch (item->data->nivelMejora)
					{

					case 0:
					{


						break;
					}
					case 1:
					{


						break;
					}
					case 2:
					{


						break;
					}
					case 3:
					{


						break;
					}
					}

					break;
				}
				}
			}
			}


		}


	}

}

void TreeManager::OnMovePointer()
{
	bool pointer = false;
	
		if (pointer == false)
		{
			PointerItemText = app->tex->Load(PointerPath);
			pointer = true;
		}

		if (app->input->GetButton(RIGHT) == KEY_DOWN) {
			if (horitzontalPointerId + 1 > 4)
			{
				horitzontalPointerId = 0;
				PointerPosition.x = 660;
				PointerId -= 4;
			}
			else
			{
				if (PointerId != -1)
				{
					horitzontalPointerId += 1;
					PointerPosition.x += 83;
				}
				PointerId += 1;
			}

		}
		if (app->input->GetButton(LEFT) == KEY_DOWN) {
			if (horitzontalPointerId - 1 < 0)
			{

				horitzontalPointerId += 4;
				PointerPosition.x = 660;
				PointerId += 4;
			}
			else
			{
				horitzontalPointerId -= 1;
				PointerPosition.x -= 83;
				PointerId -= 1;
			}
		}

		if (app->input->GetButton(DOWN) == KEY_DOWN) {
			if (verticalPointerId < 3)
			{
				PointerId += 5;
				verticalPointerId += 1;
				PointerPosition.y += 83;
			}
			else
			{
				verticalPointerId = 0;
				PointerId -= 16;
				PointerPosition.y = 230;
			}



		}
		if (app->input->GetButton(UP) == KEY_DOWN) {
			if (verticalPointerId + 1 > 3)
			{
				PointerPosition.y -= 83;
				PointerId -= 5;
				verticalPointerId -= 1;
			}
			else
			{
				verticalPointerId = 3;
				PointerId += 16;
				PointerPosition.y = 230 + 83 * (3);
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

	if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN) {
		mostrar == true;

	}

	if (mostrar == true)
	{
		OnMovePointer();

		if (app->input->GetButton(SELECT) == KEY_DOWN) {
			/*options = true;
			selected = { PointerPosition.x, PointerPosition.y };
			selectedId = PointerId;*/
			UseTreeSelected(PointerId);

		}

		if (app->input->GetButton(APP_EXIT) == KEY_DOWN && zoomIn == true) {
			ListItem<Tree*>* item;
			for (item = arboles.start; item != NULL; item = item->next)
			{
				item->data->zoom = false;

			}
			zoomIn = false;

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
		/*app->render->DrawRectangle(viewport, 0, 0, 0, 200, true, false);*/
		ListItem<Tree*>* item;
		Tree* pEntity = NULL;






		for (item = arboles.start; item != nullptr; item = item->next)
		{





			pEntity = item->data;
			int rowIndex = item->data->id / maxItemsPerRow; // Calcula el índice de la fila
			int columnIndex = item->data->id % maxItemsPerRow; // Calcula el índice de la columna
			int horizontalPosition = 660 + columnIndex * 83; // Calcula la posición horizontal
			int verticalPosition = 230 + rowIndex * 83; // Calcula la posición vertical

			int y2 = PointerPosition.y - scrollY;

			int y = verticalPosition - scrollY;

			if (y2 >= viewport.y && y2 <= viewport.y + viewport.h)
			{
				if (y >= viewport.y && y <= viewport.y + viewport.h) {
					if (zoomIn == false && app->notesManager->zoomIn == false)
					{
						app->render->DrawTexture(listTexture, horizontalPosition, verticalPosition, SDL_FLIP_NONE, 0, 0);

						app->render->DrawTexture(pEntity->icon, horizontalPosition, verticalPosition, 0.8, SDL_FLIP_NONE, 0, 0);
					}

				}
			}

			else
			{
				int targetY = PointerId / 4 * 83;


				scrollY = targetY;

				scrollY = std::max(0, scrollY);
			}
			app->render->DrawTexture(PointerItemText, PointerPosition.x, PointerPosition.y, SDL_FLIP_NONE, 0, 0);
		}


	}
	ret = true;
	int knobY = 200;
	if (numberRows == 0)
	{
		knobY = 200;
	}
	else
	{
		knobY = 200 + (341 / numberRows) * verticalPointerId;
	}


	if (mostrar == true)
	{
		app->render->DrawTexture(sliderTexture, 960, 200, SDL_FLIP_NONE, 0, 0);
		app->render->DrawTexture(knobTexture, 960, knobY, SDL_FLIP_NONE, 0, 0);

		ListItem<Tree*>* itum;
		for (itum = arboles.start; itum != nullptr; itum = itum->next)
		{
			if (itum->data->zoom)
			{



				if (PointerId == itum->data->id)
				{
					app->render->DrawTexture(itum->data->closeUpTrees, 400, 100, SDL_FLIP_NONE, 0, 0);
					app->render->DrawText(itum->data->desc.c_str(), 450, 200, 270, 400, 0, 0, 0, 0, false);
				}
			}
		}


	}

	return ret;
}






