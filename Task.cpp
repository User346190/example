// Task.cpp
#include "Task.h"
#include <ctime>
#include <sstream>

using namespace std;

// 生成ID
string generateTaskId() {
    static int counter = 0;
    time_t now = time(nullptr);
    stringstream ss;
    ss << now << "_" << (counter++);
    return ss.str();
}

Task::Task() 
    : m_id(""), m_name(""), m_description(""), m_type(TaskType::KILL_ENEMY),
      m_status(TaskStatus::AVAILABLE), m_target(0), m_progress(0),
      m_expReward(0), m_goldReward(0) {}

Task::Task(const string& name, const string& desc, TaskType type,
           int target, int expReward, int goldReward)
    : m_name(name), m_description(desc), m_type(type),
      m_status(TaskStatus::AVAILABLE), m_target(target), m_progress(0),
      m_expReward(expReward), m_goldReward(goldReward) {
    m_id = generateTaskId();
}

void Task::setProgress(int progress) {
    m_progress = min(progress, m_target);
}

void Task::addProgress(int amount) {
    m_progress += amount;
    if (m_progress > m_target) m_progress = m_target;
    if (m_progress >= m_target && m_status == TaskStatus::ACCEPTED) {
        m_status = TaskStatus::COMPLETED;
    }
}

bool Task::isComplete() const {
    return m_progress >= m_target && m_status == TaskStatus::ACCEPTED;
}

void Task::addItemReward(const Item& item) {
    m_itemRewards.push_back(item);
}

string Task::getTypeString() const {
    switch(m_type) {
        case TaskType::KILL_ENEMY: return "击杀任务";
        case TaskType::COLLECT_ITEM: return "收集任务";
        case TaskType::REACH_LEVEL: return "等级任务";
        case TaskType::EXPLORE: return "探索任务";
        default: return "未知";
    }
}

string Task::getStatusString() const {
    switch(m_status) {
        case TaskStatus::AVAILABLE: return "📋 可接取";
        case TaskStatus::ACCEPTED: return "⏳ 进行中";
        case TaskStatus::COMPLETED: return "✅ 已完成";
        case TaskStatus::REWARDED: return "🎁 已领奖";
        default: return "未知";
    }
}

void Task::display() const {
    cout << "📜 " << m_name << " [" << getStatusString() << "]" << endl;
    cout << "  类型: " << getTypeString() << endl;
    cout << "  描述: " << m_description << endl;
    cout << "  进度: " << m_progress << "/" << m_target << endl;
    cout << "  奖励: " << m_expReward << " EXP, " << m_goldReward << " 金币";
    if (!m_itemRewards.empty()) {
        cout << ", 物品: ";
        for (const auto& item : m_itemRewards) {
            cout << item.getName() << " ";
        }
    }
    cout << endl;
}

string Task::toString() const {
    stringstream ss;
    ss << m_id << "|" << m_name << "|" << m_description << "|"
       << (int)m_type << "|" << (int)m_status << "|" 
       << m_target << "|" << m_progress << "|"
       << m_expReward << "|" << m_goldReward;
    return ss.str();
}

// 工厂方法
Task Task::createKillTask(int level) {
    int target = 3 + level / 2;
    int expReward = 30 + level * 10;
    int goldReward = 20 + level * 8;
    Task task("消灭怪物", "击杀 " + to_string(target) + " 只怪物", 
              TaskType::KILL_ENEMY, target, expReward, goldReward);
    task.addItemReward(Item("生命药水", "恢复50HP", ItemType::MEDICINE, 30, 50, 1));
    return task;
}

Task Task::createCollectTask(int level) {
    int target = 5 + level;
    Task task("收集材料", "收集 " + to_string(target) + " 个材料", 
              TaskType::COLLECT_ITEM, target, 20 + level * 5, 15 + level * 5);
    return task;
}

Task Task::createLevelTask(int level) {
    Task task("等级挑战", "达到 " + to_string(level + 2) + " 级", 
              TaskType::REACH_LEVEL, level + 2, 50 + level * 15, 30 + level * 10);
    return task;
}

Task Task::createExploreTask(int level) {
    Task task("探索新区域", "探索校园的 " + to_string(2 + level/2) + " 个区域", 
              TaskType::EXPLORE, 2 + level/2, 25 + level * 8, 20 + level * 5);
    return task;
}