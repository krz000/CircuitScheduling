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

// ���� Verilog �ļ�
ModuleInfo parseVerilogFile(const string& verilogFileName) {
    ModuleInfo info;
    ifstream verilogFile(verilogFileName);
    if (verilogFile.is_open()) {
        string line;
        while (getline(verilogFile, line)) {
            line = regex_replace(line, regex("\\s*;\\s*$"), ""); // ȥ����ĩ�ֺ�
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
                    if (port != "clk" && port != "rst") { // ���˵��޹��ź�
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

// ���� assign ��䲢���� .blif �߼�
string convertToBlifLogic(const string& assignStatement) {
    size_t pos = assignStatement.find("=");
    if (pos == string::npos) return "";

    string left = assignStatement.substr(6, pos - 6); // ��ȡ assign �����ź���
    string right = assignStatement.substr(pos + 1);  // ��ȡ�߼����ʽ
    left.erase(remove(left.begin(), left.end(), ' '), left.end());
    right.erase(remove(right.begin(), right.end(), ' '), right.end());

    string blifLogic;
    if (right.find('|') != string::npos) {
        // ���� OR ����
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
        // ���� AND ����
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
        // ���� NOT ����
        string operand = right.substr(1);
        blifLogic += ".names " + operand + " " + left + "\n";
        blifLogic += "0 1\n";
    }
    return blifLogic;
}


// ���� .blif �ļ�����
string generateBlifContent(const ModuleInfo& info) {
    string blifContent = ".model " + info.moduleName + "\n";

    // ��������˿�
    blifContent += ".inputs ";
    for (const auto& input : info.inputs) {
        blifContent += input + " ";
    }
    blifContent += "\n";

    // ��������˿�
    blifContent += ".outputs ";
    for (const auto& output : info.outputs) {
        blifContent += output + " ";
    }
    blifContent += "\n";

    // �����߼���ϵ
    for (const auto& assignStmt : info.assignStatements) {
        blifContent += convertToBlifLogic(assignStmt);
    }

    blifContent += ".end\n";
    return blifContent;
}


// ���� .blif �ļ�
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

    // ���� .v �ļ�
    ModuleInfo moduleInfo = parseVerilogFile(inputFileName);

    // ���� .blif �ļ�����
    string blifContent = generateBlifContent(moduleInfo);

    // ���浽 .blif �ļ�
    int a = saveBlifFile(outputFileName, blifContent);
