#include"Circuit.h"
#include"ASAP.h"
#include <numeric>
#include <algorithm>

//实现ASAPScheduler类的方法
bool ASAPScheduler::areAllPredecessorsScheduled(const Gate& gate, const Circuit& circuit) const {
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


void ASAPScheduler::schedule(Circuit& circuit) {
    // 初始化
    getScheduledGatesWithCycles().clear();
    totalCycles = 0;

    // 获取所有门
    std::vector<Gate>& gates = circuit.getGates();//获取所有门的引用，以便于对其修改
    std::vector<Gate> scheduledGates;//存放已经被调度的门


    // 初始化未调度索引
    unscheduledIndices.resize(gates.size());
    std::iota(unscheduledIndices.begin(), unscheduledIndices.end(), 0);//建立隐式索引，即索引值代表门存储位置

    //初始化gatesWithCycles
    std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles = getScheduledGatesWithCycles();


    // 实现ASAP算法
    int cycleNum = 0;//记录当前周期数
    while (!unscheduledIndices.empty()) {
        // 存储本周期可以调度的门的索引
        std::vector<size_t> readyGatesIndices;

        // 遍历所有未调度的门
        for (auto it = unscheduledIndices.begin(); it != unscheduledIndices.end();) {
            Gate& gate = gates[*it];
            if (gate.getInputs().empty()) {
                // 如果门没有输入，可以立即调度
                gate.setScheduledCycle(cycleNum);
                gate.setScheduled(true);
                scheduledGates.push_back(gate);
                // 将指向门的指针添加到当前周期的调度列表中
                gatesWithCycles[cycleNum].push_back(&gates[*it]);
                // 从未调度列表中移除这个门的索引
                it = unscheduledIndices.erase(it);
            }
            else if (areAllPredecessorsScheduled(gate, circuit)) {
                // 如果所有前驱都已调度，将这个门标记为准备调度
                readyGatesIndices.push_back(*it);
                ++it;
            }
            else {
                // 如果不能调度，移动到下一个门
                ++it;
            }
        }
        // 移动到下一个周期
        if (!readyGatesIndices.empty())
        {
			cycleNum++;
			
		}

        // 处理所有准备调度的门
        for (size_t index : readyGatesIndices) {
            
            Gate& gate = gates[index];
            gate.setScheduledCycle(cycleNum);
            gate.setScheduled(true);
            scheduledGates.push_back(gate);
            // 将指向门的指针添加到当前周期的调度列表中
            //getScheduledGatesWithCycles()[cycleNum].push_back(&gates[index]);
            gatesWithCycles[cycleNum].push_back(&gates[index]);
            // 从未调度列表中移除这个门的索引
            unscheduledIndices.erase(std::remove(unscheduledIndices.begin(), unscheduledIndices.end(), index), unscheduledIndices.end());
        }

       totalCycles= cycleNum;
    }
}

