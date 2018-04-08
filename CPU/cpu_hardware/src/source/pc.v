`timescale 10ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
//
// Create Date: 03/08/2018 08:10:46 AM
// Design Name:
// Module Name: pc
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


/// @brief the pc controller
module Pc(
        input wire[31: 0] originalPc,
        input wire[31: 0] regSValue,
        input wire[25: 0] instruction,
        input wire[1: 0] pcWrite,
        input wire aluEqual,
        input wire reset,
        input wire enable,
        input wire clock,
        input wire jump,

        // interrupt additions
        input wire interrupted,
        input wire interruptReturned,
        input wire[2: 0] interrupt,
        input wire[31: 0] epc,

        input wire bltz,
        input wire bubble,
        input wire predicted,
        output reg jumped,
        output reg[31: 0] pc = 0,
        output reg[31: 0] totalCycle = 0,
        output reg[31: 0] unconditionalJump = 0,
        output reg[31: 0] conditionalJump = 0,
        output reg[31: 0] conditionalSuccessfulJump = 0,
        output reg[1: 0] cycleAfterJump = 0
    );
    wire pcEnable;
    assign pcEnable = (enable && !bubble);

    wire[15: 0] immediate;
    assign immediate = instruction[15: 0];

    wire[31: 0] jumpDestination;
    assign jumpDestination = (({4'b000000, instruction, 2'b00}) - 32'h0000_3000);

    wire[31: 0] branchDestination;
    assign branchDestination = {{14{immediate[15]}}, immediate, 2'b00} + originalPc + 4;

    reg [31: 0] interruptLocation;
    always @(interrupt) begin
        case(interrupt)
            3'b100, 3'b101, 3'b110, 3'b111: interruptLocation = 32'h0c;
            3'b010, 3'b011:                 interruptLocation = 32'h08;
            3'b001:                         interruptLocation = 32'h04;
            3'b000:                         interruptLocation = 32'b00;
        endcase
    end

    // 'jumped' should be a combinational circuit rather than a timing one
    always @(bltz, regSValue, jump, pcWrite, aluEqual) begin
        case({bltz && regSValue, jump})
            2'b00: begin
                case (pcWrite)
                    2'b00: jumped = 0;
                    2'b01: jumped = 1;
                    2'b10: begin
                        case (aluEqual)
                            1'b0: jumped = 0;
                            1'b1: jumped = 1;
                        endcase
                    end
                    2'b11: begin
                        case (aluEqual)
                            1'b0: jumped = 1;
                            1'b1: jumped = 0;
                        endcase
                    end
                endcase
            end
            2'b01: jumped = 1;
            2'b10: jumped = 1;
            2'b11: jumped = 0;
        endcase
    end

    always @(posedge clock) begin
        if (jumped == 1) begin
            cycleAfterJump <= 0;
        end else if(cycleAfterJump != 2) begin
            cycleAfterJump = cycleAfterJump + 1;
        end

        if (pcEnable && !reset) begin
            if(interrupted || interruptReturned) begin
                case({interrupted, interruptReturned})
                    2'b10: pc <= interruptLocation;
                    2'b01: pc <= epc;
                    2'b11: pc <= epc;
                    2'b00: pc <= 0;
                endcase
            end else begin
                // judge if this is a J instruction (or an interrupt)
                case ({(bltz && regSValue), jump})
                    // other jump conditino
                    2'b00: begin
                        // this is not a J instruction, judging by pcWrite hardware
                        // interface
                        case (pcWrite)
                            2'd0: pc <= pc + 4;
                            // jr
                            2'd1: pc <= regSValue;
                            // beq
                            2'd2: begin
                                if (aluEqual) begin
                                    pc <= branchDestination;
                                end else begin
                                    pc <= pc + 4;
                                end
                            end
                            // bne
                            2'd3: begin
                                if (!aluEqual) begin
                                    pc <= branchDestination;
                                end else begin
                                    pc <= pc + 4;
                                end
                            end
                        endcase
                    end
                    2'b01: pc <= jumpDestination;   // this is a J instruction
                    2'b10: pc <= branchDestination; // bltz, HuSixu's ccmb
                    default: pc <= 0;
                endcase
            end
        end else if (reset) begin
            pc <= 0;
        end else begin
            pc <= pc;
        end

        if (enable && !reset) begin
            if(predicted) begin
                totalCycle <= totalCycle - 1;
            end else begin
                totalCycle <= totalCycle + 1;
            end

            unconditionalJump <= unconditionalJump + (pcWrite == 1 || jump);
            conditionalJump <= conditionalJump + (pcWrite == 2 || pcWrite == 3);
            conditionalSuccessfulJump <= conditionalSuccessfulJump +
                ((pcWrite == 2 && aluEqual) || (pcWrite == 3 && !aluEqual));
        end else if (reset) begin
            totalCycle <= 0;
            unconditionalJump <= 0;
            conditionalJump <= 0;
            conditionalSuccessfulJump <= 0;
        end else begin
            totalCycle <= totalCycle;
            unconditionalJump <= unconditionalJump;
            conditionalJump <= conditionalJump;
            conditionalSuccessfulJump <= conditionalSuccessfulJump;
        end
    end
endmodule
