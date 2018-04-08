`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
//
// Create Date: 03/20/2018 03:59:27 PM
// Design Name:
// Module Name: pipeIDEX
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


module PipeIDEX(
        input wire clock,
        input wire enable,
        input wire clear,
        input wire[31: 0] control,
        input wire[31: 0] instruction,
        input wire[31: 0] pc,
        input wire[31: 0] regSValue,
        input wire[31: 0] regTValue,
        input wire[31: 0] epc,
        output reg[31: 0] controlOut = 0,
        output reg[31: 0] instructionOut = 0,
        output reg[31: 0] pcOut = 0,
        output reg[31: 0] regSValueOut = 0,
        output reg[31: 0] regTValueOut = 0,
        output reg[31: 0] epcOut = 0
    );
    always @(posedge clock) begin
        if(enable) begin
            if (clear) begin
                controlOut <= 32'h0000_0000;
                instructionOut <= 32'h0000_0000;
                pcOut <= 32'h0000_0000;
                regSValueOut <= 32'h0000_0000;
                regTValueOut <= 32'h0000_0000;
                epcOut <= 32'h0000_0000;
            end else begin
                controlOut <= control;
                instructionOut <= instruction;
                pcOut <= pc;
                regSValueOut <= regSValue;
                regTValueOut <= regTValue;
                epcOut <= epc;
            end
        end else begin
            controlOut <= controlOut;
            instructionOut <= instructionOut;
            pcOut <= pcOut;
            regSValueOut <= regSValueOut;
            regTValueOut <= regTValueOut;
            epcOut <= epcOut;
        end
    end
endmodule
