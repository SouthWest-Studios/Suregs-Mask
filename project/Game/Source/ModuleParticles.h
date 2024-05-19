//#ifndef __MODULE_PARTICLES_H__
//#define __MODULE_PARTICLES_H__
//
//#include "Module.h"
//
//#include "Defs.h"
//#include "App.h"
//#include "Render.h"
//#include "Textures.h"
//#include "Particle.h"
//#include "Physics.h"
//
//#define MAX_ACTIVE_PARTICLES 100
//
//struct SDL_Texture;
//struct Collider;
//
//class ModuleParticles : public Module
//{
//public:
//	// Constructor
//	// Initializes all the particles in the array to nullptr
//	ModuleParticles(App* app, bool start_enabled = true);
//
//	//Destructor
//	~ModuleParticles();
//
//	// Called when the module is activated
//	// Loads the necessary textures for the particles
//	bool Start();
//
//	// Called at the beginning of the application loop
//	// Removes all particles pending to delete
//	bool PreUpdate();
//
//	// Called at the middle of the application loop
//	// Iterates all the particles and calls its Update()
//	// Removes any "dead" particles
//	bool Update();
//
//	// Called at the end of the application loop
//	// Iterates all the particles and draws them
//	bool PostUpdate();
//
//		// Called on application exit
//		// Destroys all active particles left in the array
//	bool CleanUp();
//	void OnCollision(PhysBody* c1, PhysBody* c2);
//
//	// Creates a new particle and adds it to the array
//	// Param particle	- A template particle from which the new particle will be created
//	// Param x, y		- Position x,y in the screen (upper left axis)
//	// Param delay		- Delay time from the moment the function is called until the particle is displayed in screen
//	Particle* AddParticle(const Particle& particle, int x, int y, ColliderType colliderType = ColliderType::ENEMY, uint delay = 0);
//
//public:
//	//Template particle for an explosion
//	Particle explosion;
//	Particle bigExplosion;
//
//	Particle bossFire;
//
//	//Template particle for a laser
//	Particle laser;
//	Particle laserR;
//	Particle laserL;
//
//	Particle bat_shotsR;
//	Particle bat_shotsL;
//
//	Particle redShot;
//
//	Particle fireBall;
//
//private:
//	// Particles spritesheet loaded into an SDL Texture
//	SDL_Texture* texture = nullptr;
//
//	// An array to store and handle all the particles
//	Particle* particles[MAX_ACTIVE_PARTICLES] = { nullptr };
//
//};
//
//#endif // !__MODULEPARTICLES_H__