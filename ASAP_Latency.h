#pragma once
#ifndef ASAP_LATENCY_H
#include"Circuit.h"
#include"ASAP.h"
#include"ALAP.h"

class ASAP_L : public ASAPScheduler {
public:
    void schedule(Circuit& circuit) override;
    bool areAllPredecessorsScheduled_L(const Gate& gate, const Circuit& circuit) const ;

private:
    std::vector<Gate*> ongoingGates;  // ����ִ�е���
    int getGateDelay(const Gate& gate) const;
    void updateOngoingGates(int currentCycle);
    bool isGateReady(const Gate& gate, const Circuit& circuit) const;
    int totalCycles;
};






#endif // !ASAP_LATENCY_H

