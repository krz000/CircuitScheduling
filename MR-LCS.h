#pragma once

#ifndef MR-LCS_H
#define MR-LCS_H

#include "Circuit.h"
#include <algorithm>

class MR_LCS : public Scheduler {
public:
	void schedule(Circuit& circuit) override;
	void MR_LCSschedule(Circuit& circuit, int timeLimit);

    int resourceLimit;

private:
	int scheduleGate(Circuit& circuit, const std::string& gateName, int currentCycle);

    int timeLimit;


    // ʵ�ֶ��У�����װ��MR_RCS������gate

    // �����е�gate����ǰ�����˳�����μ��뵽һ�����ݽṹ�� ���У�Ҳ������hashmap��vector��
    // ʵ��һ������

    std::vector<Gate*> findReadyGates(Circuit& circuit);

    int calculateSlack(const Gate& gate, int currentCycle);

    void scheduleZeroSlackGates(std::vector<std::pair<Gate*, int>>& gateSlacks,
        int currentCycle,
        std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles);

    void scheduleAdditionalGates(std::vector<std::pair<Gate*, int>>& gateSlacks,
        int currentCycle,
        std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles);
    
    bool checkAllScheduled(Circuit& circuit);
};

#endif // !MR-LCS_H
