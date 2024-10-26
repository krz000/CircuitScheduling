#include "circuit.h"
#include "ALAP.h"
#include "ASAP.h"

int main() {
    Circuit verilog;
    std::string blifFile = "test1.blif";//TODO
    std::string verilogFile = "test1.v";
    
    parseBLIF(blifFile, verilog);
    writeVerilog(verilogFile, verilog);

    std::cout << "×ª»»Íê³É" << std::endl;

    //TEST ASAP
    ASAPScheduler asapScheduler;
    asapScheduler.schedule(verilog);
    verilog.printSchedule(verilog, asapScheduler);

	//TEST ALAP
    ALAP ALAPscheduler;
	ALAPscheduler.schedule(verilog);
    ALAPscheduler.ALAPschedule(verilog);
    verilog.printSchedule(ALAPscheduler);

    return 0;
}
