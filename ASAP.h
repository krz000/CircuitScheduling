#pragma once


#include <vector>
#include <string>
#include <unordered_map>
#include"Circuit.h"


class ASAPScheduler : public Scheduler {
public:
    bool areAllPredecessorsScheduled(const Gate& gate,const Circuit &circuit) const;
    void schedule(Circuit& circuit) override;
    //void printSchedule(Circuit& circuit) const override;
    //void printGatesOfType(int cycle, GateType type) const;
    //»ù´¡º¯Êý
   

private:
    std::vector<size_t> unscheduledIndices;
    int totalCycles;
};


