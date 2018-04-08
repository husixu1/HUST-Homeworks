`timescale 10ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
//
// Create Date: 03/07/2018 05:53:18 PM
// Design Name:
// Module Name: ram
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

/// @brief random access memory
module Ram (
        input wire clock,
        input wire reset,
        input wire store,
        input wire [9: 0] address,
        input wire [31: 0] data,
        output wire [31: 0] result
    );
    reg [31: 0] ram[255: 0];
    assign result = ram[(address >> 2)];

    integer i;
    initial begin
        for(i = 0; i < 256; i = i+1) begin
            ram[i] = 0;
        end
    end
//    reg [31:0] counter;
    always @(posedge clock) begin
        if (store) begin
            ram[(address >> 2)] <= data;
        end
        // TODO: don't add this, use readmemh instead
//        if (reset) begin
//            for(counter=0; counter < RAM_SIZE; counter = counter + 1) begin
//                ram[counter] = 0;
//            end
//        end
    end
endmodule
