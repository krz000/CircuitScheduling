#include "circuit.h"
#include "ALAP.h"
#include "ASAP.h"

/*
思路：
1. 先用ASAP算出cycle数量，得到最后的结果，找到所有前驱，作为alap最后一轮的scheduled节点，也就是cycle最大
2. 循环：找到alap已经scheduled的节点，把他们的前驱节点作为前一轮的scheduled节点，cycle=-1，直到cycle变成0
3. 从每个output遍历，能得到节点的cycle数，每次比较cycle，得到最小的cycle数

调用ASAP之后得重新覆盖bool
    int scheduledCycle = -1;//第n个
    bool Scheduled = false

循环一遍，scheduled=true

找到circuit的output，把他们作为最后的节点，cycle改为maxcycle，scheduled改成true

循环：
得到this.gate.input.cycle=max-1，

*/




void ALAP::schedule(Circuit& circuit)
{
    ASAPScheduler asapScheduler;
    asapScheduler.schedule(circuit);
}

void ALAP::ALAPschedule(Circuit& circuit) {
	// 获取最大周期
	int maxCycle = circuit.getMaxScheduledCycle();
    // 重置所有门的调度状态
    for (auto& gate : circuit.getGates()) {
        gate.setScheduledCycle(-1);
        gate.setScheduled(false);
    }

    // 从输出开始反向遍历
    std::vector<std::string> outputs = circuit.getOutputs();
    for (const auto& output : outputs) {
        scheduleGate(circuit, output, maxCycle);
    }

    // 输出每个 gate 的调度周期
    //std::cout << "门调度结果 (ALAP):" << std::endl;
    //for (const auto& gate : circuit.getGates()) {
    //    std::cout << "门 " << gate.getGateId() << " 调度在周期 " << gate.getScheduledCycle() << std::endl;
    //}

}

int ALAP::scheduleGate(Circuit& circuit, const std::string& gateName, int currentCycle) {
    try {
        Gate& gate = const_cast<Gate&>(circuit.findGateByOutput(gateName));

        // 如果门已经被调度，直接返回其调度周期
        if (gate.isScheduled()) {
            return gate.getScheduledCycle();
        }

        // 如果是输入门，调度在第-1周期
        const auto& inputs = circuit.getInputs();
        if (std::find(inputs.begin(), inputs.end(), gateName) != inputs.end()) {
            gate.setScheduled(true);
            gate.setScheduledCycle(-1);
            return 0;
        }

        // 递归调度所有输入门
        int maxInputCycle = -1;
        for (const auto& input : gate.getInputs()) {
            int inputCycle = scheduleGate(circuit, input, currentCycle - 1);
            maxInputCycle = std::max(maxInputCycle, inputCycle);
        }

        // 调度当前门
        int scheduledCycle = std::min(currentCycle, maxInputCycle + 1);
        gate.setScheduled(true);
        gate.setScheduledCycle(scheduledCycle);

        return scheduledCycle;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "错误：找不到门 " << gateName << std::endl;
        return -1;
    }
}
