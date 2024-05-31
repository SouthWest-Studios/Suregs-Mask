#include "NPC_Abuela.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene_Testing.h"
#include "Log.h"
#include "InventoryManager.h"
#include "Point.h"
#include "Physics.h"
#include "Window.h"
#include "ModuleFadeToBlack.h"

NPCAbuela::NPCAbuela() : Entity(EntityType::NPC_ABUELA)
{
    name = ("npc_abuelito");
}

NPCAbuela::~NPCAbuela() {}

bool NPCAbuela::Awake() {
    return true;
}

bool NPCAbuela::Start() {
    //position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

    TSprite = config.attribute("Tsprite").as_int();
    SpriteX = config.attribute("sprite_x").as_int();
    SpriteY = config.attribute("sprite_y").as_int();
    Photowidth = config.attribute("Pwidth").as_int();
    spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

    idleAnim.LoadAnim((char*)name.c_str(), "idleAnim", spritePositions);

    std::string texturePath = config.attribute("texturePath").as_string();
    texture = app->tex->Load(texturePath.c_str());

    app->entityManager->objectsToDraw.push_back({
        texture,
        position.x - 50, // x
        position.y - 100, // y
        position.y + 183, // anchorY
        185, // width
        110, // height
        true, // isFacingLeft
        false // isDynamic
        });

    //pbody = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
    //pbody->entity = this;
    //pbody->listener = this;
    //pbody->ctype = ColliderType::PLAYER;

    return true;
}

bool NPCAbuela::Update(float dt) {
    //nextState = EntityState::IDLE;

    currentAnimation->Update();
    return true;
}

bool NPCAbuela::PostUpdate() {
    if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
    SDL_Rect rect = currentAnimation->GetCurrentFrame();
    app->render->DrawTexture(texture, position.x, position.y, 0.45f, SDL_FLIP_NONE, &rect);

    return true;
}

bool NPCAbuela::CleanUp() {
    //app->physics->GetWorld()->DestroyBody(pbody->body);
    app->tex->UnLoad(texture);

    RELEASE(spritePositions);
    delete spritePositions;

    return true;
}

// L07 DONE 6: Define OnCollision function for the player.
void NPCAbuela::OnCollision(PhysBody* physA, PhysBody* physB) {
    switch (physB->ctype) {
    case ColliderType::PLATFORM:
        LOG("Collision PLATFORM");
        break;
    case ColliderType::ITEM:
        LOG("Collision ITEM");
        break;
    default:
        break;
    }
}
