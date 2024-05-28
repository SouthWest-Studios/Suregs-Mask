
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"
#include "Scene_Testing.h"
#include "Optick/include/optick.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "DialogTriggerEntity.h"
#include "Item_Garra.h"
#include "Item_Nota.h"
#include "NPC_Vendedor.h"
#include "NPC_Pescador.h"
#include "NPC_Abuelo.h"
#include "NPC_Enamorados.h"
#include "NPC_Abuelito.h"
#include "NPC_Abuelitas.h"
#include "NPC_Guardias.h"
#include "NPC_Bruja.h"
#include "InventoryManager.h"
#include "NPC_Bully1.h"
#include "NPC_Bully2.h"
#include "NPC_Herrera.h"
#include "NPC_MujerEnamorada.h"
#include "NPC_MujerPreocupada.h"
#include "Item_Diamante.h"
#include "Item_Ojo.h"
#include "Item_Hueso.h"
#include "Enemy_Ols.h"
#include "Enemy_Ols_Variation.h"
#include "Enemy_Muur.h"
#include "Enemy_Boorok.h"
#include "Enemy_Osiris_Variation.h"
#include "TPEntity.h"
#include "PuzzleButtonEntity.h"
#include "Window.h"
#include "Boss_Inuit.h"
#include "Boss_Igory.h"
#include "Boss_Musri.h"
#include "Enemy_Khurt.h"
#include "Enemy_Khurt_Variation.h"
#include "Cofre.h"
#include "Dialog.h"
#include "Boss_Surma.h"
#include "Elevator.h"
#include "ModuleFadeToBlack.h"
#include "Enemy_Spawner.h"

Map::Map(App* app, bool start_enabled) : Module(app, start_enabled), mapLoaded(false)
{
	name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node config)
{
	LOG("Loading Map Parser");
	bool ret = true;
	
	

	return ret;
}

bool Map::Start() {

	OPTICK_EVENT();
	//Calls the functon to load the map, make sure that the filename is assigned
	SString mapPath = path;
	mapPath += name;
	Load(mapPath);


	pugi::xml_document configFile;
	pugi::xml_node configNode;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	configNode = configFile.child("config").child("map");
	pathTextures = configNode.child("pathfindingTexture").attribute("pathtexture").as_string();


	//Initialize pathfinding 
	pathfinding = new PathFinding();

	//Initialize the navigation map
    uchar* navigationMap = NULL;
    CreateNavigationMap(mapData.width, mapData.height, &navigationMap);
    pathfinding->SetNavigationMap((uint)mapData.width, (uint)mapData.height, navigationMap);
	pathfinding->mouseTileTex = app->tex->Load(pathTextures.GetString());
    RELEASE_ARRAY(navigationMap);

	app->win->GetWindowSize(windowW, windowH);


	return true;
}

bool Map::Update(float dt)
{

	//Mirar el orden de los botones

	if (!recompensaPuzzle && puzzleButtonEntities.size() > 0) {

		bool falta = false;
		bool resetBotones = false;
		for (int i = 0; i < puzzleButtonEntities.size(); i++) {

			if (puzzleButtonEntities.at(i)->pressed && falta) {
				resetBotones = true;
				break;
			}

			if (!puzzleButtonEntities.at(i)->pressed) {
				falta = true;
			}
		}
		if (resetBotones) {
			for (int i = 0; i < puzzleButtonEntities.size(); i++) {
				puzzleButtonEntities.at(i)->pressed = false;
			}
		}
		if (!falta) {
			app->inventoryManager->monedasObtenidas += 100;
			recompensaPuzzle = true;
		}
	}


	return true;
}

bool Map::PostUpdate()
{

	OPTICK_CATEGORY("Map", Optick::Category::GameLogic)
		bool ret = true;

	if (mapLoaded == false)
		return false;

	ListItem<MapLayer*>* mapLayer;
	mapLayer = mapData.layers.start;

	// L06: DONE 5: Prepare the loop to draw all tiles in a layer + DrawTexture()

	// iterates the layers in the map
	while (mapLayer != NULL) {
		//Check if the property Draw exist get the value, if it's true draw the lawyer
		if (mapLayer->data->properties.GetProperty("Draw") != NULL && mapLayer->data->properties.GetProperty("Draw")->value == "true") {

			//iPoint playerPos = app->entityManager->GetPlayer()->position;
			iPoint playerPos = iPoint(-app->render->camera.x + (windowW/2), -app->render->camera.y + (windowH/2));
			int xToTiledLeft = MAX((playerPos.x / 32) - TILES_TO_LOAD, 0);
			int xToTiledRight = MIN((playerPos.x / 32) + TILES_TO_LOAD, mapLayer->data->width);

			int yToTiledTop = MAX((playerPos.y / 32) - TILES_TO_LOAD, 0);
			int yToTiledDown = MIN((playerPos.y / 32) + TILES_TO_LOAD, mapLayer->data->height);




			//iterate all tiles in a layer
			for (int x = xToTiledLeft; x < xToTiledRight; x++)
			{
				for (int y = yToTiledTop; y < yToTiledDown; y++)
				{
					//Get the gid from tile
					unsigned int gid = mapLayer->data->Get(x, y);
					TileSet* tileset = GetTilesetFromTileId(gid);
					SDL_Rect r = tileset->GetRect(gid);
					iPoint pos = MapToWorld(x, y);
					int bits = 0;
					SDL_RendererFlip flip = SDL_FLIP_NONE;
					int angle = 0;

					if (gid >= 100000) {
						uint tiledID = static_cast<uint>(gid & ~0xE0000000);
						bits = gid >> 29;
						tileset = GetTilesetFromTileId(tiledID);
						r = tileset->GetRect(tiledID);

					}
					//1 = hoz_flip -> True || 1 = vert_flip -> True  || 0 = anti-diag flip -> False
					switch (bits) {
					case 0b101: flip = SDL_FLIP_NONE;           angle = 90;         break;
					case 0b110: flip = SDL_FLIP_NONE;           angle += 180;       break;
					case 0b011: flip = SDL_FLIP_NONE;           angle += 270;       break;
					case 0b100: flip = SDL_FLIP_HORIZONTAL;     angle = 0;          break;
					case 0b111: flip = SDL_FLIP_HORIZONTAL;     angle += 90;        break;
					case 0b010: flip = SDL_FLIP_HORIZONTAL;     angle += 180;       break;
					case 0b001: flip = SDL_FLIP_HORIZONTAL;     angle += 270;       break;
					}



					//SDL_SetTextureColorMod(tileset->texture, 255 * mapLayer->data->opacity, 255 * mapLayer->data->opacity, 255 * mapLayerItem->data->opacity);


					app->render->DrawTexture(tileset->texture,
						pos.x,
						pos.y, flip,
						&r, 1, angle);

				}
			}
		}
		mapLayer = mapLayer->next;
	}

	return ret;
}

