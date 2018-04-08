`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date: 03/23/2018 01:15:16 AM
// Design Name:
// Module Name: interrupt_test
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


module interrupt_test();
    reg clock = 0;
    reg[2: 0] interrupt = 0;         // 3 bits, 3 interrupts
    reg interruptEnd = 0;
    wire [2: 0] interruptOut;
    wire interrupted;
    wire interruptReturned;

    Interrupt interrupt_test(
        .clock(clock),
        .interrupt(interrupt),         // 3 bits, 3 interrupts
        .interruptEnd(interruptEnd),
        .interruptOut(interruptOut),
        .interrupted(interrupted),
        .interruptReturned(interruptReturned)
    );

    always #5 clock = !clock;

    initial begin
        #3 interrupt = 3'b001;
        #10 interrupt = 3'b000;

        #20 interrupt = 3'b010;
        #10 interrupt = 3'b000;

        #20 interrupt = 3'b100;
        #10 interrupt = 3'b000;

        #30 interruptEnd = 1;
        #10 interruptEnd = 0;

        #30 interruptEnd = 1;
        #10 interruptEnd = 0;

        #30 interruptEnd = 1;
        #10 interruptEnd = 0;


        #30 interrupt = 3'b010;
        #10 interrupt = 3'b000;

        #20 interrupt = 3'b100;
        #10 interrupt = 3'b000;

        #20 interrupt = 3'b001;
        #10 interrupt = 3'b000;

        #30 interruptEnd = 1;
        #10 interruptEnd = 0;

        #30 interruptEnd = 1;
        #10 interruptEnd = 0;

        #30 interruptEnd = 1;
        #10 interruptEnd = 0;
    end
endmodule
