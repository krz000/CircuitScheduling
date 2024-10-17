#include "circuit.h"
#include <iostream>

// ����� .v �ļ��ĺ���
void writeVerilog(const std::string& filename,  Circuit& verilog) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "�޷������ļ�: " << filename << std::endl;
        return;
    }

    outfile << "module " << /*verilog.moduleName*/verilog.getModuleName() << "(clk, rst";
    for (const auto& output : verilog.getOutputs()) {//verilog.outputs) {
        outfile << ", " << output;
    }
    for (const auto& input : verilog.getInputs()) {//verilog.inputs) {
        outfile << ", " << input;
    }
    outfile << ");\n\n";

    outfile << "input clk, rst;\n\n";
    for (const auto& output : verilog.getOutputs()) {//verilog.outputs) {
        outfile << "output " << output << ";\n";
    }
    outfile << "\n";
    for (const auto& input : verilog.getInputs()) {//verilog.inputs) {
        outfile << "input " << input << ";\n";
    }
    outfile << "\n";
    for (const auto& wire : verilog.getWires()) {//verilog.wires) {
        outfile << "wire " << wire << ";\n";
    }
    outfile << "\n";

    std::vector<Gate> gates = verilog.getGates();
    for (const auto& gate : gates) {
        if (gate.getInputs().size() == 0) {
            continue;
        }
        else {
            outfile << "assign " << gate.getOutput() << " = ";
            const auto& expressions = gate.getExpressions();
            for (size_t i = 0; i < expressions.size(); ++i) {
                outfile << " " << expressions[i] << " ";
                if (i < expressions.size() - 1) {
                    outfile << " | ";
                }
            }
            outfile << ";\n";
        }

    }
    /*for (const auto& gate : verilog.getGates()) {//verilog.gates) {
        outfile << "assign " << gate.getOutput() << " = "; //gate.gateoutput << " = ";
        /*for (size_t i = 0; i < gate.gateexpressions.size(); ++i) {
            outfile << " " << gate.gateexpressions[i] << " ";
            if (i < gate.gateexpressions.size() - 1) {
                outfile << " | ";
            }
        }*/
        /*       const auto& expressions = gate.getExpressions();
               for (size_t i = 0; i < expressions.size(); ++i) {
                   outfile << " " << expressions[i] << " ";
                   if (i < expressions.size() - 1) {
                       outfile << " | ";
                   }
               }
               outfile << ";\n";
           }*/
    outfile << "\n";
    outfile << "endmodule\n";

    outfile.close();
}
/*��Щ�޸���Ҫ������

1. ʹ�� `Circuit` �� `Gate` ����·��������ʺ��޸����ݡ�
2. �Ƴ���ֱ�ӷ���˽�г�Ա�����Ĵ��롣
3. ʹ�� `addInput`��`addOutput`��`addWire` �� `addGate` ���������Ԫ�ص� `Circuit` ����
4. ʹ�� `getInputs`��`getOutputs`��`getWires` �� `getGates` ��������ȡ `Circuit` ��������ݡ�
5. ʹ�� `Gate` ��� setter �� getter ���������úͻ�ȡ�ŵ����ԡ�

��Щ�޸�Ӧ��ʹ�������µ� `Circuit` �� `Gate` ��ʵ�ֱ���һ�¡���ע�⣬��������Ҫ�� `Circuit.h` ������ `parseBLIF` �� `writeVerilog` �������Ա��������ļ���ʹ�����ǡ�*/