`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
//
// Create Date: 03/08/2018 09:31:40 AM
// Design Name:
// Module Name: pc_test
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


module pc_test();
    reg [31: 0] originalPc = 0;
    reg [31: 0] regSValue=0;
    reg [25: 0] instruction=0;
    //reg [15: 0] immediate=0;
    reg [1: 0] pcWrite=0;
    reg aluEqual=0;
    reg reset=0;
    reg enable=1;
    reg clock=0;
    reg jump=0;
    reg bltz = 0;
    reg bubble = 0;
    wire jumped;
    wire [31: 0] pc;
    wire [24: 0] totalCycle;
    wire [24: 0] unconditionalJump;
    wire [24: 0] conditionalJump;
    wire [24: 0] conditionalSuccessfulJump;

    Pc testpc(.originalPc(originalPc), .regSValue(regSValue), .instruction(instruction), //.immediate(immediate),
            .pcWrite(pcWrite), .aluEqual(aluEqual), .reset(reset), .enable(enable),
            .clock(clock), .jump(jump), .bltz(bltz), .bubble(bubble), .jumped(jumped),
            .pc(pc), .totalCycle(totalCycle), .unconditionalJump(unconditionalJump),
            .conditionalJump(conditionalJump), .conditionalSuccessfulJump(conditionalSuccessfulJump)
        );

    always #5 clock = ~clock;
    initial begin
        #5 reset = 1;
        #10 reset = 0;

        // jump test
        #10 jump = 1;
        instruction = 32'h0c25;
        #10 instruction = 32'h0c64;

        // do noting test
        #10 jump = 0;
        pcWrite = 0;
        #10 pcWrite = 0;

        // jr test
        #10 pcWrite = 1;
        regSValue = 4000;
        #10 regSValue = 8;

        // beq test
        #10 pcWrite =2;
        aluEqual = 1;
        //immediate = 10;
        //#10 immediate = -6;
        #10 aluEqual = 0;

        // bne test
        #10 pcWrite =3;
        aluEqual = 0;
        //Immediate = 10;
        //#10 immediate = -6;
        #10 aluEqual = 1;

        // bltz test
        #10 bltz = 1;
        pcWrite = 0;
        jump = 0;
        instruction = 4;
        regSValue = -32'h1;
        #10 regSValue = 32'h1;

        // enable test
        #10 enable = 0;
        #10 enable = 1;

        // reset test
        #10 reset = 1;
        #10 reset = 0;
    end
endmodule
