#include"Hu.h"

bool HuScheduler::areAllPredecessorsScheduled(const Gate& gate, const Circuit& circuit) const
{
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

void HuScheduler::scheduling(Circuit& circuit, int resources)
{
    //����ASAPΪÿ���Ŵ��ǩ
    label(circuit);
    //չƽ���ں���˳����
    flatten(this->weightMap);
    //����weightMap����Ȩ�ش�Ŀ�ʼ����
    std::vector<Gate*> curVec = getFlattenedVec();
    //��ʼ��gatesWithCycles
    std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles = getScheduledGatesWithCycles();

    for (int i = 0; i <= flattenedVector.size(); ) {
        Gate* curGate = curVec[i];
        int cycleNum = 0;
        for (int j = 0; j < resources; j++) {
            //���µ��������cycle��
            curGate->setScheduled(true);
            curGate->setScheduledCycle(cycleNum);
            cycleNum++;//cycle������
            gatesWithCycles[cycleNum].push_back(&curGate[i+j]);
            curGate++;//չƽ�����ĵ���
            i += j+1;
        }
    }

}

void HuScheduler::schedule(Circuit& circuit)
{
}

void HuScheduler::label(Circuit& circuit)
{
    ASAPScheduler asapScheduler;
    asapScheduler.schedule(circuit);
    std::unordered_map<int, std::vector<Gate*>>& curCycledGatesMap=asapScheduler.getScheduledGatesWithCycles();
    int asapMaxCycle=circuit.getMaxScheduledCycle();
    for (int i = asapMaxCycle-1; i >= 0; i--) {
        std::vector<Gate*>& curLevelGates = curCycledGatesMap[i];
        
    }
}

void HuScheduler::flatten(std::unordered_map<int, std::vector<Gate*>> weightMap)
{

}
