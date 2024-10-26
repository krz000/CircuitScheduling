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

void ALAP::ALAPschedule(Circuit& circuit) {
	// ��ȡ�������
	int maxCycle = circuit.getMaxScheduledCycle();
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

    // ���ÿ�� gate �ĵ�������
    //std::cout << "�ŵ��Ƚ�� (ALAP):" << std::endl;
    //for (const auto& gate : circuit.getGates()) {
    //    std::cout << "�� " << gate.getGateId() << " ���������� " << gate.getScheduledCycle() << std::endl;
    //}

}

int ALAP::scheduleGate(Circuit& circuit, const std::string& gateName, int currentCycle) {
    try {
        Gate& gate = const_cast<Gate&>(circuit.findGateByOutput(gateName));

        // ������Ѿ������ȣ�ֱ�ӷ������������
        if (gate.isScheduled()) {
            return gate.getScheduledCycle();
        }

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
            maxInputCycle = std::max(maxInputCycle, inputCycle);
        }

        // ���ȵ�ǰ��
        int scheduledCycle = std::min(currentCycle, maxInputCycle + 1);
        gate.setScheduled(true);
        gate.setScheduledCycle(scheduledCycle);

        return scheduledCycle;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "�����Ҳ����� " << gateName << std::endl;
        return -1;
    }
}
