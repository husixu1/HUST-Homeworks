`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date: 03/07/2018 09:59:07 AM
// Design Name:
// Module Name: mux_test
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


module mux_test();
    reg  [2:0] select;
    reg  [7:0] data [7:0];
    wire [7:0] result;

    reg [8:0]temp;
    Mux #(.SELECT_WIDTH(3), .DATA_WIDTH(8)) mux1(.datas({data[7], data[6], data[5], data[4], data[3], data[2], data[1], data[0]}), .select(select), .result(result));
    initial begin
        select = 0;
        for(temp = 0; temp < 8; temp = temp + 1) begin
            data[temp] = temp;
        end

        #10 select = 1;
        #10 select = 2;
        #10 select = 3;
        #10 select = 4;
        #10 select = 5;
        #10 select = 6;
        #10 select = 7;
        #10 select = 6;
        #10 select = 5;
        #10 select = 4;
        #10 select = 3;
        #10 select = 2;
        #10 select = 1;
        #10 select = 0;
    end
endmodule
