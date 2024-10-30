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
* 
* 
* 
*/

void MR_LCS::schedule(Circuit& circuit) {
    // 调用MR_RCS，暴力得到minCycle
    while (minCycle > timeLimit) {
		int _and = 0, _or = 0, _not = 0;
		int _sum = _and + _or + _not;
		minCycle = MR_RCS(circuit, _and, _or, _not);
        // 算法优化
        // 000开始 对于总资源数为_sum，分别分配，得到最小的cycle，记录各类门的资源数
		// A(n, 3) = n(n-1)(n-2)/6;
    }

}

void MR_LCS::MR_LCSschedule(Circuit& circuit, int timeLimit) {
    // 1. 初始化 传参加入时间限制
    std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles = getScheduledGatesWithCycles();
    gatesWithCycles.clear();

	// 2. 计算 2.4 时间 得到该资源数下的最大周期
    //  maxcycle = mintime
    // 3. 检查 给定时间限定 的可行性
	// ALAP延迟版得到 minTime + 运行顺序
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

    // 资源数
	int _and = 0, _or = 0, _not = 0;
	

	int maxCycle = minTime;
    // 从输出开始反向遍历
    std::vector<std::string> outputs = circuit.getOutputs();
    for (const auto& output : outputs) {
        scheduleGate(circuit, output, maxCycle);
    }
    
    
    // 5. 开始调度过程
    int currentCycle = 0;
    bool allScheduled = false;


    while (!allScheduled) {
        // 5.1 找出当前可调度的门（就绪状态的门）
        std::vector<Gate*> readyGates = findReadyGates(circuit);
        
        // 5.2 计算每个就绪门的 slack=最晚完成时间-当前时间
        std::vector<std::pair<Gate*, int>> gateSlacks;
        for (Gate* gate : readyGates) {
            int slack = calculateSlack(*gate, currentCycle);
            gateSlacks.push_back({ gate, slack });
        }

        // 5.3 优先调度 slack 为 0 的门
        scheduleZeroSlackGates(gateSlacks, currentCycle, gatesWithCycles);

        // 5.4 在资源约束下调度额外的门
        scheduleAdditionalGates(gateSlacks, currentCycle, gatesWithCycles);

        // 5.5 检查是否所有门都已调度
        allScheduled = checkAllScheduled(circuit);

        // 5.6 进入下一个周期
        currentCycle++;
    }
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


        auto& gatesInCycle = gatesWithCycles[scheduledCycle];

        // 删除重复门

        gatesInCycle.push_back(&gate);

        return scheduledCycle;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "错误：找不到门 " << gateName << std::endl;
        return -1;
    }
}

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