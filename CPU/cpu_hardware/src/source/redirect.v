`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
//
// Create Date: 03/20/2018 08:30:05 PM
// Design Name:
// Module Name: redirect
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


module Redirect(
        input wire regARead,
        input wire regBRead,
        input wire[4: 0] regA,
        input wire[4: 0] regB,
        input wire regWriteEable_ME,
        input wire[4: 0] regWrite_ME,
        input wire regWriteEable_WB,
        input wire[4: 0] regWrite_WB,
        input wire epcRead,
        input wire epcWrite_ME,
        input wire epcWrite_WB,
        output wire [3: 0] redirect,
        output wire[1:0] redirectEpc
    );
    assign redirect = {
            ((regARead && (regWrite_ME != 0)) && ((regWrite_ME == regA) && (regA != 0)) && regWriteEable_ME),
            ((regBRead && (regWrite_ME != 0)) && ((regWrite_ME == regB) && (regB != 0)) && regWriteEable_ME),
            ((regARead && (regWrite_WB != 0)) && ((regWrite_WB == regA) && (regA != 0)) && regWriteEable_WB),
            ((regBRead && (regWrite_WB != 0)) && ((regWrite_WB == regB) && (regB != 0)) && regWriteEable_WB)
        };

    assign redirectEpc = {
            epcRead && epcWrite_ME,
            epcRead && epcWrite_WB
        };
endmodule
