`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: CodeDragon
//
// Create Date: 2018/03/07 20:56:33
// Design Name:
// Module Name: reg_test
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


module reg_test();
    reg clk = 0;
    reg [31: 0] data;
    reg we;
    reg [4: 0] write, a, b;
    wire [31: 0] resultA;
    wire [31: 0] resultB;

    always #10 clk = ~clk;
    Reg register(.clock(clk), .writeData(data), .writeEnable(we),
            .regWrite(write), .regA(a), .regB(b),
            .resultA(resultA), .resultB(resultB));

    initial begin

    data = 100;
    we = 0;
    write = 0;
    a = 0;
    b = 0;

    // try to write 100 to reg0
    #10 we = 1;
    #10 we = 0;

    // try to write 200 to reg10
    // and read value from reg10
    write = 10;
    a = 10;
    data = 200;
    #10 we = 1;
    #10 we = 0;

    // try to read value from reg31 and reg10
    #5 write = 31;
    data = 300;
    a = 31;
    b = 10;

    end
endmodule
