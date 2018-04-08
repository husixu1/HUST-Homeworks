`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date: 03/20/2018 04:16:49 PM
// Design Name:
// Module Name: pipe_test
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


module pipe_test();
    reg clock = 0;
    reg clear = 0;
    reg enable = 1;
    reg[31: 0] data1;
    reg[31: 0] data2;

    wire[31: 0] data1Out;
    wire[31: 0] data2Out;

    PipeIFID pipeIFID(.clock(clock), .clear(clear), .enable(enable),
        .instruction(data1), .instructionOut(data1Out),
        .pc(data2), .pcOut(data2Out));

    always #5 clock = ~clock;
    initial begin
        #10 data1 = 32'h1234_5678;
        #10 data2 = 32'h8765_4321;
        #10 clear = 1;
        #10 clear = 0;
        #10 enable = 0;
        #20 enable = 1;
    end
endmodule