bool Map::PrintMapFront()
{
	OPTICK_CATEGORY("Map", Optick::Category::GameLogic)
		bool ret = true;

	if (mapLoaded == false)
		return false;

	ListItem<MapLayer*>* mapLayer;
	mapLayer = mapData.layers.start;

	// L06: DONE 5: Prepare the loop to draw all tiles in a layer + DrawTexture()

	// iterates the layers in the map
	while (mapLayer != NULL) {
		//Check if the property Draw exist get the value, if it's true draw the lawyer
		if (mapLayer->data->properties.GetProperty("DrawFront") != NULL && mapLayer->data->properties.GetProperty("DrawFront")->value == "true") {

			//iPoint playerPos = app->entityManager->GetPlayer()->position;
			iPoint playerPos = iPoint(-app->render->camera.x + (windowW / 2), -app->render->camera.y + (windowH / 2));
			int xToTiledLeft = MAX((playerPos.x / 32) - TILES_TO_LOAD, 0);
			int xToTiledRight = MIN((playerPos.x / 32) + TILES_TO_LOAD, mapLayer->data->width);

			int yToTiledTop = MAX((playerPos.y / 32) - TILES_TO_LOAD, 0);
			int yToTiledDown = MIN((playerPos.y / 32) + TILES_TO_LOAD, mapLayer->data->height);




			//iterate all tiles in a layer
			for (int x = xToTiledLeft; x < xToTiledRight; x++)
			{
				for (int y = yToTiledTop; y < yToTiledDown; y++)
				{
					//Get the gid from tile
					unsigned int gid = mapLayer->data->Get(x, y);
					TileSet* tileset = GetTilesetFromTileId(gid);
					SDL_Rect r = tileset->GetRect(gid);
					iPoint pos = MapToWorld(x, y);
					int bits = 0;
					SDL_RendererFlip flip = SDL_FLIP_NONE;
					int angle = 0;

					if (gid >= 100000) {
						uint tiledID = static_cast<uint>(gid & ~0xE0000000);
						bits = gid >> 29;
						tileset = GetTilesetFromTileId(tiledID);
						r = tileset->GetRect(tiledID);

					}
					//1 = hoz_flip -> True || 1 = vert_flip -> True  || 0 = anti-diag flip -> False
					switch (bits) {
					case 0b101: flip = SDL_FLIP_NONE;           angle = 90;         break;
					case 0b110: flip = SDL_FLIP_NONE;           angle += 180;       break;
					case 0b011: flip = SDL_FLIP_NONE;           angle += 270;       break;
					case 0b100: flip = SDL_FLIP_HORIZONTAL;     angle = 0;          break;
					case 0b111: flip = SDL_FLIP_HORIZONTAL;     angle += 90;        break;
					case 0b010: flip = SDL_FLIP_HORIZONTAL;     angle += 180;       break;
					case 0b001: flip = SDL_FLIP_HORIZONTAL;     angle += 270;       break;
					}



					//SDL_SetTextureColorMod(tileset->texture, 255 * mapLayer->data->opacity, 255 * mapLayer->data->opacity, 255 * mapLayerItem->data->opacity);


					app->render->DrawTexture(tileset->texture,
						pos.x,
						pos.y, flip,
						&r, 1, angle);

				}
			}
		}
		mapLayer = mapLayer->next;
	}

	return ret;
}

// L08: DONE 2: Implement function to the Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int gid) const
{
	TileSet* set = NULL;

	ListItem<TileSet*>* tileSet;
	tileSet = mapData.tilesets.start;
	while (tileSet != NULL) {
		set = tileSet->data;
		if (gid >= tileSet->data->firstgid && gid < (tileSet->data->firstgid + tileSet->data->tilecount)) break;
		tileSet = tileSet->next;
	}

	return set;
}

// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	//Clean up pathfing class
	pathfinding->CleanUp();

	// L05: DONE 2: Make sure you clean up any memory allocated from tilesets/map
	ListItem<TileSet*>* tileset;
	tileset = mapData.tilesets.start;

	while (tileset != NULL) {
		app->tex->UnLoad(tileset->data->texture);
		RELEASE(tileset->data);
		tileset = tileset->next;
	}

	mapData.tilesets.Clear();

	// L06: DONE 2: clean up all layer data
	ListItem<MapLayer*>* layerItem;
	layerItem = mapData.layers.start;

	while (layerItem != NULL) {
		RELEASE(layerItem->data->tiles);
		RELEASE(layerItem->data);
		layerItem = layerItem->next;
	}
	mapData.layers.Clear();


	// Remove all groupobject
	ListItem<MapObjects*>* objectsItem;
	objectsItem = mapData.mapObjects.start;

	while (objectsItem != NULL)
	{

		ListItem<MapObject*>* objectItem;
		objectItem = objectsItem->data->objects.start;
		while (objectItem != NULL)
		{
			RELEASE(objectItem->data);
			delete objectItem->data;

			objectItem = objectItem->next;
		}

		RELEASE(objectsItem->data);
		delete objectsItem->data;




		objectsItem = objectsItem->next;
	}
	mapData.mapObjects.Clear();
	
	// El clean Up de la camara, no borrar
	smallRoomsList.Clear();
	largeRoomsList.Clear();
	LRoomsList.Clear();
	dlRoomsList.Clear();
	drRoomsList.Clear();
	ulRoomsList.Clear();
	urRoomsList.Clear();
	dRoomsList.Clear();
	uRoomsList.Clear();
	lRoomsList.Clear();
	rRoomsList.Clear();

	// Remove all colisions
	ListItem<PhysBody*>* collision;
	collision = collisionsList.start;
	while (collision != NULL) {
		collision->data->body->SetActive(false);
		//collision->data->body->SetTransform(b2Vec2(-100000, -100000), 0);
		app->physics->GetWorld()->DestroyBody(collision->data->body);
		RELEASE(collision->data);
		delete collision->data;
		
		collision = collision->next;
	}
	collisionsList.Clear();


	pathfinding->CleanUp();

	puzzleButtonEntities.clear(); //En teoria las entidades se borrar al desactivar el entity manager.
	recompensaPuzzle = false;

	return true;
}

