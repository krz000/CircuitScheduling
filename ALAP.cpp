#include "circuit.h"
#include "ALAP.h"
#include "ASAP.h"

/*
˼·��
1. ����ASAP���cycle�������õ����Ľ�����ҵ�����ǰ������Ϊalap���һ�ֵ�scheduled�ڵ㣬Ҳ����cycle���
2. ѭ�����ҵ�alap�Ѿ�scheduled�Ľڵ㣬�����ǵ�ǰ���ڵ���Ϊǰһ�ֵ�scheduled�ڵ㣬cycle=-1��ֱ��cycle���0
3. ��ÿ��output�������ܵõ��ڵ��cycle����ÿ�αȽ�cycle���õ���С��cycle��

����ASAP֮������¸���bool
    int scheduledCycle = -1;//��n��
    bool Scheduled = false

ѭ��һ�飬scheduled=true

�ҵ�circuit��output����������Ϊ���Ľڵ㣬cycle��Ϊmaxcycle��scheduled�ĳ�true

ѭ����
�õ�this.gate.input.cycle=max-1��

*/




void ALAP::schedule(Circuit& circuit)
{
    ASAPScheduler asapScheduler;
    asapScheduler.schedule(circuit);
}

int maxCycle;

void ALAP::ALAPschedule(Circuit& circuit) {
	// ��ȡ�������
    maxCycle = circuit.getMaxScheduledCycle();
    // ���������ŵĵ���״̬
    for (auto& gate : circuit.getGates()) {
        gate.setScheduledCycle(-1);
        gate.setScheduled(false);
    }

    // �������ʼ�������
    std::vector<std::string> outputs = circuit.getOutputs();
    for (const auto& output : outputs) {
        scheduleGate(circuit, output, maxCycle);
    }

	// ɾ���ظ�����
    //removeDuplicateGates(circuit);

    // ���ÿ�� gate �ĵ�������
    //std::cout << "�ŵ��Ƚ�� (ALAP):" << std::endl;
    //for (const auto& gate : circuit.getGates()) {
    //    std::cout << "�� " << gate.getGateId() << " ���������� " << gate.getScheduledCycle() << std::endl;
    //}
}

// �Ƴ��ظ�����
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
//    // �Ƴ�δ���ȵ���
//    gates.erase(std::remove_if(gates.begin(), gates.end(),
//        [](const Gate& g) { return !g.isScheduled(); }), gates.end());
//}

int ALAP::scheduleGate(Circuit& circuit, const std::string& gateName, int currentCycle) {

    //�洢��
    std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles = getScheduledGatesWithCycles();
    
    try {
        Gate& gate = const_cast<Gate&>(circuit.findGateByOutput(gateName));

		// ���������ţ��������������һ������
        const auto& outputs = circuit.getOutputs();
        if (std::find(outputs.begin(), outputs.end(), gateName) != outputs.end()) {
            gate.setScheduledCycle(maxCycle);
            gate.setScheduled(false);
        }

        // ������Ѿ������ȣ�ֱ�ӷ������������
        //if (gate.isScheduled()) {
        //    return gate.getScheduledCycle();
        //}

        // ����������ţ������ڵ�-1����
        const auto& inputs = circuit.getInputs();
        if (std::find(inputs.begin(), inputs.end(), gateName) != inputs.end()) {
            gate.setScheduled(true);
            gate.setScheduledCycle(-1);
            return 0;
        }

        // �ݹ��������������
        int maxInputCycle = -1;
        for (const auto& input : gate.getInputs()) {
            int inputCycle = scheduleGate(circuit, input, currentCycle - 1);
            maxInputCycle = std::max(std::max(maxInputCycle, inputCycle), gate.getScheduledCycle());
        }

        // ���ȵ�ǰ��
		int scheduledCycle = std::min(currentCycle, maxInputCycle + 1);
		if (gate.getScheduledCycle() == -1) {
            scheduledCycle = std::min(currentCycle, maxInputCycle + 1);
        } else{
			scheduledCycle = std::min(std::min(currentCycle, maxInputCycle + 1), gate.getScheduledCycle());
        }
        
        gate.setScheduledCycle(scheduledCycle);
        gate.setScheduled(false);
		//�����ظ���gate��ɾ��gate�����gate
        auto& gatesInCycle = gatesWithCycles[scheduledCycle];
        
        // �������� unordered_map�������ظ��� Gate
        for (auto it = gatesWithCycles.begin(); it != gatesWithCycles.end();) {
            auto& gatesInCycle = it->second;

            // �����Ƿ�����ͬ����� Gate
            auto gateIt = std::find_if(gatesInCycle.begin(), gatesInCycle.end(), [&gate](const Gate* g) {
                return g->getOutput() == gate.getOutput();
                });

            if (gateIt != gatesInCycle.end()) {
                // ����ҵ���ͬ����� Gate���Ƚ� int ֵ
                if (it->first >= scheduledCycle) {
                    // ��ǰ�� int ����С��ɾ���� Gate
                    gatesInCycle.erase(gateIt);
                    ++it; // �ƶ�����һ��Ԫ��
                }
                else {
                    // ��ǰ int ���ϴ�ɾ���ϴ�� scheduledCycle ��Ӧ�� Gate
                    // �˳���ǰ��������Ϊ�����Ѿ������˸����
                }
            }
            else {
                ++it; // ���û���ҵ���ͬ�� Gate����������
            }
        }
        
        gatesInCycle.push_back(&gate);

        return scheduledCycle;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "�����Ҳ����� " << gateName << std::endl;
        return -1;
    }

    
}
