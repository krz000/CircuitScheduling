#include "ALAP_Latency.h"
#include"Circuit.h"
#include"ALAP.h"
#include"ASAP.h"
#include"ASAP_Latency.h"
#include <numeric>

int ALAP_L::getGateDelay(const Gate& gate) const
{
    switch (gate.getType()) {
    case GateType::AND: return 2;
    case GateType::OR: return 3;
    case GateType::NOT: return 1;
    default: return 0;
    }
}

int ALAP_L::scheduleGate(Circuit& circuit, Gate* gate, int currentCycle) {
    if (!gate) {
        throw std::runtime_error("Gate is null");
    }

    // 处理输出门
    const auto& outputs = circuit.getOutputs();
    if (std::find(outputs.begin(), outputs.end(), gate->getOutput()) != outputs.end()) {
        gate->setScheduledCycle(currentCycle);
        gate->setScheduled(true);
        getScheduledGatesWithCycles()[currentCycle].push_back(gate);
        
    }

    // 处理输入门
    const auto& inputs = circuit.getInputs();
    if (std::find(inputs.begin(), inputs.end(), gate->getOutput()) != inputs.end() ||
        gate->getInputs().empty()) {
        gate->setScheduled(true);
        gate->setScheduledCycle(-1);
        getScheduledGatesWithCycles()[-1].push_back(gate);
        return 0;//上一级为输入，则当前必为第0个cycle
    }

    // 获取当前门的延迟
    int delay = getGateDelay(*gate);
   
    // 递归处理所有输入门
    int maxCycleInOnePath = -1;  // 一条路径上的最大周期
    int maxInputCycle = -1;//当前门的输入门的最大周期
    
    for (const auto& inputName : gate->getInputs()) {
        try {
            Gate& inputGate = const_cast<Gate&>(circuit.findGateByOutput(inputName));
            // 对前驱门递归调用
            int inputDelay = getGateDelay(inputGate);
            int curCycle = currentCycle;//中间临时变量，存储当前门路径通过output得到的Cycle
            curCycle -= inputDelay;//当前门cycle数为下一层（越靠近输出的层）的cycle数减去当前门的延迟
            int inputCycle = scheduleGate(circuit, &inputGate, curCycle);
            //用以处理同一路径一门的cycle数，尽量将同一路径的cycle往靠近输出方向调度
            maxCycleInOnePath = std::max(currentCycle, inputCycle);
        }
        catch (const std::runtime_error& e) {
            continue;
        }
    }

    int finalCycle = std::max(maxCycleInOnePath, gate->getScheduledCycle());//存储最初的输出门的cycle数
    
    int finalOtherCycle ;//存储除输出以外的门的cycle数
    if (gate->getScheduledCycle() == -1) {
        finalOtherCycle = finalCycle;//若是第一次，则cycle数为-1，则不需要取小
    }
    else
    {
        finalOtherCycle = std::min(maxCycleInOnePath, gate->getScheduledCycle());//若是第二次及以上发现同一gate，则需要取小
    }
    //输出门需要先添加
    if (std::find(outputs.begin(), outputs.end(), gate->getOutput()) != outputs.end())

    {
        return finalCycle + delay;
    }



    // 更新门的调度周期
    if (finalCycle == -1) {
        //gate->setScheduledCycle(minCycle);//如果gate的cycle为-1，那么gate的cycle为-1，表示输入门
    }
    else {
        //单独处理输出部分，输出部分cycle实际上进入递归时已确定，为最大cycle
        gate->setScheduledCycle(finalOtherCycle);//
    }
    gate->setScheduled(true);



    
    // 处理重复的门
    auto& gatesWithCycles = getScheduledGatesWithCycles();
   
    // 将当前门添加到对应周期
    gatesWithCycles[gate->getScheduledCycle()].push_back(gate);
    
    int  reptGatesNum =0;//重复的gate数

    // 遍历整个 unordered_map，查找重复的 Gate
    for (auto it = gatesWithCycles.begin(); it != gatesWithCycles.end();) {
        auto& gatesInCycle = it->second;
        auto gateIt = std::find_if(gatesInCycle.begin(), gatesInCycle.end(),
            [gate](const Gate* g) { return g->getOutput() == gate->getOutput(); });

        // 假设Gate是一个类，gatesInCycle是一个包含Gate*指针的容器
        std::vector<const Gate*> reptGates;
        std::copy_if(gatesInCycle.begin(), gatesInCycle.end(), std::back_inserter(reptGates),
            [gate](const Gate* g) { return g->getOutput() == gate->getOutput(); });
        reptGatesNum = reptGates.size();
       
       if (gateIt != gatesInCycle.end()) {
            if (it->first >gate->getScheduledCycle()) {

               
                gatesInCycle.erase(gateIt);
                if (gatesInCycle.empty()) {
                   //先找出所有重复的gate
                   
                    //it = gatesWithCycles.erase(it);
                    continue;
                }
            }
            else if(it->first == gate->getScheduledCycle()){
                while (reptGatesNum != 1) {
                    gatesInCycle.erase(gateIt);
					reptGatesNum--;
                }
            }
        }
       else {
           //reptGatesNum++;
       }
        ++it;
    }
 

    return finalOtherCycle+delay;
}

void ALAP_L::ALAP_Lschedule(Circuit& circuit)
{
    // 获取ASAP_L调度的最大周期作为ALAP的起点
    ASAP_L asapLScheduler;
    asapLScheduler.schedule(circuit);
    int maxCycle = circuit.getMaxScheduledCycle();

    // 重置所有门的调度状态
    for (auto& gate : circuit.getGates()) {
        gate.setScheduledCycle(-1);
        gate.setScheduled(false);
    }
    getScheduledGatesWithCycles().clear();

    // 从输出开始反向遍历
    std::vector<std::string> outputs = circuit.getOutputs();
    for (const auto& output : outputs) {
        try {
            Gate& outputGate = const_cast<Gate&>(circuit.findGateByOutput(output));
            scheduleGate(circuit, &outputGate, maxCycle);
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Error finding output gate: " << output << std::endl;
            continue;
        }
    }
}