// Load new map
bool Map::Load(SString mapFileName)
{
	bool ret = true;

	// L05: DONE 3: Implement LoadMap to load the map properties
	// retrieve the paremeters of the <map> node and save it into map data
	pugi::xml_document mapFileXML;
	pugi::xml_parse_result result = mapFileXML.load_file(mapFileName.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", mapFileName, result.description());
		ret = false;
	}


	if (ret == true)
	{
		ret = LoadMap(mapFileXML);
	}

	if (ret == true)
	{
		ret = LoadTileSet(mapFileXML);
	}

	if (ret == true)
	{
		ret = LoadAllLayers(mapFileXML.child("map"));
	}
	if (ret == true)
	{
		ret = LoadAllObjectGroups(mapFileXML.child("map"));
	}


	LoadObjects();
	LoadCollisions("Collisions");
	LoadEntities("Entities");
	LoadTPEntities("TPLayers");
	LoadPuzzleEntities("PuzzleLayer");




	if (ret == true)
	{
		LOG("Successfully parsed map XML file :%s", mapFileName.GetString());
		LOG("width : %d height : %d", mapData.width, mapData.height);
		LOG("tile_width : %d tile_height : %d", mapData.tilewidth, mapData.tileheight);

		LOG("Tilesets----");

		ListItem<TileSet*>* tileset;
		tileset = mapData.tilesets.start;
		while (tileset != NULL) {
			//iterate the tilesets
			LOG("name : %s firstgid : %d", tileset->data->name.GetString(), tileset->data->firstgid);
			LOG("tile width : %d tile height : %d", tileset->data->tilewidth, tileset->data->tileheight);
			LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
			tileset = tileset->next;
		}

		LOG("Layers----");

		ListItem<MapLayer*>* mapLayer;
		mapLayer = mapData.layers.start;

		while (mapLayer != NULL) {
			LOG("id : %d name : %s", mapLayer->data->id, mapLayer->data->name.GetString());
			LOG("Layer width : %d Layer height : %d", mapLayer->data->width, mapLayer->data->height);
			mapLayer = mapLayer->next;
		}
	}

	ListItem<MapLayer*>* mapLayerItem;
	mapLayerItem = mapData.layers.start;
	navigationLayer = mapLayerItem->data;

	//Search the layer in the map that contains information for navigation
	while (mapLayerItem != NULL) {
		if (mapLayerItem->data->properties.GetProperty("Navigation") != NULL && mapLayerItem->data->properties.GetProperty("Navigation")->value == "true") {
			navigationLayer = mapLayerItem->data;
			break;
		}
		mapLayerItem = mapLayerItem->next;
	}

	//Resets the map
	if (mapFileXML) mapFileXML.reset();

	mapLoaded = ret;
	return ret;
}

// L06: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	// L09: DONE 3: Get the screen coordinates of tile positions for isometric maps 
	if (mapData.orientation == MapOrientation::ORTOGRAPHIC) {
		ret.x = x * mapData.tilewidth;
		ret.y = y * mapData.tileheight;
	}

	if (mapData.orientation == MapOrientation::ISOMETRIC) {
		ret.x = x * mapData.tilewidth / 2 - y * mapData.tilewidth / 2;
		ret.y = x * mapData.tileheight / 2 + y * mapData.tileheight / 2;
	}

	return ret;
}

// L08: DONE 6: Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
	{
		Properties::Property* p = new Properties::Property();
		p->name = propertieNode.attribute("name").as_string();
		p->value = propertieNode.attribute("value").as_string();

		properties.propertyList.Add(p);
	}

	return ret;
}

// L08: DONE 7: Implement a method to get the value of a custom property
Properties::Property* Properties::GetProperty(const char* name)
{
	ListItem<Property*>* property = propertyList.start;
	Property* p = NULL;

	while (property)
	{
		if (property->data->name == name) {
			p = property->data;
			break;
		}
		property = property->next;
	}

	return p;
}

// L09: DONE 5: Add method WorldToMap to obtain  map coordinates from screen coordinates 
iPoint Map::WorldToMap(int x, int y) {

	iPoint ret(0, 0);

	if (mapData.orientation == MapOrientation::ORTOGRAPHIC) {
		ret.x = x / mapData.tilewidth;
		ret.y = y / mapData.tileheight;
	}

	if (mapData.orientation == MapOrientation::ISOMETRIC) {
		float half_width = mapData.tilewidth / 2;
		float half_height = mapData.tileheight / 2;
		ret.x = int((x / half_width + y / half_height) / 2);
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}

	return ret;
}

int Map::GetTileWidth() {
	return mapData.tilewidth;
}

int Map::GetTileHeight() {
	return mapData.tileheight;
}

bool Map::LoadMap(pugi::xml_node mapFile)
{
	bool ret = false;
	pugi::xml_node map = mapFile.child("map");
	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else {
		ret = true;
		//Fill mapData variable
		mapData.width = map.attribute("width").as_int();
		mapData.height = map.attribute("height").as_int();
		mapData.tilewidth = map.attribute("tilewidth").as_int();
		mapData.tileheight = map.attribute("tileheight").as_int();

		// L09: DONE 2: Define a property to store the MapType and Load it from the map
		SString orientationStr = map.attribute("orientation").as_string();
		if (orientationStr == "orthogonal") {
			mapData.orientation = MapOrientation::ORTOGRAPHIC;
		}
		else if (orientationStr == "isometric") {
			mapData.orientation = MapOrientation::ISOMETRIC;
		}
		else {
			LOG("Map orientation not found");
			//mapData.orientation = MapOrientation::ORTOGRAPHIC;
			ret = false;
		}

	}
	return ret;
}

bool Map::LoadTileSet(pugi::xml_node mapFile)
{
	bool ret = false;
	for (pugi::xml_node tilesetNode = mapFile.child("map").child("tileset"); tilesetNode != NULL; tilesetNode = tilesetNode.next_sibling("tileset")) {

		TileSet* tileset = new TileSet();

		//Load Tileset attributes
		tileset->name = tilesetNode.attribute("name").as_string();
		tileset->firstgid = tilesetNode.attribute("firstgid").as_int();
		tileset->margin = tilesetNode.attribute("margin").as_int();
		tileset->spacing = tilesetNode.attribute("spacing").as_int();
		tileset->tilewidth = tilesetNode.attribute("tilewidth").as_int();
		tileset->tileheight = tilesetNode.attribute("tileheight").as_int();
		tileset->columns = tilesetNode.attribute("columns").as_int();
		tileset->tilecount = tilesetNode.attribute("tilecount").as_int();

		//Load Tileset image
		SString mapTex = "Assets/Textures/Mapas/";
		mapTex += tilesetNode.child("image").attribute("source").as_string();
		tileset->texture = app->tex->Load(mapTex.GetString());

		mapData.tilesets.Add(tileset);
		ret = true;
	}
	return ret;
}

