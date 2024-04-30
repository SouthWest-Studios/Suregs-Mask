#ifndef __COMMERCEMANAGER_H__
#define __COMMERCEMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "inventity.h"
#include "List.h"
#include "Textures.h"
#include "Commerce.h"


enum class CommerceType
{
	BASICO,
	HERRERIA,
	UNKNOWN 
};


class CommerceManager : public Module
{
public:

	CommerceManager(App* app, bool start_enabled = true);

	// Destructor
	virtual ~CommerceManager();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


	Commerce* CreateCommerce(CommerceType type, uint id);
	void PlayCommerce(uint id);

	

public:





private:


	
};

#endif // __COMMERCEMANAGER_H__
