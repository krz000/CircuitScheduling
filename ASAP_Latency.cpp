#include"ASAP_Latency.h"
#include"ASAP.h"
#include <numeric>

void ASAP_L::schedule(Circuit& circuit) {
    std::vector<Gate>& gates = circuit.getGates();
    getScheduledGatesWithCycles().clear();
    ongoingGates.clear();

    // ��ʼ��δ��������
    std::vector<size_t> unscheduledIndices;
    unscheduledIndices.resize(gates.size());
    std::iota(unscheduledIndices.begin(), unscheduledIndices.end(), 0);

    // ���ȴ���������
    const auto& circuitInputs = circuit.getInputs();
    for (auto it = unscheduledIndices.begin(); it != unscheduledIndices.end();) {
        Gate& gate = gates[*it];
        if (gate.getInputs().empty() ||
            std::find(circuitInputs.begin(), circuitInputs.end(), gate.getOutput()) != circuitInputs.end()) {
            // ���������ţ���������ĵ������ں�״̬
            gate.setScheduled(true);
            gate.setScheduledCycle(-1);  // ��������Ϊ0
            getScheduledGatesWithCycles()[-1].push_back(&gate);  // �������ŷ������������-1��
            it = unscheduledIndices.erase(it);
        }
        else {
            ++it;
        }
    }

    int currentCycle = 0;
    while (!unscheduledIndices.empty() || !ongoingGates.empty()) {
        // ��������ִ�е��ŵ�״̬
        updateOngoingGates(currentCycle);

        // ��ȡ��������
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

        // ���Ⱦ�������
        for (size_t index : readyGatesIndices) {
            Gate& gate = gates[index];
            gate.setScheduled(true);
            gate.setScheduledCycle(currentCycle);
            getScheduledGatesWithCycles()[currentCycle].push_back(&gate);
            // ֻ�з������Ų���Ҫ�����ӳٺ�ongoing״̬
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

    // ����������ţ�ֱ�ӷ���true
    if (gate.getInputs().empty()) {
        return true;
    }

    // �������ǰ���Ƿ��ѱ������������
    for (const std::string& inputName : gate.getInputs()) {
        try {
            const Gate& predGate = circuit.findGateByOutput(inputName);

            // ���ǰ��δ�����ȣ���ǰ�Ų�����
            if (!predGate.isScheduled()) {
                return false;
            }

            // ���ǰ�����������ţ�����Ҫ������Ƿ���ongoing״̬
            if (!predGate.getInputs().empty()) {
                auto it = std::find_if(ongoingGates.begin(), ongoingGates.end(),
                    [&predGate](const Gate* g) { return g->getOutput() == predGate.getOutput(); });
                if (it != ongoingGates.end()) {
                    return false;
                }
            }
        }
        catch (const std::runtime_error& e) {
            // ����ǵ�·�������룬���������һ������
            continue;
        }
    }
    return true;
}

int ASAP_L::getGateDelay(const Gate& gate) const {
    // ������û���ӳ�
    if (gate.getInputs().empty()) {
        return 0;
    }

    // �����ŵ��ӳ�
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