#include "DialogManager.h"
#include "MissionManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene_Testing.h"
#include "Scene_Pueblo.h"
#include "Scene_Pueblo_Tutorial.h"
#include "Scene_Arena_Tutorial.h"
#include "ModuleFadeToBlack.h"
#include "CommerceManager.h"
#include "Window.h"
#include "Hud.h"
#include "Audio.h"
#include "Menu.h"

#include "Defs.h"
#include "Log.h"
#include "SDL_ttf/include/SDL_ttf.h"



DialogManager::DialogManager(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name = ("dialogmanager");
}

// Destructor
DialogManager::~DialogManager()
{}

// Called before render is available
bool DialogManager::Awake(pugi::xml_node config)
{
	LOG("Loading Dialog Manager");
	bool ret = true;

	background_tex_path = config.child("textures").child("background_dialog").attribute("texturepath").as_string();


	return ret;

}

bool DialogManager::Start() {

	bool ret = true; 

	uint windowW, windowH;
	app->win->GetWindowSize(windowW, windowH);
	
	//Calculo del tamaño de la caja de texto
	textBoundWidth = windowW - dialogMargin[1] - dialogMargin[3];
	

	indexText = 1;


	background_tex = app->tex->Load(background_tex_path.c_str());

	dialog_fx = app->audio->LoadAudioFx("dialog_fx");
	dialogAlt_fx = app->audio->LoadAudioFx("dialogAlt_fx");
	dialogAlt2_fx = app->audio->LoadAudioFx("dialogAlt2_fx");

	return ret;
}

// Called before quitting
bool DialogManager::CleanUp()
{
	bool ret = true;
	ListItem<Dialog*>* item;
	item = dialogues.end;

	while (item != NULL && ret == true)
	{

		ret = item->data->CleanUp();
		item = item->prev;
	}
	app->tex->UnLoad(background_tex);
	dialogues.Clear();

	return ret;
}

Dialog* DialogManager::CreateDialog(pugi::xml_node itemNode, std::string name, const char* faceTexturePath, const char* font)
{
	//Dialogo a crear
	Dialog* dialog = new Dialog(itemNode.attribute("text").as_string());
	dialog->name = name;
	dialog->name = itemNode.attribute("name").as_string(dialog->name.c_str());
	dialog->face_tex = app->tex->Load(itemNode.attribute("facetexturepath").as_string(faceTexturePath));
	dialog->font = FontSelector(itemNode.attribute("font").as_string(font));

	const char* type = itemNode.attribute("type").as_string("");

	// Crear un DialogEvent si el nodo XML tiene una etiqueta <mission_event>
	pugi::xml_node missionEventNode = itemNode.child("mission_event");

	if (missionEventNode) {
		DialogEvent* event = new DialogEvent();
		event->type = missionEventNode.attribute("type").as_string();
		event->mission_id = missionEventNode.attribute("mission_id").as_uint();
		dialog->event_ = event;
		printf("Se ha creado un DialogEvent con tipo: %s y mission_id: %d\n", event->type.c_str(), event->mission_id);
	}

	if (strcmp(type, "choose") == 0) {

		dialog->type = DialogType::CHOOSE;

		//Options1
		dialog->option1 = itemNode.child("option1").attribute("text").as_string();
		for (pugi::xml_node optionNode = itemNode.child("option1").child("sentence"); optionNode; optionNode = optionNode.next_sibling("sentence")) {
			Dialog* dialogOp1 = CreateDialog(optionNode, name, faceTexturePath, font);
			dialogOp1->commerceId = itemNode.child("option1").child("commerce").attribute("id").as_int();
			dialogOp1->comer = itemNode.child("option1").child("comer").attribute("id").as_int();
			dialogOp1->tutorial = itemNode.child("option1").child("tutorial").attribute("id").as_int();
			dialog->options1.Add(dialogOp1);

		}

		//Options2
		dialog->option2 = itemNode.child("option2").attribute("text").as_string();
		for (pugi::xml_node optionNode = itemNode.child("option2").child("sentence"); optionNode; optionNode = optionNode.next_sibling("sentence")) {
			Dialog* dialogOp2 = CreateDialog(optionNode, name, faceTexturePath, font);
			dialog->options2.Add(dialogOp2);
		}
	}

	return dialog;
}

//New//
void DialogManager::CreateDialogSinEntity(std::string Texto, std::string nombre, const char* texture)
{
	Dialog* dialogoPesca = new Dialog(Texto);
	dialogoPesca->name = nombre;
	dialogoPesca->font = app->render->primaryFont;
	if (texture != nullptr) {
		dialogoPesca->face_tex = app->tex->Load(texture);
	}
	//dialogoPesca->face_tex = ;
	app->dialogManager->AddDialog(dialogoPesca);
}

