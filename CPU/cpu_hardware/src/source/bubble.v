`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
//
// Create Date: 03/20/2018 08:17:44 PM
// Design Name:
// Module Name: bubble
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


// bubble generator
module Bubble(
        input wire regARead,
        input wire regBRead,
        input wire[4: 0] regA,
        input wire[4: 0] regB,
        input wire regWriteEnable_EX,
        input wire[4: 0] regWrite_EX,
        input wire[31: 0] instruction_EX,
        output wire bubble
    );
    assign bubble = (
            ((regARead && (regWrite_EX != 0)) && ((regWrite_EX == regA) && (regA != 0)) && regWriteEnable_EX) ||
            ((regBRead && (regWrite_EX != 0)) && ((regWrite_EX == regB) && (regB != 0)) && regWriteEnable_EX)
        ) && ( instruction_EX[31: 26] == 6'h23 || instruction_EX[31: 26] == 6'h24);
endmodule
