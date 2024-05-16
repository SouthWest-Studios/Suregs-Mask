#include "Item_Nota.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene_testing.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Window.h"

Item_Nota::Item_Nota(EntityType type, int id, int ataque, int durabilidad, int magia, float peso)
	: type(type), ataque(ataque), durabilidad(durabilidad), magia(magia), peso(peso), Entity(EntityType::ITEM_NOTA)
{
	name = ("item_nota");
}

Item_Nota::~Item_Nota() {}

bool Item_Nota::Awake() {

	
	 

	return true;
}

bool Item_Nota::Start() {
	
	//initilize textures
	/*position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();*/

	texture = app->tex->Load(config.attribute("texturePath").as_string());
	CloseTexture = app->tex->Load(config.attribute("closeTexturePath").as_string());
	switch (id)
	{
	case 0:
		description = config.attribute("texto1").as_string();
		titulo = config.attribute("titulo1").as_string();
		break;
	case 1:
		description = config.attribute("texto2").as_string();
		titulo = config.attribute("titulo2").as_string();
		break;
	case 2:
		description = config.attribute("texto3").as_string();
		titulo = config.attribute("titulo3").as_string();
		break;
	case 3:
		description = config.attribute("texto4").as_string();
		titulo = config.attribute("titulo4").as_string();
		break;
	case 4:
		description = config.attribute("texto5").as_string();
		titulo = config.attribute("titulo5").as_string();
		break;
	case 5:
		description = config.attribute("texto6").as_string();
		titulo = config.attribute("titulo6").as_string();
		break;
	case 6:
		description = config.attribute("texto7").as_string();
		titulo = config.attribute("titulo7").as_string();
		break;
	case 7:
		description = config.attribute("texto8").as_string();
		titulo = config.attribute("titulo8").as_string();
		break;
	case 8:
		description = config.attribute("texto9").as_string();
		titulo = config.attribute("titulo9").as_string();
		break;
	case 9:
		description = config.attribute("texto10").as_string();
		titulo = config.attribute("titulo10").as_string();
		break;
	default:
		break;
	}
	/*CloseTexture = app->tex->Load("Assets/Textures/Entidades/Items/textura_NoteCloseUp.png");*/
	/*texture = app->tex->Load("Assets/Textures/Entidades/Items/item_Garra.png");*/
	// L07 DONE 4: Add a physics to an item - initialize the physics body
	app->tex->GetSize(texture, texW, texH);
	pbody = app->physics->CreateCircle(position.x, position.y, 11, bodyType::STATIC);
	pbody->ctype = ColliderType::NOTA;
	pbody->listener = this;
	pbody->body->GetFixtureList()->SetSensor(true);


	return true;
}

bool Item_Nota::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 11;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 11;

	
	
	return true;
}

bool Item_Nota::PostUpdate()
{
	uint windowWidth, windowHeight;
	app->win->GetWindowSize(windowWidth, windowHeight);

	app->render->DrawTexture(texture, position.x, position.y, 0.5f);
	return true;
}

bool Item_Nota::CleanUp()
{
	app->physics->GetWorld()->DestroyBody(pbody->body);
	app->tex->UnLoad(texture);
	return true;
}


