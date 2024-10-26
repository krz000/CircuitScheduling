#ifndef ALAP_H
#define ALAP_H

#include "Circuit.h"
#include <algorithm>

class ALAP : public Scheduler {
public:
    void schedule(Circuit &circuit) override;
    void ALAPschedule(Circuit& circuit);
private:
    int scheduleGate(Circuit& circuit, const std::string& gateName, int currentCycle);
};

#endif // !ALAP_H