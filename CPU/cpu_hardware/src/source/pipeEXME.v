`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date: 03/20/2018 04:09:06 PM
// Design Name:
// Module Name: pipeEXME
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


module PipeEXME(
        input wire clock,
        input wire enable,
        input wire clear,
        input wire[31: 0] control,
        input wire[31: 0] instruction,
        input wire[31: 0] aluR,
        input wire[31: 0] regSValue,
        input wire[31: 0] regTValue,
        input wire[31: 0] epc,
        output reg[31: 0] controlOut = 0,
        output reg[31: 0] instructionOut = 0,
        output reg[31: 0] aluROut = 0,
        output reg[31: 0] regSValueOut = 0,
        output reg[31: 0] regTValueOut = 0,
        output reg[31: 0] epcOut = 0
    );
    always @(posedge clock) begin
        if(enable) begin
            if (clear) begin
                controlOut <= 32'h0000_0000;
                instructionOut <= 32'h0000_0000;
                aluROut <= 32'h0000_0000;
                regSValueOut <= 32'h0000_0000;
                regTValueOut <= 32'h0000_0000;
                epcOut <= 32'h0000_0000;
            end else begin
                controlOut <= control;
                instructionOut <= instruction;
                aluROut <= aluR;
                regSValueOut <= regSValue;
                regTValueOut <= regTValue;
                epcOut <= epc;
            end
        end else begin
            controlOut <= controlOut;
            instructionOut <= instructionOut;
            aluROut <= aluROut;
            regSValueOut <= regSValueOut;
            regTValueOut <= regTValueOut;
            epcOut <= epcOut;
        end
    end
endmodule
