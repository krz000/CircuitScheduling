#pragma once

#ifndef MR-LCS_H
#define MR-LCS_H

#include "Circuit.h"
#include <algorithm>

class MR_LCS : public Scheduler {
public:
	void schedule(Circuit& circuit) override;
	void MR_LCSschedule(Circuit& circuit, int timeLimit);
    //int resourceLimit;

private:
	int scheduleGate(Circuit& circuit, const std::string& gateName, int currentCycle);

    int timeLimit;


    // 实现队列，依次装入MR_RCS处理后的gate

    // 把所有的gate按照前后调度顺序，依次加入到一个数据结构中 队列，也可以是hashmap，vector等
    // 实现一个函数

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

class Resource {
public:
	Resource(int resourceLimit) : resourceLimit(resourceLimit) {}

	bool isAvailable() const {
		return usedResources < resourceLimit;
	}

	void setResource(int resourceLimit) {
		this->resourceLimit = resourceLimit;
	}

	void useResource() {
		usedResources++;
	}

	void freeResource() {
		usedResources--;
	}
private:
	int resourceLimit;
	int usedResources = 0;
};

class _And : public Resource {
public:
	_And(int resourceLimit) : Resource(resourceLimit) {}
private:
	int resourceLimit;
	int usedResources = 0;
};

class _Or : public Resource {
public:
	_Or(int resourceLimit) : Resource(resourceLimit) {}
private:
	int resourceLimit;
	int usedResources = 0;
};

class _Not : public Resource {
public:
	_Not(int resourceLimit) : Resource(resourceLimit) {}
private:
	int resourceLimit;
	int usedResources = 0;
};
#endif // !MR-LCS_H
