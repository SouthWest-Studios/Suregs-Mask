
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
#include "Menu.h"
#include "InventoryManager.h"
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
	LOG("Loading Tree Manager");
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
	PointerPath2 = ((char*)config.child("arbol").attribute("Pointer2").as_string());
	PointerPath3 = ((char*)config.child("arbol").attribute("Pointer3").as_string());
	CloseUpPath = ((char*)config.child("arbol").attribute("closeUp").as_string());
	BackGroundPath = ((char*)config.child("arbol").attribute("BackGround").as_string());

	alcancePath= ((char*)config.child("arbol").attribute("alcance").as_string());
	aumentoPath= ((char*)config.child("arbol").attribute("aumento").as_string());
	cooldownPath= ((char*)config.child("arbol").attribute("cooldown").as_string());
	danoPath= ((char*)config.child("arbol").attribute("dano").as_string());
	distanciaPath= ((char*)config.child("arbol").attribute("distancia").as_string());
	durationPath= ((char*)config.child("arbol").attribute("duration").as_string());
	invisiblePath= ((char*)config.child("arbol").attribute("invisible").as_string());
	poisonPath= ((char*)config.child("arbol").attribute("poison").as_string());
	rayoPath= ((char*)config.child("arbol").attribute("rayo").as_string());

	alcanceLockedPath = ((char*)config.child("arbol").attribute("alcancelocked").as_string());
	aumentoLockedPath = ((char*)config.child("arbol").attribute("aumentolocked").as_string());
	cooldownLockedPath = ((char*)config.child("arbol").attribute("cooldownlocked").as_string());
	danoLockedPath = ((char*)config.child("arbol").attribute("danolocked").as_string());
	distanciaLockedPath = ((char*)config.child("arbol").attribute("distancialocked").as_string());
	durationLockedPath = ((char*)config.child("arbol").attribute("durationlocked").as_string());
	invisibleLockedPath = ((char*)config.child("arbol").attribute("invisiblelocked").as_string());
	poisonLockedPath = ((char*)config.child("arbol").attribute("poisonlocked").as_string());
	rayoLockedPath = ((char*)config.child("arbol").attribute("rayolocked").as_string());

	alcanceSelectedPath = ((char*)config.child("arbol").attribute("alcanceselected").as_string());
	aumentoSelectedPath = ((char*)config.child("arbol").attribute("aumentoselected").as_string());
	cooldownSelectedPath = ((char*)config.child("arbol").attribute("cooldownselected").as_string());
	danoSelectedPath = ((char*)config.child("arbol").attribute("danoselected").as_string());
	distanciaSelectedPath = ((char*)config.child("arbol").attribute("distanciaselected").as_string());
	durationSelectedPath = ((char*)config.child("arbol").attribute("durationselected").as_string());
	invisibleSelectedPath = ((char*)config.child("arbol").attribute("invisibleselected").as_string());
	poisonSelectedPath = ((char*)config.child("arbol").attribute("poisonselected").as_string());
	rayoSelectedPath = ((char*)config.child("arbol").attribute("rayoselected").as_string());

	mascara0Path = ((char*)config.child("arbol").attribute("mascara0Path").as_string());
	mascara1Path = ((char*)config.child("arbol").attribute("mascara1Path").as_string());
	mascara2Path = ((char*)config.child("arbol").attribute("mascara2Path").as_string());
	mascara3Path = ((char*)config.child("arbol").attribute("mascara3Path").as_string());

	mascara0SelectedPath = ((char*)config.child("arbol").attribute("mascaraselected0Path").as_string());
	mascara1SelectedPath = ((char*)config.child("arbol").attribute("mascaraselected1Path").as_string());
	mascara2SelectedPath = ((char*)config.child("arbol").attribute("mascaraselected2Path").as_string());	
	mascara3SelectedPath = ((char*)config.child("arbol").attribute("mascaraselected3Path").as_string());

	return ret;

}

