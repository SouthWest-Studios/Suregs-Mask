#ifndef __DIALOGTRIGGERENTITY_H__
#define __DIALOGTRIGGERENTITY_H__

#include <string>
#include "Entity.h"
#include "Dialog.h"
#include "List.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Missions.h"
#include "MissionManager.h"

struct SDL_Texture;

class DialogTrigger : public Entity
{
public:

	DialogTrigger();
	virtual ~DialogTrigger();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void PlayDialog();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	//misiones
	void SetDialogId(uint new_id);
	uint GetDialogId() const;
	void UpdateDialog();

private:

	uint dialogId;

public:

private:

	//En caso de querer añadirle una textura a la entidad
	SDL_Texture* texture = nullptr;

	//La textura de la cara general para el dialogo
	SDL_Texture* faceTexture = nullptr;

	//El path de la textura de la entidad
	const char* texturePath = nullptr;

	//El path de la textura de la cara general
	const char* faceTexturePath = nullptr;

	//Cuerpo de fisicas, para detectar cuando el jugador esta cerca
	//y activar el dialogo en caso de que el jugador asi lo quiera
	PhysBody* pbody = nullptr;
	PhysBody* pbody2 = nullptr;
	//Lista de los dialogos que puede decir este trigger
	List<Dialog*> dialogues;

	//Variables para saber si se tiene que repetir el dialogo y si
	//ya se a activado minimo una vez
	bool repeatDialog = false;
	bool played = false;

	//Lista de dialogos a repetir
	List<Dialog*> dialoguesRepeat;

	int dialog_fx;
};

#endif // __DIALOGTRIGGERENTITY_H__