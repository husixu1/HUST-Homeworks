`timescale 10ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
//
// Create Date: 03/07/2018 07:28:42 PM
// Design Name:
// Module Name: alu
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


/// @brief the arithmetic logic unit
module Alu(
        input wire[31: 0] x,
        input wire[31: 0] y,
        input wire[3: 0] operator,
        output reg[31: 0] result = 0,
        output reg[31: 0] result2 = 0,
//        output reg signedOverflow,
//        output reg unsignedOverflow,
        output wire equal
    );

    assign equal = (x == y);

//    wire[63: 0] wideResult;
//    assign wideResult = {{32{x[31]}}, x} * {{32{y[31]}}, y};

    always @(x, y, operator) begin
        case (operator)
            4'd0: result = x << y[4: 0];
            4'd1: result = x[31] ? ((~(32'hffff_ffff >> y[4: 0])) | (x >> y[4: 0])) : (x >> y[4:0]);
            4'd2: result = x >> y[4: 0];
            4'd3: result = x * y;
            4'd4: result = x / y;
            4'd5: result = x + y;
            4'd6: result = x - y;
            4'd7: result = x & y;
            4'd8: result = x | y;
            4'd9: result = x ^ y;
            4'd10: result = ~(x | y);
            4'd11: result = {31'd0, ($signed(x) < $signed(y))};
            4'd12: result = {31'd0, (x < y)};
            default: result = 0;
        endcase

//        // set result2
//        case (operator)
//            4'd3: result2 = wideResult[63: 32];
//            4'd4: result2 = x % y;
//            default: result2 = 0;
//        endcase
    end
endmodule
