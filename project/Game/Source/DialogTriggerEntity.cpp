#include "DialogTriggerEntity.h"
#include "DialogManager.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene_Testing.h"
#include "Scene_Pueblo.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Hud.h"
#include "CommerceManager.h"
#include "TreeManager.h"
#include "Menu.h"
#include "ElevatorMenu.h"
#include "Scene_Pueblo_Tutorial.h"

DialogTrigger::DialogTrigger() : Entity(EntityType::DIALOG_TRIGGER)
{
	name = ("dialogTrigger");
}

DialogTrigger::~DialogTrigger() {}

bool DialogTrigger::Awake() {

	return true;
}

bool DialogTrigger::Start() {



	//position.x = parameters.attribute("x").as_int(0);
	//position.y = parameters.attribute("y").as_int(0);
	texturePath = parameters.attribute("texturepath").as_string();
	faceTexturePath = parameters.attribute("facetexturepath").as_string("");
	repeatDialog = parameters.attribute("repeat").as_bool(false);

	std::string fontTarget = parameters.attribute("font").as_string("primary");

	


	played = false;

	//Cargar dialogos
	for (pugi::xml_node itemNode = parameters.child("sentences").child("sentence"); itemNode; itemNode = itemNode.next_sibling("sentence"))
	{
		dialogues.Add(app->dialogManager->CreateDialog(itemNode, parameters.attribute("name").as_string(), faceTexturePath, fontTarget.c_str()));
	}

	//Si el dialogo se reite, cargar las lineas que se repite
	if (repeatDialog) {
		for (pugi::xml_node itemNode = parameters.child("repeat_sentences").child("sentence"); itemNode; itemNode = itemNode.next_sibling("sentence"))
		{
			dialoguesRepeat.Add(app->dialogManager->CreateDialog(itemNode, parameters.attribute("name").as_string(), faceTexturePath, fontTarget.c_str()));
		}
	}


	//initilize textures
	texture = app->tex->Load(texturePath);

	dialog_fx = app->audio->LoadAudioFx("dialogAlt_fx");

	if (faceTexturePath != "") {
		faceTexture = app->tex->Load(faceTexturePath);
	}

	pbody = app->physics->CreateRectangleSensor(position.x+10, position.y, 200, 200, bodyType::KINEMATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::DIALOG_TRIGGER;

	CreateArbol();


	return true;
}

bool DialogTrigger::Update(float dt)
{
	//app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool DialogTrigger::CleanUp()
{

	app->tex->UnLoad(texture);
	app->tex->UnLoad(faceTexture);
	app->physics->DestroyBody(pbody);
	app->physics->DestroyBody(pbody2); app->entityManager->DestroyEntity(this); active = false;

	ListItem<Dialog*>* item;
	Dialog* pDialog = nullptr;

	for (item = dialogues.start; item != NULL; item = item->next)
	{

		pDialog = item->data;
		//pDialog->face_tex = faceTexture;
		pDialog->CleanUp();
		app->tex->UnLoad(pDialog->face_tex);

		RELEASE(item->data);
	}

	dialogues.Clear();

	
	pDialog = nullptr;

	for (item = dialoguesRepeat.start; item != NULL; item = item->next)
	{
		pDialog = item->data;
		//pDialog->face_tex = faceTexture;
		app->tex->UnLoad(pDialog->face_tex);
		RELEASE(item->data);
	}

	dialoguesRepeat.Clear();

	

	return true;
}

void DialogTrigger::PlayDialog()
{
	if (app->commerceManager->AnyCommerceActive()) { return; }
	//Play el dialogo normal
	if ((played && !repeatDialog) || !played) {
		ListItem<Dialog*>* item;
		Dialog* pDialog = nullptr;

		for (item = dialogues.start; item != NULL; item = item->next)
		{
			pDialog = item->data;
			app->dialogManager->AddDialog(pDialog);
			app->audio->PlayFx(dialog_fx);
			// Si el diálogo tiene un evento asociado, manéjalo
			if (pDialog->event_ != nullptr) {
				app->missionManager->HandleDialogEvent(*(pDialog->event_));
			}
		}
		played = true;
	}
	//Play el dialogo repetido
	else if (played && repeatDialog) {
		ListItem<Dialog*>* item;
		Dialog* pDialog = nullptr;

		for (item = dialoguesRepeat.start; item != NULL; item = item->next)
		{
			pDialog = item->data;
			app->dialogManager->AddDialog(pDialog);
			// Si el diálogo tiene un evento asociado, manéjalo
			if (pDialog->event_ != nullptr) {
				app->missionManager->HandleDialogEvent(*(pDialog->event_));
			}
		}
	}
}



void DialogTrigger::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
		case ColliderType::PLAYER:
			if (physA->ctype == ColliderType::ARBOL)
			{
				if (app->input->GetButton(CONFIRM) == KEY_DOWN && !app->scene_pueblo->GetRod()->fishing.rodReady  && !app->scene_pueblo_tutorial->GetRod()->fishing.rodReady)
				{
					app->treeManager->mostrar = true;
					app->menu->active = false;

				}
			}
			else if (!app->dialogManager->isPlaying && (app->input->GetButton(CONFIRM) == KEY_DOWN) && !app->scene_pueblo->GetRod()->fishing.rodReady  && !app->scene_pueblo_tutorial->GetRod()->fishing.rodReady) {
				PlayDialog();
				//printf("COLISSION TRIGGERDIALOG");

				//// Manejo de la etiqueta <mission_event>
				//pugi::xml_node option1Node = itemNode.child("option1");
				//pugi::xml_node missionEventNode = option1Node.child("mission_event");


				//	//printf("ASDASDASD");
				//	DialogEvent* dialogEvent = new DialogEvent();
				//	dialogEvent->type = missionEventNode.attribute("type").as_string();
				//	dialogEvent->mission_id = missionEventNode.attribute("mission_id").as_uint();

				//	app->missionManager->HandleDialogEvent(*dialogEvent);

				/*if (strcmp(parameters.attribute("name").as_string(), "FishingMaster") == 0) {
					app->scene_testing->GetRod()->fishing.rodReady = true;
					app->scene_testing->GetRod()->fishing.playerGetRod = true;
				}*/


				/*if (parameters.attribute("id").as_int() == 941) {
					//printf("REEL TRUE");
					app->scene_pueblo->GetRod()->fishing.rodReady  && !app->scene_pueblo_tutorial->GetRod()->fishing.rodReady = true;
					app->scene_pueblo->GetRod()->fishing.playerGetRod = true;
				}*/
				/*if (strcmp(parameters.attribute("name").as_string(), "pescador") == 0) {
					app->scene_pueblo->GetRod()->fishing.rodReady  && !app->scene_pueblo_tutorial->GetRod()->fishing.rodReady = true;
					app->scene_pueblo->GetRod()->fishing.playerGetRod = true;
				}*/
			}
			break;
	}
}

