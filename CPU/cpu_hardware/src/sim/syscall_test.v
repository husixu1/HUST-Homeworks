`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
//
// Create Date: 03/08/2018 11:17:35 AM
// Design Name:
// Module Name: syscall_test
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


module syscall_test();
    reg[31: 0] regSValue=0;
    reg[31: 0] regTValue=0;
    reg continue=0;
    reg syscall=0;
    reg clock=0;
    reg reset=0;
    wire enable;
    wire[31: 0] syscallOutput;

    Syscall syscallTest(.regSValue(regSValue), .regTValue(regTValue),
        .syscall(syscall), .clock(clock), .reset(reset), .continue(continue),
        .enable(enable), .syscallOutput(syscallOutput));

    always #5 clock = ~clock;
    initial begin
        #2 reset = 1;
        #10 reset = 0;

        #10 syscall = 1;
        regTValue = 100;
        regSValue = 1;

        #10 syscall = 0;

        #10 syscall = 1;
        regSValue = 32'h0000_000a;

        #10 syscall = 0;
        #10 syscall = 1;
        regSValue = 2;
        regTValue = 200;

        #30 syscall = 0;

        #10 reset = 1;
        #10 reset = 0;

        #10 syscall = 1;
        regSValue = 32'h0000_000a;

        #10 syscall = 0;
        #50 continue = 1;
        #10 continue = 0;
    end
endmodule
