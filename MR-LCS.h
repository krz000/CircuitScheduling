#pragma once

#ifndef MR-LCS_H
#define MR-LCS_H

#include "Circuit.h"
#include <algorithm>

class MR_LCS : public Scheduler {
public:
	void schedule(Circuit& circuit) override;
	void MR_LCSschedule(Circuit& circuit, int timeLimit);
private:
	int scheduleGate(Circuit& circuit, const std::string& gateName, int currentCycle);

    int timeLimit;
	int resourceLimit;

    // 实现队列，依次装入MR_RCS处理后的gate

    // 把所有的gate按照前后调度顺序，依次加入到一个数据结构中 队列，也可以是hashmap，vector等
    // 实现一个函数

    std::vector<Gate*> findReadyGates(Circuit& circuit) {
		// 就绪状态门: 没有scheduled+前驱都已经scheduled

    }

    int calculateSlack(const Gate& gate, int currentCycle) {
        // 计算 slack: 用MR_RCS得到最后的cycle， 
    }

    void scheduleZeroSlackGates(std::vector<std::pair<Gate*, int>>& gateSlacks,
        int currentCycle,
        std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles) {
        // 实现调度 slack 为 0 的门的逻辑
    }

    void scheduleAdditionalGates(std::vector<std::pair<Gate*, int>>& gateSlacks,
        int currentCycle,
        std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles) {
        // 实现在资源约束下调度额外门的逻辑
    }

    bool checkAllScheduled(Circuit& circuit) {
        // 实现检查是否所有门都已调度的逻辑
    }
};

#endif // !MR-LCS_H
