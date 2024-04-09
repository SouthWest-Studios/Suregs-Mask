#ifndef __ITEMINV_H__
#define __ITEMINV_H__

#include "inventity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Iteminv : public Inventity
{
public:

	
	Iteminv::Iteminv() : Inventity(InventityType::ITEM)
	{
		name.Create("ItemInv");
	}


public:

	bool original = false;
	

	


};

#endif // __SWORDINV_H__