#include "TPEntity.h"
#include "DialogManager.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene_Testing.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

TPEntity::TPEntity() : Entity(EntityType::TP_ENTITY)
{
	name.Create("TPEntity");
}

TPEntity::~TPEntity() {}

bool TPEntity::Awake() {

	return true;
}

bool TPEntity::Start() {

	pbody = app->physics->CreateRectangleSensor(position.x+10, position.y, 32, 32, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::TP_ENTITY;

	return true;
}

bool TPEntity::Update(float dt)
{
	return true;
}

bool TPEntity::CleanUp()
{


	return true;
}

void TPEntity::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
		case ColliderType::PLAYER:
			LOG("Player se mueve a: X: %d, Y: %d", targetPosition.x, targetPosition.y);
			iPoint targetPos;

			int difX = abs(targetPosition.x - position.x);
			int difY = abs(targetPosition.y - position.y);

			if (difY > difX) {
				if (targetPosition.y < position.y) { targetPos.y = targetPosition.y - 64; }
				else { targetPos.y = targetPosition.y + 64; }
				targetPos.x = targetPosition.x;
			}
			else {
				if (targetPosition.x < position.x) { targetPos.x = targetPosition.x - 64; }
				else { targetPos.x = targetPosition.x + 64; }
				targetPos.y = targetPosition.y;
			}

			

			

			app->entityManager->GetPlayer()->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(targetPos.x),PIXEL_TO_METERS(targetPos.y)), 0);
			break;
	}
}



