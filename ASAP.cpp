#include"Circuit.h"
#include"ASAP.h"

//ʵ��ASAPScheduler��ķ���
bool ASAPScheduler::areAllPredecessorsScheduled(const Gate& gate, const Circuit& circuit) const {
    for (const std::string& inputName : gate.getInputs()) {
        // ��������Ƿ�Ϊ��·����Ҫ����
        if (std::find(circuit.getInputs().begin(), circuit.getInputs().end(), inputName) != circuit.getInputs().end()) {
            continue;  // ��Ҫ�������Ǳ���Ϊ���ѵ��ȵ�
        }

        // ���Ҳ�������������
        const Gate* predecessorGate = circuit.findGateByOutput(inputName);
        if (predecessorGate && !predecessorGate->isScheduled()) {
            return false;  // �����ǰ����δ�����ȣ�����false
        }
    }
    return true;  // ����ǰ���Ŷ��ѱ�����
}


int ASAPScheduler::getMaxPredecessorCycle(const Gate& gate, const Circuit& circuit) const {
    int maxCycle = -1;
    for (const std::string& inputName : gate.getInputs()) {
        // ��������Ƿ�Ϊ��·����Ҫ����
        if (std::find(circuit.getInputs().begin(), circuit.getInputs().end(), inputName) != circuit.getInputs().end()) {
            maxCycle = std::max(maxCycle, 0);  // ��Ҫ���������Ϊ0
            continue;
        }

        // ���Ҳ�������������
        const Gate* predecessorGate = circuit.findGateByOutput(inputName);
        if (predecessorGate) {
            maxCycle = std::max(maxCycle, predecessorGate->getScheduledCycle());
        }
    }
    return maxCycle;
}
void ASAPScheduler::schedule(Circuit& circuit) {
    // ��ʼ��
    scheduledGates.clear();
    totalCycles = 0;

    // ��ȡ������
    std::vector<Gate>& unscheduledGates = circuit.getGates();
    std::vector<Gate> scheduledGates;

    // ʵ��ASAP�㷨
    bool allScheduled = false;
    while (!allScheduled) {
        allScheduled = true;

        //��������δ�����ȵ���
        for (Gate& gate : unscheduledGates) {
            //�ҵ�����input��,��ʱ�����ƣ�����input�����Ա�����
            if (gate.getInputs().size() == 0)
            {
                gate.setScheduledCycle(0);
                scheduledGates.push_back(gate);
                continue;
            }

            // �������ǰ�����Ƿ��ѱ�����
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