//New//
void DialogManager::AutoNextDiagolo(int autoNextTime)
{
	isPlaying = (dialogues.Count() > 0);
	autoNextTime_show = autoNextTime_TimerDown.CountDown(autoNextTime);

	if (isPlaying) {
		if ((int)autoNextTime_show == 0) {
			/*if (app->scene_testing->GetRod()->fishingEndCloseDialogo == true) {
				app->scene_testing->GetRod()->isEnd = true;
			}*/
			if (app->scene_pueblo->GetRod()->fishingEndCloseDialogo == true) {
				app->scene_pueblo->GetRod()->isEnd = true;
			}
			indexText = 1;
			dialogues.Del(dialogues.At(0));
			//app->scene_testing->GetRod()->dialogoautoclose = false;
			app->scene_pueblo->GetRod()->dialogoautoclose = false;
		}
	}
}

bool DialogManager::AddDialog(Dialog* dialog)
{
	dialogues.Add(dialog);
	return true;
}

bool DialogManager::ShowDialog(Dialog* dialog)
{
	//Mostrar fondo
	//app->render->DrawTexture(background_tex, dialogPosition.x, dialogPosition.y, (int)app->win->GetScale(), 0);
	app->render->DrawTexture(background_tex, dialogPosition.x, dialogPosition.y, (int)app->win->GetScale(), SDL_FLIP_NONE, 0, 0);

	std::string actualText = dialog->sentence.substr(0, indexText);
	

	//Inicializando las variables para las texturas
	SDL_Texture* textTexture = nullptr;
	SDL_Texture* textNameTexture = nullptr;
	SDL_Texture* options1NameTexture = nullptr;
	SDL_Texture* options2NameTexture = nullptr;

	//Setting de las variables
	int _textBoundWidth;
	iPoint _dialogPosition;

	if (dialog->face_tex != nullptr) {
		_textBoundWidth = textBoundWidth - faceTextureSize.x;
		_dialogPosition.x = dialogMargin[3] + dialogPosition.x + faceTextureSize.x;
		_dialogPosition.y = dialogMargin[0] + dialogPosition.y;
	}
	else {
		_textBoundWidth = textBoundWidth;
		_dialogPosition.x = dialogMargin[3] + dialogPosition.x;
		_dialogPosition.y = dialogMargin[0] + dialogPosition.y;
	}

	if (dialog->type == DialogType::CHOOSE) {
		_textBoundWidth = optionsPosition.x - faceTextureSize.x - 10; //CAMBIAR PARA AJUSTAR LAS OPCINES A LA DERECHA
	}

	//Textura dialogo
	textTexture = CreateTextTexture(dialog->font, actualText.c_str(), textColor, _textBoundWidth);
	app->render->DrawTexture(textTexture, _dialogPosition.x, _dialogPosition.y, (int)app->win->GetScale(), SDL_FLIP_NONE, 0, 0);
	
	//Imagen del personaje
	if (dialog->face_tex != nullptr) {
		app->render->DrawTexture(dialog->face_tex, dialogMargin[3] + dialogPosition.x, dialogMargin[0] + dialogPosition.y, (int)app->win->GetScale(), SDL_FLIP_NONE, 0, 0);
	}



	//Nombre personaje
	textNameTexture = CreateTextTexture(app->render->titleFont, dialog->name.c_str(), textColor, textNameBoundWidth);
	app->render->DrawTexture(textNameTexture, dialogMargin[3] + dialogPosition.x + namePosition.x, dialogMargin[0] + dialogPosition.y + namePosition.y, (int)app->win->GetScale(), SDL_FLIP_NONE, 0, 0);


	//Opciones
	if (dialog->type == DialogType::CHOOSE) {

		//Textura opcion1
		options1NameTexture = CreateTextTexture(app->render->primaryFont, dialog->option1.c_str(), (optionSelected == 1) ? optionSelectedColor : optionColor, optionsBoundWidth);
		app->render->DrawTexture(options1NameTexture, dialogMargin[3] + dialogPosition.x + optionsPosition.x, dialogMargin[0] + dialogPosition.y + optionsDistanceBetween, (int)app->win->GetScale(), SDL_FLIP_NONE, 0, 0);

		//Textura opcion2
		options2NameTexture = CreateTextTexture(app->render->primaryFont, dialog->option2.c_str(), (optionSelected == 2) ? optionSelectedColor : optionColor, optionsBoundWidth);
		app->render->DrawTexture(options2NameTexture, dialogMargin[3] + dialogPosition.x + optionsPosition.x, dialogMargin[0] + dialogPosition.y + optionsDistanceBetween*2, (int)app->win->GetScale(), SDL_FLIP_NONE, 0, 0);

	}


	

	//Optimizacion de la memoria
	SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(textNameTexture);
	SDL_DestroyTexture(options1NameTexture);
	SDL_DestroyTexture(options2NameTexture);


	for (const auto& NPCs : app->audio->NPCs) {
		if (NPCs.name == dialog->name) {
			voice = NPCs.voice;
			break;
		}
	}

	//Añadir caracter segun el tiempo
	if (actualText.size() < dialog->sentence.size()) {
		switch (voice) 
		{
		case 1:
			if (charTimer.ReadMSec() >= charTimeMS) {
				indexText++;
				charTimer.Start();
				app->audio->StopFx(-1);
				app->audio->PlayFx(dialog_fx);
			}
			break;

		case 2:
			if (charTimer.ReadMSec() >= charTimeMS) {
				indexText++;
				charTimer.Start();
				app->audio->StopFx(-1);
				app->audio->PlayFx(dialogAlt2_fx);
			}
			break;

		case 3:
			if (charTimer.ReadMSec() >= charTimeMS) {
				indexText++;
				charTimer.Start();
				app->audio->StopFx(-1);
				app->audio->PlayFx(dialogAlt_fx);
			}
			break;

		default:
			if (charTimer.ReadMSec() >= charTimeMS) {
				indexText++;
				charTimer.Start();
				app->audio->StopFx(-1);
				app->audio->PlayFx(dialog_fx);
			}

		}
		
		return false;
	}

	return true;
}

