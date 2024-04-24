#ifndef __RENDER_H__
#define __RENDER_H__

#include "Module.h"

#include "Point.h"

#include "SDL/include/SDL.h"
#include "SDL_ttf/include/SDL_ttf.h"

//struct DrawableObject
//{
//	SDL_Texture* texture;
//	int x, y;
//	int anchorY;
//	int width, height;
//	SDL_Rect currentFrame;
//	bool isFacingLeft;
//};

class Render : public Module
{
public:

	Render(App* app, bool start_enabled = true);

	// Destructor
	virtual ~Render();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool PostLateUpdate();

	// Called before quitting
	bool CleanUp();

	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();

	// Drawing
	bool DrawTexture(SDL_Texture* texture, int x, int y, SDL_RendererFlip flip = SDL_FLIP_NONE, const SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX, float scale = 0);
	bool DrawTexture(SDL_Texture* texture, int x, int y, double scale, SDL_RendererFlip flip = SDL_FLIP_NONE, const SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX) const;
	bool DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool useCamera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawText(const char* text, int posX, int posY, int w, int h, TTF_Font* font = NULL, int r = 255, int g = 255, int b = 255, bool adjustToText = true);

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	//
	bool LoadState(pugi::xml_node node);

	//
	bool SaveState(pugi::xml_node node);

public:

	SDL_Renderer* renderer;
	SDL_Rect camera;
	SDL_Rect viewport;
	SDL_Color background;
	TTF_Font* consoleFont;
	TTF_Font* primaryFont;
	TTF_Font* secondaryFont;
	TTF_Font* buttonFont;

	bool vsync = false;

	//std::vector<DrawableObject> objectsToDraw;

};

#endif // __RENDER_H__