`timescale 10ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: CodeDragon
//
// Create Date: 2018/03/08 08:35:37
// Design Name:
// Module Name: controller
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

// @brief CPU controller

module Controller(
        input wire[31: 0] instruction,
//        output wire[3: 0] aluOperator,
//        output wire[1: 0] aluX,
//        output wire[2: 0] aluY,
//        output wire regWriteEnable,
//        output wire[1: 0] regWriteDestinationControl,
//        output wire regWriteSourceControl,
//        output wire ramWrite,
//        output wire[1: 0] pcWrite,
//        output wire jump,
//        output wire syscall,
//        output wire lbu,
//        output wire bltz,
//        output wire regARead,
//        output wire regBRead,
	output reg[31: 0] result = 0
    );

    wire[5: 0] operator;
    wire[5: 0] special;
    wire[4: 0] interrupt;
    assign operator = instruction[31: 26];
    assign special = instruction[5: 0];
    assign interrupt = instruction[25: 21];
    // reg[31: 0] result;

    //  result layout
    //   aluOperator aluX aluY rWE rWD rWS ramWrite pcWrite jump syscall lbu bltz rARead rBRead mfc0 mtc0 eret RESERVED
    //  |          |    |    |   |   |   |        |       |    |       |   |    |      |      |    |    |    |        |
    // 32         28   26   23  22  20  19       18      16   15      14  13   12     11     10    9    8    7        0
    // first bit of RESERVE is lbu
    // second bit of RESERVE is bltz

    //assign aluOperator = result[31: 28];
    //assign aluX = result[27: 26];
    //assign aluY = result[25: 23];
    //assign regWriteEnable = result[22];
    //assign regWriteDestinationControl = result[21: 20];
    //assign regWriteSourceControl = result[19];
    //assign ramWrite = result[18];
    //assign pcWrite = result[17: 16];
    //assign jump = result[15];
    //assign syscall = result[14];
    //assign lbu = result[13];
    //assign bltz = result[12];
    //assign regARead = result[11];
    //assign regBRead = result[10];

    always @ (operator, special, interrupt) begin
        if(operator) begin
            case (operator)
                //                                 22   19     15  13  11
                //                    32  28 26  23 | 20 |18 16 |14 |12 |10          0
                //                     |   |  |   | |  | | |  | | | | | | |          |
                6'b001000: result = 32'b0101_00_010_1_10_1_0_00_0_0_0_0_1_0_000_0000000; // addi
                6'b001001: result = 32'b0101_00_010_1_10_1_0_00_0_0_0_0_1_0_000_0000000; // addiu
                6'b001100: result = 32'b0111_00_101_1_10_1_0_00_0_0_0_0_1_0_000_0000000; // andi
                6'b001101: result = 32'b1000_00_101_1_10_1_0_00_0_0_0_0_1_0_000_0000000; // ori
                6'b100011: result = 32'b0101_00_010_1_10_0_0_00_0_0_0_0_1_0_000_0000000; // lw
                6'b101011: result = 32'b0101_00_010_0_10_1_1_00_0_0_0_0_1_1_000_0000000; // sw
                6'b000100: result = 32'b1110_00_000_0_10_1_0_10_0_0_0_0_1_1_000_0000000; // beq
                6'b000101: result = 32'b1110_00_000_0_10_1_0_11_0_0_0_0_1_1_000_0000000; // bne
                6'b001010: result = 32'b1011_00_010_1_10_1_0_00_0_0_0_0_1_0_000_0000000; // slti
                6'b000010: result = 32'b0000_00_000_0_10_1_0_00_1_0_0_0_0_0_000_0000000; // j
                6'b000011: result = 32'b0101_10_011_1_11_1_0_00_1_0_0_0_0_0_000_0000000; // jal
                6'b001011: result = 32'b1100_00_010_1_10_1_0_00_0_0_0_0_1_0_000_0000000; // sltiu (HuSixu's ccmb)
                6'b100100: result = 32'b0101_00_010_1_10_0_0_00_0_0_1_0_1_0_000_0000000; // lbu (HuSixu's ccmb)
                6'b000001: result = 32'b1011_00_000_0_10_1_0_00_0_0_0_1_1_0_000_0000000; // bltz (HuSixu's ccmb)
                6'b010000: begin
                    case (interrupt)
                 5'b00100: result = 32'b0000_00_000_0_00_1_0_00_0_0_0_0_0_1_100_0000000; // mtc0
                 5'b00000: result = 32'b0000_00_000_1_10_1_0_00_0_0_0_0_0_0_010_0000000; // mfc0
                 5'b10000: result = 32'b0000_00_000_0_00_0_0_00_0_0_0_0_0_0_001_0000000; // eret
                 default: result = 32'h0;
                    endcase
                end
                default: result = 32'h0;
            endcase
        end else begin
            case (special)
                6'b100000: result = 32'b0101_00_000_1_00_1_0_00_0_0_0_0_1_1_000_0000000; // add
                6'b100001: result = 32'b0101_00_000_1_00_1_0_00_0_0_0_0_1_1_000_0000000; // addu
                6'b100100: result = 32'b0111_00_000_1_00_1_0_00_0_0_0_0_1_1_000_0000000; // and
                6'b000000: result = 32'b0000_01_001_1_00_1_0_00_0_0_0_0_0_1_000_0000000; // sll
                6'b000011: result = 32'b0001_01_001_1_00_1_0_00_0_0_0_0_0_1_000_0000000; // sra
                6'b000010: result = 32'b0010_01_001_1_00_1_0_00_0_0_0_0_1_1_000_0000000; // srl
                6'b100010: result = 32'b0110_00_000_1_00_1_0_00_0_0_0_0_1_1_000_0000000; // sub
                6'b100101: result = 32'b1000_00_000_1_00_1_0_00_0_0_0_0_1_1_000_0000000; // or
                6'b100111: result = 32'b1010_00_000_1_00_1_0_00_0_0_0_0_1_1_000_0000000; // nor
                6'b101010: result = 32'b1011_00_000_1_00_1_0_00_0_0_0_0_1_1_000_0000000; // slt
                6'b101011: result = 32'b1100_00_000_1_00_1_0_00_0_0_0_0_1_1_000_0000000; // sltu
                6'b001000: result = 32'b0000_00_000_0_00_1_0_01_0_0_0_0_1_0_000_0000000; // jr
                6'b001100: result = 32'b0000_00_000_0_00_1_0_00_0_1_0_0_1_1_000_0000000; // sys
                6'b000110: result = 32'b0010_01_100_1_00_1_0_00_0_0_0_0_0_1_000_0000000; // srlv (HuSixu's ccmb)
                default: result = 32'h0;
            endcase
        end
    end
endmodule
