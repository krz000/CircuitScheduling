#pragma once
#include "Circuit.h"
#include "ALAP_Latency.h"

class MLRCSScheduler : public Scheduler {
private:
    // 资源结构体
    struct GateResources {
        int and_gates;
        int or_gates;
        int not_gates;
    };

public:
    // 构造函数
    MLRCSScheduler(int and_gates, int or_gates, int not_gates);

    // 继承自Scheduler的主调度函数
    void schedule(Circuit& circuit) override;

private:
    // 成员变量
    GateResources resources;
    std::vector<Gate*> readyGates;
    std::vector<Gate*> ongoingGates;
    Circuit* circuit;  // 存储当前处理的电路引用
    int currentCycle;
    const std::unordered_map<int, std::vector<Gate*>>* alapSchedule; // 添加ALAP调度结果的引用

    // 核心功能函数
    void updateReadyGates(const std::vector<Gate>& gates, Circuit& circuit);  // 更新就绪队列
    void updateOngoingGates();  // 更新正在执行的门
    void scheduleGates(std::vector<Gate>& gates);  // 调度当前就绪的门

    // 辅助函数
    bool areAllPredecessorsFinished(const Gate& gate, Circuit& circuit) const;  // 检查所有前驱是否完成
    int getGateDelay(const Gate& gate) const;  // 获取门的延迟
    int getAvailableResources(GateType type) const;  // 获取特定类型的可用资源数

    // 资源管理函数
    void decrementResources(const Gate& gate);  // 减少资源
    void incrementResources(const Gate& gate);  // 增加资源


    // 按类型调度门
    void scheduleGatesOfType(std::vector<Gate*>& gates,
        std::vector<Gate>& allGates,
        GateType type);  // 调度特定类型的门
};