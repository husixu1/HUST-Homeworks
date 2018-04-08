`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
//
// Create Date: 03/08/2018 02:18:10 PM
// Design Name:
// Module Name: clock_test
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


module clock_test();
    reg clock = 0;
    reg reset = 0;
    wire clockOut1;
    wire clockOut2;

    ClockDivider #(.DIVIDE(10))divider1(.clock(clock), .reset(reset), .clockOut(clockOut1));
    ClockDivider #(.DIVIDE(100))divider2(.clock(clock), .reset(reset), .clockOut(clockOut2));

    always #5 clock = ~clock;
    initial begin
        #5 reset = 1;
        #10 reset = 0;
        #10000 reset = 1;
        #10 reset = 0;
    end
endmodule
