#include "Circuit.h"
#include "MR-LCS.h"
#include "ALAP.h"
#include "ASAP.h"

/*
* 思路：
* 1. 不同资源数：
* 每个资源数都有不同的2.4 2.5
* 2. 每个gate消耗的资源数不同
* 资源数 = gateType & 2 | 3 ! 1
* 如何排序
* 
* 
*/

void MR_LCS::schedule(Circuit& circuit) {
    // 调用 MR-RCS 的主要调度函数
}

void MR_LCS::MR_LCSschedule(Circuit& circuit, int timeLimit) {
    // 1. 初始化 是否传参加入时间限制? 资源限制?
    std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles = getScheduledGatesWithCycles();
    gatesWithCycles.clear();

	// 2. 计算 2.4 时间 得到该资源数下的最大周期
    //  maxcycle = mintime
    // 3. 检查 给定时间限定 的可行性
    
    //int minTime = MR_RCS();
	int minTime = 5;
	// 给定时间和MR_RCS的最小时间比较 小于说明不可能做到
    if (timeLimit < minTime) {
        throw std::runtime_error("调度不可行：ALAP 时间小于 0");
    }

    // 4. 重置所有门的调度状态
    for (auto& gate : circuit.getGates()) {
         gate.setScheduledCycle(-1);
         gate.setScheduled(false);
    }

	int maxCycle = minTime;
    // 从输出开始反向遍历
    std::vector<std::string> outputs = circuit.getOutputs();
    for (const auto& output : outputs) {
        scheduleGate(circuit, output, maxCycle);
    }
    
    
    // 5. 开始调度过程
    //int currentCycle = 0;
    //bool allScheduled = false;

    //while (!allScheduled) {
        //// 5.1 找出当前可调度的门（就绪状态的门）
        //std::vector<Gate*> readyGates = findReadyGates(circuit);

        //// 5.2 计算每个就绪门的 slack
        //std::vector<std::pair<Gate*, int>> gateSlacks;
        //for (Gate* gate : readyGates) {
        //    int slack = calculateSlack(*gate, currentCycle);
        //    gateSlacks.push_back({ gate, slack });
        //}

        //// 5.3 优先调度 slack 为 0 的门
        //scheduleZeroSlackGates(gateSlacks, currentCycle, gatesWithCycles);

        //// 5.4 在资源约束下调度额外的门
        //scheduleAdditionalGates(gateSlacks, currentCycle, gatesWithCycles);

        //// 5.5 检查是否所有门都已调度
        //allScheduled = checkAllScheduled(circuit);

        //// 5.6 进入下一个周期
        //currentCycle++;
    //}
}

int MR_LCS::scheduleGate(Circuit& circuit, const std::string& gateName, int currentCycle) {
    //存储门
    std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles = getScheduledGatesWithCycles();

    try {
        Gate& gate = const_cast<Gate&>(circuit.findGateByOutput(gateName));

        // 如果是输出门，队列尾部先出列调度

        // 如果是输入门，调度在第-1周期
        const auto& inputs = circuit.getInputs();
        if (std::find(inputs.begin(), inputs.end(), gateName) != inputs.end()) {
            gate.setScheduled(true);
            gate.setScheduledCycle(-1);
            return 0;
        }

		// 队列依次出列调度，
        // 跳过什么时候回来？


        auto& gatesInCycle = gatesWithCycles[scheduledCycle];

        gatesInCycle.push_back(&gate);

        return scheduledCycle;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "错误：找不到门 " << gateName << std::endl;
        return -1;
    }
}

