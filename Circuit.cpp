#include "Circuit.h"
#include <iostream>



//实现Gate类的方法

void Gate::setScheduledCycle(int cycle) {
    scheduledCycle = cycle;
    Scheduled = true;
}
















//实现Circuit类的方法




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
    // 比较两个门电路的关键属性
    return this->getOutput() == other.getOutput() &&
        this->type == other.type &&
        this->getInputs() == other.getInputs();
}

//打印调度总结果
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
//打印每轮指定类型的门
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
    //当type为NOT时，输出完毕后需要输出一个}不包含"，"
    if (type == GateType::NOT)
    {
        std::cout << "}";
    }*/
}





