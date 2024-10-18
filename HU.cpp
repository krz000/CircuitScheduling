#include"Hu.h"

bool HuScheduler::areAllPredecessorsScheduled(const Gate& gate, const Circuit& circuit) const
{
    bool flag = true;
    for (const std::string& inputName : gate.getInputs())
    {

        try {
            const Gate& curGate = circuit.findGateByOutput(inputName);
            // 使用 curGate
            bool tmpFlag = curGate.isScheduled();
            flag = flag && tmpFlag;
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            // 处理错误情况
        }
    }

    return flag;  // 所有前驱门都已被调度
}

void HuScheduler::scheduling(Circuit& circuit, int resources)
{
    //基于ASAP为每个门打标签
    label(circuit);
    //展平便于后续顺序处理
    flatten(this->weightMap);
    //遍历weightMap，从权重大的开始遍历
    std::vector<Gate*> curVec = getFlattenedVec();
    //初始化gatesWithCycles
    std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles = getScheduledGatesWithCycles();

    for (int i = 0; i <= flattenedVector.size(); ) {
        Gate* curGate = curVec[i];
        int cycleNum = 0;
        for (int j = 0; j < resources; j++) {
            //更新调度情况和cycle数
            curGate->setScheduled(true);
            curGate->setScheduledCycle(cycleNum);
            cycleNum++;//cycle数迭代
            gatesWithCycles[cycleNum].push_back(&curGate[i+j]);
            curGate++;//展平容器的迭代
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
