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
* 
* 
* 
* ���ȿ���û��ĳһ���͵��ţ��в���Դ��ֵ1������0
* 
* asap�õ�����ʹ�õ�cycle
* alap�õ������cycle��ͬʱҲ��Ϊ���ȼ���ԽСԽ����
* ��ȵľ���ȷ����gate������ȵľ��Ǹ���gate������alap��cycle��������
* 
* ����need��Դ����
* col = cycle��row = gateType
* ����-����֮�䣬�ڶ�Ӧ��type��һ
* ȷ������ʹ��������Դ++����Χ��col = cycle ��currentCycle��gateType + currentCycle
* 
* ���Խ����������������
* ����+=1�����������Դ������cycle++
* �������������ȷ��Ϊ��һ����
* ���cycle==alap��cycle�������Դ++
* 
*/

void MR_LCS::schedule(Circuit& circuit) {
    // ����MR_RCS�������õ�minCycle
    // timeLimit = targetCycle;
  //  while (minCycle > timeLimit) {
		//int _and = 0, _or = 0, _not = 0;
		//int _sum = _and + _or + _not;
		//minCycle = MR_RCS(circuit, _and, _or, _not);
  //      // �㷨�Ż�
  //      // 000��ʼ ��������Դ��Ϊ_sum���ֱ���䣬�õ���С��cycle����¼�����ŵ���Դ��
		//// A(n, 3) = n(n-1)(n-2)/6;
  //  }
}

void MR_LCS::MR_LCSscheduleBF(Circuit& circuit, int timeLimit) {
    //�ж�������Դ�����ִ�
    
    //const int minCycle = ML_RCS({ INF, INF, INF });
	int minCycle = 0;
    //������Դ���ִδ���Ҫ���������

    if (minCycle > timeLimit) {
        //����
		throw std::runtime_error("�޷��ڸ���ʱ����������ɵ���");
    }

    //���������ű�������
    std::array<int, 3> gateNum = { 0, 0, 0 };
    for (auto& gate : circuit.getGates()) {
        switch (gate.getType()) {
        case GateType::AND: ++gateNum[0]; break;
        case GateType::OR: ++gateNum[1]; break;
        case GateType::NOT: ++gateNum[2]; break;
        }
    }
    //�ų�����Ҫ����
    std::array<bool, 3> needGate = { true, true, true };
    for (int i = 0; i < gateNum.size(); ++i) {
        if (gateNum[i] == 0) needGate[i] = false;
    }
    //�������Ҫĳ���ţ�������Դ��0
    // ��Դ��
    std::array<int, 3> resourseNum = { 0, 0, 0 };
    for (int i = 0; i < resourseNum.size(); ++i) {
        if (needGate[i]) resourseNum[i] = 1;
    }

    //int cycle = ML_RCS(resourseNum);
    //if (cycle <= timeLimit);
	// ȷ����Դ��

    // ����С��Դ����ʼ����
    int totalResource = 3; // ����ÿ����1����Դ
    bool found = false;
    std::array<int, 3> bestAllocation = { 1, 1, 1 };

    while (!found) {
        // ���Ե�ǰ����Դ�������з��䷽��
        for (int and_res = 1; and_res <= totalResource - 2; ++and_res) {
            for (int or_res = 1; or_res <= totalResource - and_res - 1; ++or_res) {
                int not_res = totalResource - and_res - or_res;
                if (not_res < 1) continue;

                std::array<int, 3> currentAlloc = { and_res, or_res, not_res };
                // ��������Ҫ��������
                bool valid = true;
                for (int i = 0; i < 3; ++i) {
                    if (!needGate[i] && currentAlloc[i] > 0) {
                        valid = false;
                        break;
                    }
                }
                if (!valid) continue;

                // ��鵱ǰ�����Ƿ�����Ŀ������
                //int cycle = ML_RCS(currentAlloc);
				int cycle = 0;
                if (cycle <= timeLimit) {
                    found = true;
                    bestAllocation = currentAlloc;
                    break;
                }
            }
            if (found) break;
        }
        if (!found) totalResource++;
    }

}

