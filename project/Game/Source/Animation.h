#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "PugiXml/src/pugixml.hpp"

#define MAX_FRAMES 2500

class Animation
{
public:
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
	bool loop = true;
	// Allows the animation to keep going back and forth
	bool pingpong = false;
	const char* aniName;

private:
	float currentFrame = 0.0f;
	int totalFrames = 0;
	int loopCount = 0;
	int pingpongDirection = 1;

public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[totalFrames++] = rect;
	}

	void Reset()
	{
		currentFrame = 0;
		loopCount = 0;
	}

	bool HasFinished()
	{
		return !loop && !pingpong && loopCount > 0;
	}

	void Update()
	{
		currentFrame += speed;
		if (currentFrame >= totalFrames)
		{
			currentFrame = (loop || pingpong) ? 0.0f : totalFrames - 1;
			++loopCount;

			if (pingpong)
				pingpongDirection = -pingpongDirection;
		}
	}

	SDL_Rect& GetCurrentFrame()
	{
		int actualFrame = currentFrame;
		if (pingpongDirection == -1)
			actualFrame = totalFrames - currentFrame;

		return frames[actualFrame];
	}


	void LoadAnimation(const char* entity, const char* nombre) {
		pugi::xml_document configFile;
		pugi::xml_node animationsNode;
		pugi::xml_node animationNode;
		pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
		animationsNode = configFile.child("config").child("animations").child(entity).child(nombre);

		animationNode = animationsNode.child("frame");
		//animationsNode.child("anim").next_sibling
		while(animationNode != NULL){
		
			//map.attribute("height").as_int();
			this->PushBack({
					animationNode.attribute("x").as_int(),
					animationNode.attribute("y").as_int(),
					animationNode.attribute("w").as_int(),
					animationNode.attribute("h").as_int()
				});


			animationNode = animationNode.next_sibling();
		}


		this->loop = animationsNode.attribute("loop").as_bool();
		this->speed = animationsNode.attribute("speed").as_float();
	}


	SDL_Rect* SpritesPos(int totalPos, int width, int hight, int ancho)
	{

		SDL_Rect* anirec = new SDL_Rect[totalPos];
		int x = 0;
		int y = 0;
		for (int i = 0; i < totalPos; i++)
		{
			anirec[i].x = x;
			anirec[i].y = y;
			anirec[i].w = width;
			anirec[i].h = hight;
			if (x == ancho - width) {
				x = 0;
				y += hight;
			}
			else {
				x += width;

			}
		}
		return anirec;
	}

	void LoadAnim(char* Anipart, const char* NombreAni, SDL_Rect* spritePositions) {


		aniName = NombreAni;
		pugi::xml_document configFile;
		pugi::xml_node AniInfo;
		pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
		AniInfo = configFile.child("config").child("animations").child(Anipart).child(NombreAni);

		if (AniInfo.attribute("start").as_int() > AniInfo.attribute("end").as_int()) {
			for (int i = AniInfo.attribute("start").as_int(); i > AniInfo.attribute("end").as_int(); i--)
			{
				this->PushBack({ spritePositions[i] });

			}
		}
		else {
			for (int i = AniInfo.attribute("start").as_int(); i < AniInfo.attribute("end").as_int(); i++)
			{
				this->PushBack({ spritePositions[i] });

			}
		}
		this->speed = AniInfo.attribute("speed").as_float();
		this->loop = AniInfo.attribute("loop").as_bool();
	}

	const char* getNameAnimation() {

		return aniName;
	}
};

#endif