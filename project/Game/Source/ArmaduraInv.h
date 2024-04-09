#ifndef __ARMADURAINV_H__
#define __ARMADURAINV_H__

#include "inventity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class ArmaduraInv : public Inventity
{
public:

	
	ArmaduraInv::ArmaduraInv() : Inventity(InventityType::ARMADURA)
	{
		name.Create("ArmaduraInv");
	}
	//virtual ~ArmaduraInv();

	///*bool Awake();

	//bool Start();

	//bool Update(float dt);

	//bool PostUpdate();

	//bool CleanUp();*/

public:


	int defense;
	int durability;
	int magic;
	int weight;




};

#endif // __ARMADURAINV_H__