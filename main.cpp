#include "circuit.h"
#include "ALAP.h"
#include "ASAP.h"

int main() {
    Circuit verilog;
    std::string blifFile = "test1.blif";//TODO
    std::string verilogFile = "test1.v";
    
    parseBLIF(blifFile, verilog);
    writeVerilog(verilogFile, verilog);

    std::cout << "转换完成" << std::endl;

    //TEST ASAP
    //ASAPScheduler asapScheduler;
    //asapScheduler.schedule(verilog);
    //verilog.printSchedule(verilog, asapScheduler);

	//TEST ALAP
    ALAP ALAPscheduler;
	ALAPscheduler.schedule(verilog);
    ALAPscheduler.ALAPschedule(verilog);
    verilog.printSchedule(verilog, ALAPscheduler);
    
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
