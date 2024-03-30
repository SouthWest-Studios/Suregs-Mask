
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"
#include "Scene.h"
#include "Optick/include/optick.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

	//Calls the functon to load the map, make sure that the filename is assigned
	SString mapPath = path;
	mapPath += name;
	Load(mapPath);

	//Initialize pathfinding 
	pathfinding = new PathFinding();

	//Initialize the navigation map
   // uchar* navigationMap = NULL;
   ///* CreateNavigationMap(mapData.width, mapData.height, &navigationMap);*/
   // pathfinding->SetNavigationMap((uint)mapData.width, (uint)mapData.height, navigationMap);
   // RELEASE_ARRAY(navigationMap);

	return true;
}

bool Map::Update(float dt)
{
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
		if (mapLayer->data->properties.GetProperty("Draw") != NULL && mapLayer->data->properties.GetProperty("Draw")->value) {

			iPoint playerPos = app->entityManager->GetPlayer()->position;
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


	LoadCollisionsObject();
	LoadCollisions("Collisions");
	LoadEntities("Entities");





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
		if (mapLayerItem->data->properties.GetProperty("Navigation") != NULL && mapLayerItem->data->properties.GetProperty("Navigation")->value) {
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
		p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

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
		SString mapTex = pathTextures;
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

bool Map::LoadCollisionsObject()
{
	ListItem<MapObjects*>* mapObjectsItem;
	mapObjectsItem = mapData.mapObjects.start;
	bool ret = false;


	while (mapObjectsItem != NULL) {
		for (int i = 0; i < mapObjectsItem->data->objects.Count(); i++) {

			MapObject* object = mapObjectsItem->data->objects[i];
			PhysBody* c1;

			if (object->properties.GetProperty("dialogID") != NULL) {
				//Spawn dialogo con x id
				c1 = app->physics->CreateRectangleSensor(object->x + object->width / 2, object->y + object->height / 2, object->width, object->height, STATIC);
				c1->ctype = ColliderType::UNKNOWN;
			}
			else {

				if (object->type == RECTANGULO) {
					c1 = app->physics->CreateRectangle(object->x + object->width / 2, object->y + object->height / 2, object->width, object->height, STATIC);
				}
				else {
					c1 = app->physics->CreateChain(object->x, object->y, object->points.data(), object->points.size(), STATIC);
				}

				c1->ctype = ColliderType::PLATFORM;
			}

			
			collisionsList.Add(c1);
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
						app->entityManager->GetPlayer()->config = configNode.child("scene").child("player");
						app->entityManager->GetPlayer()->position = iPoint(pos.x + 16, pos.y + 16);
						app->entityManager->GetPlayer()->Start();

					}



					////Monedas
					//if (gid == tileset->firstgid) {
					//	Coin* coin = (Coin*)app->entityManager->CreateEntity(EntityType::COIN);
					//	coin->parameters = configNode.child("scene").child("textures").child("coin");
					//	coin->position = iPoint(pos.x + 16, pos.y + 16);
					//}

					////cofre con Monedas
					//if (gid == tileset->firstgid + 1) {
					//	Chest* chest = (Chest*)app->entityManager->CreateEntity(EntityType::CHEST_COIN);
					//	chest->parameters = configNode.child("scene").child("textures").child("chest");
					//	chest->position = iPoint(pos.x + 16, pos.y + 16);

					//}

					////espina que se rompe
					//if (gid == tileset->firstgid + 2) {
					//	PlantBreakable* entity = (PlantBreakable*)app->entityManager->CreateEntity(EntityType::PLANT_BREAKABLE);
					//	entity->parameters = configNode.child("scene").child("textures").child("plant_breakable");
					//	entity->position = iPoint(pos.x + 16, pos.y + 16);
					//}

					////barrera de espinas
					//if (gid == tileset->firstgid + 3) {
					//	PlantBarrier* entity = (PlantBarrier*)app->entityManager->CreateEntity(EntityType::PLANT_BARRIER);
					//	entity->parameters = configNode.child("scene").child("textures").child("plant_barrier");
					//	entity->position = iPoint(pos.x, pos.y + 16);
					//}

					////player
					//if (gid == tileset->firstgid + 4) {

					//	app->scene->setPlayer((Player*)app->entityManager->CreateEntity(EntityType::PLAYER));
					//	app->scene->getPlayer()->parameters = configNode.child("scene").child("player");
					//	app->scene->getPlayer()->position = iPoint(pos.x + 16, pos.y + 16);

					//}

					////Enemy armadillo
					//if (gid == tileset->firstgid + 5) {

					//	EnemyArmadillo* entity = (EnemyArmadillo*)app->entityManager->CreateEntity(EntityType::ENEMY_ARMADILLO);
					//	entity->parameters = configNode.child("scene").child("enemyArmadillo");
					//	entity->position = iPoint(pos.x + 16, pos.y + 16);

					//}

					////Entidad de guardar
					//if (gid == tileset->firstgid + 6) {

					//	SaveStatue* entity = (SaveStatue*)app->entityManager->CreateEntity(EntityType::SAVE_STATUE);
					//	entity->parameters = configNode.child("scene").child("textures").child("saveStatue");
					//	entity->position = iPoint(pos.x + 16, pos.y + 16 - 32);

					//}

					////Enemy pajaro con cuchillo
					//if (gid == tileset->firstgid + 7) {

					//	SaveStatue* entity = (SaveStatue*)app->entityManager->CreateEntity(EntityType::ENEMY_PAJARO);
					//	entity->parameters = configNode.child("scene").child("enemyPajaro");
					//	entity->position = iPoint(pos.x + 16, pos.y + 16 - 32);

					//}

					////Comida
					//if (gid == tileset->firstgid + 8) {
					//	Food* food = (Food*)app->entityManager->CreateEntity(EntityType::FOOD);
					//	food->parameters = configNode.child("scene").child("textures").child("food");
					//	food->position = iPoint(pos.x + 16, pos.y + 16);
					//}

					////Boss
					//if (gid == tileset->firstgid + 9) {
					//	EnemyBoss* boss = (EnemyBoss*)app->entityManager->CreateEntity(EntityType::BOSS);
					//	boss->parameters = configNode.child("scene").child("enemyBoss");
					//	boss->position = iPoint(pos.x + 16, pos.y + 16);
					//}

					////Wall Boss
					//if (gid == tileset->firstgid + 10) {
					//	WallBoss* boss = (WallBoss*)app->entityManager->CreateEntity(EntityType::WALL_BOSS);
					//	boss->parameters = configNode.child("scene").child("textures").child("wallBoss");
					//	boss->position = iPoint(pos.x + 16, pos.y + 16);
					//}




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

