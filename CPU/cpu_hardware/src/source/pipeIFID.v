`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
//
// Create Date: 03/20/2018 03:50:47 PM
// Design Name:
// Module Name: pipeIFID
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


module PipeIFID(
        input wire clock,
        input wire enable,
        input wire clear,
        input wire[31: 0] instruction,
        input wire[31: 0] pc,
        output reg[31: 0] instructionOut = 0,
        output reg[31: 0] pcOut = 0
    );

    always @(posedge clock) begin
        if(enable) begin
            if (clear) begin
                instructionOut <= 32'h0000_0000;
                pcOut <= 32'h0000_0000;
            end else begin
                instructionOut <= instruction;
                pcOut <= pc;
            end
        end else begin
            instructionOut <= instructionOut;
            pcOut <= pcOut;
        end
    end
endmodule
