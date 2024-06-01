#ifndef __DIALOG_H__
#define __DIALOG_H__

#include "Point.h"
#include "Input.h"
#include "Render.h"
#include "List.h"


struct MissionEvent {
	std::string type;
	uint mission_id;
};

struct DialogEvent {
	std::string type;
	uint mission_id;
	MissionEvent* mission_event; // Puntero a MissionEvent
};

enum class DialogType
{
	TEXT,
	CHOOSE,
	QUEST_UPDATE,
	SELECT_CHOOSE
};

class Dialog
{
public:

	Dialog(std::string sentence) : sentence(sentence) {}

	Dialog(uint id) : id(id) {}

	virtual bool CleanUp()
	{

		ListItem<Dialog*>* item;
		Dialog* pDialog = nullptr;

		for (item = options1.start; item != NULL; item = item->next)
		{
			pDialog = item->data;
			SDL_DestroyTexture(pDialog->face_tex);
		}
		options1.Clear();

		for (item = options2.start; item != NULL; item = item->next)
		{
			pDialog = item->data;
			SDL_DestroyTexture(pDialog->face_tex);
		}
		options2.Clear();

		return true;
	}

	bool has_event() const { return event_ != nullptr; }
	const DialogEvent& event() const { return *event_; }

public:

	//Definir el tipo del dialogo, si es normal o de elecciones
	DialogType type;

	//Frase del dialogo
	std::string sentence;

	//Nombre de quien realiza el dialogo
	std::string name;

	//Texturas
	SDL_Texture* face_tex = nullptr;
	const char* face_tex_path = nullptr;

	//El texto de las opciones
	std::string option1;
	std::string option2;

	//El resultado al escoger dichas opciones
	List<Dialog*> options1;
	List<Dialog*> options2;

	int commerceId = -1;

	int comer = -1;

	int tutorial = -1;

	TTF_Font* font = nullptr;

	DialogEvent* event_ = nullptr;

	int questLine = -1;
	int nextTargetID = -1;
	int actualTargetID = -1;
	int selectChoose = -1;

	uint id;

private:


};

#endif // __DIALOG_H__
