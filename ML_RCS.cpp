#include "ML_RCS.h"
#include "Circuit.h"
#include <algorithm>

MLRCSScheduler::MLRCSScheduler(int and_gates, int or_gates, int not_gates)
    : resources{ and_gates, or_gates, not_gates }, currentCycle(0) {}



void MLRCSScheduler::updateReadyGates(const std::vector<Gate>& gates, Circuit& circuit) {
    readyGates.clear();
    for (const Gate& gate : gates) {
        if (!gate.isScheduled() && areAllPredecessorsFinished(gate,circuit)) {
            readyGates.push_back(const_cast<Gate*>(&gate));
        }
    }
}

void MLRCSScheduler::updateOngoingGates() {
    for (auto it = ongoingGates.begin(); it != ongoingGates.end();) {
        Gate* gate = *it;
        if (currentCycle - gate->getScheduledCycle() >= getGateDelay(*gate)) {
            incrementResources(*gate);
            it = ongoingGates.erase(it);
        }
        else {
            ++it;
        }
    }
}

void MLRCSScheduler::scheduleGates(std::vector<Gate>& gates) {
   

    // �����ͷ��������
    std::vector<Gate*> readyAndGates, readyOrGates, readyNotGates;
    for (Gate* gate : readyGates) {
        switch (gate->getType()) {
        case GateType::AND: readyAndGates.push_back(gate); break;
        case GateType::OR: readyOrGates.push_back(gate); break;
        case GateType::NOT: readyNotGates.push_back(gate); break;
        }
    }


    // ʹ���ѱ����ALAP���Ƚ����������
    auto findCycle = [this](Gate* gate) {
        if (!gate) {
            
            return INT_MAX;
        }

       

        if (!alapSchedule) {
           
            return INT_MAX;
        }

        for (const auto& pair : *alapSchedule) {
            auto it = std::find_if(pair.second.begin(), pair.second.end(),
                [gate](const Gate* g) {
                    return g && gate && g->getOutput() == gate->getOutput();
                });
            if (it != pair.second.end()) {
                
                return pair.first;
            }
        }

       
        return INT_MAX;
        };

    // ʹ�ñ��غ������бȽ�
    auto compareByCycle = [&findCycle](Gate* a, Gate* b) {
        if (!a || !b) {
           
            return false;
        }

        int cycleA = findCycle(a);
        int cycleB = findCycle(b);

       

        return cycleA < cycleB;
        };

    // �Էǿյ����б��������
    if (!readyAndGates.empty()) {
        
        std::sort(readyAndGates.begin(), readyAndGates.end(), compareByCycle);
    }
    if (!readyOrGates.empty()) {
       
        std::sort(readyOrGates.begin(), readyOrGates.end(), compareByCycle);
    }
    if (!readyNotGates.empty()) {
       
        std::sort(readyNotGates.begin(), readyNotGates.end(), compareByCycle);
    }

    // ����ÿ�����͵���
    scheduleGatesOfType(readyAndGates, gates, GateType::AND);
    scheduleGatesOfType(readyOrGates, gates, GateType::OR);
    scheduleGatesOfType(readyNotGates, gates, GateType::NOT);
}

bool MLRCSScheduler::areAllPredecessorsFinished(const Gate& gate, Circuit& circuit) const {
    for (const std::string& inputName : gate.getInputs()) {
        try {
            const Gate& predGate = circuit.findGateByOutput(inputName);
            if (!predGate.isScheduled()) {
                return false;
            }
            auto it = std::find_if(ongoingGates.begin(), ongoingGates.end(),
                [&predGate](const Gate* g) { return g->getOutput() == predGate.getOutput(); });
            if (it != ongoingGates.end()) {
                return false;//�����ҵ�ǰ����ongoing�У���δ���
            }
        }
        catch (const std::runtime_error& e) {
            continue;
        }
    }
    return true;
}

int MLRCSScheduler::getGateDelay(const Gate& gate) const {
    switch (gate.getType()) {
    case GateType::AND: return 2;
    case GateType::OR: return 3;
    case GateType::NOT: return 1;
    default: return 0;
    }
}

int MLRCSScheduler::getAvailableResources(GateType type) const
{
    int ongoingCount = std::count_if(ongoingGates.begin(), ongoingGates.end(),
        [type](const Gate* g) { return g->getType() == type; });
    switch (type) {
    case GateType::AND: return resources.and_gates - ongoingCount;
    case GateType::OR: return resources.or_gates - ongoingCount;
    case GateType::NOT: return resources.not_gates - ongoingCount;
    default: return 0;
    }
}



void MLRCSScheduler::decrementResources(const Gate& gate) {
    switch (gate.getType()) {
    case GateType::AND: resources.and_gates--; break;
    case GateType::OR: resources.or_gates--; break;
    case GateType::NOT: resources.not_gates--; break;
    }
}

void MLRCSScheduler::incrementResources(const Gate& gate) {
    switch (gate.getType()) {
    case GateType::AND: resources.and_gates++; break;
    case GateType::OR: resources.or_gates++; break;
    case GateType::NOT: resources.not_gates++; break;
    }
}


void MLRCSScheduler::scheduleGatesOfType(std::vector<Gate*>& gates,
    std::vector<Gate>& allGates, GateType type) {
    int availableResources = getAvailableResources(type);

    auto it = gates.begin();
    while (it != gates.end() && availableResources > 0) {
        Gate* gate = *it;
        std::string gateOutput = gate->getOutput();  // �����������

        // ���ȵ�ǰ��
        gate->setScheduled(true);
        gate->setScheduledCycle(currentCycle);
        ongoingGates.push_back(gate);
        getScheduledGatesWithCycles()[currentCycle].push_back(gate);
        decrementResources(*gate);
        availableResources--;

        it = gates.erase(it);
    }
}
void MLRCSScheduler::schedule(Circuit& circuit) {
    // �����·����
    this->circuit = &circuit;



    // ����ALAP_L��ȡ������Ϣ��ֻ����һ�Σ�
    ALAP_L alapScheduler;
    alapScheduler.ALAP_Lschedule(circuit);
    alapSchedule = &alapScheduler.getScheduledGatesWithCycles();
    
    // ���������ŵĵ���״̬
    for (auto& gate : circuit.getGates()) {
        gate.setScheduledCycle(-1);
        gate.setScheduled(false);
    }

    // ���֮ǰ�ĵ��Ƚ��
    getScheduledGatesWithCycles().clear();
    readyGates.clear();
    ongoingGates.clear();
    currentCycle = 0;

  

    std::vector<Gate>& unScheduledGates = circuit.getGates();

    // ���ȴ���������
    const auto& circuitInputs = circuit.getInputs();
    for (auto& gate : unScheduledGates) {
        if (gate.getInputs().empty() ||
            std::find(circuitInputs.begin(), circuitInputs.end(), gate.getOutput()) != circuitInputs.end()) {
            // ���������ŵ��������ں�״̬
            gate.setScheduled(true);
            gate.setScheduledCycle(-1);
            getScheduledGatesWithCycles()[-1].push_back(&gate);
        }
    }

    // ������ѭ��

    bool allGatesScheduled = false;//���Ƚ�����־
    while (!allGatesScheduled) {
        // ��������ִ�е���
        updateOngoingGates();

        // ���¾�������
        updateReadyGates(unScheduledGates,circuit);

        // ���Ⱦ�������
        scheduleGates(unScheduledGates);

        //���������ж�
        allGatesScheduled = std::all_of(unScheduledGates.begin(), unScheduledGates.end(),
            [](const Gate& gate) { return gate.isScheduled(); });
        currentCycle++;
    }

}