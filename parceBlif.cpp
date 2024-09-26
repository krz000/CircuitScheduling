#include "Circuit.h"

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
            verilog.moduleName = line.substr(7);
        }
        else if (line.find(".inputs") == 0) {
            std::istringstream iss(line.substr(8));
            std::string input;
            while (iss >> input) {
                verilog.inputs.push_back(input);
                verilog.wires.push_back(input);
            }
        }
        else if (line.find(".outputs") == 0) {
            std::istringstream iss(line.substr(9));
            std::string output;
            while (iss >> output) {
                verilog.outputs.push_back(output);
                verilog.wires.push_back(output);
            }
        }
        else if (line.find(".names") == 0) {
            std::istringstream iss(line.substr(7));
            std::vector<std::string> vars;
            std::string var;
            while (iss >> var) {
                vars.push_back(var);
                if (std::find(verilog.wires.begin(), verilog.wires.end(), var) == verilog.wires.end()) {
                    verilog.wires.push_back(var);
                }
            }

            Gate gate;
            gate.gateoutput = vars.back();
            gate.gateid = gate.gateoutput; // 使用输出名称作为唯一标识符
            vars.pop_back();
            gate.gateinputs = vars;

            while (std::getline(inblif, line) && !line.empty() && (isdigit(line[0]) || line[0] == '-')) {
                if (line.size() < vars.size()) continue;
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
                    gate.gateexpressions.push_back(expr);
                }

                // Set gate type based on the line content
                if (line.find('-') != std::string::npos) {
                    gate.type = GateType::OR;
                }
                else if (line.find('0') != std::string::npos) {
                    gate.type = GateType::NOT;
                }
                else {
                    gate.type = GateType::AND;
                }
            }

            verilog.gates.push_back(gate);
            verilog.gateMap[gate.gateid] = gate;

            // Push back the gate after processing
            verilog.gates.push_back(gate);
            verilog.gateMap[gate.gateid] = gate;

            // 将读取的行放回输入流中
            if (!line.empty() && !isdigit(line[0])) {
                inblif.putback('\n');
                for (auto it = line.rbegin(); it != line.rend(); ++it) {
                    inblif.putback(*it);
                }
            }
        }
    }

    inblif.close();
}
