#include "circuit.h"
#include "ALAP.h"
#include "ASAP.h"
#include "ALAP_Latency.h"
#include "ASAP_Latency.h"
#include "ML_RCS.h"
#include "MR-LCS.h"

int main() {
    Circuit verilog;

    //std::cout << "������BLIF�ļ���" << std::endl;
    //std::string blifFile;
    //std::cin >> blifFile;
    //std::string verilogFile = blifFile + ".v";
    // 
    //std::string blifFile = "03.txt";//TODO
    //std::string verilogFile = "test1.v";

    //parseBLIF(blifFile, verilog);
    //writeVerilog(verilogFile, verilog);

    //std::cout << "blif to verilog ת�����" << std::endl;

    //TEST sample1

    std::string blifFile = "test1.blif";//TODO
    std::string verilogFile = "test1.v";
    
    parseBLIF(blifFile, verilog);
    writeVerilog(verilogFile, verilog);

    std::cout << "blif to verilog ת�����" << std::endl;

  /*  ALAP_L al;

    al.ALAP_Lschedule(verilog);
    verilog.printSchedule(verilog, al);*/

   /* MLRCSScheduler ml(1, 1, 1);
    ml.schedule(verilog);
    verilog.printSchedule(verilog, ml);*/

    //TEST sample02
    //Circuit verilog02;
    //std::string blifFile02 = "sample03.blif";//TODO
    //std::string verilogFile02 = "sample03.v";

    //parseBLIF(blifFile02, verilog02);
    //writeVerilog(verilogFile02, verilog02);

    //std::cout << "ת�����02" << std::endl;

    //TEST MR_LCS
    //����Ŀ�� cycle
    int targetCycle;
    std::cout << "������Ŀ��cycle" << std::endl;
    std::cin >> targetCycle;
    MR_LCS mr_lcsScheduler;
    //mr_lcsScheduler.schedule(verilog);
    //mr_lcsScheduler.MR_LCSscheduleBF(verilog, targetCycle);
    std::array<int, 3> res = mr_lcsScheduler.MR_LCSschedule(verilog, targetCycle);
    //verilog.printSchedule(verilog, mr_lcsScheduler);
    std::cout << "MR_LCS ���� cycle = " + targetCycle << std::endl; 
    // ����ŵĸ���
    std::cout << "�ŵ��Ƚ�� (MR_LCS):" << std::endl;
    std::cout << "\t" << "&" << "\t" << "|" << "\t" << "!" << std::endl;
    std::cout << "\t" << res[0] << "\t" << res[1] << "\t" << res[2] << std::endl;


    //MLRCSScheduler ml(1, 1, 1);
    //ml.schedule(verilog02);
    //verilog.printSchedule(verilog02, ml);
    /*ALAP al02;
    al02.schedule(verilog02);
    al02.ALAPschedule(verilog02);
    verilog02.printSchedule(verilog02, al02);*/
    /*
//    Total 6 Cycles
//Cycle 0: {g}, {f}, {},
//Cycle 1: {}, {j}, {h},
//Cycle 2: {}, {i}, {},
//Cycle 3: {k}, {}, {},
//Cycle 4: {}, {}, {l},
//Cycle 5: {}, {}, {o p},
//    
//    */
    //ALAP_L alap_l;
    //alap_l.ALAP_Lschedule(verilog02);
    //verilog02.printSchedule(verilog02, alap_l);

  


   /* ASAPScheduler asapScheduler;
    asapScheduler.schedule(verilog02);
    verilog.printSchedule(verilog02, asapScheduler);*/

  
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
