#ifndef __OJOINV_H__
#define __OJOINV_H__

#include "inventity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class OjoInv : public Inventity
{
public:
	OjoInv::OjoInv() : Inventity(InventityType::OJO)
	{
		name.Create("OjoInv");
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