bool Map::LoadLayer(pugi::xml_node& layerNode, MapLayer* mapLayer)
{
	bool ret = true;


	mapLayer->id = layerNode.attribute("id").as_int();
	mapLayer->name = layerNode.attribute("name").as_string();
	mapLayer->width = layerNode.attribute("width").as_int();
	mapLayer->height = layerNode.attribute("height").as_int();

	//L08: DONE 6 Call Load Layer Properties
	LoadProperties(layerNode, mapLayer->properties);

	//Reserve the memory for the data 
	mapLayer->tiles = new uint[mapLayer->width * mapLayer->height];
	memset(mapLayer->tiles, 0, mapLayer->width * mapLayer->height);

	//Iterate over all the tiles and assign the values in the data array
	int i = 0;
	for (pugi::xml_node tileNode = layerNode.child("data").child("tile"); tileNode != NULL; tileNode = tileNode.next_sibling("tile")) {
		mapLayer->tiles[i] = tileNode.attribute("gid").as_uint();
		i++;
	}
	return ret;
}

bool Map::LoadAllLayers(pugi::xml_node mapNode)
{
	bool ret = true;
	for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode != NULL; layerNode = layerNode.next_sibling("layer")) {


		MapLayer* mapLayer = new MapLayer();
		ret = LoadLayer(layerNode, mapLayer);

		//add the layer to the map
		mapData.layers.Add(mapLayer);
	}
	return ret;
}

bool Map::LoadObject(pugi::xml_node& node, MapObjects* mapObjects)
{
	bool ret = true;

	//Load the attributes
	mapObjects->id = node.attribute("id").as_int();
	mapObjects->name = node.attribute("name").as_string();
	mapObjects->width = node.attribute("width").as_int();
	mapObjects->width = node.attribute("width").as_int();
	mapObjects->x = node.attribute("x").as_int();
	mapObjects->y = node.attribute("y").as_int();

	LoadProperties(node, mapObjects->properties);

	//Reserve the memory for the data 
	//mapObjects->objects = new MapObject[mapObjects->width * mapObjects->height];
	//memset(mapObjects->objects, 0, mapObjects->width * mapObjects->height);

	//Iterate over all the objects and assign the values
	pugi::xml_node object;
	int i = 0;
	for (object = node.child("object"); object && ret; object = object.next_sibling("object"))
	{

		MapObject* mapObject = new MapObject;
		mapObject->id = object.attribute("id").as_uint();
		mapObject->x = object.attribute("x").as_uint();
		mapObject->y = object.attribute("y").as_uint();
		mapObject->width = object.attribute("width").as_uint();
		mapObject->height = object.attribute("height").as_uint();

		LoadProperties(object, mapObject->properties);

		

		if (object.child("polygon")) {


			std::vector<int> points = GetObjectGroupPoints(object.child("polygon").attribute("points").as_string());
			mapObject->points = points;
			mapObject->type = POLIGONO;
		}
		else {
			mapObject->type = RECTANGULO;
		}

		/*mapObjects->objects.Add(new MapObject{
			object.attribute("id").as_uint(),
			object.attribute("x").as_uint(),
			object.attribute("y").as_uint(),
			object.attribute("width").as_uint(),
			object.attribute("height").as_uint(),
			List<uint>()

			});*/
		mapObjects->objects.Add(mapObject);
		i++;
	}

	return ret;
}

bool Map::LoadAllObjectGroups(pugi::xml_node mapNode)
{
	bool ret = true;

	for (pugi::xml_node objectNode = mapNode.child("objectgroup"); objectNode && ret; objectNode = objectNode.next_sibling("objectgroup"))
	{
		//Load the layer
		MapObjects* mapObjects = new MapObjects();
		ret = LoadObject(objectNode, mapObjects);

		//add the layer to the map
		mapData.mapObjects.Add(mapObjects);
	}

	return ret;
}

bool Map::LoadCollisions(std::string layerName)
{
	ListItem<MapLayer*>* mapLayerItem;
	mapLayerItem = mapData.layers.start;
	bool ret = false;

	while (mapLayerItem != NULL) {

		if (mapLayerItem->data->name.GetString() == layerName) {


			for (int x = 0; x < mapLayerItem->data->width; x++)
			{

				for (int y = 0; y < mapLayerItem->data->height; y++)
				{
					int gid = mapLayerItem->data->Get(x, y);
					TileSet* tileset = GetTilesetFromTileId(gid);
					//SDL_Rect r = tileset->GetTileRect(gid);
					iPoint pos = MapToWorld(x, y);



					/*No borrar, original sistema de colisiones*/
					PhysBody* c1;
					if (gid == tileset->firstgid + 0) {
						c1 = app->physics->CreateRectangle(pos.x + 16, pos.y + 16, 32, 32, STATIC);
						c1->ctype = ColliderType::PLATFORM;
						collisionsList.Add(c1);
						ret = true;
					}


					
					if (gid == tileset->firstgid + 2) {
						c1 = app->physics->CreateRectangleSensor(pos.x + 16, pos.y + 16, 32, 32, STATIC);
						c1->ctype = ColliderType::LAGO;
						collisionsList.Add(c1);
						ret = true;
					}


					//Salida mazmorra/Entrada pueblo
					if (gid == tileset->firstgid + 14) {
						c1 = app->physics->CreateRectangleSensor(pos.x + 16, pos.y + 16, 32, 32, STATIC);
						c1->ctype = ColliderType::SALIDA_MAZMORRA0;
						collisionsList.Add(c1);
						ret = true;
					}

					//Entrada mazmorra
					if (gid == tileset->firstgid + 15) {
						c1 = app->physics->CreateRectangleSensor(pos.x + 16, pos.y + 16, 32, 32, STATIC);
						c1->ctype = ColliderType::ENTRADA_MAZMORRA0;
						collisionsList.Add(c1);
						ret = true;
					}

					//if (gid == tileset->firstgid + 1) {
					//    c1 = app->physics->CreateRectangle(pos.x + 16, pos.y + 2, 32, 4, STATIC);
					//    c1->ctype = ColliderType::PLATFORM_TRASPASS;
					//    traspasedPlatformList.Add(c1);
					//    collisionsList.Add(c1);
					//    ret = true;
					//}
					//if (gid == tileset->firstgid + 6) {
					//    c1 = app->physics->CreateRectangleSensor(pos.x + 16, pos.y + 16, 32, 32, STATIC);
					//    c1->ctype = ColliderType::SPYKES;
					//    collisionsList.Add(c1);
					//    ret = true;
					//}

					////Detectar entrado boss
					//if (gid == tileset->firstgid + 7) {
					//    c1 = app->physics->CreateRectangleSensor(pos.x + 16, pos.y + 16, 32, 32, STATIC);
					//    c1->ctype = ColliderType::ZONA_BOSS;
					//    collisionsList.Add(c1);
					//    ret = true;
					//}


					////sONIDO DE VICTORIA AL FINAL
					//if (gid == tileset->firstgid + 10) {
					//    c1 = app->physics->CreateRectangleSensor(pos.x + 16, pos.y + 16, 32, 32, STATIC);
					//    c1->ctype = ColliderType::VICTORY_COLLISION;
					//    collisionsList.Add(c1);
					//    ret = true;
					//}


					////Fuera del mapa
					//if (gid == tileset->firstgid + 15) {
					//    c1 = app->physics->CreateRectangleSensor(pos.x + 16, pos.y + 16, 32, 32, STATIC);
					//    c1->ctype = ColliderType::DIE_HOLE;
					//    collisionsList.Add(c1);
					//    ret = true;
					//}
				}
			}
		}
		mapLayerItem = mapLayerItem->next;

	}

	return ret;
}

