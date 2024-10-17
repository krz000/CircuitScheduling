#include "circuit.h"
#include <iostream>

// 输出到 .v 文件的函数
void writeVerilog(const std::string& filename,  Circuit& verilog) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "无法创建文件: " << filename << std::endl;
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
/*这些修改主要包括：

1. 使用 `Circuit` 和 `Gate` 类的新方法来访问和修改数据。
2. 移除了直接访问私有成员变量的代码。
3. 使用 `addInput`、`addOutput`、`addWire` 和 `addGate` 方法来添加元素到 `Circuit` 对象。
4. 使用 `getInputs`、`getOutputs`、`getWires` 和 `getGates` 方法来获取 `Circuit` 对象的数据。
5. 使用 `Gate` 类的 setter 和 getter 方法来设置和获取门的属性。

这些修改应该使代码与新的 `Circuit` 和 `Gate` 类实现保持一致。请注意，您可能需要在 `Circuit.h` 中声明 `parseBLIF` 和 `writeVerilog` 函数，以便在其他文件中使用它们。*/