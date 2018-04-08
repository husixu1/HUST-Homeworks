`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
//
// Create Date: 03/22/2018 11:49:38 PM
// Design Name:
// Module Name: interrupt
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


module Interrupt(
        input wire clock,
        input wire[2: 0] interrupt,         // 3 bits, 3 interrupts
        input wire interruptEnd,
        input wire reset,
        output reg[2: 0] interruptOut,
        output reg interrupted = 0
    );

    reg[2: 0] activatedInterrupt = 0;
    reg[2: 0] lastInterruptOut = 0;
    reg[2: 0] startedProcess = 0;

    always @(activatedInterrupt) begin
        case (activatedInterrupt)
            3'b100, 3'b101, 3'b110, 3'b111: interruptOut = 3'b100;
            3'b010, 3'b011:                 interruptOut = 3'b010;
            3'b001:                         interruptOut = 3'b001;
            3'b000:                         interruptOut = 3'b000;
        endcase
    end

    always @(posedge clock) begin
        if (!reset) begin
            // processing interrupt hierarchy
            if (interruptOut > lastInterruptOut) begin
                // start processing higher priority interrupt
                startedProcess <= startedProcess | interruptOut;
                lastInterruptOut <= interruptOut;
                interrupted <= 1;
            end else if (interruptOut < lastInterruptOut) begin
                // stop processing higher priority interrupt
                startedProcess <= startedProcess & (~lastInterruptOut);

                // judge if this is a new interrupt or and interrupted interrupt
                if((startedProcess & interruptOut) || interruptOut == 0) begin
                    // if already started
                    lastInterruptOut <= interruptOut;
                    interrupted <= interrupted;
                end else begin
                    // if this is a new interrupt
                    lastInterruptOut <= interruptOut;
                    interrupted <= 1;
                end
            end else begin
                startedProcess <= startedProcess;
                lastInterruptOut <= interruptOut;
                interrupted <= 0;
            end
        end else begin
            startedProcess <= 0;
            lastInterruptOut <= 0;
            interrupted <= 0;
        end

            // interrupt capture and ending
        if(!reset) begin
            if(interruptEnd) begin
                if(activatedInterrupt[2]) begin
                    activatedInterrupt[2] <= 0;
                end else if(activatedInterrupt[1]) begin
                    activatedInterrupt[1] <= 0;
                end else if(activatedInterrupt[0]) begin
                    activatedInterrupt[0] <= 0;
                end else begin
                    activatedInterrupt <= activatedInterrupt;
                end
            end else begin
                activatedInterrupt <= activatedInterrupt | interrupt;
            end
        end else begin
            activatedInterrupt <= 0;
        end
    end
endmodule