bool Map::LoadTPEntities(std::string layerName)
{
	ListItem<MapLayer*>* mapLayerItem;
	mapLayerItem = mapData.layers.start;
	bool ret = false;

	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	if (parseResult) {
		configNode = configFile.child("config");
	}
	else {
		LOG("Error in Map::LoadEntities(): %s", parseResult.description());
		return false;
	}
	LOG("CARGA TP ENTIDADES");
	while (mapLayerItem != NULL) {

		if (mapLayerItem->data->name.GetString() == layerName) {
			for (int x = 0; x < mapLayerItem->data->width; x++)
			{

				for (int y = 0; y < mapLayerItem->data->height; y++)
				{


					int gid = mapLayerItem->data->Get(x, y);
					TileSet* tileset = GetTilesetFromTileId(gid);
					SDL_Rect r = tileset->GetRect(gid);
					iPoint pos = MapToWorld(x, y);

					
					if (gid != 0) {
						int tpID = gid - tileset->firstgid;
						TPEntity* tp = (TPEntity*)app->entityManager->CreateEntity(EntityType::TP_ENTITY);
						tp->tpID = tpID;
						tp->sceneLevel = app->fadeToBlack->activeScene->GetSceneNumber();
						tp->position = iPoint(pos.x + 16, pos.y + 16);
						//tp->Start();
					}
					
					


					////PLAYER
					//if (gid == tileset->firstgid + 0) {

					//	app->entityManager->SetPlayer((Player*)app->entityManager->CreateEntity(EntityType::PLAYER));
					//	app->entityManager->GetPlayer()->config = configNode.child("entities_data").child("player");
					//	app->entityManager->GetPlayer()->position = iPoint(pos.x + 16, pos.y + 16);
					//	app->entityManager->GetPlayer()->Start();

					//}

					////NPC_VENDEDOR
					//if (gid == tileset->firstgid + 1) {


					//	NPCVendedor* npc = (NPCVendedor*)app->entityManager->CreateEntity(EntityType::NPC_VENDEDOR);
					//	npc->config = configNode.child("entities_data").child("npc_vendedor");
					//	npc->position = iPoint(pos.x + 16, pos.y + 16);
					//	npc->Start();

					//}

					
				}
			}

		}
		mapLayerItem = mapLayerItem->next;

	}
	app->entityManager->LinkTPEntities();



	return ret;
}

bool Map::LoadPuzzleEntities(std::string layerName)
{
	bool ret = true;

	ListItem<MapLayer*>* mapLayerItem;
	mapLayerItem = mapData.layers.start;

	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	if (parseResult) {
		configNode = configFile.child("config");
	}
	else {
		LOG("Error in Map::LoadEPuzzleEntities(): %s", parseResult.description());
		return false;
	}
	LOG("CARGA Puzzle ENTIDADES");
	while (mapLayerItem != NULL) {

		if (mapLayerItem->data->name.GetString() == layerName) {
			for (int x = 0; x < mapLayerItem->data->width; x++)
			{

				for (int y = 0; y < mapLayerItem->data->height; y++)
				{


					int gid = mapLayerItem->data->Get(x, y);
					TileSet* tileset = GetTilesetFromTileId(gid);
					SDL_Rect r = tileset->GetRect(gid);
					iPoint pos = MapToWorld(x, y);


					if (gid != 0) {

						int elementID = gid - tileset->firstgid;

						if (elementID < 15) { //Botones
							PuzzleButtonEntity* button = (PuzzleButtonEntity*)app->entityManager->CreateEntity(EntityType::PUZZLE_BUTTON);
							button->buttonID = elementID;
							button->config = configNode.child("entities_data").child("puzzle_button");
							button->position = iPoint(pos.x + 16, pos.y + 16);
							puzzleButtonEntities.push_back(button);
							button->Start();
							
							
							BubbleSort(&puzzleButtonEntities);

						}
						else if (elementID < 30) { //Palancas
							//button->buttonID = elementID - 15;
						}
						else {

						}
					}
				}
			}

		}
		mapLayerItem = mapLayerItem->next;

	}



	return ret;
}

