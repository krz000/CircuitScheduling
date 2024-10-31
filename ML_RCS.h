#pragma once
#include "Circuit.h"
#include "ALAP_Latency.h"

class MLRCSScheduler : public Scheduler {
private:
    // ��Դ�ṹ��
    struct GateResources {
        int and_gates;
        int or_gates;
        int not_gates;
    };

public:
    // ���캯��
    MLRCSScheduler(int and_gates, int or_gates, int not_gates);

    // �̳���Scheduler�������Ⱥ���
    void schedule(Circuit& circuit) override;

private:
    // ��Ա����
    GateResources resources;
    std::vector<Gate*> readyGates;
    std::vector<Gate*> ongoingGates;
    Circuit* circuit;  // �洢��ǰ����ĵ�·����
    int currentCycle;
    const std::unordered_map<int, std::vector<Gate*>>* alapSchedule; // ���ALAP���Ƚ��������

    // ���Ĺ��ܺ���
    void updateReadyGates(const std::vector<Gate>& gates, Circuit& circuit);  // ���¾�������
    void updateOngoingGates();  // ��������ִ�е���
    void scheduleGates(std::vector<Gate>& gates);  // ���ȵ�ǰ��������

    // ��������
    bool areAllPredecessorsFinished(const Gate& gate, Circuit& circuit) const;  // �������ǰ���Ƿ����
    int getGateDelay(const Gate& gate) const;  // ��ȡ�ŵ��ӳ�
    int getAvailableResources(GateType type) const;  // ��ȡ�ض����͵Ŀ�����Դ��

    // ��Դ������
    void decrementResources(const Gate& gate);  // ������Դ
    void incrementResources(const Gate& gate);  // ������Դ


    // �����͵�����
    void scheduleGatesOfType(std::vector<Gate*>& gates,
        std::vector<Gate>& allGates,
        GateType type);  // �����ض����͵���
};