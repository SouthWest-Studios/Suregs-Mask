#ifndef __DIAMANTEINV_H__
#define __DIAMANTEINV_H__

#include "inventity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class DiamanteInv : public Inventity
{
public:
	DiamanteInv::DiamanteInv() : Inventity(InventityType::DIAMANTE)
	{
		name = ("DiamanteInv");
	}
	
	/*virtual ~Swordinv();*/

	/*bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();*/

public:

	
	

	


};

#endif // __DIAMANTEINV_H__