void DialogTrigger::SetDialogId(uint new_id) {
	dialogId = new_id;
	UpdateDialog(); // Actualizar el diálogo cuando cambia el id
}

uint DialogTrigger::GetDialogId() const {
	return dialogId;
}

void DialogTrigger::UpdateDialog() {
	// Comprobar si hay alguna actividad comercial activa
	if (app->commerceManager->AnyCommerceActive()) { return; }

	// Obtener el id del diálogo
	uint id = GetDialogId();

	// Buscar el diálogo con el id correspondiente
	ListItem<Dialog*>* item;
	Dialog* pDialog = nullptr;

	// Si el diálogo ha sido reproducido y no se debe repetir, o si no ha sido reproducido
	if ((played && !repeatDialog) || !played) {
		for (item = dialogues.start; item != NULL; item = item->next) {
			if (item->data->id == id) {
				pDialog = item->data;
				app->dialogManager->AddDialog(pDialog);
				app->audio->PlayFx(dialog_fx);
				break;
			}
		}
		played = true;
	}
	// Si el diálogo ha sido reproducido y se debe repetir
	else if (played && repeatDialog) {
		for (item = dialoguesRepeat.start; item != NULL; item = item->next) {
			if (item->data->id == id) {
				pDialog = item->data;
				app->dialogManager->AddDialog(pDialog);
				break;
			}
		}
	}
}

void Entity::CreateArbol()
{
	
}



