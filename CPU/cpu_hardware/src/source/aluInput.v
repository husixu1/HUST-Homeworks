`timescale 10ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date: 03/08/2018 04:06:19 PM
// Design Name:
// Module Name: aluInput
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


/// @brief this module decide where alu's inpu came from
module AluInput(
        input wire[31: 0] pc,
        input wire[31: 0] regTValue,
        input wire[31: 0] regSValue,
        input wire[15: 0] instruction,
        input wire[1: 0] aluX,
        input wire[2: 0] aluY,
        output reg[31: 0] resultX = 0,
        output reg[31: 0] resultY = 0
    );

    always @(pc, regTValue, regSValue, instruction, aluX, aluY) begin
        case (aluX)
            2'd0: resultX = regSValue;
            2'd1: resultX = regTValue;
            2'd2: resultX = pc;
            default: resultX = 32'h0;
        endcase

        case (aluY)
            3'd0: resultY = regTValue;
            3'd1: resultY = {27'h0, instruction[10: 6]};
            3'd2: resultY = {{16{instruction[15]}}, instruction[15: 0]};
            3'd3: resultY = 32'h4;
            3'd4: resultY = regSValue;
            3'd5: resultY = {16'h0000, instruction[15: 0]};
            default: resultY = 32'h0;
        endcase
    end
endmodule