SDL_Texture* DialogManager::CreateTextTexture(TTF_Font* font, const char* text, SDL_Color color, int textBoundWidth)
{
	SDL_Surface* textSurface = nullptr;
	SDL_Texture* textTexture = nullptr;

	

	textSurface = TTF_RenderUTF8_Blended_Wrapped(font, text, color, textBoundWidth);
	textTexture = SDL_CreateTextureFromSurface(app->render->renderer, textSurface);

	SDL_FreeSurface(textSurface);

	return textTexture;
}

TTF_Font* DialogManager::FontSelector(const char* font)
{

	if (std::strcmp(font, "primary") == 0) {
		return app->render->primaryFont;

	}
	else if (std::strcmp(font, "secondary") == 0) {
		return app->render->titleFont;
	}


	//Por defecto
	return app->render->primaryFont;
}

bool DialogManager::Update(float dt)
{
	return true;
}

bool DialogManager::PostUpdate() {

	bool ret = true;

	//Para saber si hay algun dialogo en funcionamiento
	isPlaying = (dialogues.Count() > 0);

	if (isPlaying) { //Entonces mostrar dialogos
		app->menu->active = false;
		
		Dialog* actualDialog = dialogues.At(0)->data;
		bool dialogFinished = ShowDialog(actualDialog);



		//Gestionar la opcion seleccionada
		if (app->input->GetButton(UP) == KEY_DOWN) {
			optionSelected = 1;
		}
		if (app->input->GetButton(DOWN) == KEY_DOWN) {
			optionSelected = 2;
		}


		//Siguiente dialogo
		if (dialogFinished && (app->input->GetButton(CONFIRM) == KEY_DOWN || app->input->GetButton(SELECT) == KEY_DOWN) && actualDialog->type != DialogType::CHOOSE) {
			
			
			indexText = 1;
			if (dialogues.At(0)->data->commerceId != -1) {
				app->commerceManager->PlayCommerce(dialogues.At(0)->data->commerceId);
			}

			if (dialogues.At(0)->data->comer != -1) {
				app->entityManager->GetPlayer()->currentStats.currentHealth = app->entityManager->GetPlayer()->currentStats.maxHealth;
			}

			if (dialogues.At(0)->data->tutorial != -1) {
				app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_arena_tutorial, 90);
				app->tutorialHasStarted == true;
			}

			dialogues.Del(dialogues.At(0));
			
			


		}
		//Gestion de las opciones
		else if (dialogFinished && (app->input->GetButton(CONFIRM) == KEY_DOWN || app->input->GetButton(SELECT) == KEY_DOWN) && optionSelected != 0 && actualDialog->type == DialogType::CHOOSE) {
		
			
			if (optionSelected == 1) {
				dialogues.InsertAfter(0, actualDialog->options1);
			}
			else if (optionSelected == 2) {
				dialogues.InsertAfter(0, actualDialog->options2);
			}
			
			if (dialogues.At(0)->data->commerceId != -1) {
				app->commerceManager->PlayCommerce(dialogues.At(0)->data->commerceId);
			}

			if (dialogues.At(0)->data->comer != -1) {
				app->entityManager->GetPlayer()->currentStats.currentHealth = app->entityManager->GetPlayer()->currentStats.maxHealth;
			}

			if (dialogues.At(0)->data->tutorial != -1) {
				app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_arena_tutorial);
			}
			//Reiniciar varialbes de dialogo y quitar el dialogo actual de su lista
			optionSelected = 0;
			indexText = 1;
			dialogues.Del(dialogues.At(0));

		}
		//Terminar el dialogo empezado
		else if (!dialogFinished && (app->input->GetButton(CONFIRM) == KEY_DOWN || app->input->GetButton(SELECT) == KEY_DOWN) && indexText > 2) {
			indexText = 999;
		}
	
	}
	else {
		//Reiniciar variables mientras no este algun dialogo en marcha
		indexText = 1;
		optionSelected = 0;
	}

	return ret;

}

