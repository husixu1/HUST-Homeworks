`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
//
// Create Date: 03/07/2018 09:46:19 PM
// Design Name:
// Module Name: alu_test
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


module alu_test();
    reg[31: 0] x = 0;
    reg[31: 0] y = 0;
    reg[3: 0] op = 0;
    wire[31:0] result;
    wire[31:0] result2;
    wire eq;

    Alu alu(.x(x), .y(y), .operator(op), .result(result), .result2(result2), .equal(eq));

    initial begin
        // shift left
        #10 op = 0;
        x = 2;
        y = 4;
        #10 x = 3;
        y = 4;

        // arithmetic shift right
        #10 op = 1;
        x = 8;
        y = 1;
        #10 x = -19;
        y = 3;

        // logic shift rightt
        #10 op = 2;
        x = 8;
        y = 2;
        #10 x = -20;
        y = 4;

        // multiply
        #10 op = 3;
        x = 2;
        y = 3;
        #10 x = 65537;
        y = 65537;
        #10 x = -3;
        y = 4;
        #10 x = -3;
        y = -4;

        // divide
        #10 op = 4;
        x = 16;
        y = 4;
        #10 x = 19;
        y = 7;

        // add
        #10 op = 5;
        x = 2;
        y = 15;
        #10 x = 1;
        y = -3;
        #10 x = 2000000000;
        y = 2000000000;
        #10 x = 4000000000;
        y = 4000000000;
        #10 x = -2000000000;
        y = -2000000000;

        // sub
        #10 op = 6;
        x = 20;
        y = 9;
        #10 x = 1;
        y = -1;
        #10 x = -2000000000;
        y = 2000000002;
        #10 x = 0;
        y = 1;
        #10 x = 10;
        y = -2000000000;

        // and
        #10 op = 7;
        x = 3;
        y = 9;

        // or
        #10 op = 8;
        x = 3;
        y = 9;

        // xor
        #10 op = 9;
        x = 3;
        y = 9;

        // nor
        #10 op = 10;
        x = 3;
        y = 9;

        // <
        #10 op = 11;
        x = 2;
        y = 3;
        #10 x = 3;
        y = 2;

        // >
        #10 op = 12;
        x = 2;
        y = 3;
        #10 x = 3;
        y = 2;
    end
endmodule
