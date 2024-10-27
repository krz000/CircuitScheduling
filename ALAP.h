#ifndef ALAP_H
#define ALAP_H

#include "Circuit.h"
#include <algorithm>

class ALAP : public Scheduler {
public:
    void schedule(Circuit &circuit) override;
    void ALAPschedule(Circuit& circuit);
    int maxCycle;//TODO
private:
    int scheduleGate(Circuit& circuit, const std::string& gateName, int currentCycle);
    void removeDuplicateGates(Circuit& circuit);
	
};

#endif // !ALAP_H