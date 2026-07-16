#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include <map>

using namespace std;

enum class AreaType {
    SAFE_ZONE,
    BATTLE_ZONE,
    SHOP_ZONE,
    EXPLORE_ZONE,
    BOSS_ZONE,
    REST_ZONE
};

struct MapArea {
    string id;
    string name;
    string description;
    AreaType type;
    int minLevel;
    int maxLevel;
    vector<string> enemies;
    vector<string> connections;
    map<string, string> npcs;
};

class Map {
public:
    static vector<MapArea> getAreas();
    static const MapArea* getAreaById(const string& id);
    static string getAreaTypeName(AreaType type);
};

#endif