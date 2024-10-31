#include "Circuit.h"
#include "MR-LCS.h"
#include "ALAP.h"
#include "ASAP.h"

/*
* 思路：
* 1. 不同资源数：
* 每个资源数都有不同的2.4 2.5
* 2. 每个gate消耗的资源数不同
* 资源数 = gateType & 2 | 3 ! 1
* 
* 
* 
* 首先看有没有某一类型的门，有才资源赋值1，否则0
* 
* asap得到最早使用的cycle
* alap得到最晚的cycle，同时也作为优先级，越小越优先
* 相等的就是确定的gate，不相等的就是浮动gate，按照alap的cycle降序排列
* 
* 填入need资源矩阵：
* col = cycle，row = gateType
* 最早-最晚之间，在对应的type加一
* 确定的门使得需求资源++，范围：col = cycle 从currentCycle到gateType + currentCycle
* 
* 尝试将浮动塞入需求矩阵，
* 需求+=1，如果超过资源数，就cycle++
* 如果不超过，则确定为这一周期
* 如果cycle==alap的cycle，则该资源++
* 
*/

void MR_LCS::schedule(Circuit& circuit) {
    // 调用MR_RCS，暴力得到minCycle
    // timeLimit = targetCycle;
  //  while (minCycle > timeLimit) {
		//int _and = 0, _or = 0, _not = 0;
		//int _sum = _and + _or + _not;
		//minCycle = MR_RCS(circuit, _and, _or, _not);
  //      // 算法优化
  //      // 000开始 对于总资源数为_sum，分别分配，得到最小的cycle，记录各类门的资源数
		//// A(n, 3) = n(n-1)(n-2)/6;
  //  }
}

