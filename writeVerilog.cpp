#include "Circuit.h"
#include <iostream>








// 输出到 .v 文件的函数
void writeVerilog(const std::string& filename, const Circuit& verilog) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "无法创建文件: " << filename << std::endl;
        return;
    }

    outfile << "module " << verilog.moduleName << "(clk, rst";
    for (const auto& output : verilog.outputs) {
        outfile << ", " << output;
    }
    for (const auto& input : verilog.inputs) {
        outfile << ", " << input;
    }
    outfile << ");\n\n";

    outfile << "input clk, rst;\n\n";
    for (const auto& output : verilog.outputs) {
        outfile << "output " << output << ";\n";
    }
    outfile << "\n";
    for (const auto& input : verilog.inputs) {
        outfile << "input " << input << ";\n";
    }
    outfile << "\n";
    for (const auto& wire : verilog.wires) {
        outfile << "wire " << wire << ";\n";
    }
    outfile << "\n";
    for (const auto& gate : verilog.gates) {
        outfile << "assign " << gate.gateoutput << " = ";
        for (size_t i = 0; i < gate.gateexpressions.size(); ++i) {
            outfile << " " << gate.gateexpressions[i] << " ";
            if (i < gate.gateexpressions.size() - 1) {
                outfile << " | ";
            }
        }
        outfile << ";\n";
    }
    outfile << "\n";
    outfile << "endmodule\n";

    outfile.close();
}
