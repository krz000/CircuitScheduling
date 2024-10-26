#pragma once

#ifndef MYVERILOG_H
#define MYVERILOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>


//���ĵ�������+1
enum class GateType {
    UNKOWN,
    NOT,
    //AND,
    OR
};

class Gate
{
public:
    Gate() {};
    //~Gate() {};

    bool operator==(const Gate& other) const;
    Gate(const Gate& other) = default;
    Gate& operator=(const Gate& other) = default;

    //����˽�г�Ա�����ķ���
    void setScheduledCycle(int cycle);
    void setGateid(const std::string& id) { gateId = id; }
    void addInput(const std::string& input) { gateInputs.push_back(input); }
    void setOutput(const std::string& output) { gateOutput = output; }
    void addExpression(const std::string& expr) { gateExpressions.push_back(expr); }
    void setType(GateType t) { type = t; }
    void setScheduled(bool scheduled) { Scheduled = scheduled; }


    //��ȡ˽�г�Ա�����ķ���
    std::string getOutput() const {
        return gateOutput;
    }
    std::string getGateId() const {
        return gateId;
    }
    std::vector<std::string> getInputs() const {
        return gateInputs;
    }
    std::vector<std::string> getExpressions() const {
        return gateExpressions;
    }
    GateType getType() const {
        return type;
    }
    int getScheduledCycle() const {
        return scheduledCycle;
    }
    bool isScheduled() const {
        return Scheduled;
    }

private:
    std::string gateId; // Ψһ��ʶ��name
    std::string gateOutput;//
    std::vector<std::string> gateInputs;//c d
    std::vector<std::string> gateExpressions;//c & !d
    GateType type;//& ! |
    int scheduledCycle = -1;//��n��
    bool Scheduled = false;
};

class Scheduler;//ǰ������

class Circuit {
public:
    void addInput(const std::string& input);
    void addOutput(const std::string& output);
    void addGate(const Gate& gate);
    void setModuleName(const std::string& name);
    void addWire(const std::string& wire);

    std::vector<Gate>& getGates();
    std::vector<std::string> getInputs() const;
    std::vector<std::string> getOutputs() const;
    std::string getModuleName() const;
    std::vector<std::string> getWires() const;

    //ͨ��circuit��ȡ���cycle��
    int getMaxScheduledCycle() const;

    //��ӡ���-ֱ�Ӹ���circuit��ӡ
    void printSchedule(Circuit& circuit, Scheduler& scheduler) const;
    void printGatesOfType(int cycle, GateType type, Scheduler& scheduler) const;

    //ͨ�������string�ҵ���Ӧ��gate
    const Gate& findGateByOutput(const std::string& outputName) const {
        for (const Gate& gate : gates) {
            if (gate.getOutput() == outputName) {
                return gate;
            }
        }
        throw std::runtime_error("Gate not found: " + outputName);
    }

    // ����µķ�������ȡ�����ѵ��ȵ���
    std::vector<const Gate*> getAllScheduledGates() const;

    // �޸� printSchedule ����
    void printSchedule(const Scheduler& scheduler) const;

private:
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
    std::vector<Gate> gates;//TODO
    std::string moduleName;
    std::vector<std::string> wires;
    std::unordered_map<std::string, Gate> gateMap;//name gate useless
    
    // ����µĸ�������
    void printGatesOfType(int cycle, GateType type, const std::vector<const Gate*>& scheduledGates) const;

};

class Scheduler {
public:
    virtual void schedule(Circuit& circuit) = 0;
    //virtual void printSchedule(Circuit& circuit) const = 0;
    
    //�洢�ڵ�����HU���������
    std::unordered_map<int, std::vector<Gate*>>& getScheduledGatesWithCycles() {
        return scheduledGatesWithCycles;
    }

private:
    std::unordered_map<int, std::vector<Gate*>> scheduledGatesWithCycles;//���ڴ洢�Ѿ������ȵ��ţ�ÿ����ֵ�Ա�ʾһ�����ڵ���
};

// Function declarations
void parseBLIF(const std::string& filename, Circuit& verilog);
void writeVerilog(const std::string& filename, Circuit& verilog);

#endif // MYVERILOG_H