//void MR_LCS::MR_LCSschedule(Circuit& circuit, int timeLimit) {
//    // 1. ��ʼ�� ���μ���ʱ������
//    std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles = getScheduledGatesWithCycles();
//    gatesWithCycles.clear();
//
//	// 2. ���� 2.4 ʱ�� �õ�����Դ���µ��������
//    //  maxcycle = mintime
//    // 3. ��� ����ʱ���޶� �Ŀ�����
//    // 
//	// ALAP�ӳٰ�õ� minTime + ����˳��
//    // 
//    // int minTime = MR_RCS();
//	int minTime = 5;
//	// ����ʱ���MR_RCS����Сʱ��Ƚ� С��˵������������
//    if (timeLimit < minTime) {
//        throw std::runtime_error("���Ȳ����У�ALAP ʱ��С�� 0");
//    }
//
//    // 4. ���������ŵĵ���״̬
//    for (auto& gate : circuit.getGates()) {
//         gate.setScheduledCycle(-1);
//         gate.setScheduled(false);
//    }
//
//    // ��Դ��
//    _And _and(0);
//	//_and.freeResource();
//	int _and = 0, _or = 0, _not = 0;
//	
//
//	int maxCycle = minTime;
//    // �������ʼ�������
//    std::vector<std::string> outputs = circuit.getOutputs();
//    for (const auto& output : outputs) {
//        scheduleGate(circuit, output, maxCycle);
//    }
//    
//    
//    // 5. ��ʼ���ȹ���
//    int currentCycle = 0;
//    bool allScheduled = false;
//
//
//    while (!allScheduled) {
//        // 5.1 �ҳ���ǰ�ɵ��ȵ��ţ�����״̬���ţ�
//        std::vector<Gate*> readyGates = findReadyGates(circuit);
//        
//        // 5.2 ����ÿ�������ŵ� slack=�������ʱ��-��ǰʱ��
//        std::vector<std::pair<Gate*, int>> gateSlacks;
//        for (Gate* gate : readyGates) {
//            int slack = calculateSlack(*gate, currentCycle);
//            gateSlacks.push_back({ gate, slack });
//        }
//
//        // 5.3 ���ȵ��� slack Ϊ 0 ����
//        scheduleZeroSlackGates(gateSlacks, currentCycle, gatesWithCycles);
//
//        // 5.4 ����ԴԼ���µ��ȶ������
//        scheduleAdditionalGates(gateSlacks, currentCycle, gatesWithCycles);
//
//        // 5.5 ����Ƿ������Ŷ��ѵ���
//        allScheduled = checkAllScheduled(circuit);
//
//        // 5.6 ������һ������
//        currentCycle++;
//    }
//}
//
//int MR_LCS::scheduleGate(Circuit& circuit, const std::string& gateName, int currentCycle) {
//    //�洢��
//    std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles = getScheduledGatesWithCycles();
//
//  //  try {
//  //      Gate& gate = const_cast<Gate&>(circuit.findGateByOutput(gateName));
//
//  //      // ���������ţ�����β���ȳ��е���
//
//  //      // ����������ţ������ڵ�-1����
//  //      const auto& inputs = circuit.getInputs();
//  //      if (std::find(inputs.begin(), inputs.end(), gateName) != inputs.end()) {
//  //          gate.setScheduled(true);
//  //          gate.setScheduledCycle(-1);
//  //          return 0;
//  //      }
//
//		//// �������γ��е��ȣ�
//
//
//  //      auto& gatesInCycle = gatesWithCycles[scheduledCycle];
//
//  //      // ɾ���ظ���
//
//        //gatesInCycle.push_back(&gate);
//
//  //      return scheduledCycle;
//  //  }
//  //  catch (const std::runtime_error& e) {
//  //      std::cerr << "�����Ҳ����� " << gateName << std::endl;
//  //      return -1;
//  //  }
//}
//
//
//std::vector<Gate*> MR_LCS::findReadyGates(Circuit& circuit) {
//    // ����״̬��: û��scheduled+ǰ�����Ѿ�scheduled
//    // ����Ҫ���򣬰���˳��
//        std::vector<Gate*> readyGates;
//        std::vector<Gate>& gates = circuit.getGates();
//
//        for (Gate& gate : gates) {
//            // �����Ѿ����ȵ���
//            if (gate.isScheduled()) {
//                continue;
//            }
//
//            // ������������Ƿ����
//            bool allInputsScheduled = true;
//            const std::vector<std::string>& inputs = gate.getInputs();
//
//            for (const std::string& input : inputs) {
//                // ����Ƿ���������
//                bool isPrimaryInput = false;
//                for (const std::string& primaryInput : circuit.getInputs()) {
//                    if (input == primaryInput) {
//                        isPrimaryInput = true;
//                        break;
//                    }
//                }
//
//                // ����������룬���������һ������
//                if (isPrimaryInput) {
//                    continue;
//                }
//
//                try {
//                    // ���������Ӧ����
//                    const Gate& inputGate = circuit.findGateByOutput(input);
//                    // ���������δ���ȣ����Ϊδ����
//                    if (!inputGate.isScheduled()) {
//                        allInputsScheduled = false;
//                        break;
//                    }
//                }
//                catch (const std::runtime_error& e) {
//                    // ����Ҳ�����Ӧ���ţ������������룬�������
//                    continue;
//                }
//            }
//
//            // ����������붼�Ѿ�������������ӵ������б�
//            if (allInputsScheduled) {
//                readyGates.push_back(&gate);
//            }
//        }
//
//        return readyGates;
//}
//
//
//
//int MR_LCS::calculateSlack(const Gate& gate, int currentCycle) {
//    // ���� slack: ��MR_RCS�õ�����cycle�� 
//    
//    // 1. ��ȡ���ŵ�ALAPʱ��
//    int alapTime = gate.getScheduledCycle();
//    
//    // 2. ����Ż�û��ALAPʱ�䣬����һ���ϴ��slackֵ
//    // �����Ŵ���
//    //if (alapTime == -1) {
//    //    return INT_MAX;
//    //}
//
//    // 3. ����slack = ALAPʱ�� - ��ǰʱ��
//    int slack = alapTime - currentCycle;
//
//    // 4. ȷ��slack��Ϊ��
//    return std::max(0, slack);
//}
//
//
//void MR_LCS::scheduleZeroSlackGates(std::vector<std::pair<Gate*, int>>& gateSlacks,
//    int currentCycle,
//    std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles) {
//    // ʵ�ֵ��� slack Ϊ 0 ���ŵ��߼�
//    // 1. ͳ�Ƶ�ǰ���ڸ������ŵ���Դʹ����� ��ʹ��
//    // ��
//
//    int andGates = 0, orGates = 0, notGates = 0;
//    if (gatesWithCycles.find(currentCycle) != gatesWithCycles.end()) {
//        for (Gate* gate : gatesWithCycles[currentCycle]) {
//            switch (gate->getType()) {
//            case GateType::AND: andGates++; break;
//            case GateType::OR:  orGates++;  break;
//            case GateType::NOT: notGates++; break;
//            default: break;
//            }
//        }
//    }
//
//    // 2. ��������slackΪ0����
//    auto it = gateSlacks.begin();
//    while (it != gateSlacks.end()) {
//        Gate* gate = it->first;
//        int slack = it->second;
//
//        if (slack == 0) {
//            bool canSchedule = false;
//            // �����Դ����
//            switch (gate->getType()) {
//            case GateType::AND:
//                if (andGates < resourceLimit._add) {
//                    andGates++;
//					//resourceLimit._and--;
//                    canSchedule = true;
//                }
//                break;
//            case GateType::OR:
//                if (orGates < resourceLimit._or) {
//                    orGates++;
//                    canSchedule = true;
//                }
//                break;
//            case GateType::NOT:
//                if (notGates < resourceLimit._not) {
//                    notGates++;
//                    canSchedule = true;
//                }
//                break;
//            default:
//                break;
//            }
//
//            if (canSchedule) {
//                // ������
//                gate->setScheduled(true);
//                gate->setScheduledCycle(currentCycle);
//                gatesWithCycles[currentCycle].push_back(gate);
//
//                // �Ӵ������б����Ƴ�
//                it = gateSlacks.erase(it);
//            }
//            else {
//                // ����޷����ȣ��׳��쳣�������
//                throw std::runtime_error("�޷�����slackΪ0���ţ�xx��Դ����");
//            }
//        }
//        else {
//            ++it;
//        }
//    }
//}
//
//void MR_LCS::scheduleAdditionalGates(std::vector<std::pair<Gate*, int>>& gateSlacks,
//    int currentCycle,
//    std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles) {
//    // ʵ������ԴԼ���µ��ȶ����ŵ��߼�
//    // 1. ͳ�Ƶ�ǰ������ʹ�õ���Դ
//    int andGates = 0, orGates = 0, notGates = 0;
//    if (gatesWithCycles.find(currentCycle) != gatesWithCycles.end()) {
//        for (Gate* gate : gatesWithCycles[currentCycle]) {
//            switch (gate->getType()) {
//            case GateType::AND: andGates++; break;
//            case GateType::OR:  orGates++;  break;
//            case GateType::NOT: notGates++; break;
//            default: break;
//            }
//        }
//    }
//
//    // 2. ����slackֵ�������ȵ���slackС���ţ�
//    std::sort(gateSlacks.begin(), gateSlacks.end(),
//        [](const auto& a, const auto& b) {
//            return a.second < b.second;
//        });
//
//    // 3. ���Ե��ȶ������
//    // gate ��gateready����
//    auto it = gateSlacks.begin();
//    while (it != gateSlacks.end()) {
//        Gate* gate = it->first;
//        bool canSchedule = false;
//
//        // �����Դ����
//        switch (gate->getType()) {
//        case GateType::AND:
//            if (andGates < resourceLimit) {
//                andGates++;
//                canSchedule = true;
//            }
//            break;
//        case GateType::OR:
//            if (orGates < resourceLimit) {
//                orGates++;
//                canSchedule = true;
//            }
//            break;
//        case GateType::NOT:
//            if (notGates < resourceLimit) {
//                notGates++;
//                canSchedule = true;
//            }
//            break;
//        default:
//            break;
//        }
//
//        if (canSchedule) {
//            // ������
//            gate->setScheduled(true);
//            gate->setScheduledCycle(currentCycle);
//            gatesWithCycles[currentCycle].push_back(gate);
//
//            // �Ӵ������б����Ƴ�
//            it = gateSlacks.erase(it);
//        }
//        else {
//            // ����޷����ȣ��ƶ�����һ����
//            ++it;
//        }
//    }
//}
//
//bool MR_LCS::checkAllScheduled(Circuit& circuit) {
//    // ʵ�ּ���Ƿ������Ŷ��ѵ��ȵ��߼�
//        // ��ȡ������
//    std::vector<Gate>& gates = circuit.getGates();
//
//    // ���������ţ�����Ƿ��ѵ���
//    for (const Gate& gate : gates) {
//        // ���������ţ����ǵ�scheduledCycleΪ-1��
//        const auto& inputs = circuit.getInputs();
//        if (std::find(inputs.begin(), inputs.end(), gate.getOutput()) != inputs.end()) {
//            continue;
//        }
//
//        // ����ҵ�δ���ȵ��ţ�����false
//        if (!gate.isScheduled() || gate.getScheduledCycle() == -1) {
//            return false;
//        }
//    }
//
//    // �����Ŷ��ѵ���
//    return true;
//}