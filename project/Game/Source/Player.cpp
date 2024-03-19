#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Window.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("player");


	idleAnim.LoadAnimation(name.GetString(), "idleAnim");
	runAnim.LoadAnimation(name.GetString(), "runAnim");


}

Player::~Player() {

}

bool Player::Awake() {

	//L03: DONE 2: Initialize Player parameters
	position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	return true;
}

bool Player::Start() {

	texture = app->tex->Load(config.attribute("texturePath").as_string());

	pbody = app->physics->CreateCircle(position.x, position.y, 20, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect
	pickCoinFxId = app->audio->LoadFx(config.attribute("coinfxpath").as_string());


	return true;
}

bool Player::Update(float dt)
{
	//L03: DONE 4: render the player texture and modify the position of the player using WSAD keys and render the texture

	b2Vec2 velocity = b2Vec2(0, 0);

	currentAnimation = &idleAnim;

	

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		velocity.y += -0.2 * dt;
		currentAnimation = &runAnim;
		isFacingLeft = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		velocity.y += 0.2 * dt;
		currentAnimation = &runAnim;
		isFacingLeft = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = -0.2 * dt;
		currentAnimation = &runAnim;
		isFacingLeft = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = 0.2 * dt;
		currentAnimation = &runAnim;
		isFacingLeft = false;
	}


	pbody->body->SetLinearVelocity(velocity);
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;


	CameraMovement(dt);



		
	currentAnimation->Update();

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();


	
	if (isFacingLeft) {
		app->render->DrawTexture(texture, position.x-50, position.y-200 , SDL_FLIP_HORIZONTAL, &rect, 1, 0, INT_MAX, INT_MAX, 0.1);
	}
	else {
		app->render->DrawTexture(texture, position.x-50, position.y -200, SDL_FLIP_NONE, &rect, 1, 0, INT_MAX, INT_MAX, 0.1);
	}
	

	return true;
}

bool Player::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. 
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}

void Player::CameraMovement(float dt)
{

	uint windowH;
	uint windowW;
	app->win->GetWindowSize(windowW, windowH);

	int targetPosX = (-position.x * app->win->GetScale() + (windowW / 2) - 10);
	int targetPosY = (-position.y * app->win->GetScale() + (windowH / 2) - 10) + 100;

	app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
	app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
	
}
