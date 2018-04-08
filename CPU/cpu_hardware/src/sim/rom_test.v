`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
// 
// Create Date: 03/07/2018 05:43:44 PM
// Design Name: 
// Module Name: rom_test
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


module rom_test();
    reg [31:0] address;
    wire [31:0] result;
    Rom rom(.address(address), .result(result));
    initial begin
        address = 0;
        #10 address = 4;
        #10 address = 8;
        #10 address = 12;
        #10 address = 40;
        #10 address = 3;
        #10 address = 1;
        #10 address = 1000;
    end
endmodule
