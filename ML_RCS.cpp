#include "ML_RCS.h"
#include "Circuit.h"
#include <algorithm>
#include <numeric>

MLRCSScheduler::MLRCSScheduler(int and_gates, int or_gates, int not_gates)
    : resources{ and_gates, or_gates, not_gates }, currentCycle(0) {}

void MLRCSScheduler::schedule(Circuit& circuit) {
    std::vector<Gate>& unScheduledGates = circuit.getGates();
    currentCycle = 0;

    // 初始化未调度索引
    std::vector<size_t> unscheduledIndices;
    unscheduledIndices.resize(unScheduledGates.size());
    std::iota(unscheduledIndices.begin(), unscheduledIndices.end(), 0);

    // 首先处理输入门
    const auto& circuitInputs = circuit.getInputs();
    for (auto it = unscheduledIndices.begin(); it != unscheduledIndices.end();) {
        Gate& gate = unScheduledGates[*it];
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
    while (!unScheduledGates.empty() ) {

       

        updateOngoingGates();
        updateReadyGates(unScheduledGates);
        scheduleGates(unScheduledGates);
        currentCycle++;
    }
}

void MLRCSScheduler::updateReadyGates(const std::vector<Gate>& gates) {
    for (const Gate& gate : gates) {
        if (!gate.isScheduled() && areAllPredecessorsScheduled(gate, circuit)) {
            readyGates.push_back(const_cast<Gate*>(&gate));
        }
    }
}

void MLRCSScheduler::updateOngoingGates() {
    for (auto it = ongoingGates.begin(); it != ongoingGates.end();) {
        Gate* gate = *it;
        if (currentCycle - gate->getScheduledCycle() >= getGateDelay(*gate)) {
            incrementResources(*gate);
            it = ongoingGates.erase(it);
        }
        else {
            ++it;
        }
    }
}

void MLRCSScheduler::scheduleGates(std::vector<Gate>& gates) {
    std::sort(readyGates.begin(), readyGates.end(),
        [this](Gate* a, Gate* b) { return getGateDelay(*a) > getGateDelay(*b); });

    for (auto it = readyGates.begin(); it != readyGates.end();) {
        Gate* gate = *it;
        if (canScheduleGate(*gate)) {
            gate->setScheduled(true);
            gate->setScheduledCycle(currentCycle);
            ongoingGates.push_back(gate);
            getScheduledGatesWithCycles()[currentCycle].push_back(gate);
            decrementResources(*gate);
            it = readyGates.erase(it);
            gates.erase(std::remove_if(gates.begin(), gates.end(),
                [gate](const Gate& g) { return &g == gate; }), gates.end());
        }
        else {
            ++it;
        }
    }
}

bool MLRCSScheduler::areAllPredecessorsScheduled(const Gate& gate, const Circuit& circuit) const {
    for (const std::string& inputName : gate.getInputs()) {
        try {
            const Gate& inputGate = circuit.findGateByOutput(inputName);
            if (!inputGate.isScheduled()) {
                return false;
            }
        }
        catch (const std::runtime_error& e) {
            // 输入可能是电路的主输入，不需要调度
            continue;
        }
    }
    return true;
}

int MLRCSScheduler::getGateDelay(const Gate& gate) const {
    switch (gate.getType()) {
    case GateType::AND: return 2;
    case GateType::OR: return 3;
    case GateType::NOT: return 1;
    default: return 0;
    }
}

bool MLRCSScheduler::canScheduleGate(const Gate& gate) const {
    switch (gate.getType()) {
    case GateType::AND: return resources.and_gates > 0;
    case GateType::OR: return resources.or_gates > 0;
    case GateType::NOT: return resources.not_gates > 0;
    default: return false;
    }
}

void MLRCSScheduler::decrementResources(const Gate& gate) {
    switch (gate.getType()) {
    case GateType::AND: resources.and_gates--; break;
    case GateType::OR: resources.or_gates--; break;
    case GateType::NOT: resources.not_gates--; break;
    }
}

void MLRCSScheduler::incrementResources(const Gate& gate) {
    switch (gate.getType()) {
    case GateType::AND: resources.and_gates++; break;
    case GateType::OR: resources.or_gates++; break;
    case GateType::NOT: resources.not_gates++; break;
    }
}