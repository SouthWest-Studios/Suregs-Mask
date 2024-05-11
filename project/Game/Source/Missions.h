#ifndef __MISSIONS_H__
#define __MISSIONS_H__

#include "Module.h"
#include "Entity.h"
#include "inventity.h"
#include "List.h"
#include "Textures.h"
#include "MissionManager.h"

// Aquí se pondran todas las misiones y todo lo que las envuelve
// 


class Mission
{
public:

	Mission::Mission();


	// Destructor
	virtual ~Mission();

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Hacer las diferentes funciones para las misiones
	bool IsCompleted() const ;
	void SetCompleted(bool value);

	// Métodos específicos para diferentes tipos de misiones
	void AddRequiredItem(uint itemId);
	void RemoveRequiredItem(uint itemId);
	void SetRequiredEntity(Entity* entity);

	uint GetId() const { return id; }

public:

	//Variables publicas necesarias para el manager
	enum class State {
		NotStarted,
		InProgress,
		Completed
	};

	State state = State::NotStarted;


private:

	//Variables privadas

	uint id;
	std::string description;
	bool completed;
	std::vector<uint> requiredItems;
	Entity* requiredEntity;
};
#endif // __MISSIONS_H__

