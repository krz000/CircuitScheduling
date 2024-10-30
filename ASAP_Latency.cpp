#include"ASAP_Latency.h"
#include"ASAP.h"
#include <numeric>

void ASAP_L::schedule(Circuit& circuit) {
    std::vector<Gate>& gates = circuit.getGates();
    getScheduledGatesWithCycles().clear();
    ongoingGates.clear();

    // 初始化未调度索引
    std::vector<size_t> unscheduledIndices;
    unscheduledIndices.resize(gates.size());
    std::iota(unscheduledIndices.begin(), unscheduledIndices.end(), 0);

    // 首先处理输入门
    const auto& circuitInputs = circuit.getInputs();
    for (auto it = unscheduledIndices.begin(); it != unscheduledIndices.end();) {
        Gate& gate = gates[*it];
        if (gate.getInputs().empty() ||
            std::find(circuitInputs.begin(), circuitInputs.end(), gate.getOutput()) != circuitInputs.end()) {
            // 对于输入门，设置特殊的调度周期和状态
            gate.setScheduled(true);
            gate.setScheduledCycle(-1);  // 或者设置为0
            getScheduledGatesWithCycles()[-1].push_back(&gate);  // 将输入门放在特殊的周期-1中
            it = unscheduledIndices.erase(it);
        }
        else {
            ++it;
        }
    }

    int currentCycle = 0;
    while (!unscheduledIndices.empty() || !ongoingGates.empty()) {
        // 更新正在执行的门的状态
        updateOngoingGates(currentCycle);

        // 获取就绪的门
        std::vector<size_t> readyGatesIndices;
        for (auto it = unscheduledIndices.begin(); it != unscheduledIndices.end();) {
            Gate& gate = gates[*it];
            if (isGateReady(gate, circuit)) {
                readyGatesIndices.push_back(*it);
                it = unscheduledIndices.erase(it);
            }
            else {
                ++it;
            }
        }

        // 调度就绪的门
        for (size_t index : readyGatesIndices) {
            Gate& gate = gates[index];
            gate.setScheduled(true);
            gate.setScheduledCycle(currentCycle);
            getScheduledGatesWithCycles()[currentCycle].push_back(&gate);
            // 只有非输入门才需要考虑延迟和ongoing状态
            if (!gate.getInputs().empty()) {
                ongoingGates.push_back(&gate);
            }
        }

        currentCycle++;
    }

    totalCycles = currentCycle;
}

void ASAP_L::updateOngoingGates(int currentCycle) {
    auto it = ongoingGates.begin();
    while (it != ongoingGates.end()) {
        Gate* gate = *it;
        int finishTime = gate->getScheduledCycle() + getGateDelay(*gate);
        if (currentCycle >= finishTime) {
            it = ongoingGates.erase(it);
        }
        else {
            ++it;
        }
    }
}

bool ASAP_L::isGateReady(const Gate& gate, const Circuit& circuit) const {
    if (gate.isScheduled()) {
        return false;
    }

    // 如果是输入门，直接返回true
    if (gate.getInputs().empty()) {
        return true;
    }

    // 检查所有前驱是否都已被调度且已完成
    for (const std::string& inputName : gate.getInputs()) {
        try {
            const Gate& predGate = circuit.findGateByOutput(inputName);

            // 如果前驱未被调度，则当前门不就绪
            if (!predGate.isScheduled()) {
                return false;
            }

            // 如果前驱不是输入门，则需要检查其是否在ongoing状态
            if (!predGate.getInputs().empty()) {
                auto it = std::find_if(ongoingGates.begin(), ongoingGates.end(),
                    [&predGate](const Gate* g) { return g->getOutput() == predGate.getOutput(); });
                if (it != ongoingGates.end()) {
                    return false;
                }
            }
        }
        catch (const std::runtime_error& e) {
            // 如果是电路的主输入，继续检查下一个输入
            continue;
        }
    }
    return true;
}

int ASAP_L::getGateDelay(const Gate& gate) const {
    // 输入门没有延迟
    if (gate.getInputs().empty()) {
        return 0;
    }

    // 其他门的延迟
    switch (gate.getType()) {
    case GateType::AND: return 2;
    case GateType::OR: return 3;
    case GateType::NOT: return 1;
    default: return 0;
    }
}

bool ASAP_L::areAllPredecessorsScheduled_L(const Gate& gate, const Circuit& circuit) const {
    return isGateReady(gate, circuit);
}