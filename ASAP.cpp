#include"Circuit.h"
#include"ASAP.h"
#include <numeric>
#include <algorithm>

//ʵ��ASAPScheduler��ķ���
bool ASAPScheduler::areAllPredecessorsScheduled(const Gate& gate, const Circuit& circuit) const {
    bool flag = true;
    for (const std::string& inputName : gate.getInputs())
    {
       
        try {
            const Gate& curGate = circuit.findGateByOutput(inputName);
            // ʹ�� curGate
            bool tmpFlag = curGate.isScheduled();
            flag = flag && tmpFlag;
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            // ����������
        }

    
       
    }
        
    return flag;  // ����ǰ���Ŷ��ѱ�����
}


void ASAPScheduler::schedule(Circuit& circuit) {
    // ��ʼ��
    getScheduledGatesWithCycles().clear();
    totalCycles = 0;

    // ��ȡ������
    std::vector<Gate>& gates = circuit.getGates();//��ȡ�����ŵ����ã��Ա��ڶ����޸�
    std::vector<Gate> scheduledGates;//����Ѿ������ȵ���


    // ��ʼ��δ��������
    unscheduledIndices.resize(gates.size());
    std::iota(unscheduledIndices.begin(), unscheduledIndices.end(), 0);//������ʽ������������ֵ�����Ŵ洢λ��

    //��ʼ��gatesWithCycles
    std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles = getScheduledGatesWithCycles();


    // ʵ��ASAP�㷨
    int cycleNum = 0;//��¼��ǰ������
    while (!unscheduledIndices.empty()) {
        // �洢�����ڿ��Ե��ȵ��ŵ�����
        std::vector<size_t> readyGatesIndices;

        // ��������δ���ȵ���
        for (auto it = unscheduledIndices.begin(); it != unscheduledIndices.end();) {
            Gate& gate = gates[*it];
            if (gate.getInputs().empty()) {
                // �����û�����룬������������
                gate.setScheduledCycle(cycleNum);
                gate.setScheduled(true);
                scheduledGates.push_back(gate);
                // ��ָ���ŵ�ָ����ӵ���ǰ���ڵĵ����б���
                gatesWithCycles[cycleNum].push_back(&gates[*it]);
                // ��δ�����б����Ƴ�����ŵ�����
                it = unscheduledIndices.erase(it);
            }
            else if (areAllPredecessorsScheduled(gate, circuit)) {
                // �������ǰ�����ѵ��ȣ�������ű��Ϊ׼������
                readyGatesIndices.push_back(*it);
                ++it;
            }
            else {
                // ������ܵ��ȣ��ƶ�����һ����
                ++it;
            }
        }
        // �ƶ�����һ������
        if (!readyGatesIndices.empty())
        {
			cycleNum++;
			
		}

        // ��������׼�����ȵ���
        for (size_t index : readyGatesIndices) {
            
            Gate& gate = gates[index];
            gate.setScheduledCycle(cycleNum);
            gate.setScheduled(true);
            scheduledGates.push_back(gate);
            // ��ָ���ŵ�ָ����ӵ���ǰ���ڵĵ����б���
            //getScheduledGatesWithCycles()[cycleNum].push_back(&gates[index]);
            gatesWithCycles[cycleNum].push_back(&gates[index]);
            // ��δ�����б����Ƴ�����ŵ�����
            unscheduledIndices.erase(std::remove(unscheduledIndices.begin(), unscheduledIndices.end(), index), unscheduledIndices.end());
        }

       totalCycles= cycleNum;
    }
}

