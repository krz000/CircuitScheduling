#pragma once
#include"Circuit.h"
#include"ALAP.h"


class ALAP_L : public ALAP {
public:
    int getGateDelay(const Gate& gate) const;
    //int scheduleGate(Circuit& circuit, const std::string& gateName, int currentCycle);
    int scheduleGate(Circuit& circuit, Gate* gate, int currentCycle);
    void ALAP_Lschedule(Circuit& circuit);
};