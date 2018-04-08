`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
//
// Create Date: 03/07/2018 06:36:01 PM
// Design Name:
// Module Name: ram_test
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


module ram_test();
    reg [31:0] address;
    reg [31:0] data;
    reg store;
    reg clk = 0;
    reg reset = 0;
    wire [31:0] result;

    always #5 clk = ~clk;
    Ram ram(.clock(clk), .reset(reset), .store(store), .address(address), .data(data), .result(result));
    initial begin
        address = 0;
        data = 0;

        // store 100 to addr 4
        #5 store = 1;
        address = 4;
        data = 100;
        #10 store = 0;

        // store 200 to addr 8
        #10 store = 1;
        address = 8;
        data = 200;
        #10 store = 0;

        // display data at 4 / 8
        #10 address = 4;
        #10 address = 8;

        // reset all data
        #10 reset = 1;
        #10 reset = 0;

        #10 address = 4;
    end
endmodule
