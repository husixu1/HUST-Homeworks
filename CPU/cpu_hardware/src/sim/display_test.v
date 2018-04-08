`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
//
// Create Date: 03/08/2018 02:47:44 PM
// Design Name:
// Module Name: display_test
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


module display_test();
        reg[31: 0] data = 0;
        reg clock = 0;
        reg reset = 0;
        wire[7:0] cathode;    // from 7:CA to 0:DP
        wire[7:0] anode;      // form 7:AN7 to 0:AN0

        Display display(.data(data), .clock(clock), .reset(reset), .cathode(cathode), .anode(anode));

        always #0.001 clock = ~clock;
        initial begin
            #10 reset =1;
            #1000 reset = 0;
            #1000 data = 32'hffff_ffff;
            #1000 data = 32'h0000_0000;
            #1000 data = 32'h1234_5678;
            #1000 data = 32'h90ab_cdef;
            #1000 reset = 1;
            #1000 reset = 0;
        end
endmodule
