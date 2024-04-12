#ifndef __CUERNOINV_H__
#define __CUERNOINV_H__

#include "inventity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class CuernoInv : public Inventity
{
public:
	CuernoInv::CuernoInv() : Inventity(InventityType::CUERNO)
	{
		name.Create("Cuernoinv");
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