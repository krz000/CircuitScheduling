#include "Circuit.h"
#include <iostream>

bool Gate::isScheduled()const
{
    return Scheduled;
}

//ʵ��Gate��ķ���
std::vector<std::string> Gate::getInputs() const
{
    	return inputs;
}

void Gate::setScheduledCycle(int cycle) {
    scheduledCycle = cycle;
    Scheduled = true;
}

std::string Gate::getOutput() const
{
	return output;
}

















//ʵ��Circuit��ķ���

std::vector<Gate>& Circuit::getGates() {
	return gates;
}


std::vector<std::string> Circuit::getInputs() const {
	return inputs;
}

std::vector<std::string> Circuit::getOutputs() const {
	return outputs;
}














