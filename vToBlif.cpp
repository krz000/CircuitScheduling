#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_set>

using namespace std;

struct ModuleInfo {
    string moduleName;
    vector<string> inputs;
    vector<string> outputs;
    vector<string> wires;
    vector<string> assignStatements;
};

// 解析 Verilog 文件
ModuleInfo parseVerilogFile(const string& verilogFileName) {
    ModuleInfo info;
    ifstream verilogFile(verilogFileName);
    if (verilogFile.is_open()) {
        string line;
        while (getline(verilogFile, line)) {
            line = regex_replace(line, regex("\\s*;\\s*$"), ""); // 去掉行末分号
            if (line.find("module") != string::npos) {
                smatch match;
                if (regex_search(line, match, regex("module\\s+(\\w+)"))) {
                    info.moduleName = match[1];
                }
            }
            else if (line.find("input") != string::npos) {
                string ports = line.substr(line.find("input") + 5);
                stringstream ss(ports);
                string port;
                while (ss >> port) {
                    port = regex_replace(port, regex(","), "");
                    if (port != "clk" && port != "rst") { // 过滤掉无关信号
                        info.inputs.push_back(port);
                    }
                }
            }
            else if (line.find("output") != string::npos) {
                string ports = line.substr(line.find("output") + 6);
                stringstream ss(ports);
                string port;
                while (ss >> port) {
                    port = regex_replace(port, regex(","), "");
                    info.outputs.push_back(port);
                }
            }
            else if (line.find("wire") != string::npos) {
                string ports = line.substr(line.find("wire") + 4);
                stringstream ss(ports);
                string port;
                while (ss >> port) {
                    port = regex_replace(port, regex(","), "");
                    info.wires.push_back(port);
                }
            }
            else if (line.find("assign") != string::npos) {
                info.assignStatements.push_back(line);
            }
        }
        verilogFile.close();
    }
    return info;
}

// 解析 assign 语句并生成 .blif 逻辑
string convertToBlifLogic(const string& assignStatement) {
    size_t pos = assignStatement.find("=");
    if (pos == string::npos) return "";

    string left = assignStatement.substr(6, pos - 6); // 提取 assign 左侧的信号名
    string right = assignStatement.substr(pos + 1);  // 提取逻辑表达式
    left.erase(remove(left.begin(), left.end(), ' '), left.end());
    right.erase(remove(right.begin(), right.end(), ' '), right.end());

    string blifLogic;
    if (right.find('|') != string::npos) {
        // 处理 OR 操作
        vector<string> terms;
        stringstream ss(right);
        string token;
        while (getline(ss, token, '|')) {
            terms.push_back(token);
        }
        blifLogic += ".names";
        for (const auto& term : terms) {
            blifLogic += " " + term;
        }
        blifLogic += " " + left + "\n";
        for (size_t i = 0; i < terms.size(); ++i) {
            string row(terms.size(), '-');
            row[i] = '1';
            blifLogic += row + " 1\n";
        }
    }
    else if (right.find('&') != string::npos) {
        // 处理 AND 操作
        vector<string> terms;
        stringstream ss(right);
        string token;
        while (getline(ss, token, '&')) {
            terms.push_back(token);
        }
        blifLogic += ".names";
        for (const auto& term : terms) {
            blifLogic += " " + term;
        }
        blifLogic += " " + left + "\n";
        blifLogic += string(terms.size(), '1') + " 1\n";
    }
    else if (right.find('!') != string::npos) {
        // 处理 NOT 操作
        string operand = right.substr(1);
        blifLogic += ".names " + operand + " " + left + "\n";
        blifLogic += "0 1\n";
    }
    return blifLogic;
}


// 生成 .blif 文件内容
string generateBlifContent(const ModuleInfo& info) {
    string blifContent = ".model " + info.moduleName + "\n";

    // 生成输入端口
    blifContent += ".inputs ";
    for (const auto& input : info.inputs) {
        blifContent += input + " ";
    }
    blifContent += "\n";

    // 生成输出端口
    blifContent += ".outputs ";
    for (const auto& output : info.outputs) {
        blifContent += output + " ";
    }
    blifContent += "\n";

    // 生成逻辑关系
    for (const auto& assignStmt : info.assignStatements) {
        blifContent += convertToBlifLogic(assignStmt);
    }

    blifContent += ".end\n";
    return blifContent;
}


// 保存 .blif 文件
int saveBlifFile(const string& blifFileName, const string& blifContent) {
    ofstream blifFile(blifFileName);
    if (blifFile.is_open()) {
        blifFile << blifContent;
        blifFile.close();
    }
    cout << "Conversion completed: " << blifFileName << "\n";
    return 0;
}

    string inputFileName = "testV.v";
    string outputFileName = "testOut.blif";

    // 解析 .v 文件
    ModuleInfo moduleInfo = parseVerilogFile(inputFileName);

    // 生成 .blif 文件内容
    string blifContent = generateBlifContent(moduleInfo);

    // 保存到 .blif 文件
    int a = saveBlifFile(outputFileName, blifContent);
