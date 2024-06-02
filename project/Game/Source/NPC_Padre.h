#pragma once
#pragma once
#ifndef __NPC_PADRE_H__
#define __NPC_PADRE_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Timer.h"
#include "PugiXml\src\pugixml.hpp"
#include "DialogManager.h"
#include "Dialog.h"
#include "DialogTriggerEntity.h"


struct SDL_Texture;

class NPCPadre : public Entity
{
public:
    NPCPadre();
    virtual ~NPCPadre();

    bool Awake();
    bool Start();
    bool Update(float dt);
    bool PostUpdate();
    bool CleanUp();
  
    // L07 DONE 6: Define OnCollision function for the player.
    void OnCollision(PhysBody* physA, PhysBody* physB);



public:
    SDL_Texture* texture = nullptr;
    uint texW, texH;

    pugi::xml_document dialogoFile;
    pugi::xml_node dialogNode;

    bool dialogoMostrado = false;
    Animation* currentAnimation = nullptr;

    PhysBody* pbody = nullptr;
    //Animation
    Animation SPosition;
    SDL_Rect* spritePositions = nullptr;


private:
    Animation idleAnim;

   

public:
};

#endif // __NPC_ABUELA_H__
