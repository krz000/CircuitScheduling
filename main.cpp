#include "circuit.h"
#include "ALAP.h"
#include "ASAP.h"
#include "MR-LCS.h"


int main() {
    Circuit verilog;
	//std::cout << "������BLIF�ļ���" << std::endl;
    //std::string blifFile;
    //std::cin >> blifFile;
	//std::string verilogFile = blifFile + ".v";
    std::string blifFile = "03.txt";//TODO
    std::string verilogFile = "test1.v";
    
    parseBLIF(blifFile, verilog);
    writeVerilog(verilogFile, verilog);

    std::cout << "blif to verilog ת�����" << std::endl;

    //TEST ASAP
    //ASAPScheduler asapScheduler;
    //asapScheduler.schedule(verilog);
    //verilog.printSchedule(verilog, asapScheduler);

	//TEST ALAP
    ALAP ALAPscheduler;
	ALAPscheduler.schedule(verilog);
    ALAPscheduler.ALAPschedule(verilog);
    verilog.printSchedule(verilog, ALAPscheduler);
    
    /* ALAP��
    Input :a ,b ,c ,d ,e ,f   Output :o ,p ,q
    Total 4 Cycles
    Cycle 0: {h}, {j}, {i},
    Cycle 1: {l m}, {g}, {},
    Cycle 2: {n}, {k}, {},
    Cycle 3: {o}, {}, {p q},
    */

    //TEST MR_RCS
	// ������Դ�ý��


    // TEST MR_LCS
    // ����Ŀ�� cycle
 //   int targetCycle;
	//std::cout << "������Ŀ��cycle" << std::endl;
 //   std::cin >> targetCycle;
 //   MR_LCS mr_lcsScheduler;
	//mr_lcsScheduler.schedule(verilog);
	//mr_lcsScheduler.MR_LCSscheduleBF(verilog, targetCycle);
	//verilog.printSchedule(verilog, mr_lcsScheduler);
	//std::cout << "MR_LCS ���� cycle = " + targetCycle << "������Դ��: " + mr_lcsScheduler.getResourseNum() << std::endl;

    return 0;
}
