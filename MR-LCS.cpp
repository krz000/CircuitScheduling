#include "Circuit.h"
#include "MR-LCS.h"
#include "ALAP.h"
#include "ASAP.h"

/*
* ˼·��
* 1. ��ͬ��Դ����
* ÿ����Դ�����в�ͬ��2.4 2.5
* 2. ÿ��gate���ĵ���Դ����ͬ
* ��Դ�� = gateType & 2 | 3 ! 1
* �������
* 
* 
*/

void MR_LCS::schedule(Circuit& circuit) {
    // ���� MR-RCS ����Ҫ���Ⱥ���
}

void MR_LCS::MR_LCSschedule(Circuit& circuit, int timeLimit) {
    // 1. ��ʼ�� �Ƿ񴫲μ���ʱ������? ��Դ����?
    std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles = getScheduledGatesWithCycles();
    gatesWithCycles.clear();

	// 2. ���� 2.4 ʱ�� �õ�����Դ���µ��������
    //  maxcycle = mintime
    // 3. ��� ����ʱ���޶� �Ŀ�����
    
    //int minTime = MR_RCS();
	int minTime = 5;
	// ����ʱ���MR_RCS����Сʱ��Ƚ� С��˵������������
    if (timeLimit < minTime) {
        throw std::runtime_error("���Ȳ����У�ALAP ʱ��С�� 0");
    }

    // 4. ���������ŵĵ���״̬
    for (auto& gate : circuit.getGates()) {
         gate.setScheduledCycle(-1);
         gate.setScheduled(false);
    }

	int maxCycle = minTime;
    // �������ʼ�������
    std::vector<std::string> outputs = circuit.getOutputs();
    for (const auto& output : outputs) {
        scheduleGate(circuit, output, maxCycle);
    }
    
    
    // 5. ��ʼ���ȹ���
    //int currentCycle = 0;
    //bool allScheduled = false;

    //while (!allScheduled) {
        //// 5.1 �ҳ���ǰ�ɵ��ȵ��ţ�����״̬���ţ�
        //std::vector<Gate*> readyGates = findReadyGates(circuit);

        //// 5.2 ����ÿ�������ŵ� slack
        //std::vector<std::pair<Gate*, int>> gateSlacks;
        //for (Gate* gate : readyGates) {
        //    int slack = calculateSlack(*gate, currentCycle);
        //    gateSlacks.push_back({ gate, slack });
        //}

        //// 5.3 ���ȵ��� slack Ϊ 0 ����
        //scheduleZeroSlackGates(gateSlacks, currentCycle, gatesWithCycles);

        //// 5.4 ����ԴԼ���µ��ȶ������
        //scheduleAdditionalGates(gateSlacks, currentCycle, gatesWithCycles);

        //// 5.5 ����Ƿ������Ŷ��ѵ���
        //allScheduled = checkAllScheduled(circuit);

        //// 5.6 ������һ������
        //currentCycle++;
    //}
}

int MR_LCS::scheduleGate(Circuit& circuit, const std::string& gateName, int currentCycle) {
    //�洢��
    std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles = getScheduledGatesWithCycles();

    try {
        Gate& gate = const_cast<Gate&>(circuit.findGateByOutput(gateName));

        // ���������ţ�����β���ȳ��е���

        // ����������ţ������ڵ�-1����
        const auto& inputs = circuit.getInputs();
        if (std::find(inputs.begin(), inputs.end(), gateName) != inputs.end()) {
            gate.setScheduled(true);
            gate.setScheduledCycle(-1);
            return 0;
        }

		// �������γ��е��ȣ�
        // ����ʲôʱ�������


        auto& gatesInCycle = gatesWithCycles[scheduledCycle];

        gatesInCycle.push_back(&gate);

        return scheduledCycle;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "�����Ҳ����� " << gateName << std::endl;
        return -1;
    }
}

