#include "Circuit.h"
/*
现在需要处理的问题是：
blif文件中的.name格式，正常来说，
.names a d g
1- 1
-1 1
.names a c h
11 1
.names c i
0 1
这些分别代表或门| 与门& 非门！
如果在一个.name中出现多个门比如
.names a d g
11 1
-1 1
.names a c h
10 1
分别代表
g=a&d|d其实等价于d 不用处理？

h=a&!c

.names a d g
11 1
01 1
g=a&d|!a&d 等价于d
.names a d g
10 1
01 1
g=a&!d|!a&d 

*/
// 解析 .blif 文件
void parseBLIF(const std::string& filename, Circuit& verilog) {
    std::ifstream inblif(filename);
    if (!inblif.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return;
    }

    std::string line;
    
    while (std::getline(inblif, line)) {
        if (line.find(".model") == 0) {
            verilog.setModuleName(line.substr(7));
           
        }
        else if (line.find(".inputs") == 0) {
            std::istringstream iss(line.substr(8));
            std::string input;
            while (iss >> input) {
                verilog.addInput(input);
                verilog.addWire(input);
               
            }
        }
        else if (line.find(".outputs") == 0) {
            std::istringstream iss(line.substr(9));
            std::string output;
            while (iss >> output) {
                verilog.addOutput(output);
                verilog.addWire(output);
               
            }
        }
        else if (line.find(".names") == 0) {
            std::istringstream iss(line.substr(7));
            std::vector<std::string> vars;
            std::string var;
            while (iss >> var) {
                vars.push_back(var);

               
                const auto& wires = verilog.getWires();
                if (std::find(wires.begin(), wires.end(), var) == wires.end()) {
                    verilog.addWire(var);
                   
                }
            }

            Gate gate;
           
            gate.setOutput(vars.back());
            gate.setGateid(gate.getOutput()); // 使用输出名称作为唯一标识符
            vars.pop_back();
            for (const auto& input : vars) {
                gate.addInput(input);
            }

            while (std::getline(inblif, line) && !line.empty() && (isdigit(line[0]) || line[0] == '-')) {
                if (line.size() < vars.size()) continue;
                //TODO

                std::string expr;
                bool first = true;
                for (size_t i = 0; i < vars.size(); ++i) {
                    if (line[i] == '1') {
                        if (!first) expr += " & ";
                        expr += vars[i];
                        first = false;
                    }
                    else if (line[i] == '0') {
                        if (!first) expr += " & ";
                        expr += "!" + vars[i];
                        first = false;
                    }
                }

                if (!expr.empty()) {
                    //gate.gateexpressions.push_back(expr);
                    gate.addExpression(expr);
                }


                // Set gate type based on the line content
                if (line.find('-') != std::string::npos) {
                    gate.setType(GateType::OR);
                    //gate.type = GateType::OR;
                }
                else if (line.find('0') != std::string::npos) {
                    //gate.type = GateType::NOT;
                    gate.setType(GateType::NOT);
                }
                else {
                    gate.setType(GateType::AND);
                    //gate.type = GateType::AND;
                }
            }

          
            verilog.addGate(gate);

            // 将读取的行放回输入流中
            if (!line.empty() && !isdigit(line[0])) {
                inblif.putback('\n');
                for (auto it = line.rbegin(); it != line.rend(); ++it) {
                    inblif.putback(*it);
                }
            }

           
        }
    }
    
    for (const auto& input : verilog.getInputs()) {
        Gate gate;
        gate.setOutput(input);
        gate.setGateid(gate.getOutput());
        verilog.addGate(gate);
    }

    inblif.close();
}
