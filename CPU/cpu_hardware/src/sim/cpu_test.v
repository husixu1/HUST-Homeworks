`timescale 10ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
//
// Create Date: 03/10/2018 10:18:01 PM
// Design Name:
// Module Name: cpu_test
// Project Name:
// Target Devices:
// Tool Versions:
// Description:
//
// Dependencies:
//
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
//
//////////////////////////////////////////////////////////////////////////////////


module cpu_test();
    reg clock = 0;
    reg resetButton = 1;
    reg frequency = 0;
    reg continue = 0;
    reg[2: 0] interrupt = 0;
    reg[2: 0] functionNumber = 0;
    reg[9: 0] checkRamAddress = 0;

    wire[7: 0]  anode;
    wire[7: 0]  cathode;

    Cpu cpu(
        .rawClock(clock),
        .resetButton(resetButton),
        .frequency(frequency),
        .radix(1'b0),
        .continue(continue),
        .interrupt(interrupt),
        .functionNumber(functionNumber),
        .checkRamAddress(checkRamAddress),
        .anode(anode),
        .cathode(cathode)
    );

    always #0.5 clock = ~clock;
    initial begin
        resetButton = 0;
        #10 resetButton = 1;

//        #110 interrupt = 3'b010;
//        #50 interrupt = 3'b000;

//        #200 interrupt = 3'b100;
//        #50 interrupt = 3'b000;

//        #1900 continue = 1;
//        #10 continue = 0;

//        #10000 continue = 1;
//        #10 continue = 0;
    end
endmodule
