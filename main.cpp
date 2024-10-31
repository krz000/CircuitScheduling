#include "circuit.h"
#include "ALAP.h"
#include "ASAP.h"
#include"ALAP_Latency.h"
#include"ASAP_Latency.h"
#include"ML_RCS.h"

int main() {

    //TEST sample1
    Circuit verilog;
    std::string blifFile = "test1.blif";//TODO
    std::string verilogFile = "test1.v";
    
    parseBLIF(blifFile, verilog);
    writeVerilog(verilogFile, verilog);

    std::cout << "转换完成" << std::endl;

  /*  ALAP_L al;

    al.ALAP_Lschedule(verilog);
    verilog.printSchedule(verilog, al);*/

   /* MLRCSScheduler ml(1, 1, 1);
    ml.schedule(verilog);
    verilog.printSchedule(verilog, ml);*/

    //TEST sample02
    Circuit verilog02;
    std::string blifFile02 = "sample03.blif";//TODO
    std::string verilogFile02 = "sample03.v";

    parseBLIF(blifFile02, verilog02);
    writeVerilog(verilogFile02, verilog02);

    std::cout << "转换完成02" << std::endl;


    MLRCSScheduler ml(1, 1, 1);
    ml.schedule(verilog02);
    verilog.printSchedule(verilog02, ml);
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
    ALAP_L alap_l;
    alap_l.ALAP_Lschedule(verilog02);
    verilog02.printSchedule(verilog02, alap_l);

  


   /* ASAPScheduler asapScheduler;
    asapScheduler.schedule(verilog02);
    verilog.printSchedule(verilog02, asapScheduler);*/

  
	//TEST ALAP
    
   /* ALAP ALAPscheduler;
	ALAPscheduler.schedule(verilog02);
    ALAPscheduler.ALAPschedule(verilog02);
    verilog.printSchedule(verilog02, ALAPscheduler);*/

    
   
    
    
    /* ALAP答案
    Input :a ,b ,c ,d ,e ,f   Output :o ,p ,q
    Total 4 Cycles
    Cycle 0: {h}, {j}, {i},
    Cycle 1: {l m}, {g}, {},
    Cycle 2: {n}, {k}, {},
    Cycle 3: {o}, {}, {p q},
    */
    return 0;
}
