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
	name = ("TPEntity");
}

TPEntity::~TPEntity() {}

bool TPEntity::Awake() {

	return true;
}

bool TPEntity::Start() {

	pbody = app->physics->CreateRectangleSensor(position.x+10, position.y, 32, 32, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::TP_ENTITY;

	room = GetCurrentRoom();

    for(TPDoor* openDoor : app->entityManager->openDoors)
    {
        if(openDoor->sceneLevel == this->sceneLevel && openDoor->tpID == this->tpID)
        {
            this->isOpened = true;
            ////printf("TP abierto\n");
            break;
        }
    }

	return true;
}

bool TPEntity::Update(float dt)
{
	// if(isOpened) return true;
	
	// for(MapObject* openRoom : app->entityManager->openRooms)
	// {
	// 	if(openRoom == room)
	// 	{
	// 		this->isOpened = true;
	// 		////printf("TP abierto\n");
	// 		break;
	// 	}
	// }
	return true;
}

bool TPEntity::CleanUp()
{
	if (pbody != nullptr) {
		app->physics->DestroyBody(pbody);
	}
	return true;
}

void TPEntity::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
		case ColliderType::PLAYER:
			if(isOpened){
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


				

				app->entityManager->GetPlayer()->pbodyFoot->body->SetTransform(b2Vec2(PIXEL_TO_METERS(targetPos.x),PIXEL_TO_METERS(targetPos.y)), 0);
				break;
			}
	}
}

MapObject* TPEntity::GetCurrentRoom()
{
	//salas pequeñas
	for (ListItem<MapObject*>* item = app->map->smallRoomsList.start; item != nullptr; item = item->next)
	{
		MapObject* room = item->data;

		// tp dentro de la sala
		if (position.x >= room->x && position.x <= room->x + room->width &&
			position.y >= room->y && position.y <= room->y + room->height)
		{
			return room;
		}
	}

	//salas grandes
	for (ListItem<MapObject*>* item = app->map->largeRoomsList.start; item != nullptr; item = item->next)
	{
		MapObject* room = item->data;

		// tp dentro de la sala
		if (position.x >= room->x && position.x <= room->x + room->width &&
			position.y >= room->y && position.y <= room->y + room->height)
		{
			return room;
		}
	}

	//salas l
	for (ListItem<MapObject*>* item = app->map->LRoomsList.start; item != nullptr; item = item->next)
	{
		MapObject* room = item->data;

		// tp dentro de la sala
		if (position.x >= room->x && position.x <= room->x + room->width &&
			position.y >= room->y && position.y <= room->y + room->height)
		{
			return room;
		}
	}

	return nullptr;
}

