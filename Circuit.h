#pragma once

#ifndef MYVERILOG_H
#define MYVERILOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

// Gate 结构
/*
* a = c & !d;
* gate需要多个，对应每个name
* 需要gateout，多个gatein， 
*/

enum class GateType {
    AND,
    OR,
    NOT
};

class Gate
{
public:
	//Gate();
	//~Gate();
    void addInput(const std::string& input);
    void setOutput(const std::string& output);
    std::vector<std::string> getInputs(Gate gate) const;
    std::string getOutput(Gate gate) const;

    bool isScheduled() const;
    void setScheduledCycle(int cycle);
    int  getScheduledCycle() const;

    std::string gateid; // 唯一标识符name
    std::string gateoutput;//a
    std::vector<std::string> gateinputs;//c d
    std::vector<std::string> gateexpressions;//c & !d
    GateType type;//& ! |
    int scheduledCycle = -1;//第n个
    bool Scheduled = false;
private:

};

//name 后驱/输出 type 符号
/*
Gate::Gate()
{
}

Gate::~Gate()
{
} 
*/

class Circuit {
public:
    void addInput(const std::string& input);
    void addOutput(const std::string& output);
    void addGate(const Gate& gate);
    std::vector<Gate>& getGates();
    std::vector<std::string> getInputs() const;
    std::vector<std::string> getOutputs() const;

    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
    std::vector<Gate> gates;//TODO
    std::string moduleName;
    std::vector<std::string> wires;
    std::unordered_map<std::string, Gate> gateMap;//name gate useless

private:

};

class Scheduler {
public:
    virtual void schedule(Circuit& circuit) = 0;
    virtual void printSchedule() const = 0;
};

/*
struct Gate{

    std::string gateid; // 唯一标识符
    std::string gateoutput;//a
    std::vector<std::string> gateinputs;//c d
    std::vector<std::string> gateexpressions;//c & !d
};

struct MyVerilog {
    std::string moduleName;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
    std::vector<std::string> wires;
    std::vector<Gate> gates;
    std::unordered_map<std::string, Gate> gateMap;
};

*/

// Function declarations
void parseBLIF(const std::string& filename, Circuit& verilog);
void writeVerilog(const std::string& filename, const Circuit& verilog);

#endif // MYVERILOG_H

