#ifndef __NPC_ABUELA_H__
#define __NPC_ABUELA_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Timer.h"

struct SDL_Texture;

class NPCAbuela : public Entity
{
public:
    NPCAbuela();
    virtual ~NPCAbuela();

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

    Animation* currentAnimation = nullptr;
    EntityState state;

    //Animation
    Animation SPosition;
    SDL_Rect* spritePositions;
    pugi::xml_node config;
    int TSprite;
    int SpriteX;
    int SpriteY;
    int Photowidth;

private:
    Animation idleAnim;

public:
};

#endif // __NPC_ABUELA_H__