void MR_LCS::MR_LCSscheduleBF(Circuit& circuit, int timeLimit) {
    //判断无限资源所需轮次
    
    //const int minCycle = ML_RCS({ INF, INF, INF });
	int minCycle = 0;
    //无限资源下轮次大于要求则不能完成

    if (minCycle > timeLimit) {
        //报错
		throw std::runtime_error("无法在给定时间限制内完成调度");
    }

    //遍历所有门保存数量
    std::array<int, 3> gateNum = { 0, 0, 0 };
    for (auto& gate : circuit.getGates()) {
        switch (gate.getType()) {
        case GateType::AND: ++gateNum[0]; break;
        case GateType::OR: ++gateNum[1]; break;
        case GateType::NOT: ++gateNum[2]; break;
        }
    }
    //排除不需要的门
    std::array<bool, 3> needGate = { true, true, true };
    for (int i = 0; i < gateNum.size(); ++i) {
        if (gateNum[i] == 0) needGate[i] = false;
    }
    //如果不需要某种门，则其资源是0
    // 资源数
    std::array<int, 3> resourseNum = { 0, 0, 0 };
    for (int i = 0; i < resourseNum.size(); ++i) {
        if (needGate[i]) resourseNum[i] = 1;
    }

    //int cycle = ML_RCS(resourseNum);
    //if (cycle <= timeLimit);
	// 确定资源数

    // 从最小资源数开始尝试
    int totalResource = 3; // 最少每种门1个资源
    bool found = false;
    std::array<int, 3> bestAllocation = { 1, 1, 1 };

    while (!found) {
        // 尝试当前总资源数的所有分配方案
        for (int and_res = 1; and_res <= totalResource - 2; ++and_res) {
            for (int or_res = 1; or_res <= totalResource - and_res - 1; ++or_res) {
                int not_res = totalResource - and_res - or_res;
                if (not_res < 1) continue;

                std::array<int, 3> currentAlloc = { and_res, or_res, not_res };
                // 跳过不需要的门类型
                bool valid = true;
                for (int i = 0; i < 3; ++i) {
                    if (!needGate[i] && currentAlloc[i] > 0) {
                        valid = false;
                        break;
                    }
                }
                if (!valid) continue;

                // 检查当前分配是否满足目标周期
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
//    // 1. 初始化 传参加入时间限制
//    std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles = getScheduledGatesWithCycles();
//    gatesWithCycles.clear();
//
//	// 2. 计算 2.4 时间 得到该资源数下的最大周期
//    //  maxcycle = mintime
//    // 3. 检查 给定时间限定 的可行性
//    // 
//	// ALAP延迟版得到 minTime + 运行顺序
//    // 
//    // int minTime = MR_RCS();
//	int minTime = 5;
//	// 给定时间和MR_RCS的最小时间比较 小于说明不可能做到
//    if (timeLimit < minTime) {
//        throw std::runtime_error("调度不可行：ALAP 时间小于 0");
//    }
//
//    // 4. 重置所有门的调度状态
//    for (auto& gate : circuit.getGates()) {
//         gate.setScheduledCycle(-1);
//         gate.setScheduled(false);
//    }
//
//    // 资源数
//    _And _and(0);
//	//_and.freeResource();
//	int _and = 0, _or = 0, _not = 0;
//	
//
//	int maxCycle = minTime;
//    // 从输出开始反向遍历
//    std::vector<std::string> outputs = circuit.getOutputs();
//    for (const auto& output : outputs) {
//        scheduleGate(circuit, output, maxCycle);
//    }
//    
//    
//    // 5. 开始调度过程
//    int currentCycle = 0;
//    bool allScheduled = false;
//
//
//    while (!allScheduled) {
//        // 5.1 找出当前可调度的门（就绪状态的门）
//        std::vector<Gate*> readyGates = findReadyGates(circuit);
//        
//        // 5.2 计算每个就绪门的 slack=最晚完成时间-当前时间
//        std::vector<std::pair<Gate*, int>> gateSlacks;
//        for (Gate* gate : readyGates) {
//            int slack = calculateSlack(*gate, currentCycle);
//            gateSlacks.push_back({ gate, slack });
//        }
//
//        // 5.3 优先调度 slack 为 0 的门
//        scheduleZeroSlackGates(gateSlacks, currentCycle, gatesWithCycles);
//
//        // 5.4 在资源约束下调度额外的门
//        scheduleAdditionalGates(gateSlacks, currentCycle, gatesWithCycles);
//
//        // 5.5 检查是否所有门都已调度
//        allScheduled = checkAllScheduled(circuit);
//
//        // 5.6 进入下一个周期
//        currentCycle++;
//    }
//}
//
//int MR_LCS::scheduleGate(Circuit& circuit, const std::string& gateName, int currentCycle) {
//    //存储门
//    std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles = getScheduledGatesWithCycles();
//
//  //  try {
//  //      Gate& gate = const_cast<Gate&>(circuit.findGateByOutput(gateName));
//
//  //      // 如果是输出门，队列尾部先出列调度
//
//  //      // 如果是输入门，调度在第-1周期
//  //      const auto& inputs = circuit.getInputs();
//  //      if (std::find(inputs.begin(), inputs.end(), gateName) != inputs.end()) {
//  //          gate.setScheduled(true);
//  //          gate.setScheduledCycle(-1);
//  //          return 0;
//  //      }
//
//		//// 队列依次出列调度，
//
//
//  //      auto& gatesInCycle = gatesWithCycles[scheduledCycle];
//
//  //      // 删除重复门
//
//        //gatesInCycle.push_back(&gate);
//
//  //      return scheduledCycle;
//  //  }
//  //  catch (const std::runtime_error& e) {
//  //      std::cerr << "错误：找不到门 " << gateName << std::endl;
//  //      return -1;
//  //  }
//}
//
//
//std::vector<Gate*> MR_LCS::findReadyGates(Circuit& circuit) {
//    // 就绪状态门: 没有scheduled+前驱都已经scheduled
//    // 还需要排序，按照顺序
//        std::vector<Gate*> readyGates;
//        std::vector<Gate>& gates = circuit.getGates();
//
//        for (Gate& gate : gates) {
//            // 跳过已经调度的门
//            if (gate.isScheduled()) {
//                continue;
//            }
//
//            // 检查所有输入是否就绪
//            bool allInputsScheduled = true;
//            const std::vector<std::string>& inputs = gate.getInputs();
//
//            for (const std::string& input : inputs) {
//                // 检查是否是主输入
//                bool isPrimaryInput = false;
//                for (const std::string& primaryInput : circuit.getInputs()) {
//                    if (input == primaryInput) {
//                        isPrimaryInput = true;
//                        break;
//                    }
//                }
//
//                // 如果是主输入，继续检查下一个输入
//                if (isPrimaryInput) {
//                    continue;
//                }
//
//                try {
//                    // 查找输入对应的门
//                    const Gate& inputGate = circuit.findGateByOutput(input);
//                    // 如果输入门未调度，标记为未就绪
//                    if (!inputGate.isScheduled()) {
//                        allInputsScheduled = false;
//                        break;
//                    }
//                }
//                catch (const std::runtime_error& e) {
//                    // 如果找不到对应的门，可能是主输入，继续检查
//                    continue;
//                }
//            }
//
//            // 如果所有输入都已就绪，将该门添加到就绪列表
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
//    // 计算 slack: 用MR_RCS得到最后的cycle， 
//    
//    // 1. 获取该门的ALAP时间
//    int alapTime = gate.getScheduledCycle();
//    
//    // 2. 如果门还没有ALAP时间，返回一个较大的slack值
//    // 输入门处理
//    //if (alapTime == -1) {
//    //    return INT_MAX;
//    //}
//
//    // 3. 计算slack = ALAP时间 - 当前时间
//    int slack = alapTime - currentCycle;
//
//    // 4. 确保slack不为负
//    return std::max(0, slack);
//}
//
//
//void MR_LCS::scheduleZeroSlackGates(std::vector<std::pair<Gate*, int>>& gateSlacks,
//    int currentCycle,
//    std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles) {
//    // 实现调度 slack 为 0 的门的逻辑
//    // 1. 统计当前周期各类型门的资源使用情况 已使用
//    // ？
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
//    // 2. 遍历所有slack为0的门
//    auto it = gateSlacks.begin();
//    while (it != gateSlacks.end()) {
//        Gate* gate = it->first;
//        int slack = it->second;
//
//        if (slack == 0) {
//            bool canSchedule = false;
//            // 检查资源限制
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
//                // 调度门
//                gate->setScheduled(true);
//                gate->setScheduledCycle(currentCycle);
//                gatesWithCycles[currentCycle].push_back(gate);
//
//                // 从待调度列表中移除
//                it = gateSlacks.erase(it);
//            }
//            else {
//                // 如果无法调度，抛出异常或处理错误
//                throw std::runtime_error("无法调度slack为0的门，xx资源不足");
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
//    // 实现在资源约束下调度额外门的逻辑
//    // 1. 统计当前周期已使用的资源
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
//    // 2. 按照slack值排序（优先调度slack小的门）
//    std::sort(gateSlacks.begin(), gateSlacks.end(),
//        [](const auto& a, const auto& b) {
//            return a.second < b.second;
//        });
//
//    // 3. 尝试调度额外的门
//    // gate 从gateready中来
//    auto it = gateSlacks.begin();
//    while (it != gateSlacks.end()) {
//        Gate* gate = it->first;
//        bool canSchedule = false;
//
//        // 检查资源限制
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
//            // 调度门
//            gate->setScheduled(true);
//            gate->setScheduledCycle(currentCycle);
//            gatesWithCycles[currentCycle].push_back(gate);
//
//            // 从待调度列表中移除
//            it = gateSlacks.erase(it);
//        }
//        else {
//            // 如果无法调度，移动到下一个门
//            ++it;
//        }
//    }
//}
//
//bool MR_LCS::checkAllScheduled(Circuit& circuit) {
//    // 实现检查是否所有门都已调度的逻辑
//        // 获取所有门
//    std::vector<Gate>& gates = circuit.getGates();
//
//    // 遍历所有门，检查是否都已调度
//    for (const Gate& gate : gates) {
//        // 跳过输入门（它们的scheduledCycle为-1）
//        const auto& inputs = circuit.getInputs();
//        if (std::find(inputs.begin(), inputs.end(), gate.getOutput()) != inputs.end()) {
//            continue;
//        }
//
//        // 如果找到未调度的门，返回false
//        if (!gate.isScheduled() || gate.getScheduledCycle() == -1) {
//            return false;
//        }
//    }
//
//    // 所有门都已调度
//    return true;
//}