bool TreeManager::Start() {


	BackGroundTexture = app->tex->Load("Assets/Textures/Interfaz/Arbol/ArbolFondo.png");
	CloseUp = app->tex->Load(CloseUpPath);
	listTexture = app->tex->Load(listTexturePath);
	PointerItemText = app->tex->Load("Assets/Textures/Interfaz/Arbol/noteselect.png");
	listTextureNot = app->tex->Load(listTexturePathNot);
	PointerItemText2 = app->tex->Load(PointerPath2);
	PointerItemText3 = app->tex->Load(PointerPath3);
	PointerId = 0;

	mascara0Texture = app->tex->Load(mascara0Path);
	mascara1Texture = app->tex->Load(mascara1Path);
	mascara2Texture = app->tex->Load(mascara2Path);
	mascara3Texture = app->tex->Load(mascara3Path);

	mascara0SelectedTexture = app->tex->Load(mascara0SelectedPath);
	mascara1SelectedTexture = app->tex->Load(mascara1SelectedPath);
	mascara2SelectedTexture = app->tex->Load(mascara2SelectedPath);
	mascara3SelectedTexture = app->tex->Load(mascara3SelectedPath);



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

	/*entity->icon = app->tex->Load(iconoTreeTexturePath);*/
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
			entity->icon0 = app->tex->Load(danoPath);
			entity->iconLocked0 = app->tex->Load(danoLockedPath);
			entity->iconSelected0 = app->tex->Load(danoSelectedPath);
			break;
		}
		case 2:
		{
			entity->name = "Dano 2";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";
			entity->icon0 = app->tex->Load(danoPath);
			entity->iconLocked0 = app->tex->Load(danoLockedPath);
			entity->iconSelected0 = app->tex->Load(danoSelectedPath);
			break;
		}
		case 3:
		{
			entity->name = "Dano 3";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";
			entity->icon0 = app->tex->Load(danoPath);
			entity->iconLocked0 = app->tex->Load(danoLockedPath);
			entity->iconSelected0 = app->tex->Load(danoSelectedPath);
			break;
		}
		case 4:
		{
			entity->name = "Dano 4";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";
			entity->icon0 = app->tex->Load(danoPath);
			entity->iconLocked0 = app->tex->Load(danoLockedPath);
			entity->iconSelected0 = app->tex->Load(danoSelectedPath);
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
			entity->icon0 = app->tex->Load(cooldownPath);
			entity->iconLocked0 = app->tex->Load(cooldownLockedPath);
			entity->iconSelected0 = app->tex->Load(cooldownSelectedPath);
			break;
		}
		case 2:
		{
			entity->name = "CoolDown 2";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";
			entity->icon0 = app->tex->Load(cooldownPath);
			entity->iconLocked0 = app->tex->Load(cooldownLockedPath);
			entity->iconSelected0 = app->tex->Load(cooldownSelectedPath);
			break;
		}
		case 3:
		{
			entity->name = "CoolDown 3";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";
			entity->icon0 = app->tex->Load(cooldownPath);
			entity->iconLocked0 = app->tex->Load(cooldownLockedPath);
			entity->iconSelected0 = app->tex->Load(cooldownSelectedPath);
			break;
		}
		case 4:
		{
			entity->name = "CoolDown 4";
			entity->desc = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			entity->desc1 = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
			entity->desc2 = "ccccccccccccccccccccccccccccccccccccccccccccccccc";
			entity->desc3 = "dddddddddddddddddddddddddddddddddddddddddddddddddd";
			entity->icon0 = app->tex->Load(cooldownPath);
			entity->iconLocked0 = app->tex->Load(cooldownLockedPath);
			entity->iconSelected0 = app->tex->Load(cooldownSelectedPath);
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

			entity->icon0 = app->tex->Load(rayoPath);
			entity->iconLocked0 = app->tex->Load(rayoLockedPath);
			entity->iconSelected0 = app->tex->Load(rayoSelectedPath);

			entity->icon1 = app->tex->Load(alcancePath);
			entity->iconLocked1 = app->tex->Load(alcanceLockedPath);
			entity->iconSelected1 = app->tex->Load(alcanceSelectedPath);

			entity->icon2 = app->tex->Load(distanciaPath);
			entity->iconLocked2 = app->tex->Load(distanciaLockedPath);
			entity->iconSelected2 = app->tex->Load(distanciaSelectedPath);

			entity->icon3 = app->tex->Load(durationPath);
			entity->iconLocked3 = app->tex->Load(durationLockedPath);
			entity->iconSelected3 = app->tex->Load(durationSelectedPath);



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
			entity->icon0 = app->tex->Load(rayoPath);
			entity->iconLocked0 = app->tex->Load(rayoLockedPath);
			entity->iconSelected0 = app->tex->Load(rayoSelectedPath);

			entity->icon1 = app->tex->Load(alcancePath);
			entity->iconLocked1 = app->tex->Load(alcanceLockedPath);
			entity->iconSelected1 = app->tex->Load(alcanceSelectedPath);

			entity->icon2 = app->tex->Load(distanciaPath);
			entity->iconLocked2 = app->tex->Load(distanciaLockedPath);
			entity->iconSelected2 = app->tex->Load(distanciaSelectedPath);

			entity->icon3 = app->tex->Load(durationPath);
			entity->iconLocked3 = app->tex->Load(durationLockedPath);
			entity->iconSelected3 = app->tex->Load(durationSelectedPath);
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
			entity->icon0 = app->tex->Load(rayoPath);
			entity->iconLocked0 = app->tex->Load(rayoLockedPath);
			entity->iconSelected0 = app->tex->Load(rayoSelectedPath);

			entity->icon1 = app->tex->Load(alcancePath);
			entity->iconLocked1 = app->tex->Load(alcanceLockedPath);
			entity->iconSelected1 = app->tex->Load(alcanceSelectedPath);

			entity->icon2 = app->tex->Load(distanciaPath);
			entity->iconLocked2 = app->tex->Load(distanciaLockedPath);
			entity->iconSelected2 = app->tex->Load(distanciaSelectedPath);

			entity->icon3 = app->tex->Load(durationPath);
			entity->iconLocked3 = app->tex->Load(durationLockedPath);
			entity->iconSelected3 = app->tex->Load(durationSelectedPath);
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
			entity->icon0 = app->tex->Load(rayoPath);
			entity->iconLocked0 = app->tex->Load(rayoLockedPath);
			entity->iconSelected0 = app->tex->Load(rayoSelectedPath);

			entity->icon1 = app->tex->Load(alcancePath);
			entity->iconLocked1 = app->tex->Load(alcanceLockedPath);
			entity->iconSelected1 = app->tex->Load(alcanceSelectedPath);

			entity->icon2 = app->tex->Load(distanciaPath);
			entity->iconLocked2 = app->tex->Load(distanciaLockedPath);
			entity->iconSelected2 = app->tex->Load(distanciaSelectedPath);

			entity->icon3 = app->tex->Load(durationPath);
			entity->iconLocked3 = app->tex->Load(durationLockedPath);
			entity->iconSelected3 = app->tex->Load(durationSelectedPath);
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

			entity->icon0 = app->tex->Load(poisonPath);
			entity->iconLocked0 = app->tex->Load(poisonLockedPath);
			entity->iconSelected0 = app->tex->Load(poisonSelectedPath);

			entity->icon1 = app->tex->Load(invisiblePath);
			entity->iconLocked1 = app->tex->Load(invisibleLockedPath);
			entity->iconSelected1 = app->tex->Load(invisibleSelectedPath);

			entity->icon2 = app->tex->Load(aumentoPath);
			entity->iconLocked2 = app->tex->Load(aumentoLockedPath);
			entity->iconSelected2 = app->tex->Load(aumentoSelectedPath);

	
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

			entity->icon0 = app->tex->Load(poisonPath);
			entity->iconLocked0 = app->tex->Load(poisonLockedPath);
			entity->iconSelected0 = app->tex->Load(poisonSelectedPath);

			entity->icon1 = app->tex->Load(invisiblePath);
			entity->iconLocked1 = app->tex->Load(invisibleLockedPath);
			entity->iconSelected1 = app->tex->Load(invisibleSelectedPath);

			entity->icon2 = app->tex->Load(aumentoPath);
			entity->iconLocked2 = app->tex->Load(aumentoLockedPath);
			entity->iconSelected2 = app->tex->Load(aumentoSelectedPath);

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

			entity->icon0 = app->tex->Load(poisonPath);
			entity->iconLocked0 = app->tex->Load(poisonLockedPath);
			entity->iconSelected0 = app->tex->Load(poisonSelectedPath);

			entity->icon1 = app->tex->Load(invisiblePath);
			entity->iconLocked1 = app->tex->Load(invisibleLockedPath);
			entity->iconSelected1 = app->tex->Load(invisibleSelectedPath);

			entity->icon2 = app->tex->Load(aumentoPath);
			entity->iconLocked2 = app->tex->Load(aumentoLockedPath);
			entity->iconSelected2 = app->tex->Load(aumentoSelectedPath);

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

			entity->icon0 = app->tex->Load(poisonPath);
			entity->iconLocked0 = app->tex->Load(poisonLockedPath);
			entity->iconSelected0 = app->tex->Load(poisonSelectedPath);

			entity->icon1 = app->tex->Load(invisiblePath);
			entity->iconLocked1 = app->tex->Load(invisibleLockedPath);
			entity->iconSelected1 = app->tex->Load(invisibleSelectedPath);

			entity->icon2 = app->tex->Load(aumentoPath);
			entity->iconLocked2 = app->tex->Load(aumentoLockedPath);
			entity->iconSelected2 = app->tex->Load(aumentoSelectedPath);

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

	// Encontrar el ID m�s alto actualmente asignado


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

	Player* player = app->entityManager->GetPlayer();
	if (id == 100)
	{
		selectPrimaryMask = true;
	}

	else if (id == 101)
	{
		selectSecondaryMask = true;
	}
	
	else
	{


		for (item = arboles.start; item != NULL; item = item->next)
		{
			if (item->data->usable1 && mask == 1 || item->data->usable2 && mask == 2 || item->data->usable3 && mask == 3 || item->data->usable4 && mask == 4)
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
							if (mask == 1 && numMejoras0 < 8 && player->maskZeroPoints > 0)
							{
								app->entityManager->GetPlayer()->maskLevels[Mask::MASK0][Branches::Rama1] += 1;
								item->data->used1 = true;
								item->data->usable1 = false;
								siguiente = true;
								player->maskZeroPoints -= 1;
								numMejoras0 += 1;
							}
							else if (mask == 2 && numMejoras1 < 8 && player->maskOnePoints > 0)
							{
								player->maskLevels[Mask::MASK1][Branches::Rama1] += 1;
								item->data->used2 = true;
								item->data->usable2 = false;
								siguiente = true;
								player->maskOnePoints -= 1;
								numMejoras1 += 1;
							}
							else if (mask == 3 && player->maskTwoPoints > 0)
							{
								player->maskLevels[Mask::MASK2][Branches::Rama1] += 1;
								item->data->used3 = true;
								item->data->usable3 = false;
								siguiente = true;
								player->maskTwoPoints -= 1;
								numMejoras2 += 1;
							}
							else if (mask == 4 && numMejoras3 < 8 && player->maskThreePoints > 0)
							{
								player->maskLevels[Mask::MASK3][Branches::Rama1] += 1;
								item->data->used4 = true;
								item->data->usable4 = false;
								siguiente = true;
								player->maskThreePoints -= 1;
								numMejoras3 += 1;
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

							}
							break;
						}
						case 1:
						{
							bool siguiente = false;
							if (mask == 1 && numMejoras0 < 8 && player->maskZeroPoints > 0)
							{
								player->maskLevels[Mask::MASK0][Branches::Rama2] += 1;
								item->data->usable1 = false;
								item->data->used1 = true;
								siguiente = true;
								player->maskZeroPoints -= 1;
								numMejoras0 += 1;
							}
							else if (mask == 2 && player->maskOnePoints > 0)
							{
								player->maskLevels[Mask::MASK1][Branches::Rama2] += 1;
								item->data->usable2 = false;
								item->data->used2 = true;
								siguiente = true;
								player->maskOnePoints -= 1;
								numMejoras1 += 1;
							}
							else if (mask == 3 && numMejoras2 < 8 && player->maskTwoPoints > 0)
							{
								player->maskLevels[Mask::MASK2][Branches::Rama2] += 1;
								item->data->usable3 = false;
								item->data->used3 = true;
								siguiente = true;
								player->maskTwoPoints -= 1;
								numMejoras2 += 1;
							}
							else if (mask == 4 && numMejoras3 < 8 && player->maskTwoPoints > 0)
							{
								player->maskLevels[Mask::MASK3][Branches::Rama2] += 1;
								item->data->usable4 = false;
								item->data->used4 = true;
								siguiente = true;
								player->maskThreePoints -= 1;
								numMejoras3 += 1;
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

							}
							break;
						}
						case 2:
						{
							bool siguiente = false;
							if (mask == 1 && numMejoras0 < 8 && player->maskZeroPoints > 0)
							{
								player->maskLevels[Mask::MASK0][Branches::Rama3] += 1;
								item->data->usable1 = false;
								item->data->used1 = true;
								siguiente = true;
								player->maskZeroPoints -= 1;
								numMejoras0 += 1;
							}
							else if (mask == 2 && numMejoras1 < 8 && player->maskOnePoints > 0)
							{
								player->maskLevels[Mask::MASK1][Branches::Rama3] += 1;
								item->data->usable2 = false;
								item->data->used2 = true;
								siguiente = true;
								player->maskOnePoints -= 1;
								numMejoras1 += 1;
							}
							else if (mask == 3 && numMejoras2 < 8 && player->maskTwoPoints > 0)
							{
								player->maskLevels[Mask::MASK2][Branches::Rama3] += 1;
								item->data->usable3 = false;
								item->data->used3 = true;
								siguiente = true;
								player->maskTwoPoints -= 1;
								numMejoras2 += 1;
							}
							else if (mask == 4 && numMejoras3 < 8 && player->maskThreePoints > 0)
							{
								player->maskLevels[Mask::MASK3][Branches::Rama3] += 1;
								item->data->usable4 = false;
								item->data->used4 = true;
								siguiente = true;
								player->maskThreePoints -= 1;
								numMejoras3 += 1;
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

							}
							break;
						}
						case 3:
						{
							bool siguiente = false;
							if (mask == 1 && numMejoras0 < 8 && player->maskZeroPoints > 0)
							{
								player->maskLevels[Mask::MASK0][Branches::Rama4] += 1;
								item->data->usable1 = false;
								item->data->used1 = true;
								siguiente = true;
								player->maskZeroPoints -= 1;
								numMejoras0 += 1;
							}
							else if (mask == 2 && numMejoras1 < 8 && player->maskOnePoints > 0)
							{
								player->maskLevels[Mask::MASK1][Branches::Rama4] += 1;
								item->data->usable2 = false;
								item->data->used2 = true;
								player->maskOnePoints -= 1;
								siguiente = true;
								numMejoras1 += 1;
							}
							else if (mask == 3 && numMejoras2 < 8 && player->maskTwoPoints > 0)
							{
								player->maskLevels[Mask::MASK2][Branches::Rama4] += 1;
								item->data->usable3 = false;
								item->data->used3 = true;
								siguiente = true;
								player->maskTwoPoints -= 1;
								numMejoras2 += 1;
							}
							else if (mask == 4 && numMejoras3 < 8 && player->maskThreePoints > 0)
							{
								player->maskLevels[Mask::MASK3][Branches::Rama4] += 1;
								item->data->usable4 = false;
								item->data->used4 = true;
								siguiente = true;
								player->maskThreePoints -= 1;
								numMejoras3 += 1;
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
					if ((itum->data->nivelArbol == item->data->nivelArbol && itum->data->nivelMejora == item->data->nivelMejora + 1) || (item->data->nivelMejora + 1 == 5 && itum->data->nivelArbol == item->data->nivelArbol && itum->data->type == TreeType::BUTTON))
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
							item->data->used1 = false;
							if (item->data->usable1 + 1 < 5)
							{
								itum->data->usable1 = false;
							}
							numMejoras0 -= 1;
							puntsRembolso += 1;
							app->entityManager->GetPlayer()->maskZeroPoints += 1;
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
							item->data->used2 = false;
							if (item->data->usable2 + 1 < 5)
							{
								/*itum->data->usable2 = false;*/
							}
							numMejoras1 -= 1;
							puntsRembolso += 1;
							app->entityManager->GetPlayer()->maskOnePoints += 1;
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
							item->data->used3 = false;
							if (item->data->usable3 + 1 < 5)
							{
								/*itum->data->usable3 = false;*/
							}
							numMejoras2 -= 1;
							puntsRembolso += 1;
							app->entityManager->GetPlayer()->maskTwoPoints += 1;
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
							item->data->used4 = false;
							if (item->data->usable4 + 1 < 5)
							{
								/*itum->data->usable4 = false;*/
							}
							numMejoras3 -= 1;
							puntsRembolso += 1;
							app->entityManager->GetPlayer()->maskThreePoints += 1;
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
		if (PointerId == 100)
		{
			PointerId = 101;
			PointerPosition.x += 281;
			selectSecondaryMask = false;
			selectPrimaryMask = false;
		}
		else if (PointerId == 101)
		{
			PointerId = 100;
			PointerPosition.x -= 281;
			selectSecondaryMask = false;
			selectPrimaryMask = false;
		}
		else
		{
			if (horitzontalPointerId + 1 == 5)
			{
				horitzontalPointerId = 0;
				verticalPointerId = 0;
				PointerPosition.x = 156;
				PointerPosition.y = 148;
				PointerId = 0;
			}
			else
			{

				if (horitzontalPointerId == 0)
				{
					PointerPosition.x = 317;
					PointerPosition.y = 138;
					PointerId = 1;
					verticalPointerId = 0;
					horitzontalPointerId = 1;
				}
				else
				{
					horitzontalPointerId += 1;
					PointerPosition.x += 143;
					PointerId += 1;
				}

			}
		}
		
		selectSecondaryMask = false;
		selectPrimaryMask = false;
	}
	if (app->input->GetButton(LEFT) == KEY_DOWN) {
		if (PointerId == 100)
		{
			PointerId = 101;
			PointerPosition.x += 281;
		}
		else if (PointerId == 101)
		{
			PointerId = 100;
			PointerPosition.x -= 281;
		}
		else
		{
			if (horitzontalPointerId == 0)
			{

			}
			else
			{
				if (horitzontalPointerId - 1 == -1)
				{

					horitzontalPointerId += 4;
					PointerPosition.x = 317 + 4 * 143;
					PointerId += 4;
				}
				else
				{
					if (horitzontalPointerId - 1 == 0)
					{
						horitzontalPointerId = 0;
						verticalPointerId = 0;
						PointerPosition.x = 156;
						PointerPosition.y = 148;
						PointerId = 0;
					}
					else
					{
						horitzontalPointerId -= 1;
						PointerPosition.x -= 143;
						PointerId -= 1;
					}

				}
			}
		}
		selectSecondaryMask = false;
		selectPrimaryMask = false;
	}

	if (app->input->GetButton(DOWN) == KEY_DOWN) {
		if (verticalPointerId < 3)
		{
			PointerId += 5;
			verticalPointerId += 1;
			if (horitzontalPointerId == 0)
			{
				PointerPosition.y += 125;
			}
			else
			{
				PointerPosition.y += 100;
			}
			
		}
		else
		{
			if (PointerId == 100 || PointerId == 101)
			{
				verticalPointerId = 0;
				horitzontalPointerId = 1;
				PointerId = 1;
				{
					PointerPosition.x = 317;
					PointerPosition.y = 138;
				}
			}
			else
			{
				verticalPointerId = 100;
				PointerId = 100;
				PointerPosition.x = 278;
				PointerPosition.y = 520;
			}
			
			
			
		}

		selectSecondaryMask = false;
		selectPrimaryMask = false;

	}
	if (app->input->GetButton(UP) == KEY_DOWN) {
		if (verticalPointerId - 1 > -1 && verticalPointerId < 80)
		{
			
			PointerId -= 5;
			verticalPointerId -= 1;
			if (horitzontalPointerId == 0)
			{
				PointerPosition.y -= 125;
			}
			else
			{
				PointerPosition.y -= 100;
			}
		}
		else
		{
			if (PointerId == 100 || PointerId == 101)
			{
				/*if (horitzontalPointerId == 0)
				{
					PointerPosition.y = 148 + 125 * (3);
				}
				else*/
				{
					PointerPosition.x = 317;
					PointerPosition.y = 138 + 100 * (3);
					verticalPointerId = 3;
					PointerId = 16;
					horitzontalPointerId = 1;
				}
				
			}
			else
			{
				PointerId = 100;
				PointerPosition.x = 278;
				PointerPosition.y = 520;
			}
			
		}

		selectSecondaryMask = false;
		selectPrimaryMask = false;
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

	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
		/*options = true;
		selected = { PointerPosition.x, PointerPosition.y };
		selectedId = PointerId;*/
		mostrar = !mostrar;

	}
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
			app->menu->active = true;
		}
		if (numMejoras0 > 8) {
			numMejoras0 = 8; // Limitar a 8 si es mayor
		}
		if (numMejoras1 > 8) {
			numMejoras1 = 8; // Limitar a 8 si es mayor
		}
		if (numMejoras2 > 8) {
			numMejoras2 = 8; // Limitar a 8 si es mayor
		}
		if (numMejoras3 > 8) {
			numMejoras3 = 8; // Limitar a 8 si es mayor
		}

		/*if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {

			selectPrimaryMask = true;

		}*/

		if (selectPrimaryMask)
		{
			if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
			{
				if (app->inventoryManager->numMasks >= 1)
				{
					app->entityManager->GetPlayer()->primaryMask = Mask::MASK0;
					selectPrimaryMask = false;
				}
			}
			else if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
			{
				if (app->inventoryManager->numMasks >= 2)
				{
					app->entityManager->GetPlayer()->primaryMask = Mask::MASK1;
					selectPrimaryMask = false;
				}
			}
			else if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
			{
				if (app->inventoryManager->numMasks >= 3)
				{
					app->entityManager->GetPlayer()->primaryMask = Mask::MASK2;
					selectPrimaryMask = false;
				}
			}
			else if (app->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
			{
				if (app->inventoryManager->numMasks >= 4)
				{
					app->entityManager->GetPlayer()->primaryMask = Mask::MASK3;
					selectPrimaryMask = false;
				}
			}

			if (app->input->GetButton(APP_EXIT) == KEY_DOWN) {
				selectPrimaryMask = false;
			}
		}

		/*if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {

			selectSecondaryMask = true;

		}*/

		if (selectSecondaryMask)
		{
			if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
			{
				if (app->inventoryManager->numMasks >= 1)
				{
					app->entityManager->GetPlayer()->secondaryMask = Mask::MASK0;
					selectSecondaryMask = false;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
			{
				if (app->inventoryManager->numMasks >= 2)
				{
					app->entityManager->GetPlayer()->secondaryMask = Mask::MASK1;
					selectSecondaryMask = false;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
			{
				if (app->inventoryManager->numMasks >= 3)
				{
					app->entityManager->GetPlayer()->secondaryMask = Mask::MASK2;
					selectSecondaryMask = false;
				}
			}
			if (app->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
			{
				if (app->inventoryManager->numMasks >= 4)
				{
					app->entityManager->GetPlayer()->secondaryMask = Mask::MASK3;
					selectSecondaryMask = false;
				}
			}
			if (app->input->GetButton(APP_EXIT) == KEY_DOWN) {
				selectSecondaryMask = false;
			}
		}

		int num;

		num = numberRows + 1;


		if (arboles.Count() > 3 * num)
		{
			numberRows += 1;
		}

		Player* player = app->entityManager->GetPlayer();
		if (player != nullptr)
		{
			if (player->maskZeroPoints > 8)
			{
				player->maskZeroPoints = 8;
			}
			if (player->maskZeroPoints < 0)
			{
				player->maskZeroPoints = 0;
			}
			if (player->maskOnePoints > 8)
			{
				player->maskOnePoints = 8;
			}
			if (player->maskOnePoints < 0)
			{
				player->maskOnePoints = 0;
			}
			if (player->maskTwoPoints > 8)
			{
				player->maskTwoPoints = 8;
			}
			if (player->maskTwoPoints < 0)
			{
				player->maskTwoPoints = 0;
			}
			if (player->maskThreePoints > 8)
			{
				player->maskThreePoints = 8;
			}
			if (player->maskThreePoints < 0)
			{
				player->maskThreePoints = 0;
			}
		}
		player->maskZeroPoints = 8;
		player->maskOnePoints = 8;
		player->maskTwoPoints = 8;
		player->maskThreePoints = 8;
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
		app->render->DrawTexture(BackGroundTexture, 100, 100, SDL_FLIP_NONE, 0, 0);
		ListItem<Tree*>* item;
		Tree* pEntity = NULL;






		for (item = arboles.start; item != nullptr; item = item->next)
		{





			pEntity = item->data;
			int rowIndex = item->data->id / maxItemsPerRow; // Calcula el �ndice de la fila
			int columnIndex = item->data->id % maxItemsPerRow; // Calcula el �ndice de la columna
			int horizontalPosition = 200 + columnIndex * 138; // Calcula la posici�n horizontal
			int verticalPosition = 150 + rowIndex * 100; // Calcula la posici�n vertical



			if (zoomIn == false && app->notesManager->zoomIn == false)
			{
				/*app->render->DrawTexture(listTexture, horizontalPosition, verticalPosition, 0.7, SDL_FLIP_NONE, 0, 0);*/
				switch (mask)
				{
				case 1:
				{
					if (pEntity->used1)
					{
						app->render->DrawTexture(pEntity->iconSelected0, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);
					}
					else
					{
						if (pEntity->usable1 == false)
						{
							app->render->DrawTexture(pEntity->iconLocked0, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);
						}
						else
						{
							app->render->DrawTexture(pEntity->icon0, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);
						}
					}
					
					/*app->render->DrawTextBound(pEntity->name.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });*/
					
					


					break;
				}
				case 2:
				{
					if (pEntity->used2)
					{
						
						if (pEntity->name2 == "")
						{
							/*app->render->DrawTextBound(pEntity->name.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });*/
							app->render->DrawTexture(pEntity->iconSelected0, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);
						}
						else
						{
							/*app->render->DrawTextBound(pEntity->name2.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });*/
							app->render->DrawTexture(pEntity->iconSelected1, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);

						}
					}
					else
					{
						if (pEntity->usable2 == false)
						{
							if (pEntity->name2 == "")
							{
								/*app->render->DrawTextBound(pEntity->name.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });*/
								app->render->DrawTexture(pEntity->iconLocked0, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);
							}
							else
							{
								/*app->render->DrawTextBound(pEntity->name2.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });*/
								app->render->DrawTexture(pEntity->iconLocked1, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);

							}
							/*app->render->DrawTexture(listTextureNot, horizontalPosition, verticalPosition, 0.7, SDL_FLIP_NONE, 0, 0);*/
						}
						else
						{
							if (pEntity->name2 == "")
							{
								/*app->render->DrawTextBound(pEntity->name.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });*/
								app->render->DrawTexture(pEntity->icon0, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);
							}
							else
							{
								/*app->render->DrawTextBound(pEntity->name2.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });*/
								app->render->DrawTexture(pEntity->icon1, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);

							}
						}
					}


					break;
				}
				case 3:
				{
					if (pEntity->used3)
					{
						if (pEntity->name3 == "")
						{
							/*app->render->DrawTextBound(pEntity->name.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });*/
							app->render->DrawTexture(pEntity->iconSelected0, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);
						}
						else
						{
							/*app->render->DrawTextBound(pEntity->name2.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });*/
							app->render->DrawTexture(pEntity->iconSelected2, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);

						}
					}
					else
					{
						if (pEntity->usable3 == false)
						{
							if (pEntity->name3 == "")
							{
								/*app->render->DrawTextBound(pEntity->name.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });*/
								app->render->DrawTexture(pEntity->iconLocked0, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);
							}
							else
							{
								/*app->render->DrawTextBound(pEntity->name3.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });*/
								app->render->DrawTexture(pEntity->iconLocked2, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);
							}
							/*app->render->DrawTexture(listTextureNot, horizontalPosition, verticalPosition, 0.7, SDL_FLIP_NONE, 0, 0);*/
						}
						else
						{
							if (pEntity->name3 == "")
							{
								/*app->render->DrawTextBound(pEntity->name.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });*/
								app->render->DrawTexture(pEntity->icon0, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);
							}
							else
							{
								/*app->render->DrawTextBound(pEntity->name2.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });*/
								app->render->DrawTexture(pEntity->icon2, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);

							}
						}
					}


					break;
				}
				case 4:
				{
					if (pEntity->used4)
					{
						if (pEntity->name4 == "")
						{
							/*app->render->DrawTextBound(pEntity->name.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });*/
							app->render->DrawTexture(pEntity->iconSelected0, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);
						}
						else
						{
							/*app->render->DrawTextBound(pEntity->name2.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });*/
							app->render->DrawTexture(pEntity->iconSelected3, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);

						}
					}
					else
					{
						if (pEntity->usable4 == false)
						{
							if (pEntity->name4 == "")
							{
								/*app->render->DrawTextBound(pEntity->name.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });*/
								app->render->DrawTexture(pEntity->iconLocked0, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);
							}
							else
							{
								/*app->render->DrawTextBound(pEntity->name4.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });*/
								app->render->DrawTexture(pEntity->iconLocked3, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);

							}
							/*app->render->DrawTexture(listTextureNot, horizontalPosition, verticalPosition, 0.7, SDL_FLIP_NONE, 0, 0);*/
						}
						else
						{
							if (pEntity->name4 == "")
							{
								/*app->render->DrawTextBound(pEntity->name.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });*/
								app->render->DrawTexture(pEntity->icon0, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);
							}
							else
							{
								/*app->render->DrawTextBound(pEntity->name4.GetString(), horizontalPosition, verticalPosition, 130, { 0,0,0 });*/
								app->render->DrawTexture(pEntity->icon3, horizontalPosition, verticalPosition, 1, SDL_FLIP_NONE, 0, 0);

							}
						}
					}

					break;
				}
				default:
					break;
				}

				/*app->render->DrawTexture(pEntity->icon, horizontalPosition, verticalPosition, 0.8, SDL_FLIP_NONE, 0, 0);*/
			}

			if (PointerId == 0 || PointerId == 5 || PointerId == 10 || PointerId == 15)
			{
				app->render->DrawTexture(PointerItemText2, PointerPosition.x, PointerPosition.y, SDL_FLIP_NONE, 0, 0);
			}
			else
			{
				if (PointerId == 100 || PointerId == 101)
				{
					app->render->DrawTexture(PointerItemText3, PointerPosition.x, PointerPosition.y, SDL_FLIP_NONE, 0, 0);
				}
				else
				{
					app->render->DrawTexture(PointerItemText, PointerPosition.x, PointerPosition.y, SDL_FLIP_NONE, 0, 0);
				}
				
			}

			
			
			if (mask == 1)
			{
				app->render->DrawTexture(mascara0SelectedTexture, 157, 145, SDL_FLIP_NONE, 0, 0);
			}
			else
			{
				app->render->DrawTexture(mascara0Texture, 157, 145, SDL_FLIP_NONE, 0, 0);
			}

			if (mask == 2)
			{
				app->render->DrawTexture(mascara1SelectedTexture, 157, 275, SDL_FLIP_NONE, 0, 0);
			}
			else
			{
				app->render->DrawTexture(mascara1Texture, 157, 275, SDL_FLIP_NONE, 0, 0);
			}

			if (mask == 3)
			{
				app->render->DrawTexture(mascara2SelectedTexture, 157, 400, SDL_FLIP_NONE, 0, 0);
			}
			else
			{
				app->render->DrawTexture(mascara2Texture, 157, 400, SDL_FLIP_NONE, 0, 0);
			}

			if (mask == 4)
			{
				app->render->DrawTexture(mascara3SelectedTexture, 157, 525, SDL_FLIP_NONE, 0, 0);
			}
			else
			{
				app->render->DrawTexture(mascara3Texture, 157, 525, SDL_FLIP_NONE, 0, 0);
			}
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
		if (mask == 1)
		{

			std::string texto = std::to_string(app->entityManager->GetPlayer()->maskZeroPoints) + "/8 Mask Points";
			app->render->DrawTextBound(texto.c_str(), 945, 400, 370, { 0,0,0 });
			std::string texto1 = std::to_string(numMejoras0) + "/8 Mejoras";
			app->render->DrawTextBound(texto1.c_str(), 975, 500, 370, { 0,0,0 });

		}
		else if (mask == 2)
		{

			std::string texto = std::to_string(app->entityManager->GetPlayer()->maskOnePoints) + "/8 Mask Points";
			app->render->DrawTextBound(texto.c_str(), 945, 400, 370, { 0,0,0 });
			std::string texto1 = std::to_string(numMejoras1) + "/8 Mejoras";
			app->render->DrawTextBound(texto1.c_str(), 975, 500, 370, { 0,0,0 });

		}
		else if (mask == 3)
		{

			std::string texto = std::to_string(app->entityManager->GetPlayer()->maskTwoPoints) + "/8 Mask Points";
			app->render->DrawTextBound(texto.c_str(), 945, 400, 370, { 0,0,0 });
			std::string texto1 = std::to_string(numMejoras2) + "/8 Mejoras";
			app->render->DrawTextBound(texto1.c_str(), 975, 500, 370, { 0,0,0 });

		}
		else if (mask == 4)
		{

			std::string texto = std::to_string(app->entityManager->GetPlayer()->maskThreePoints) + "/8 Mask Points";
			app->render->DrawTextBound(texto.c_str(), 975, 400, 370, { 0,0,0 });
			std::string texto1 = std::to_string(numMejoras3) + "/8 Mejoras";
			app->render->DrawTextBound(texto1.c_str(), 975, 500, 370, { 0,0,0 });

		}
		
		if (selectPrimaryMask || selectSecondaryMask)
		{
			app->render->DrawTextBound("1", 120, 180, 100, { 255,0,0,0 }, app->render->titleFont);
			app->render->DrawTextBound("2", 120, 310, 370, { 255,0,0,0 }, app->render->titleFont);
			app->render->DrawTextBound("3", 120, 430, 370, { 255,0,0,0 }, app->render->titleFont);
			app->render->DrawTextBound("4", 120, 550, 370, { 255,0,0,0 }, app->render->titleFont);
		}
			
		


	}

	return ret;
}






