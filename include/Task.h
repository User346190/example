// Task.h
#ifndef TASK_H
#define TASK_H

#include "Utils.h"
#include "Item.h"

enum class TaskStatus {
    AVAILABLE,
    ACCEPTED,
    COMPLETED,
    REWARDED
};

enum class TaskType {
    KILL_ENEMY,
    COLLECT_ITEM,
    REACH_LEVEL,
    EXPLORE
};

class Task {
private:
    string m_id;
    string m_name;
    string m_description;
    TaskType m_type;
    TaskStatus m_status;
    int m_target;
    int m_progress;
    int m_expReward;
    int m_goldReward;
    vector<Item> m_itemRewards;

public:
    Task();
    Task(const string& name, const string& desc, TaskType type,
         int target, int expReward, int goldReward);
    
    // Getter
    string getId() const { return m_id; }
    string getName() const { return m_name; }
    string getDescription() const { return m_description; }
    TaskType getType() const { return m_type; }
    TaskStatus getStatus() const { return m_status; }
    int getTarget() const { return m_target; }
    int getProgress() const { return m_progress; }
    int getExpReward() const { return m_expReward; }
    int getGoldReward() const { return m_goldReward; }
    const vector<Item>& getItemRewards() const { return m_itemRewards; }
    
    // Setter
    void setStatus(TaskStatus status) { m_status = status; }
    void setProgress(int progress);
    void addProgress(int amount);
    
    // 任务操作
    bool isComplete() const;
    void addItemReward(const Item& item);
    
    // 显示
    void display() const;
    string toString() const;
    string getTypeString() const;
    string getStatusString() const;
    
    // 工厂方法
    static Task createKillTask(int level = 1);
    static Task createCollectTask(int level = 1);
    static Task createLevelTask(int level = 1);
    static Task createExploreTask(int level = 1);
};

#endif