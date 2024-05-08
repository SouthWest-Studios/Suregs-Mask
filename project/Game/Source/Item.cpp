#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene_Testing.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Animation.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name = ("item");
}

Item::~Item() {}

bool Item::Awake() {


	return true;
}

bool Item::Start() {
	//personal, opcional
	//position.x = parameters.attribute("x").as_int();
	//position.y = parameters.attribute("y").as_int();

	////obligatorio
	//texturePath = parameters.attribute("texturePath").as_string();
	//TSprite = parameters.attribute("Tsprite").as_int();
	//SpriteX = parameters.attribute("sprite_x").as_int();
	//SpriteY = parameters.attribute("sprite_y").as_int();
	//Photowidth = parameters.attribute("Pwidth").as_int();
	//spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);
	////Animation
	//testNombre.LoadAnim("item", "idleAnim", spritePositions);


	//
	////initilize textures
	//texture = app->tex->Load(texturePath);
	return true;
}

bool Item::Update(float dt)
{
	/*printf("\n%s", testNombre.getNameAnimation());
	
	if (testNombre.HasFinished() && testNombre.getNameAnimation() == "idleAnim") {
		printf("Este Animation esta terminado");
	}*/
	
	return true;
}

bool Item::PostUpdate()
{
	/*if (currentAnimation == nullptr) { currentAnimation = &testNombre; }
	rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, 1400, 2300, 2, SDL_FLIP_NONE, &rect);
	currentAnimation->Update();*/

	return true;
}

bool Item::CleanUp()
{
	return true;
}