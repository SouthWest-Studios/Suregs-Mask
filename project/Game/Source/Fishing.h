#ifndef __FISHING_H__
#define __FISHING_H__

#include "Point.h"
#include "Input.h"
#include "Render.h"
#include "List.h"


enum class Fishlevel
{
	NOTHING,
	TRASH,
	SMALL,
	MEDIUM,
	BIG,
	UNKNOWN
};

enum class Rodlevel
{
	BASIC,
	INTERMEDIATE,
	ADVANCED
};


enum class FISHINGTYPE
{
	FISHING,
	LUREFISHING
};

class Fishing
{
public:
	Fishlevel fishlever;
	Rodlevel rodlevel;
	FISHINGTYPE fishingtype = FISHINGTYPE::FISHING;

	bool rodReady = true;
	bool playerGetRod = true;
	bool isFishing = false;
	bool startFishing = false;



};

#endif // __DIALOG_H__