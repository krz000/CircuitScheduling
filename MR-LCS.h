#pragma once

#ifndef MR-LCS_H
#define MR-LCS_H

#include "Circuit.h"
#include <algorithm>

class MR_LCS : public Scheduler {
public:
	void schedule(Circuit& circuit) override;
	void MR_LCSschedule(Circuit& circuit, int timeLimit);
private:
	int scheduleGate(Circuit& circuit, const std::string& gateName, int currentCycle);

    int timeLimit;
	int resourceLimit;

    // ʵ�ֶ��У�����װ��MR_RCS������gate

    // �����е�gate����ǰ�����˳�����μ��뵽һ�����ݽṹ�� ���У�Ҳ������hashmap��vector��
    // ʵ��һ������

    std::vector<Gate*> findReadyGates(Circuit& circuit) {
		// ����״̬��: û��scheduled+ǰ�����Ѿ�scheduled

    }

    int calculateSlack(const Gate& gate, int currentCycle) {
        // ���� slack: ��MR_RCS�õ�����cycle�� 
    }

    void scheduleZeroSlackGates(std::vector<std::pair<Gate*, int>>& gateSlacks,
        int currentCycle,
        std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles) {
        // ʵ�ֵ��� slack Ϊ 0 ���ŵ��߼�
    }

    void scheduleAdditionalGates(std::vector<std::pair<Gate*, int>>& gateSlacks,
        int currentCycle,
        std::unordered_map<int, std::vector<Gate*>>& gatesWithCycles) {
        // ʵ������ԴԼ���µ��ȶ����ŵ��߼�
    }

    bool checkAllScheduled(Circuit& circuit) {
        // ʵ�ּ���Ƿ������Ŷ��ѵ��ȵ��߼�
    }
};

#endif // !MR-LCS_H
