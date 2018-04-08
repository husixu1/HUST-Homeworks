`timescale 10ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
//
// Create Date: 03/07/2018 09:46:05 AM
// Design Name:
// Module Name: mux
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

// @brief paramerized multiplexer implementation
// usage:
//     Mux #(.DATA_WITDH(<width>), .SELECT_WIDTH(<width2>)) \
//     <name> (.datas({<data_highest>, ..., <data_lowest>}), .select(<select>), .result(<output-wire>))
module Mux #(
        parameter DATA_WIDTH = 8,
        parameter SELECT_WIDTH = 1
    )(
        input wire [DATA_WIDTH * (1 << SELECT_WIDTH) - 1: 0] datas,
        input wire [SELECT_WIDTH - 1: 0] select,
        output wire [DATA_WIDTH - 1: 0] result
    );
    assign result = datas[select * DATA_WIDTH +: DATA_WIDTH];
endmodule
