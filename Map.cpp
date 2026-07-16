#include "Map.h"

vector<MapArea> Map::getAreas() {
    return {
        {"square", "中央广场", "校园的中心，任务发布点",
         AreaType::SAFE_ZONE, 1, 100, {},
         {"library", "canteen", "teaching_building"},
         {{"npc_guide", "欢迎来到校园！去教学楼历练吧。"},
          {"npc_task", "任务公告板有很多任务等你。"}}},

        {"teaching_building", "教学楼", "学生上课的地方，现在被怪物占领",
         AreaType::BATTLE_ZONE, 1, 5,
         {"goblin", "goblin", "student_monster"},
         {"lab_building", "square"},
         {{"npc_teacher", "小心！教室里有怪物！"}}},

        {"lab_building", "实验楼", "充满化学和生物实验的危险区域",
         AreaType::BATTLE_ZONE, 3, 6,
         {"chemical_monster", "plant_monster", "plant_monster"},
         {"teaching_building", "library"},
         {{"npc_researcher", "实验失败了，产生了变异生物！"}}},

        {"library", "图书馆", "安静的图书馆，藏着许多秘密",
         AreaType::EXPLORE_ZONE, 2, 4,
         {"book_ghost", "book_ghost"},
         {"lab_building", "square", "garden"},
         {{"npc_librarian", "小心书架间的幽灵！"}}},

        {"garden", "后花园", "美丽的花园，但暗藏危险",
         AreaType::EXPLORE_ZONE, 2, 5,
         {"flower_fairy", "tree_spirit", "tree_spirit"},
         {"library", "canteen"},
         {{"npc_gardener", "喷泉有神奇的力量！"}}},

        {"canteen", "食堂", "可以购买食物和药品",
         AreaType::SHOP_ZONE, 1, 100,
         {},
         {"square", "garden", "playground"},
         {{"npc_chef", "今天的特供套餐很划算！"}}},

        {"playground", "操场", "体育老师的领地，强者才能挑战",
         AreaType::BOSS_ZONE, 5, 8,
         {"sports_ghost", "robot_player"},
         {"canteen", "medical_room"},
         {{"npc_coach", "想挑战我？先练好基础！"}}},

        {"medical_room", "医务室", "可以治疗伤势和合成药品",
         AreaType::REST_ZONE, 1, 100,
         {},
         {"playground", "square"},
         {{"npc_doctor", "需要治疗吗？价格很公道。"}}}
    };
}

const MapArea* Map::getAreaById(const string& id) {
    for (const auto& area : getAreas()) {
        if (area.id == id) return &area;
    }
    return nullptr;
}

string Map::getAreaTypeName(AreaType type) {
    switch(type) {
        case AreaType::SAFE_ZONE:   return "🏠 安全区";
        case AreaType::BATTLE_ZONE: return "⚔️ 战斗区";
        case AreaType::SHOP_ZONE:   return "🏪 商店区";
        case AreaType::EXPLORE_ZONE:return "🔍 探索区";
        case AreaType::BOSS_ZONE:   return "👑 Boss区";
        case AreaType::REST_ZONE:   return "💤 恢复区";
        default: return "未知";
    }
}