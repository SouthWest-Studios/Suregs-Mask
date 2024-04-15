#pragma once
#ifndef __MINIGAMEFISHING_H__
#define __MINIGAMEFISHING_H__


#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include "Timer.h"

#include "List.h"

struct SDL_Texture;



class MiniGameFishing : public Entity
{
public:

	

	MiniGameFishing();
	virtual ~MiniGameFishing();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool miniGameStart(float dt);
	bool miniGameLoop(float dt);
	bool miniGameEnd(float dt);
	void reward_pool(Fishlevel fishingType, std::vector<std::vector<const char*>>* chosefishing_path_ptr);


	void castingline(FISHINGTYPE type);
	void ani_castingline(Direction direction);
	void playNormalFishing();
	void playLureFishing();
	void GamePlaye();
	void fishingOver();
	void hooked(int player_click_count);
	void resetProbability();
	void changeProbability(double nothing, double trash, double small, double medium, double big);
	void floatCollision(Direction direction,float cheke_x, float cheke_y);
	void fishing_line(Direction direction, float cheke_x, float cheke_y);
	void dialogoClose(int time);
	bool check_isFishCaught();
	int getRandomNumber(int min, int max);

public:

	pugi::xml_node config;
	uint texW, texH;

	SString floatPath;
	float cheke_x;
	float cheke_y;
	bool startAniFloat = false;


	Direction player_Direction;
	//Animacion
	Animation* currentAnimation = nullptr;
	SDL_Rect rect;
	int TSprite;
	int SpriteX;
	int SpriteY;
	int PhotoWeight;
	const char* Path;
	SDL_Texture* texture = NULL;

	Fishing fishing;


	/*List<const char*>chosefishing_path;
	List<const char*>choseName_path;*/

	

	
	//fishingfloat
	const char* fishingfloat_path;
	SDL_Texture* fishingfloat_texture = NULL;
	float fishingflota_position_x;
	float fishingflota_position_y;
	float testX;
	float testY;
	float fishingflota_CenterX;
	float fishingflota_CenterY;
	bool fishingfloat_lineReady = false;
	bool fishingfloat_getPlayerPosition = true;;
	bool isFishingta = false;
	bool crearfloatbody = false;
	PhysBody* floatbody = nullptr;
	float floatDistance;
	float lureDistance;
	float floatChangeDistance = 100;
	bool startFinishingLine = false;
	bool lureDistanceGetRandom;

	//Lure
	bool lureRandomTime = false;
	bool isFishCaught_result = false;
	bool lureFinishLine = false;
	Timer luretimeFishing;
	//bool selected_prize;

	//fishingtype
	FISHINGTYPE fishingtype;

	//StartFishing
	Timer timeFishing;
	int lotteryrandomNum;
	int lure_lotteryrandomNum;
	bool thistimehooked = true;
	bool playerGoplay = false;
	bool playerGoplay_TimeOver = false;
	Fishlevel selected_fish;
	int fishLevel;
	

	//GamePlay
	Timer gamePlayTimeLimit;
	uint32 gamePlayTimeLimit_show = 0;
	bool gamePlayStart;
	int player_click_count;
	int player_click_count_TimeOver;
	bool dialogoautoclose = false;
	bool dialogoPlayerMoving = false;
	int dialogoTimeCount;
	bool fishingEndCloseDialogo = false;
	bool isEnd = false;
	int gamePlayTime;

private:
	std::vector<std::vector<const char*>>* chosefishing_path_ptr = new std::vector<std::vector<const char*>>();
	std::vector<std::vector<const char*>>* choseName_path = new std::vector<std::vector<const char*>>();
	const char* fishing_path;
	const char* name_path;
	
};
#endif 
#pragma once
