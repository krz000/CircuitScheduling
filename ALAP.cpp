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

int maxCycle;

void ALAP::ALAPschedule(Circuit& circuit) {
	// 获取最大周期
    maxCycle = circuit.getMaxScheduledCycle();
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

	// 删除重复的门
    //removeDuplicateGates(circuit);

    // 输出每个 gate 的调度周期
    //std::cout << "门调度结果 (ALAP):" << std::endl;
    //for (const auto& gate : circuit.getGates()) {
    //    std::cout << "门 " << gate.getGateId() << " 调度在周期 " << gate.getScheduledCycle() << std::endl;
    //}
}

// 移除重复的门
//void ALAP::removeDuplicateGates(Circuit& circuit) {
//    std::unordered_map<std::string, Gate*> uniqueGates;
//    std::vector<Gate>& gates = circuit.getGates();
//
//    for (auto& gate : gates) {
//        std::string gateId = gate.getGateId();
//        if (uniqueGates.find(gateId) == uniqueGates.end()) {
//            uniqueGates[gateId] = &gate;
//        }
//        else {
//            Gate* existingGate = uniqueGates[gateId];
//            if (gate.getScheduledCycle() < existingGate->getScheduledCycle()) {
//                existingGate->setScheduledCycle(gate.getScheduledCycle());
//                existingGate->setScheduled(true);
//            }
//            gate.setScheduled(false);
//        }
//    }
//
//    // 移除未调度的门
//    gates.erase(std::remove_if(gates.begin(), gates.end(),
//        [](const Gate& g) { return !g.isScheduled(); }), gates.end());
//}

int ALAP::scheduleGate(Circuit& circuit, const std::string& gateName, int currentCycle) {

    //存储门
    std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles = getScheduledGatesWithCycles();
    
    try {
        Gate& gate = const_cast<Gate&>(circuit.findGateByOutput(gateName));

		// 如果是输出门，调度周期是最后一个周期
        const auto& outputs = circuit.getOutputs();
        if (std::find(outputs.begin(), outputs.end(), gateName) != outputs.end()) {
            gate.setScheduledCycle(maxCycle);
            gate.setScheduled(false);
        }

        // 如果门已经被调度，直接返回其调度周期
        //if (gate.isScheduled()) {
        //    return gate.getScheduledCycle();
        //}

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
            maxInputCycle = std::max(std::max(maxInputCycle, inputCycle), gate.getScheduledCycle());
        }

        // 调度当前门
		int scheduledCycle = std::min(currentCycle, maxInputCycle + 1);
		if (gate.getScheduledCycle() == -1) {
            scheduledCycle = std::min(currentCycle, maxInputCycle + 1);
        } else{
			scheduledCycle = std::min(std::min(currentCycle, maxInputCycle + 1), gate.getScheduledCycle());
        }
        
        gate.setScheduledCycle(scheduledCycle);
        gate.setScheduled(false);
		//对于重复的gate，删除gate再添加gate
        auto& gatesInCycle = gatesWithCycles[scheduledCycle];
        
        // 遍历整个 unordered_map，查找重复的 Gate
        for (auto it = gatesWithCycles.begin(); it != gatesWithCycles.end();) {
            auto& gatesInCycle = it->second;

            // 查找是否有相同输出的 Gate
            auto gateIt = std::find_if(gatesInCycle.begin(), gatesInCycle.end(), [&gate](const Gate* g) {
                return g->getOutput() == gate.getOutput();
                });

            if (gateIt != gatesInCycle.end()) {
                // 如果找到相同输出的 Gate，比较 int 值
                if (it->first >= scheduledCycle) {
                    // 当前的 int 键较小，删除该 Gate
                    gatesInCycle.erase(gateIt);
                    ++it; // 移动到下一个元素
                }
                else {
                    // 当前 int 键较大，删除较大的 scheduledCycle 对应的 Gate
                    // 退出当前操作，因为我们已经保留了更大的
                }
            }
            else {
                ++it; // 如果没有找到相同的 Gate，继续遍历
            }
        }
        
        gatesInCycle.push_back(&gate);

        return scheduledCycle;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "错误：找不到门 " << gateName << std::endl;
        return -1;
    }

    
}
