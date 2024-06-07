
#include "EntityManager.h"
#include "BestiarioManager.h"
#include "NotesManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene_testing.h"
#include "Bestiario.h"
#include "SwordInv.h"
#include "GarraInv.h"
#include "ArmaduraInv.h"
#include "DiamanteInv.h"
#include "ItemInv.h"
#include "OjoInv.h"
#include "Defs.h"
#include "Log.h"
#include "SString.h"
#include "Scene_Pueblo.h"
#include "Scene_Pueblo_Tutorial.h"
#include "Menu.h"
#include "Window.h"

BestiarioManager::BestiarioManager(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name = ("bestiariomanager");
}

// Destructor
BestiarioManager::~BestiarioManager()
{}

// Called before render is available
bool BestiarioManager::Awake(pugi::xml_node config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Bestiario*>* item;
	Bestiario* pEntity = NULL;

	for (item = bestiario.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	iconoBestiarioTexturePath = ((char*)config.child("bestiario").attribute("texturePath").as_string());
	listTexturePath = ((char*)config.child("bestiario").attribute("list_texture").as_string());
	sliderTexturePath = ((char*)config.child("bestiario").attribute("slider_texture").as_string());
	knobTexturePath = ((char*)config.child("bestiario").attribute("knob_texture").as_string());
	PointerPath = ((char*)config.child("bestiario").attribute("pointer").as_string());
	textoOsiris = ((char*)config.child("bestiario").attribute("text_osiris").as_string());
	textoOsirisV = ((char*)config.child("bestiario").attribute("text_osirisV").as_string());
	pathOsiris = ((char*)config.child("bestiario").attribute("texture_osiris").as_string());
	textoMuur = ((char*)config.child("bestiario").attribute("text_muur").as_string());
	textoMuurV = ((char*)config.child("bestiario").attribute("text_muurV").as_string());
	pathMuur = ((char*)config.child("bestiario").attribute("texture_muur").as_string());
	textoKhurt = ((char*)config.child("bestiario").attribute("text_khurt").as_string());
	textoKhurtV = ((char*)config.child("bestiario").attribute("text_khurtV").as_string());
	pathKhurt = ((char*)config.child("bestiario").attribute("texture_khurt").as_string());
	textoBoorok = ((char*)config.child("bestiario").attribute("text_boorok").as_string());
	textoBoorokV = ((char*)config.child("bestiario").attribute("text_boorokV").as_string());
	pathBoorok = ((char*)config.child("bestiario").attribute("texture_boorok").as_string());
	textoOls = ((char*)config.child("bestiario").attribute("text_ols").as_string());
	textoOlsV = ((char*)config.child("bestiario").attribute("text_olsV").as_string());
	pathOls = ((char*)config.child("bestiario").attribute("texture_ols").as_string());
	textoInuit = ((char*)config.child("bestiario").attribute("text_inuit").as_string());
	textoInuitV = ((char*)config.child("bestiario").attribute("text_inuitV").as_string());
	pathInuit = ((char*)config.child("bestiario").attribute("texture_inuit").as_string());
	textoMusri = ((char*)config.child("bestiario").attribute("text_musri").as_string());
	textoMusriV = ((char*)config.child("bestiario").attribute("text_musriV").as_string());
	pathMusri = ((char*)config.child("bestiario").attribute("texture_musri").as_string());
	textoSurma = ((char*)config.child("bestiario").attribute("text_surma").as_string());
	textoSurmaV = ((char*)config.child("bestiario").attribute("text_surmaV").as_string());
	pathSurma = ((char*)config.child("bestiario").attribute("texture_surma").as_string());
	CloseUpPath = ((char*)config.child("bestiario").attribute("closeUp").as_string());
	iconPathOsiris = ((char*)config.child("bestiario").attribute("texture_icono_osiris").as_string());
	iconPathMuur = ((char*)config.child("bestiario").attribute("texture_icono_muur").as_string());
	iconPathKhurt = ((char*)config.child("bestiario").attribute("texture_icono_khurt").as_string());
	iconPathBoorok = ((char*)config.child("bestiario").attribute("texture_icono_boorok").as_string());
	iconPathOls = ((char*)config.child("bestiario").attribute("texture_icono_ols").as_string());
	iconPathInuit = ((char*)config.child("bestiario").attribute("texture_icono_inuit").as_string());
	iconPathMusri = ((char*)config.child("bestiario").attribute("texture_icono_musri").as_string());
	iconPathSurma = ((char*)config.child("bestiario").attribute("texture_icono_surma").as_string());

	listTexturePath2 = ((char*)config.child("bestiario").attribute("list_texture2").as_string());
	listTexturePath3 = ((char*)config.child("bestiario").attribute("list_texture3").as_string());


	return ret;

}

bool BestiarioManager::Start() {



	CloseUp = app->tex->Load(CloseUpPath);
	listTexture = app->tex->Load(listTexturePath);
	listTexture2 = app->tex->Load(listTexturePath2);
	listTexture3 = app->tex->Load(listTexturePath3);
	sliderTexture = app->tex->Load(sliderTexturePath);
	knobTexture = app->tex->Load(knobTexturePath);
	PointerItemText = app->tex->Load(PointerPath);
	textureOsiris = app->tex->Load(pathOsiris);
	textureMuur = app->tex->Load(pathMuur);
	textureKhurt = app->tex->Load(pathKhurt);
	textureBoorok = app->tex->Load(pathBoorok);
	textureOls = app->tex->Load(pathOls);
	textureInuit = app->tex->Load(pathInuit);
	textureMusri = app->tex->Load(pathMusri);
	textureSurma = app->tex->Load(pathSurma);
	iconTextureOsiris = app->tex->Load(iconPathOsiris);
	iconTextureMuur = app->tex->Load(iconPathMuur);
	iconTextureKhurt = app->tex->Load(iconPathKhurt);
	iconTextureBoorok = app->tex->Load(iconPathBoorok);
	iconTextureOls = app->tex->Load(iconPathOls);
	iconTextureInuit = app->tex->Load(iconPathInuit);
	iconTextureSurma = app->tex->Load(iconPathSurma);
	iconTextureMusri = app->tex->Load(iconPathMusri);
	PointerId = -2;
	PointerItemText = nullptr;

	bool ret = true;

	//Iterates over the entities and calls Start
	ListItem<Bestiario*>* item;
	Bestiario* pEntity = NULL;

	for (item = bestiario.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	scrollY = 0;

	return ret;
}

// Called before quitting
bool BestiarioManager::CleanUp()
{
	bool ret = true;
	ListItem<Bestiario*>* item;
	item = bestiario.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	bestiario.Clear();
	app->tex->UnLoad(PointerItemText);
	app->tex->UnLoad(SelectedItemText);
	app->tex->UnLoad(EquipedItemText);
	app->tex->UnLoad(listTexture);
	app->tex->UnLoad(CloseUp);
	app->tex->UnLoad(listTexture2);
	app->tex->UnLoad(listTexture3);
	app->tex->UnLoad(sliderTexture);
	app->tex->UnLoad(knobTexture);
	app->tex->UnLoad(PointerItemText);
	app->tex->UnLoad(textureOsiris);
	app->tex->UnLoad(textureMuur);
	app->tex->UnLoad(textureKhurt);
	app->tex->UnLoad(textureBoorok);
	app->tex->UnLoad(textureOls);
	app->tex->UnLoad(textureInuit);
	app->tex->UnLoad(textureMusri);
	app->tex->UnLoad(textureSurma);
	app->tex->UnLoad(iconTextureOsiris);
	app->tex->UnLoad(iconTextureMuur);
	app->tex->UnLoad(iconTextureKhurt);
	app->tex->UnLoad(iconTextureBoorok);
	app->tex->UnLoad(iconTextureOls);
	app->tex->UnLoad(iconTextureInuit);
	app->tex->UnLoad(iconTextureSurma);
	app->tex->UnLoad(iconTextureMusri);
	return ret;
}
int highestttId = -1;
int listIdd = 0;
Bestiario* BestiarioManager::CreateItem(char* name)
{
	Bestiario* entity = nullptr;

	
	highestttId = (bestiario.Count() > 0) ? bestiario.end->data->id : -1;
	entity = new Bestiario();
	entity->id = highestttId + 1;
	entity->closeUpBestiarios = CloseUp;
	entity->icon = app->tex->Load(iconoBestiarioTexturePath);
	if (listIdd == 3)
	{
		listIdd = 0;
	}
	entity->listid = listIdd;
	entity->type = BestiarioType::BEST;
	if (strcmp(name, "osiris") == 0)
	{
		entity->name = name;
		entity->desc = textoOsiris;
		entity->texturaEnemigo = textureOsiris; 
		entity->icon = iconTextureOsiris;
		if(osiris == 0)
		AddBestiario(entity);
		osiris++;
		
	}
	else if (strcmp(name, "osirisV") == 0)
	{
		entity->name = name;
		entity->desc = textoOsirisV;
		entity->texturaEnemigo = textureOsiris;
		entity->icon = iconTextureOsiris;
		entity->variacion = true;
		if(osirisV == 0)
			AddBestiario(entity);
		osirisV++;

	}
	else if (strcmp(name, "muur") == 0)
	{
		entity->name = name;
		entity->desc = textoMuur;
		entity->texturaEnemigo = textureMuur;
		entity->icon = iconTextureMuur;
		if (muur == 0)
			AddBestiario(entity);
		muur++;
	}
	else if (strcmp(name, "muurV") == 0)
	{
		entity->name = name;
		entity->desc = textoMuurV;
		entity->texturaEnemigo = textureMuur;
		entity->icon = iconTextureMuur;
		entity->variacion = true;
		if (muurV == 0)
			AddBestiario(entity);
		muurV++;
	}
	else if (strcmp(name, "khurt") == 0)
	{
		entity->name = name;
		entity->desc = textoKhurt;
		entity->texturaEnemigo = textureKhurt;
		entity->icon = iconTextureKhurt;
		if (khurt == 0)
			AddBestiario(entity);
		khurt++;
	}
	else if (strcmp(name, "khurtV") == 0)
	{
		entity->name = name;
		entity->desc = textoKhurtV;
		entity->texturaEnemigo = textureKhurt;
		entity->icon = iconTextureKhurt;
		entity->variacion = true;
		if (khurtV == 0)
			AddBestiario(entity);
		khurtV++;
	}
	else if (strcmp(name, "boorok") == 0)
	{
		entity->name = name;
		entity->desc = textoBoorok;
		entity->texturaEnemigo = textureBoorok;
		entity->icon = iconTextureBoorok;
		if (boorok == 0)
			AddBestiario(entity);
		boorok++;
	}
	else if (strcmp(name, "boorokV") == 0)
	{
		entity->name = name;
		entity->desc = textoBoorokV;
		entity->texturaEnemigo = textureBoorok;
		entity->icon = iconTextureBoorok;
		entity->variacion = true;
		if (boorokV == 0)
			AddBestiario(entity);
		boorokV++;
	}
	else if (strcmp(name, "ols") == 0)
	{
		entity->name = name;
		entity->desc = textoOls;
		entity->texturaEnemigo = textureOls;
		entity->icon = iconTextureOls;
		if (ols == 0)
			AddBestiario(entity);
		ols++;
	}
	else if (strcmp(name, "olsV") == 0)
	{
		entity->name = name;
		entity->desc = textoOlsV;
		entity->texturaEnemigo = textureOls;
		entity->icon = iconTextureOls;
		entity->variacion = true;
		if (olsV == 0)
			AddBestiario(entity);
		olsV++;
	}
	else if (strcmp(name, "khurt") == 0)
	{
		entity->name = name;
		entity->desc = textoKhurt;
		entity->texturaEnemigo = textureKhurt;
		entity->icon = iconTextureKhurt;
		if (khurt == 0)
			AddBestiario(entity);
		khurt++;
	}
	else if (strcmp(name, "khurtV") == 0)
	{
		entity->name = name;
		entity->desc = textoKhurtV;
		entity->texturaEnemigo = textureKhurt;
		entity->icon = iconTextureKhurt;
		entity->variacion = true;
		if (khurtV == 0)
			AddBestiario(entity);
		khurtV++;
		}
	else if (strcmp(name, "inuit") == 0)
	{
		entity->name = name;
		entity->desc = textoInuit;
		entity->texturaEnemigo = textureInuit;
		entity->icon = iconTextureInuit;
		if (inuit == 0)
			AddBestiario(entity);
		inuit++;
	}
	else if (strcmp(name, "musri") == 0)
	{
		entity->name = name;
		entity->desc = textoMusri;
		entity->texturaEnemigo = textureMusri;
		entity->icon = iconTextureMusri;
		if (musri == 0)
			AddBestiario(entity);
		musri++;
	}
	else if (strcmp(name, "surma") == 0)
	{
		entity->name = name;
		entity->desc = textoSurma;
		entity->texturaEnemigo = textureSurma;
		entity->icon = iconTextureSurma;
		if (surma == 0)
			AddBestiario(entity);
		surma++;
	}
	else
	{
		// Default properties if name doesn't match
	}

	///*entity->closeUpBestiario = app->tex->Load("Assets/Textures/Entidades/Items/textura_BestiarioCloseUp.png"); */
	

	

	
	

	listIdd++;
	return entity;
}

bool BestiarioManager::IsFull()
{

	// Encontrar el ID más alto actualmente asignado


	// Verificar si el siguiente ID disponible es 9
	if (bestiario.Count() == 100) {
		return true;
	}
	else {
		return false;
	}

}

bool BestiarioManager::LoadState(pugi::xml_node node)
{
	bool ret = true;

	ListItem<Bestiario*>* item;
	item = bestiario.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	osiris = 0;
	osirisV = 0;
	muur = 0;
	muurV = 0;
	khurt = 0;
	khurtV = 0;
	boorok = 0;
	boorokV = 0;
	ols = 0;
	olsV = 0;
	inuit = 0;
	inuitV = 0;
	musri = 0;
	musriV = 0;
	surma = 0;
	surmaV = 0;


	bestiario.Clear();

	//CreateItem("nombre")
	for (pugi::xml_node bestia = node.child("bestiario").child("bestia"); bestia; bestia = bestia.next_sibling("bestia"))
	{

		CreateItem((char*)bestia.attribute("name").as_string(""));


	}




	return ret;
}

bool BestiarioManager::SaveState(pugi::xml_node node)
{
	bool ret = true;
	pugi::xml_node bestiarioNode = node.append_child("bestiario");

	for (int i = 0; i < bestiario.Count(); i++) {

		Bestiario* bestiarioItem = bestiario.At(i)->data;

		pugi::xml_node bestiaItemNode = bestiarioNode.append_child("bestia");
		bestiaItemNode.append_attribute("name").set_value(bestiarioItem->name.GetString());
	}

	return ret;
}

void BestiarioManager::StartAnimation()
{
	animationTime = 0.0f;
	animating = true;
	animatingExit = false;
}

void BestiarioManager::StartExitAnimation()
{
	animationTime = 0.0f;
	animating = false;
	animatingExit = true;
}

inline float BestiarioManager::easeOutBounce(float t)
{
	return 0.0f;
}

inline float BestiarioManager::easeOutCubic(float t)
{
	t--;
	return t * t * t + 1;
}


void BestiarioManager::UseBestiarioSelected(int id)
{

	ListItem<Bestiario*>* item;

	/*for (item = bestiario.start; item != NULL; item = item->next)
	{
		item->data->active = false;
	}*/

	for (item = bestiario.start; item != NULL; item = item->next)
	{

		if (item->data->id == id) // Comprueba si el ID coincide
		{

			item->data->active = true;
			switch (item->data->type)
			{

			case BestiarioType::BEST:
			{
				item->data->zoom = true;
				StartAnimation();
				zoomIn = true;

				break;
			}
			}


		}


	}

}

void BestiarioManager::OnMovePointer()
{
	bool pointer = false;
	if (zoomIn == false)
	{
		if (PointerId != -2)
		{
			if (pointer == false)
			{
				PointerItemText = app->tex->Load(PointerPath);
				pointer = true;
			}

			if (app->input->GetButton(RIGHT) == KEY_DOWN) {
				if (horitzontalPointerId + 1 > 3)
				{
					horitzontalPointerId = 0;
					PointerPosition.x = 630;
					PointerId -= 3;
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
					if (app->notesManager->vacio == false)
					{
						PointerId = -2;
						PointerItemText = nullptr;
						app->notesManager->PointerId = 0;
					}

				}
				else
				{
					horitzontalPointerId -= 1;
					PointerPosition.x -= 83;
					PointerId -= 1;
				}
			}

			if (app->input->GetButton(DOWN) == KEY_DOWN) {
				if (verticalPointerId < numberRows)
				{
					PointerId += 4;
					verticalPointerId += 1;
					PointerPosition.y += 83;
				}
				else
				{
					verticalPointerId = 0;
					PointerId -= numberRows * 4;
					PointerPosition.y = 230;
				}



			}
			if (app->input->GetButton(UP) == KEY_DOWN) {
				if (verticalPointerId > 0)
				{
					PointerPosition.y -= 83;
					PointerId -= 4;
					verticalPointerId -= 1;
				}
				else
				{
					verticalPointerId = numberRows;
					PointerId += numberRows * 4;
					PointerPosition.y = 230 + 83 * (numberRows);
				}


			}
		}
		else
		{
			pointer = false;
		}
	}
}

void BestiarioManager::AddBestiario(Bestiario* entity)
{

	if (entity != nullptr) {

		bestiario.Add(entity);

	}

}
int couuuunt = 0;
bool BestiarioManager::Update(float dt)
{

	bool ret = true;



	if (mostrar == true)
	{
		OnMovePointer();
		
		if (couuuunt == 2)
		{
			if ((app->input->GetButton(APP_EXIT) == KEY_DOWN || app->input->GetButton(BACK) == KEY_DOWN) && zoomIn == true || app->input->GetButton(SELECT) == KEY_DOWN && !app->scene_pueblo->GetRod()->fishing.rodReady  && !app->scene_pueblo_tutorial->GetRod()->fishing.rodReady ) {
				ListItem<Bestiario*>* item;
				couuuunt = 1;
				for (item = bestiario.start; item != NULL; item = item->next)
				{
					/*item->data->zoom = false;*/
					StartExitAnimation();
				}
				zoomIn = false;
			}
		}
			if (couuuunt < 1)
			{
				if (app->input->GetButton(SELECT) == KEY_DOWN && !app->scene_pueblo->GetRod()->fishing.rodReady  && !app->scene_pueblo_tutorial->GetRod()->fishing.rodReady ) {
					/*options = true;
					selected = { PointerPosition.x, PointerPosition.y };
					selectedId = PointerId;*/
					UseBestiarioSelected(PointerId);
					couuuunt = 2;
				}
			}
		
		

		int num;

		num = numberRows + 1;
		if (couuuunt == 1)
		{
			couuuunt--;
		}

		if (bestiario.Count() > 4 * num)
		{
			numberRows += 1;
		}
		
		bestiario.Count();
	}






	return ret;
}

bool BestiarioManager::PostUpdate()
{
	bool ret = true;

	uint tradeSpacing = 83;

	SDL_Rect viewport = { 620, 200, 400, 350 };


	if (mostrar == true)
	{
		/*app->render->DrawRectangle(viewport, 0, 0, 0, 200, true, false);*/
		ListItem<Bestiario*>* item;
		Bestiario* pEntity = NULL;






		for (item = bestiario.start; item != nullptr; item = item->next)
		{





			pEntity = item->data;
			int rowIndex = item->data->id / maxItemsPerRow; // Calcula el índice de la fila
			int columnIndex = item->data->id % maxItemsPerRow; // Calcula el índice de la columna
			int horizontalPosition = 630 + columnIndex * 83; // Calcula la posición horizontal
			int verticalPosition = 230 + rowIndex * 83; // Calcula la posición vertical

			int y2 = PointerPosition.y - scrollY;

			int y = verticalPosition - scrollY;

			if (y2 >= viewport.y && y2 <= viewport.y + viewport.h)
			{
				if (y >= viewport.y && y <= viewport.y + viewport.h) {
					if (zoomIn == false && app->notesManager->zoomIn == false)
					{
						if (app->menu->animating == false && app->menu->animatingExit2 == false && app->menu->menuu == true)
						{
							/*app->render->DrawTexture(listTexture, horizontalPosition, verticalPosition, SDL_FLIP_NONE, 0, 0);*/

							if (pEntity->listid == 2)
							{
								app->render->DrawTexture(listTexture, horizontalPosition, verticalPosition - scrollY, SDL_FLIP_NONE, 0, 0);

							}
							if (pEntity->listid == 1)
							{
								app->render->DrawTexture(listTexture2, horizontalPosition, verticalPosition - scrollY, SDL_FLIP_NONE, 0, 0);

							}
							if (pEntity->listid == 0)
							{
								app->render->DrawTexture(listTexture3, horizontalPosition, verticalPosition - scrollY, SDL_FLIP_NONE, 0, 0);

							}

							if (pEntity->variacion == true)
							{
								SDL_SetTextureColorMod(pEntity->icon, 198, 115, 255);
								SDL_SetTextureAlphaMod(pEntity->icon, 255);
								app->render->DrawTexture(pEntity->icon, horizontalPosition, verticalPosition, 0.8, SDL_FLIP_NONE, 0, 0);
							}
							else
							{
								SDL_SetTextureColorMod(pEntity->icon, 255, 255, 255);
								SDL_SetTextureAlphaMod(pEntity->icon, 255);
								app->render->DrawTexture(pEntity->icon, horizontalPosition, verticalPosition, 0.8, SDL_FLIP_NONE, 0, 0);
							}
						}
					}

				}
			}
			else
			{
				int targetY = PointerId / 4 * 83;


				scrollY = targetY;

				scrollY = std::max(0, scrollY);
			}
			if (app->menu->animating == false && app->menu->animatingExit2 == false && app->menu->menuu == true)
			{
				app->render->DrawTexture(PointerItemText, PointerPosition.x, PointerPosition.y, SDL_FLIP_NONE, 0, 0);
			}
		}


	}
	ret = true;
	int knobY = 139;
	if (numberRows == 0)
	{
		knobY = 139;
	}
	else
	{
		knobY = 139 + (401 / numberRows) * verticalPointerId;
	}


	if (mostrar == true)
	{
		if (app->menu->animating == false && app->menu->animatingExit2 == false && app->menu->menuu == true)
		{
			app->render->DrawTexture(sliderTexture, 960, 200, SDL_FLIP_NONE, 0, 0);
			app->render->DrawTexture(knobTexture, 958, knobY, SDL_FLIP_NONE, 0, 0);

			uint windowWidth, windowHeight;
			app->win->GetWindowSize(windowWidth, windowHeight);

			ListItem<Bestiario*>* itum;
			for (itum = bestiario.start; itum != nullptr; itum = itum->next)
			{
				if (itum->data->zoom)
				{
					if (PointerId == itum->data->id)
					{
						int currentX = 400; // Posición final predeterminada

						if (animating || animatingExit) {
							if (animating) {
								animationTime += app->dt;

								float progress = animationTime / 1000.0f; // Duración de la animación de 1 segundo (1000 ms)
								if (progress >= 1.0f) {
									progress = 1.0f;
									animating = false;
								}
								float easedProgress = easeOutCubic(progress);

								// Calcular la nueva posición X usando easedProgress para la entrada
								int startX = static_cast<int>(windowWidth); // Comienza fuera de la pantalla a la derecha
								int endX = 400; // Posición final
								currentX = startX + (endX - startX) * easedProgress;
							}
							else if (animatingExit) {
								animationTime += app->dt;

								float progress = animationTime / 1000.0f; // Duración de la animación de 1 segundo (1000 ms)
								if (progress >= 1.0f) {
									progress = 1.0f;
									animatingExit = false;
									itum->data->zoom = false; // Ocultar el men?al finalizar la animación de salida
								}
								float easedProgress = easeOutCubic(progress);

								// Calcular la nueva posición X usando easedProgress para la salida
								int startX = 400; // Posición inicial
								int endX = static_cast<int>(windowWidth); // Posición final fuera de la pantalla a la derecha
								currentX = startX + (endX - startX) * easedProgress;
							}
						}

						// Renderizado de la textura del bestiario con la animación aplicada
						app->render->DrawTexture(itum->data->closeUpBestiarios, currentX, 100, SDL_FLIP_NONE, 0, 0);
						app->render->DrawTextBound(itum->data->name.GetString(), currentX + 100, 120, 80, { 52,25,0 }, app->render->titleFont);

						if (itum->data->variacion == true)
						{
							SDL_SetTextureColorMod(itum->data->texturaEnemigo, 198, 115, 255);
							SDL_SetTextureAlphaMod(itum->data->texturaEnemigo, 255);
							app->render->DrawTexture(itum->data->texturaEnemigo, currentX + 180, 190, 1, SDL_FLIP_NONE, 0, 0);
						}
						else
						{
							SDL_SetTextureColorMod(itum->data->texturaEnemigo, 255, 255, 255);
							SDL_SetTextureAlphaMod(itum->data->texturaEnemigo, 255);
							app->render->DrawTexture(itum->data->texturaEnemigo, currentX + 180, 190, 1, SDL_FLIP_NONE, 0, 0);
						}

						app->render->DrawTextBound(itum->data->desc.c_str(), currentX + 100, 290, 270, { 52,25,0 });
					}
				}
			}
		}

	}

	return ret;
}






