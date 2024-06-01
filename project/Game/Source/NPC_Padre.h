#pragma once
#ifndef __NPC_PADRE_H__
#define __NPC_PADRE_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Timer.h"

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

private:

public:
    SDL_Texture* texture = NULL;
    uint texW, texH;

    pugi::xml_document dialogoFile;
    pugi::xml_node dialogoPadre;
    pugi::xml_parse_result parseResult;
   

    Animation* currentAnimation = nullptr;
    EntityState state;

    PhysBody* pbody = NULL;
    //Animation
    Animation SPosition;
    SDL_Rect* spritePositions;
    pugi::xml_node config;

private:
    Animation idleAnim;

public:
};

#endif // __NPC_ABUELA_H__
