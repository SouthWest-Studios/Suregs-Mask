#ifndef __GARRAINV_H__
#define __GARRAINV_H__

#include "inventity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class GarraInv : public Inventity
{
public:
	GarraInv::GarraInv() : Inventity(InventityType::GARRA)
	{
		name.Create("GarraInv");
	}
	
	/*virtual ~Swordinv();*/

	/*bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();*/

public:

	
	

	


};

#endif // __CUERNOINV_H__