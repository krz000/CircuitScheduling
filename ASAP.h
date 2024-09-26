#pragma once


#include <vector>
#include <string>
#include <unordered_map>
#include"myVerilog.h"


class ASAPScheduler : public Scheduler {
public:
    bool areAllPredecessorsScheduled(const Gate& gate,const Circuit &circuit) const;
    int getMaxPredecessorCycle(const Gate& gate, const Circuit& circuit) const;
    void schedule(Circuit& circuit) override;
    void printSchedule() const override;

private:
    std::unordered_map<int, std::vector<Gate*>> scheduledGates;
    int totalCycles;
};

class BlifParser {
public:
    Circuit parseBlif(const std::string& filename);
};

