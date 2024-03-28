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
#include "ModuleFadeToBlack.h"




Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("player");

}

Player::~Player() {

}

bool Player::Awake() {



	


	return true;
}

bool Player::Start() {

	
	//position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	TSprite = config.attribute("Tsprite").as_int();
	SpriteX = config.attribute("sprite_x").as_int();
	SpriteY = config.attribute("sprite_y").as_int();
	Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

	idleAnim.LoadAnim("player", "idleAnim", spritePositions);
	runAnim.LoadAnim("player", "runAnim", spritePositions);

	
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
	nextState = EntityState::IDLE;

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godmode = !godmode;
		pbody->body->GetFixtureList()[0].SetSensor(godmode);
	}

	if (godmode){GodMode(dt);}
	else PlayerMovement(dt);

	CameraMovement(dt);

	switch (nextState) {
	case EntityState::RUNNING:
		Run(dt);
		break;
	case EntityState::ATTACKING:
		Attack(dt);
		break;
	case EntityState::IDLE:
		DoNothing(dt);
		break;
	default:
		break;
	}
	//switch (state)
	//{
	//case EntityState::IDLE:
	//	currentAnimation = &idleAnim;
	//	break;
	//case EntityState::RUNNING:
	//	currentAnimation = &runAnim;
	//	break;
	//case EntityState::ATTACKING:
	//	//currentAnimation = &attackAnim;
	//	break;
	//default:
	//	//currentAnimation = &idleAnim;
	//	break;
	//}

	currentAnimation->Update();
	return true;
}


bool Player::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();


	if (isFacingLeft) {
		app->render->DrawTexture(texture, position.x - 50, position.y - 200, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 50, position.y - 200, SDL_FLIP_NONE, &rect);
	}

	return true;
}


bool Player::CleanUp()
{
	return true;
}

void Player::DoNothing(float dt)
{
	currentAnimation = &idleAnim;
	//printf("idle");
}

void Player::Run(float dt)
{
	//printf("runn");
	
	currentAnimation = &runAnim;
}

void Player::Attack(float dt)
{
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

	if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		camaralibre = !camaralibre; // Esto alternar?el valor de camaralibre cada vez que se presione la tecla C en modo debug
	}
	if (camaralibre == true)
	{
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			app->render->camera.y += (int)ceil(1 * dt);

		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			app->render->camera.y -= (int)ceil(1 * dt);

		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			app->render->camera.x += (int)ceil(1 * dt);

		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			app->render->camera.x -= (int)ceil(1 * dt);
	}
	else
	{
		app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
		app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.005f);
	}
	

	
	
}

void Player::GodMode(float dt)
{
	float speedFast;
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		speedFast = speed * 2;
	}
	else {
		speedFast = speed;
	}


	b2Vec2 velocity = b2Vec2(0, 0);
	pbody->body->SetLinearVelocity(velocity);
	
	//Moverse a la izquierda
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		velocity.y += -speedFast * dt;
		nextState = EntityState::RUNNING;
		isFacingLeft = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		velocity.y += speedFast * dt;
		nextState = EntityState::RUNNING;
		isFacingLeft = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = -speedFast * dt;
		nextState = EntityState::RUNNING;
		isFacingLeft = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = speedFast * dt;
		nextState = EntityState::RUNNING;
		isFacingLeft = false;
	}
	pbody->body->SetLinearVelocity(velocity);
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;

}

void Player::PlayerMovement(float dt)
{
	b2Vec2 velocity = b2Vec2(0, 0);
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		velocity.y += -0.2 * dt;
		nextState = EntityState::RUNNING;
		isFacingLeft = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		velocity.y += 0.2 * dt;
		nextState = EntityState::RUNNING;
		isFacingLeft = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = -0.2 * dt;
		nextState = EntityState::RUNNING;
		isFacingLeft = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = 0.2 * dt;
		nextState = EntityState::RUNNING;
		isFacingLeft = false;
	}
	pbody->body->SetLinearVelocity(velocity);
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;
}

