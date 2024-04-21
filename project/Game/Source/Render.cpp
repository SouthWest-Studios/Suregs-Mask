#include "App.h"
#include "Window.h"
#include "Render.h"
#include "Menu.h"
#include "Defs.h"
#include "Log.h"

#define VSYNC true

Render::Render(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
Render::~Render()
{}

// Called before render is available
bool Render::Awake(pugi::xml_node config)
{
	LOG("Create SDL rendering context");
	bool ret = true;

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	// L04: DONE 6: Load the VSYNC status from config.xml and adapt the code to set it on / off
	if (app->menu->vsyncActive == true) {
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}
	else {
		LOG("vsync OFF");
	}
	 
	renderer = SDL_CreateRenderer(app->win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = app->win->screenSurface->w;
		camera.h = app->win->screenSurface->h;
		camera.x = 0;
		camera.y = 0;
	}

	//initialise the SDL_ttf library
	TTF_Init();

	//load a font into memory
	primaryFont = TTF_OpenFont("Assets/Fuentes/arial.ttf", 25);
	consoleFont = TTF_OpenFont("Assets/Fuentes/UbuntuMono-R.ttf", 25);
	buttonFont = TTF_OpenFont("Assets/Fuentes/MedievalSharp-Bold.ttf", 25);

	return ret;
}

// Called before the first frame
bool Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

// Called each loop iteration
bool Render::PreUpdate()
{
	//Ejemplo a�adir sprite en los Start():	app->render->objectsToDraw.push_back({ textura, posicion X, posicion Y, punto de anclaje en Y = (posY + num), ancho, largo});

	for (DrawableObject& obj : objectsToDraw)
	{
		if (obj.texture == app->entityManager->GetPlayer()->texture)
		{
			obj.x = app->entityManager->GetPlayer()->position.x - 75;
			obj.y = app->entityManager->GetPlayer()->position.y - 100;
			obj.anchorY = app->entityManager->GetPlayer()->position.y + 218; //Seg�n el sprite, a�adir el valor que corresponda -> !0
			if (app->entityManager->GetPlayer()->currentAnimation != nullptr) {
				obj.currentFrame = app->entityManager->GetPlayer()->currentAnimation->GetCurrentFrame();
			}
			obj.isFacingLeft = app->entityManager->GetPlayer()->isFacingLeft;
			break;
		}
	}

	std::sort(objectsToDraw.begin(), objectsToDraw.end(),
		[](const DrawableObject& a, const DrawableObject& b)
		{
			return a.anchorY < b.anchorY;
		});
	SDL_RenderClear(renderer);
	return true;
}

bool Render::Update(float dt)
{
	return true;
}

bool Render::PostUpdate() {
	for (const DrawableObject& obj : objectsToDraw)
	{
		// Verifica si la posici�n del objeto est?dentro de los l�mites de la c�mara
		/*if (obj.x + obj.width >= app->render->camera.x && obj.x <= app->render->camera.x + app->render->camera.w &&
			obj.y + obj.height >= app->render->camera.y && obj.y <= app->render->camera.y + app->render->camera.h){*/
		if (obj.isFacingLeft)	{
			DrawTexture(obj.texture, obj.x, obj.y, 0.5f, SDL_FLIP_NONE, &obj.currentFrame);
		}
		else if(!obj.isFacingLeft)	{
			DrawTexture(obj.texture, obj.x, obj.y, 0.5f, SDL_FLIP_HORIZONTAL, &obj.currentFrame);
		}
				//app->render->DrawTexture(obj.texture, obj.x, obj.y, 0.5f, SDL_FLIP_NONE, &obj.currentFrame);
				//app->render->DrawTexture(texture, position.x - 75, position.y - 100, 0.5f, SDL_FLIP_NONE, &rect);
		//}
		if (obj.texture != app->entityManager->GetPlayer()->texture) {
			app->render->DrawTexture(obj.texture, obj.x, obj.y);
		}
		
	}
	return true;
}

bool Render::PostLateUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

// Blit to screen


bool Render::DrawTexture(SDL_Texture* texture, int x, int y, SDL_RendererFlip flip, const SDL_Rect* section, float speed, double angle, int pivotX, int pivotY, float scale)
{
	bool ret = true;
	if (scale == 0) {
		scale = app->win->GetScale();
	}
	
	


	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * app->win->GetScale();
	rect.y = (int)(camera.y * speed) + y * app->win->GetScale();

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}



	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}



bool Render::DrawTexture(SDL_Texture* texture, int x, int y, double scale, SDL_RendererFlip flip, const SDL_Rect* section, float speed, double angle, int pivotX, int pivotY) const
{
	bool ret = true;
	//uint scale = app->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * app->win->GetScale();
	rect.y = (int)(camera.y * speed) + y * app->win->GetScale();

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}


bool Render::DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x * scale + camera.x) + (radius * cos(i * factor));
		points[i].y = (int)(y * scale + camera.y) + (radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawText(const char* text, int posx, int posy, int w, int h, TTF_Font* font) {

	if (font == NULL) {
		font = primaryFont;
	}
	SDL_Color color = { 255, 255, 255 };
	SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = { posx, posy, w, h };

	SDL_RenderCopy(renderer, texture, NULL, &dstrect);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);

	return true;
}

// L14: TODO 6: Implement a method to load the state
// for now load camera's x and y
bool Render::LoadState(pugi::xml_node node) {

	camera.x = node.child("camera").attribute("x").as_int();
	camera.y = node.child("camera").attribute("y").as_int();

	return true;
}

// L14: TODO 8: Create a method to save the state of the renderer
// using append_child and append_attribute
bool Render::SaveState(pugi::xml_node node) {
	
	pugi::xml_node camNode = node.append_child("camera");
	camNode.append_attribute("x").set_value(camera.x);
	camNode.append_attribute("y").set_value(camera.y);

	return true;
}