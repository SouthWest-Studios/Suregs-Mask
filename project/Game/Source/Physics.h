#pragma once
#include "Module.h"
#include "Entity.h"

#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y 0.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

// types of bodies
enum bodyType {
	DYNAMIC,
	STATIC,
	KINEMATIC
};

enum class ColliderType {
	PLAYER,
	PLAYER_ATTACK,
	MASK0_PASSIVE_ATTACK,
	MASK1_ATTACK,
	MASK2_ATTACK,
	ITEM,
	FLOAT,
	LAGO,
	PLATFORM,
	VACIOS,
	ENEMY,
	BOSS_INUIT,
	BOSS_IGORY,
	BOSS_MUSRI_ARROW,
	BOSS_MUSRI_CHARGED_ARROW,
	BOSS_MUSRI_CHARGED_RASTRO_ARROW,
	BOSS_SURMA_EXPLOSION,
	RESOURCE,
	NOTA,
	COFRE,
	ARBOL,
	DIALOG_TRIGGER,
	TP_ENTITY,
	PUZZLE_BUTTON_ENTITY,
	PUZZLE_LEVER_ENTITY,
	PUZZLE_BOX_ENTITY,
	ENTRADA_MAZMORRA0,
	SALIDA_MAZMORRA0,
	PROJECTILE,
	BOSSAREA,
	ATACKBMR,
	ATACK_INUIT,
	AROMAGICA,
	ASCENSOR,
	UNKNOWN,
	
	// ..
};

enum class Direction {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	UNKNOWN
};

enum class BTPDirection {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	UPLEFT,
	UPRIGHT,
	DOWNLEFT,
	DOWNRIGHT,
	UNKNOWN
};

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL), ctype(ColliderType::UNKNOWN)
	{}

	~PhysBody() {}

	void GetPosition(int& x, int& y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

	Entity* entity = nullptr;

public:
	int width, height;
	b2Body* body;
	Entity* listener;
	ColliderType ctype;
};

// Module --------------------------------------
class Physics : public Module, public b2ContactListener // TODO
{
public:

	// Constructors & Destructors
	Physics(App* app, bool start_enabled = true);
	~Physics();

	// Main module steps
	bool Start();
	bool PreUpdate();
	bool PostUpdate();
	bool PostLateUpdate();
	bool CleanUp();

	// Create basic physics objects
	PhysBody* CreateRectangle(int x, int y, int width, int height, bodyType type);
	PhysBody* CreateCircle(int x, int y, int radious, bodyType type, bool isSensor = false);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, bodyType type);
	PhysBody* CreateChain(int x, int y, int* points, int size, bodyType type);
	
	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	void EndContact(b2Contact* contact);

	//Destroy body
	void DestroyBody(PhysBody* body);

	void DestroyPendingBodies();


	b2World* GetWorld();

	// Debug mode
	bool debug;

private:

	
	std::vector<PhysBody*> bodiesToDestroy;
	// Box2D World
	b2World* world;
};