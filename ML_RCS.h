#pragma once
#include"Circuit.h"
#include"Hu.h"


class MLRCSScheduler : public Scheduler {
private:
    struct GateResources {
        int and_gates;
        int or_gates;
        int not_gates;
    };

public:
    MLRCSScheduler(int and_gates, int or_gates, int not_gates);
    void schedule(Circuit& circuit) override;

private:
    GateResources resources;
    std::vector<Gate*> readyGates;
    std::vector<Gate*> ongoingGates;
    int currentCycle;

    void updateReadyGates(const std::vector<Gate>& gates);
    void updateOngoingGates();
    void scheduleGates(std::vector<Gate>& gates);
    bool areAllPredecessorsScheduled(const Gate& gate, const Circuit& circuit) const;
    int getGateDelay(const Gate& gate) const;
    bool canScheduleGate(const Gate& gate) const;
    void decrementResources(const Gate& gate);
    void incrementResources(const Gate& gate);
};