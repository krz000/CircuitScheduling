#include "Circuit.h"
#include <iostream>

int main() {
    BlifParser parser;
    Circuit circuit = parser.parseBlif("input.blif");

    ASAPScheduler asapScheduler;
    asapScheduler.schedule(circuit);
    asapScheduler.printSchedule();

    return 0;
}