bool Map::LoadObjects()
{
	ListItem<MapObjects*>* mapObjectsItem;
	mapObjectsItem = mapData.mapObjects.start;
	bool ret = false;

	pugi::xml_parse_result parseResult = dialoguesFile.load_file("dialogs.xml");

	if (parseResult) {
		dialoguesNode = dialoguesFile.child("dialogues");
	}
	else {
		LOG("Error in Map::LoadEntities(): %s", parseResult.description());
		return false;
	}


	while (mapObjectsItem != NULL) {
		for (int i = 0; i < mapObjectsItem->data->objects.Count(); i++) {

			MapObject* object = mapObjectsItem->data->objects[i];
			SString objectName = mapObjectsItem->data->name;
			
			if (object->properties.GetProperty("dialogID") != NULL) {

			
				uint dialogId = atoi(object->properties.GetProperty("dialogID")->value.c_str());
				dialogId += app->inventoryManager->storyLevel - 1;

				Dialog* dialog = new Dialog(dialogId);

				// A�adir el Dialog a la lista de di�logos
				dialogues.Add(dialog);

				//ANTIGUO 
				//pugi::xml_node dialogNode = dialoguesNode.find_child_by_attribute("dialog", "id", object->properties.GetProperty("dialogID")->value.c_str());

				//NUEVO - ALGUNOS NPCS NO VAN?
				pugi::xml_node dialogNode = dialoguesNode.find_child_by_attribute("dialog", "id", std::to_string(dialogId).c_str());
				
				if (!dialogNode) {
					//std::cerr << "No se encontr?ning�n di�logo con el id=" << object->properties.GetProperty("dialogID")->value << std::endl;
					std::cerr << "No se encontr?ning�n di�logo con el id=" << std::to_string(dialogId).c_str() << std::endl;
					//return false;
				}
				//Spawn dialogo con x id
				else {
					//printf("Dialogo con id %d\n", dialogId);
					//printf(" %s\n", object->properties.GetProperty("dialogID")->value.c_str());
					DialogTrigger* dialogTrigger = (DialogTrigger*)app->entityManager->CreateEntity(EntityType::DIALOG_TRIGGER);
					dialogTrigger->parameters = dialogNode;
					dialogTrigger->position = iPoint(object->x + object->width / 2, object->y + object->height / 2);


					//dialogTriger
					dialogTrigger->SetDialogId(dialogId);
					dialogTrigger->Start();
				}


				
				/*c1 = app->physics->CreateRectangleSensor(object->x + object->width / 2, object->y + object->height / 2, object->width, object->height, STATIC);
				c1->ctype = ColliderType::UNKNOWN;*/
			}

			// el objeto es una sala peque�a
			else if (object->properties.GetProperty("Type") != NULL && object->properties.GetProperty("Type")->value == "isSmallRoom")
			{
				smallRoomsList.Add(object);
			}
			// el objeto es una sala grande
			else if (object->properties.GetProperty("Type") != NULL && object->properties.GetProperty("Type")->value == "isLargeRoom")
			{
				largeRoomsList.Add(object); 
			}
			// el objeto es una sala l
			else if (object->properties.GetProperty("Type") != NULL && object->properties.GetProperty("Type")->value == "isLRoom")
			{
				LRoomsList.Add(object);
				if (object->properties.GetProperty("lType") != NULL && object->properties.GetProperty("lType")->value == "dl")
				{
					dlRoomsList.Add(object);
				}
				else if (object->properties.GetProperty("lType") != NULL && object->properties.GetProperty("lType")->value == "dr")
				{
					drRoomsList.Add(object);
				}
				else if (object->properties.GetProperty("lType") != NULL && object->properties.GetProperty("lType")->value == "ul")
				{
					ulRoomsList.Add(object);
				}
				else if (object->properties.GetProperty("lType") != NULL && object->properties.GetProperty("lType")->value == "ur")
				{
					urRoomsList.Add(object);
				}
				else if (object->properties.GetProperty("lType") != NULL && object->properties.GetProperty("lType")->value == "d")
				{
					dRoomsList.Add(object);
				}
				else if (object->properties.GetProperty("lType") != NULL && object->properties.GetProperty("lType")->value == "u")
				{
					uRoomsList.Add(object);
				}
				else if (object->properties.GetProperty("lType") != NULL && object->properties.GetProperty("lType")->value == "l")
				{
					lRoomsList.Add(object);
				}
				else if (object->properties.GetProperty("lType") != NULL && object->properties.GetProperty("lType")->value == "r")
				{
					rRoomsList.Add(object);
				}
			}


			else {
				PhysBody* c1;

				if (object->type == RECTANGULO) {
					c1 = app->physics->CreateRectangle(object->x + object->width / 2, object->y + object->height / 2, object->width, object->height, STATIC);
				}
				else {
					c1 = app->physics->CreateChain(object->x, object->y, object->points.data(), object->points.size(), STATIC);
				}

			

				std::string obN = objectName.GetString();
				if (obN.compare("Vacios") == false) {

					c1->ctype = ColliderType::VACIOS;
					collisionsList.Add(c1);
					c1->body->GetFixtureList()->SetSensor(true);
				}
				else
				{
					c1->ctype = ColliderType::PLATFORM;
					collisionsList.Add(c1);
				}

			}

			
			
			ret = true;
		}
		mapObjectsItem = mapObjectsItem->next;
	}


	return ret;
}

