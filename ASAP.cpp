#include"Circuit.h"
#include"ASAP.h"

//实现ASAPScheduler类的方法
bool ASAPScheduler::areAllPredecessorsScheduled(const Gate& gate, const Circuit& circuit) const {
    for (const std::string& inputName : gate.getInputs()) {
        // 检查输入是否为电路的主要输入
        if (std::find(circuit.getInputs().begin(), circuit.getInputs().end(), inputName) != circuit.getInputs().end()) {
            continue;  // 主要输入总是被认为是已调度的
        }

        // 查找产生这个输入的门
        const Gate* predecessorGate = circuit.findGateByOutput(inputName);
        if (predecessorGate && !predecessorGate->isScheduled()) {
            return false;  // 如果有前驱门未被调度，返回false
        }
    }
    return true;  // 所有前驱门都已被调度
}


int ASAPScheduler::getMaxPredecessorCycle(const Gate& gate, const Circuit& circuit) const {
    int maxCycle = -1;
    for (const std::string& inputName : gate.getInputs()) {
        // 检查输入是否为电路的主要输入
        if (std::find(circuit.getInputs().begin(), circuit.getInputs().end(), inputName) != circuit.getInputs().end()) {
            maxCycle = std::max(maxCycle, 0);  // 主要输入的周期为0
            continue;
        }

        // 查找产生这个输入的门
        const Gate* predecessorGate = circuit.findGateByOutput(inputName);
        if (predecessorGate) {
            maxCycle = std::max(maxCycle, predecessorGate->getScheduledCycle());
        }
    }
    return maxCycle;
}
void ASAPScheduler::schedule(Circuit& circuit) {
    // 初始化
    scheduledGates.clear();
    totalCycles = 0;

    // 获取所有门
    std::vector<Gate>& unscheduledGates = circuit.getGates();
    std::vector<Gate> scheduledGates;

    // 实现ASAP算法
    bool allScheduled = false;
    while (!allScheduled) {
        allScheduled = true;

        //遍历所有未被调度的门
        for (Gate& gate : unscheduledGates) {
            //找到所有input门,此时无限制，所有input都可以被调度
            if (gate.getInputs().size() == 0)
            {
                gate.setScheduledCycle(0);
                scheduledGates.push_back(gate);
                continue;
            }

            // 检查所有前驱门是否已被调度
            if (areAllPredecessorsScheduled(gate, circuit)) {
                gate.setScheduledCycle(0);


                /*
                int maxPredCycle = getMaxPredecessorCycle(gate,circuit);
                int currentCycle = maxPredCycle + 1;
                gate.setScheduledCycle(currentCycle);
                scheduledGates[currentCycle].push_back(&gate);
                totalCycles = std::max(totalCycles, currentCycle);*/
            }
            else {
                allScheduled = false;
            }
        }
    }
}


void ASAPScheduler::printSchedule() const {
    std::cout << "Total " << totalCycles << " Cycles\n";
    for (int i = 0; i <= totalCycles; ++i) {
        std::cout << "Cycle " << i << ": ";
        printGatesOfType(i, GateType::AND);
        printGatesOfType(i, GateType::OR);
        printGatesOfType(i, GateType::NOT);
        std::cout << "\n";
    }
}

void ASAPScheduler::printGatesOfType(int cycle, GateType type) const {
    std::cout << "{";
    bool first = true;
    for (const Gate* gate : scheduledGates.at(cycle)) {
        if (gate->getType() == type) {
            if (!first) std::cout << " ";
            std::cout << gate->getName();
            first = false;
        }
    }
    std::cout << "}, ";
}