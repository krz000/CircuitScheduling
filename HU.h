#pragma once
#ifndef HU_H

#include"Circuit.h"
#include"ASAP.h"




class HuScheduler :public Scheduler {
public:
	bool areAllPredecessorsScheduled(const Gate& gate, const Circuit& circuit) const;
	void scheduling(Circuit& circuit,int resources);
	void schedule(Circuit& circuit)override;
	void label(Circuit& circuit);
    void flatten(std::unordered_map<int, std::vector<Gate*>> weightMap);

	//��ȡ����
	std::vector<Gate*>& getFlattenedVec() {
		return flattenedVector;
	}
	std::unordered_map<int, std::vector<Gate*>>& getWeightMap() {
		return weightMap;
	}
private:
	std::unordered_map<int , std::vector<Gate*>> weightMap;//�洢���ǩ֮���gate
	std::vector<Gate*> flattenedVector;//�����ǩ֮���map����Ȩ��չƽ


};




































#endif // !HU_H