bool Map::LoadEntities(std::string layerName)
{

	ListItem<MapLayer*>* mapLayerItem;
	mapLayerItem = mapData.layers.start;
	bool ret = false;

	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	if (parseResult) {
		configNode = configFile.child("config");
	}
	else {
		LOG("Error in Map::LoadEntities(): %s", parseResult.description());
		return false;
	}
	LOG("CARGA ENTIDADES");
	while (mapLayerItem != NULL) {

		if (mapLayerItem->data->name.GetString() == layerName) {
			for (int x = 0; x < mapLayerItem->data->width; x++)
			{

				for (int y = 0; y < mapLayerItem->data->height; y++)
				{


					int gid = mapLayerItem->data->Get(x, y);
					TileSet* tileset = GetTilesetFromTileId(gid);
					SDL_Rect r = tileset->GetRect(gid);
					iPoint pos = MapToWorld(x, y);

					//PLAYER
					if (gid == tileset->firstgid + 0) {

						app->entityManager->SetPlayer((Player*)app->entityManager->CreateEntity(EntityType::PLAYER));
						app->entityManager->GetPlayer()->config = configNode.child("entities_data").child("player");
						
						iPoint Npos = iPoint(pos.x + 16, pos.y + 16);
						LOG("POSX: %d, POSY: %d", pos.x, pos.y);
						app->entityManager->GetPlayer()->position = iPoint(pos.x + 16, pos.y + 16);
						app->entityManager->GetPlayer()->Start();

					}

					//NPC_VENDEDOR
					if (gid == tileset->firstgid + 1) {


						NPCVendedor* npc = (NPCVendedor*)app->entityManager->CreateEntity(EntityType::NPC_VENDEDOR);
						npc->config = configNode.child("entities_data").child("npc_vendedor");
						npc->position = iPoint(pos.x + 16, pos.y + 16);
						npc->Start();

					}

					//NPC_PESCADOR
					if (gid == tileset->firstgid + 2) {


						NPCVendedor* npc = (NPCVendedor*)app->entityManager->CreateEntity(EntityType::NPC_PESCADOR);
						npc->config = configNode.child("entities_data").child("npc_pescador");
						npc->position = iPoint(pos.x + 16, pos.y + 16);
						npc->Start();

					}

					//NPC_BRUJA
					if (gid == tileset->firstgid + 3) {


						NPCVendedor* npc = (NPCVendedor*)app->entityManager->CreateEntity(EntityType::NPC_BRUJA);
						npc->config = configNode.child("entities_data").child("npc_bruja");
						npc->position = iPoint(pos.x + 16, pos.y + 16);
						npc->Start();

					}

					//NPC_BULLY1
					if (gid == tileset->firstgid + 4) {


						NPCVendedor* npc = (NPCVendedor*)app->entityManager->CreateEntity(EntityType::NPC_BULLY_1);
						npc->config = configNode.child("entities_data").child("npc_bully1");
						npc->position = iPoint(pos.x + 16, pos.y + 16);
						npc->Start();

					}

					//NPC_BULLY2
					if (gid == tileset->firstgid + 5) {


						NPCVendedor* npc = (NPCVendedor*)app->entityManager->CreateEntity(EntityType::NPC_BULLY_2);
						npc->config = configNode.child("entities_data").child("npc_bully2");
						npc->position = iPoint(pos.x + 16, pos.y + 16);
						npc->Start();

					}

					//NPC_HERRERA
					if (gid == tileset->firstgid + 6) {


						NPCVendedor* npc = (NPCVendedor*)app->entityManager->CreateEntity(EntityType::NPC_HERRERA);
						npc->config = configNode.child("entities_data").child("npc_herrera");
						npc->position = iPoint(pos.x + 16, pos.y + 16);
						npc->Start();

					}

					//NPC_ABUELO
					if (gid == tileset->firstgid + 7) {


						NPCVendedor* npc = (NPCVendedor*)app->entityManager->CreateEntity(EntityType::NPC_ABUELO);
						npc->config = configNode.child("entities_data").child("npc_abuelo");
						npc->position = iPoint(pos.x + 16, pos.y + 16);
						npc->Start();

					}

					//NPC_MUJER_PREOCUPADA
					if (gid == tileset->firstgid + 8) {


						NPCVendedor* npc = (NPCVendedor*)app->entityManager->CreateEntity(EntityType::NPC_MUJER_PREOCUPADA);
						npc->config = configNode.child("entities_data").child("npc_mujer_preocupada");
						npc->position = iPoint(pos.x + 16, pos.y + 16);
						npc->Start();

					}

					//NPC_MUJER_ENAMORADA
					if (gid == tileset->firstgid + 9) {


						NPCVendedor* npc = (NPCVendedor*)app->entityManager->CreateEntity(EntityType::NPC_MUJER_ENAMORADA);
						npc->config = configNode.child("entities_data").child("npc_mujer_enamorada");
						npc->position = iPoint(pos.x + 16, pos.y + 16);
						npc->Start();

					}

					//NPC_ENAMORADOS
					if (gid == tileset->firstgid + 10) {


						NPCVendedor* npc = (NPCVendedor*)app->entityManager->CreateEntity(EntityType::NPC_ENAMORADOS);
						npc->config = configNode.child("entities_data").child("npc_enamorados");
						npc->position = iPoint(pos.x + 16, pos.y + 16);
						npc->Start();

					}

					//NPC_ABUELITO
					if (gid == tileset->firstgid + 11) {


						NPCVendedor* npc = (NPCVendedor*)app->entityManager->CreateEntity(EntityType::NPC_ABUELITO);
						npc->config = configNode.child("entities_data").child("npc_abuelito");
						npc->position = iPoint(pos.x + 16, pos.y + 16);
						npc->Start();

					}

					//NPC_ABUELITAS
					if (gid == tileset->firstgid + 12) {


						NPCVendedor* npc = (NPCVendedor*)app->entityManager->CreateEntity(EntityType::NPC_ABUELITAS);
						npc->config = configNode.child("entities_data").child("npc_abuelitas");
						npc->position = iPoint(pos.x + 16, pos.y + 16);
						npc->Start();

					}

					//NPC_GUARDIAS
					if (gid == tileset->firstgid + 13) {


						NPCVendedor* npc = (NPCVendedor*)app->entityManager->CreateEntity(EntityType::NPC_GUARDIAS);
						npc->config = configNode.child("entities_data").child("npc_guardias");
						npc->position = iPoint(pos.x + 16, pos.y + 16);
						npc->Start();

					}


					//OSIRIS
					if (gid == tileset->firstgid + 20) {

						
						Enemy_Osiris* osiris = (Enemy_Osiris*)app->entityManager->CreateEntity(EntityType::ENEMY_OSIRIS);
						osiris->config = configNode.child("entities_data").child("osiris");
						osiris->position = iPoint(pos.x + 16, pos.y + 16);
						osiris->Start();

					}

					//OSIRIS VARIATION
					if (gid == tileset->firstgid + 26) {


						Enemy_Osiris_Variation* osiris_variation = (Enemy_Osiris_Variation*)app->entityManager->CreateEntity(EntityType::ENEMY_OSIRIS_VARIATION);
						osiris_variation->config = configNode.child("entities_data").child("osiris_variation");
						osiris_variation->position = iPoint(pos.x + 16, pos.y + 16);
						osiris_variation->Start();

					}

					//OLS
					if (gid == tileset->firstgid + 21) {

						
						Enemy_Ols* ols = (Enemy_Ols*)app->entityManager->CreateEntity(EntityType::ENEMY_OLS);
						ols->config = configNode.child("entities_data").child("ols");
						ols->position = iPoint(pos.x + 16, pos.y + 16);
						ols->Start();

					}
					//OLS VARIATION
					if (gid == tileset->firstgid + 31) {

						
						Enemy_Ols_Variation* ols_variation = (Enemy_Ols_Variation*)app->entityManager->CreateEntity(EntityType::ENEMY_OLS_VARIATION);
						ols_variation->config = configNode.child("entities_data").child("ols_variation");
						ols_variation->position = iPoint(pos.x + 16, pos.y + 16);
						ols_variation->Start();

					}

					//SHAR
					if (gid == tileset->firstgid + 22) {

						
						Enemy_Shar* shar = (Enemy_Shar*)app->entityManager->CreateEntity(EntityType::ENEMY_SHAR);
						shar->config = configNode.child("entities_data").child("shar");
						shar->position = iPoint(pos.x + 16, pos.y + 16);
						shar->Start();

					}

					//KHURT
					if (gid == tileset->firstgid + 23) {


						Enemy_Khurt* khurt = (Enemy_Khurt*)app->entityManager->CreateEntity(EntityType::ENEMY_KHURT);
						khurt->config = configNode.child("entities_data").child("khurt");
						khurt->position = iPoint(pos.x + 16, pos.y + 16);
						khurt->Start();

					}

					//KHURT VARIATION
					if (gid == tileset->firstgid + 30) {

						Enemy_Khurt_Variation* khurt_variation = (Enemy_Khurt_Variation*)app->entityManager->CreateEntity(EntityType::ENEMY_KHURT_VARIATION);
						khurt_variation->config = configNode.child("entities_data").child("khurt_variation");
						khurt_variation->position = iPoint(pos.x + 16, pos.y + 16);
						khurt_variation->Start();

					}

					//MUUR
					if (gid == tileset->firstgid + 24) {

						Enemy_Muur* muur = (Enemy_Muur*)app->entityManager->CreateEntity(EntityType::ENEMY_MUUR);
						muur->config = configNode.child("entities_data").child("muur");
						muur->position = iPoint(pos.x + 16, pos.y + 16);
						muur->Start();

					}

					////BOOROK
					if (gid == tileset->firstgid + 25) {

						Enemy_Boorok* boorok = (Enemy_Boorok*)app->entityManager->CreateEntity(EntityType::ENEMY_BOOROK);
						boorok->config = configNode.child("entities_data").child("boorok");
						boorok->position = iPoint(pos.x + 16, pos.y + 16);
						boorok->Start();

					}

					//CUERNO
					if (gid == tileset->firstgid + 40) {
						Item_Garra* garra = (Item_Garra*)app->entityManager->CreateEntity(EntityType::ITEM_GARRA);
						garra->config = configNode.child("entities_data").child("item_garra");
						garra->position = iPoint(pos.x + 16, pos.y + 16);
						garra->Start();
						

					}
					//DIAMANTE
					if (gid == tileset->firstgid + 41) {
						Item_Diamante* diamante = (Item_Diamante*)app->entityManager->CreateEntity(EntityType::ITEM_DIAMANTE);
						diamante->config = configNode.child("entities_data").child("item_diamante");
						diamante->position = iPoint(pos.x + 16, pos.y + 16);
						diamante->Start();
					}

					//Boss_Inuit
					if (gid == tileset->firstgid + 60) {
						Boss_Inuit* boss_Inuit = (Boss_Inuit*)app->entityManager->CreateEntity(EntityType::BOSS_INUIT);
						boss_Inuit->config = configNode.child("entities_data").child("boss_inuit");
						boss_Inuit->position = iPoint(pos.x + 16, pos.y + 16);
						boss_Inuit->Start();
					}


					//Boss_Musri
					if (gid == tileset->firstgid + 61) {
						Boss_Musri* boss_Musri = (Boss_Musri*)app->entityManager->CreateEntity(EntityType::BOSS_MUSRI);
						boss_Musri->config = configNode.child("entities_data").child("boss_musri");
						boss_Musri->position = iPoint(pos.x + 16, pos.y + 16);
						boss_Musri->Start();
					}

					//Boss_Surma
					if (gid == tileset->firstgid + 62) {
						Boss_Surma* boss_Surma = (Boss_Surma*)app->entityManager->CreateEntity(EntityType::BOSS_SURMA);
						boss_Surma->config = configNode.child("entities_data").child("boss_surma");
						boss_Surma->position = iPoint(pos.x + 16, pos.y + 16);
						boss_Surma->Start();
					}

					//Boss_Igory
					if (gid == tileset->firstgid + 63) {
						Boss_Igory* boss_Igory = (Boss_Igory*)app->entityManager->CreateEntity(EntityType::BOSS_IGORY);
						boss_Igory->config = configNode.child("entities_data").child("boss_igory");
						boss_Igory->position = iPoint(pos.x + 16, pos.y + 16);
						boss_Igory->Start();
					}

					/*COFRE*/
					if (gid == tileset->firstgid + 42) { 
							Cofre* cofre = (Cofre*)app->entityManager->CreateEntity(EntityType::COFRE);
							cofre->config = configNode.child("entities_data").child("cofre");
							cofre->position = iPoint(pos.x + 16, pos.y + 16);
							cofre->Start();
						
					}
					/*ASCENSOR INICIO*/
					if (gid == tileset->firstgid + 44) {
						Elevator* elev = (Elevator*)app->entityManager->CreateEntity(EntityType::ASCENSOR);
						elev->config = configNode.child("entities_data").child("ascensor");
						elev->position = iPoint(pos.x + 16, pos.y + 16);
						elev->Start();
						elev->final = false;

					}
					/*ASCENSOR FINAL*/
					if (gid == tileset->firstgid + 45) {
						Elevator* elev = (Elevator*)app->entityManager->CreateEntity(EntityType::ASCENSOR);
						elev->config = configNode.child("entities_data").child("ascensor");
						elev->position = iPoint(pos.x + 16, pos.y + 16);
						elev->Start();
						elev->final = true;

					}

					/*SPAWNER*/
					if (gid == tileset->firstgid + 46) {

						Enemy_Spawner* spawner = (Enemy_Spawner*)app->entityManager->CreateEntity(EntityType::ENEMY_SPAWNER);
						spawner->config = configNode.child("entities_data").child("spawner");
						//spawner->configEntities = configNode.child("entities_data");
						spawner->position = iPoint(pos.x + 16, pos.y + 16);
						spawner->Start();


						/*Elevator* elev = (Elevator*)app->entityManager->CreateEntity(EntityType::ASCENSOR);
						elev->config = configNode.child("entities_data").child("ascensor");
						elev->position = iPoint(pos.x + 16, pos.y + 16);
						elev->Start();
						elev->final = true;*/

					}


					/*NOTA*/
					/*if (gid == tileset->firstgid + 42) {
						Item_Nota* nota = (Item_Nota*)app->entityManager->CreateEntity(EntityType::ITEM_NOTA);
						nota->config = configNode.child("entities_data").child("item_nota");
						nota->position = iPoint(pos.x + 16, pos.y + 16);
						nota->Start();

					}*/
				}
			}

		}
		mapLayerItem = mapLayerItem->next;

	}




	return false;
}

