`timescale 10ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: CodeDragon
//
// Create Date: 2018/03/07 19:06:56
// Design Name:
// Module Name: Reg
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

// @brief MIPS cpu registers from 0 to 31

module Reg(
        input wire clock,
        input wire[31: 0] writeData,
        input wire writeEnable,
        input wire[4: 0] regWrite,
        input wire[4: 0] regA,
        input wire[4: 0] regB,
        output wire[31: 0] resultA,
        output wire[31: 0] resultB
    );

    reg[31: 0] regfile[31: 0];

    integer i;
    initial begin
        for(i = 0; i < 32; i = i + 1) begin
            regfile[i] = 0;
        end
    end

    always @ (negedge clock) begin
        if (writeEnable && regWrite != 0) begin
            regfile[regWrite] <= writeData;
        end
    end

    assign resultA = (regA == 0) ? 0 : regfile[regA];

    assign resultB = (regB == 0) ? 0 : regfile[regB];

endmodule
