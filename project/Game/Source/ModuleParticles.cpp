//#include "ModuleParticles.h"
//
//#include "App.h"
//
//#include "Textures.h"
//#include "Render.h"
//#include "Physics.h"
//#include "LOG.h"
//
//#include "SDL/include/SDL_timer.h"
//
//ModuleParticles::ModuleParticles(App* app, bool start_enabled) : Module(app, start_enabled)
//{
//	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
//		particles[i] = nullptr;
//}
//
//ModuleParticles::~ModuleParticles()
//{
//
//}
//
//bool ModuleParticles::Start()
//{
//	LOG("Loading particles");
//
//	texture = app->tex->Load();
//
//	// Explosion particle
//	
//
//	return true;
//}
//
//bool ModuleParticles::PreUpdate()
//{
//	// Remove all particles scheduled for deletion
//	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
//	{
//		if (particles[i] != nullptr && particles[i]->pendingToDelete)
//		{
//			delete particles[i];
//			particles[i] = nullptr;
//		}
//	}
//
//	return true;
//}
//bool ModuleParticles::CleanUp()
//{
//	LOG("Unloading particles");
//
//	// Delete all remaining active particles on application exit 
//	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
//	{
//		if (particles[i] != nullptr)
//		{
//			delete particles[i];
//			particles[i] = nullptr;
//		}
//	}
//
//	return true;
//}
//
//void ModuleParticles::OnCollision(Collider* c1, Collider* c2)
//{
//	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
//	{
//		// Always destroy particles that collide
//		if (particles[i] != nullptr && particles[i]->collider == c1)
//		{
//			//spawn a explosion particle if the collider is not the screenboundingbox
//			if (c2->type != Collider::Type::SCREENBOUNDINGBOX)
//			{
//				AddParticle(explosion, particles[i]->position.x, particles[i]->position.y, Collider::Type::PARTICLE);
//			}
//
//			delete particles[i];
//			particles[i] = nullptr;
//			break;
//		}
//	}
//	//for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
//	//{
//	//	// Always destroy particles that collide
//	//	if (particles[i] != nullptr && particles[i]->collider == c1 && particles[i]->collider->type != Collider::Type::SCREENBOUNDINGBOX)
//	//	{
//	//		AddParticle(explosion, particles[i]->position.x, particles[i]->position.y);
//
//	//		//delete particles[i];
//	//		//particles[i] = nullptr;
//
// //			particles[i]->pendingToDelete = true;
//	//		particles[i]->collider->pendingToDelete = true;
//	//		break;
//	//	}
//	//}
//}
//
// bool ModuleParticles::Update()
//{
//	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
//	{
//		Particle* particle = particles[i];
//
//		if (particle == nullptr)	continue;
//
//		// Call particle Update. If it has reached its lifetime, destroy it
//		if (particle->Update() == false)
//		{
//			particles[i]->SetToDelete();
//		}
//		else if (particle->position.y < App->render->camera.y)
//		{
//			particles[i]->SetToDelete();
//		}
//	}
//
//	return true;
//}
//
//bool ModuleParticles::PostUpdate()
//{
//	//Iterating all particle array and drawing any active particles
//	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
//	{
//		Particle* particle = particles[i];
//
//		if (particle != nullptr && particle->isAlive)
//		{
//			app->render->Blit(texture, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));
//		}
//	}
//
//	return true;
//}
//
//Particle* ModuleParticles::AddParticle(const Particle& particle, int x, int y, ColliderType colliderType, uint delay)
//{
//	Particle* newParticle = nullptr;
//
//	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
//	{
//		//Finding an empty slot for a new particle
//		if (particles[i] == nullptr)
//		{
//			newParticle = new Particle(particle);
//			newParticle->frameCount = -(int)delay;			// We start the frameCount as the negative delay
//			newParticle->position.x = x;						// so when frameCount reaches 0 the particle will be activated
//			newParticle->position.y = y;
//
//			//Adding the particle's collider
//			//if (colliderType != Collider::Type::NONE)
//			newParticle->collider = App->collisions->AddCollider(newParticle->anim.GetCurrentFrame(), colliderType, this);
//
//			if (newParticle->collider == nullptr)
//			{
//				delete newParticle;
//				return nullptr;
//			}
//
//			particles[i] = newParticle;
//			break;
//		}
//	}
//
//	return newParticle;
//}