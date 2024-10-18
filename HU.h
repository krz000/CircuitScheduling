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

	//获取函数
	std::vector<Gate*>& getFlattenedVec() {
		return flattenedVector;
	}
	std::unordered_map<int, std::vector<Gate*>>& getWeightMap() {
		return weightMap;
	}
private:
	std::unordered_map<int , std::vector<Gate*>> weightMap;//存储打标签之后的gate
	std::vector<Gate*> flattenedVector;//将打标签之后的map按照权重展平


};




































#endif // !HU_H
