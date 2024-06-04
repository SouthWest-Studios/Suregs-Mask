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
	int numRect;

public:

	void PushBack(const SDL_Rect& rect)
	{
		if (totalFrames < MAX_FRAMES && totalFrames < sizeof(frames) / sizeof(frames[0]) && &rect != nullptr) {
			frames[totalFrames++] = rect;
		}
		/*if (totalFrames < MAX_FRAMES) {
		frames[totalFrames++] = rect;
		}*/
		/*if (&rect != nullptr) {
			frames[totalFrames++] = rect;
		}*/
		/*//printf("\ntotalFrames: %d", totalFrames);
		//printf("\nrect.x: %d", rect.x);
		//printf("\nrect.y: %d", rect.y);
		//printf("\nrect.w: %d", rect.w);
		//printf("\nrect.h: %d", rect.h);
		//printf("\nframcount: %d", framcount++);*/
		//frames[totalFrames++] = rect;
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
	void Clear() {
		totalFrames = 0;		
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

	void Reverse() {
		for (int i = 0; i < totalFrames / 2; ++i)
		{
			SDL_Rect temp = frames[i];
			frames[i] = frames[totalFrames - i - 1];
			frames[totalFrames - i - 1] = temp;
		}
	}

	int GetCurretFrameNumber() {
		int actualFrame = currentFrame;
		if (pingpongDirection == -1)
			actualFrame = totalFrames - currentFrame;

		return actualFrame;
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
		while (animationNode != NULL) {

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


	int checkSDLRect(pugi::xml_node AniInfo, SDL_Rect* rect) {

		if (rect == NULL) {
			//printf("SDL_Rect ES NULL£¡\n");
			return 0;
		}

		for (int i = AniInfo.attribute("start").as_int(); i < AniInfo.attribute("end").as_int(); i++) {
			try {
				int check_access = rect[i].x;
				check_access = rect[i].y;
				check_access = rect[i].w;
				check_access = rect[i].h;
			}
			catch (const std::exception& e) {
				//printf("Error£º%s\n", e.what());
				return 0;
			}
			catch (...) {
				//printf("no pillo Error£¡\n");
				return 0;
			}
		}


		return 1;
	}

	const char* getNameAnimation() {

		return aniName;
	}
};

#endif