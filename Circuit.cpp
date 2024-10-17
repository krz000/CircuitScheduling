#include "Circuit.h"
#include <iostream>



//ʵ��Gate��ķ���

void Gate::setScheduledCycle(int cycle) {
    scheduledCycle = cycle;
    Scheduled = true;
}
















//ʵ��Circuit��ķ���




void Circuit::addInput(const std::string& input) {
    inputs.push_back(input);
}

void Circuit::addOutput(const std::string& output) {
    outputs.push_back(output);
}

void Circuit::addGate(const Gate& gate) {
    gates.push_back(gate);
}

void Circuit::setModuleName(const std::string& name) {
    moduleName = name;
}

void Circuit::addWire(const std::string& wire) {
    wires.push_back(wire);
}

std::vector<Gate>& Circuit::getGates() {
    return gates;
}

std::vector<std::string> Circuit::getInputs() const {
    return inputs;
}

std::vector<std::string> Circuit::getOutputs() const {
    return outputs;
}

std::string Circuit::getModuleName() const {
    return moduleName;
}

std::vector<std::string> Circuit::getWires() const {
    return wires;
}

int Circuit::getMaxScheduledCycle() const
{
    int maxCycle = 0;
    for (const Gate& gate : gates)
    {
        if (gate.getScheduledCycle() > maxCycle)
        {
			maxCycle = gate.getScheduledCycle();
		}
	}
	return maxCycle;
		
}


bool Gate::operator==(const Gate& other) const {
    // �Ƚ������ŵ�·�Ĺؼ�����
    return this->getOutput() == other.getOutput() &&
        this->type == other.type &&
        this->getInputs() == other.getInputs();
}

//��ӡ�����ܽ��
void Circuit::printSchedule(Circuit& circuit, Scheduler& scheduler) const {
    int totalCycles = circuit.getMaxScheduledCycle();
    std::cout << "Total " << totalCycles << " Cycles\n";
    for (int i = 0; i <= totalCycles; ++i) {
        std::cout << "Cycle " << i << ": ";
        printGatesOfType(i, GateType::AND,scheduler);
        printGatesOfType(i, GateType::OR,scheduler);
        printGatesOfType(i, GateType::NOT,scheduler);
        std::cout << "\n";
    }
}
//��ӡÿ��ָ�����͵���
void Circuit::printGatesOfType(int cycle, GateType type, Scheduler& scheduler) const {
    std::cout << "{";

    bool first = true;
    std::unordered_map<int, std::vector<Gate*>> scheduledGates = scheduler.getScheduledGatesWithCycles();
    for (const Gate* gate : scheduledGates.at(cycle)) {
        if (gate->getType() == type) {
            if (!first) std::cout << " ";
            std::cout << gate->getOutput();
            first = false;
        }
    }
    std::cout << "}, ";
    /*
    //��typeΪNOTʱ�������Ϻ���Ҫ���һ��}������"��"
    if (type == GateType::NOT)
    {
        std::cout << "}";
    }*/
}





