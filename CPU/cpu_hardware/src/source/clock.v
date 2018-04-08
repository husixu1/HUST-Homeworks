`timescale 10ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
//
// Create Date: 03/08/2018 11:48:30 AM
// Design Name:
// Module Name: clock
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


///@brief frequency divider
module ClockDivider #(
        parameter DIVIDE = 100
    )(
        input wire clock,
        input wire reset,
        output reg clockOut = 0
    );

    reg[31: 0] counter = 0;

    always @(posedge clock) begin
        if(counter == DIVIDE) begin
            clockOut <= ~clockOut;
            counter <= 0;
        end else begin
            counter <= counter + 1;
        end
    end
endmodule
