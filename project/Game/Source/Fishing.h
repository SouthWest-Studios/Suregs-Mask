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

enum class FISHINGTYPE
{
	FISHING,
	LUREFISHING
};

class Fishing
{
public:
	Fishlevel fishlever;
	FISHINGTYPE fishingtype = FISHINGTYPE::FISHING;

	bool rodReady = false;
	bool playerGetRod = false;
	bool isFishing = false;
	bool startFishing = false;



};

#endif // __DIALOG_H__