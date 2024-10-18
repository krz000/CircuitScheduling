#pragma once


#include <vector>
#include <string>
#include <unordered_map>
#include"Circuit.h"


class ASAPScheduler : public Scheduler {
public:
    bool areAllPredecessorsScheduled(const Gate& gate,const Circuit &circuit) const;
    void schedule(Circuit& circuit) override;

    //»ù´¡º¯Êý
   

private:
    std::vector<size_t> unscheduledIndices;
    int totalCycles;
};


