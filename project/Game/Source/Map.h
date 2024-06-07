#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "PQueue.h"
#include "DynArray.h"
#include "Pathfinding.h"
#include "Physics.h"
#include "Dialog.h"
#include "Boss_Igory.h"
#include "PuzzleButtonEntity.h"

#include "PugiXml\src\pugixml.hpp"


enum MapOrientation
{
    ORTOGRAPHIC = 0,
    ISOMETRIC
};
enum MapObjectType {
    RECTANGULO = 0,
    POLIGONO
};



struct TileTexture {
    SDL_Texture* texture = nullptr;
    uint x;
    uint y;
};

struct TileSetTexture {

    SString name;
    int tileSize = 256;

    int tileSetW;
    int tileSetH;

    int tilesSetWMoved = -1;
    int tilesSetWMovedR = -1;
    int tilesSetHMoved = -1;
    int tilesSetHMovedR = -1;

    uint firstGID;
    uint lastGID;

    std::vector<TileTexture*> tilesTextures;

    std::string pathFolder;


    TileTexture* GetTile(uint x, uint y) {
        for (int i = 0; i < tilesTextures.size(); i++) {
            TileTexture* t = tilesTextures.at(i);
            
            if (t != nullptr && (t->x != NULL || t->x == 0) && (t->y != NULL || t->y == 0) && t->x == x && t->y == y) {
                return t;  
                break; 
            }
        }
        return nullptr;
    }

    std::string GetTileTexturePath(int x, int y) {

        return pathFolder + "/" + "row-" + std::to_string(y+1) + "-column-" + std::to_string(x+1) + ".png";
    }



};


struct TileSet
{
    int firstgid;
    SString name;
    int tilewidth;
    int tileheight;
    int spacing;
    int margin;
    int tilecount;
    int columns;



    SDL_Texture* texture;


    SDL_Rect GetRect(uint gid) {
        SDL_Rect rect = { 0 };

        int relativeIndex = gid - firstgid;
        rect.w = tilewidth;
        rect.h = tileheight;
        rect.x = margin + (tilewidth + spacing) * (relativeIndex % columns);
        rect.y = margin + (tileheight + spacing) * (relativeIndex / columns);

        return rect;
    }
};

// L08: DONE 5: Add attributes to the property structure
struct Properties
{
    struct Property
    {
        SString name;
        std::string value; //We assume that we are going to work only with bool for the moment
    };

    List<Property*> propertyList;

    ~Properties()
    {
        //...
        ListItem<Property*>* property;
        property = propertyList.start;

        while (property != NULL)
        {
            RELEASE(property->data);
            property = property->next;
        }

        propertyList.Clear();
    }

    // L08: DONE 7: Method to ask for the value of a custom property
    Property* GetProperty(const char* name);

};

// L05: DONE 1: Create a struct needed to hold the information to Map node
struct MapLayer
{
    // L06: DONE 1: Add the info to the MapLayer Struct
    int id;
    SString name;
    int width;
    int height;
    uint* tiles;
    Properties properties;

    // L06: DONE 6: Short function to get the gid value of x,y
    uint Get(int x, int y) const
    {
        return tiles[(y * width) + x];
    }
};

struct MapObject {

    uint id;
    uint x;
    uint y;
    uint width;
    uint height;
    std::vector<int> points;
    MapObjectType type;
    Properties properties;
    bool isCleared = false;
};

struct MapObjects
{
    SString	name;
    int id;
    int x;
    int y;
    int width;
    int height;
    List<MapObject*> objects;

    Properties properties;

};

struct MapData
{
    int width;
    int height;
    int tilewidth;
    int tileheight;
    List<TileSet*> tilesets;

    std::vector<TileSetTexture*> tileSetTextures;

    // L09: DONE 2: Define a property to store the MapType and Load it from the map
    MapOrientation orientation; 

    // L06: DONE 2: Add a list/array of layers to the map
    List<MapLayer*> layers;
    List<MapObjects*> mapObjects;
};

class Map : public Module
{
public:

    Map(App* app, bool start_enabled = true);

    
    virtual ~Map();

    
    bool Awake(pugi::xml_node config);

    
    bool Start();

    bool PreUpdate();
    bool Update(float dt);
    bool PostUpdate();
    bool PrintMapFront();


    
    bool CleanUp();

   
    bool Load(SString mapFileName);

   
    iPoint MapToWorld(int x, int y) const;
    iPoint WorldToMap(int x, int y);

    void SpawnEnemy(EntityType type, const char* configName, iPoint position);

    void generaSureg(FASE_Igory fase, iPoint position);
    void choseSureg(iPoint position,int suregType);
    void DestroyEntity(Entity* entity);

    
 

    
    void CreateNavigationMap(int& width, int& height, uchar** buffer) const;

    int GetTileWidth();
    int GetTileHeight();


private:


    bool LoadMap(pugi::xml_node mapFile);
    bool LoadTileSet(pugi::xml_node mapFile);
    bool LoadLayer(pugi::xml_node& layerNode, MapLayer* mapLayer);
    bool LoadAllLayers(pugi::xml_node mapNode);
    bool LoadObject(pugi::xml_node& node, MapObjects* mapObjects);
  
    bool LoadAllObjectGroups(pugi::xml_node mapNode);
    TileSet* GetTilesetFromTileId(int gid) const;
    bool LoadProperties(pugi::xml_node& node, Properties& properties);


    bool LoadCollisions(std::string layerName);
    bool LoadTPEntities(std::string layerName);
    bool LoadPuzzleEntities(std::string layerName);
    bool LoadObjects();
    bool LoadEntities(std::string layerName);

    const char* boolToString(bool b);
    std::vector<int> GetObjectGroupPoints(const std::string& puntos);

    void BubbleSort(std::vector<PuzzleButtonEntity*>* entities);



    bool LoadState(pugi::xml_node node);
    bool SaveState(pugi::xml_node node);



    //NEW MAP SYSTEM

    bool LoadTileSetTexture(pugi::xml_node mapFile);



public: 
    SString nameMazmorra;
    SString path;
    SString pathTextures;
    PathFinding* pathfinding;
    MapData mapData;

    //Camara xd
    List<MapObject*> smallRoomsList;
    List<MapObject*> largeRoomsList;
    List<MapObject*> LRoomsList;
    List<MapObject*> dlRoomsList;
    List<MapObject*> drRoomsList;
    List<MapObject*> ulRoomsList;
    List<MapObject*> urRoomsList;
    List<MapObject*> dRoomsList;
    List<MapObject*> uRoomsList;
    List<MapObject*> lRoomsList;
    List<MapObject*> rRoomsList;

    iPoint al = { 0,0 };

    std::vector<Entity*> enemies;
    int maxEnemies = 0; 

    bool boss1_defeated = false;
    bool boss2_defeated = false;
    bool boss3_defeated = false;
    bool boss4_defeated = false;

    bool tpSaleMazmorra = false;


private:
    

    bool mapLoaded;
    MapLayer* navigationLayer;
    int blockedGid = 49; //!!!! make sure that you assign blockedGid according to your map

    uint windowW, windowH;

    List<PhysBody*> collisionsList;

    pugi::xml_document configFile;
    pugi::xml_document dialoguesFile;
    pugi::xml_node configNode;
    pugi::xml_node dialoguesNode;

    List<Dialog*> dialogues;

    std::vector<PuzzleButtonEntity*> puzzleButtonEntities;
    bool recompensaPuzzle = false;


    bool puzzleComplete[8] = { false };

 };

#endif // __MAP_H__