std::vector<int> Map::GetObjectGroupPoints(const std::string& points)
{
	std::vector<int> puntos;
	std::istringstream iss(points);
	std::string punto;

	// Iterar sobre cada par de coordenadas (x, y)
	while (std::getline(iss, punto, ' ')) {
		// Separar las coordenadas x e y
		std::istringstream issPunto(punto);
		std::string coordenadaX, coordenadaY;
		std::getline(issPunto, coordenadaX, ',');
		std::getline(issPunto, coordenadaY, ',');

		// Convertir las coordenadas de cadena a enteros y agregarlas a la lista de puntos

		puntos.push_back(std::stoi(coordenadaX));
		puntos.push_back(std::stoi(coordenadaY));
	}

	return puntos;
}

void Map::BubbleSort(std::vector<PuzzleButtonEntity*>* entities) {
	bool swapped;
	do {
		swapped = false;
		for (size_t i = 1; i < entities->size(); ++i) {
			if (entities->at(i - 1)->buttonID > entities->at(i)->buttonID) {
				// Correctly swap the elements
				std::swap(entities->at(i - 1), entities->at(i));
				swapped = true;
			}
		}
	} while (swapped);
}


// L13: Create navigationMap map for pathfinding
void Map::CreateNavigationMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;

	//Sets the size of the map. The navigation map is a unidimensional array 
	uchar* navigationMap = new uchar[navigationLayer->width * navigationLayer->height];
	//reserves the memory for the navigation map
	memset(navigationMap, 1, navigationLayer->width * navigationLayer->height);

	for (int x = 0; x < mapData.width; x++)
	{
		for (int y = 0; y < mapData.height; y++)
		{
			//i is the index of x,y coordinate in a unidimensional array that represents the navigation map
			int i = (y * navigationLayer->width) + x;

			//Gets the gid of the map in the navigation layer
			int gid = navigationLayer->Get(x, y);

			//If the gid is a blockedGid is an area that I cannot navigate, so is set in the navigation map as 0, all the other areas can be navigated
			//!!!! make sure that you assign blockedGid according to your map
			if (gid == blockedGid) navigationMap[i] = 0;
			else navigationMap[i] = 1;
		}
	}

	*buffer = navigationMap;
	width = mapData.width;
	height = mapData.height;

}

