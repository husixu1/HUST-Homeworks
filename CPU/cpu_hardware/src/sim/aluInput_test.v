`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
//
// Create Date: 03/08/2018 04:30:15 PM
// Design Name:
// Module Name: aluInput_test
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


module aluInput_test();
    reg[31: 0] pc = 104;
    reg[31: 0] regTValue = 32'h1234;
    reg[31: 0] regSValue = 32'h5678;
    reg[31: 0] instruction = 32'habcd;
    reg[1: 0] aluX = 0;
    reg[2: 0] aluY = 0;
    wire[31: 0] resultX;
    wire[31: 0] resultY;

    AluInput aluInput(.pc(pc), .regTValue(regTValue), .regSValue(regSValue),
        .instruction(instruction), .aluX(aluX), .aluY(aluY), .resultX(resultX), .resultY(resultY));

    initial begin
        #10 aluX = 1;
        #10 aluX = 2;
        #10 aluX = 3;

        #10 aluY = 1;
        #10 aluY = 2;
        #10 aluY = 3;
        #10 aluY = 4;

        #10 aluX = 3;
        #10 aluX = 2;
        #10 aluX = 1;
    end
endmodule
