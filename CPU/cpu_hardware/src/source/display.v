`timescale 10ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
//
// Create Date: 03/08/2018 11:41:50 AM
// Design Name:
// Module Name: display
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

/// @brief the 7-segment display driver
module Display(
        input wire[31: 0] data,
        input wire radix,               // 1 to display as decimal format
        input wire clock,
        input wire reset,
        output reg[7:0] cathode = 0,    // from 7:CA to 0:DP
        output reg[7:0] anode = 0       // form 7:AN7 to 0:AN0
    );
    reg[31: 0] displayData = 0;
    reg[2:0] counter = 0;
    wire displayClock;

    // divide the clock by 100,000 for human-eye-fitting display
    ClockDivider #(10000)displayDivider(.clock(clock), .reset(reset), .clockOut(displayClock));

    always @(posedge displayClock) begin
        // select the right tube
        anode <= ~(8'h1 << counter);
        case (displayData[counter * 4 +: 4])
            4'h0: cathode <= 8'b0000_0011;
            4'h1: cathode <= 8'b1001_1111;
            4'h2: cathode <= 8'b0010_0101;
            4'h3: cathode <= 8'b0000_1101;
            4'h4: cathode <= 8'b1001_1001;
            4'h5: cathode <= 8'b0100_1001;
            4'h6: cathode <= 8'b0100_0001;
            4'h7: cathode <= 8'b0001_1111;
            4'h8: cathode <= 8'b0000_0001;
            4'h9: cathode <= 8'b0000_1001;
            4'ha: cathode <= 8'b0001_0001;
            4'hb: cathode <= 8'b1100_0001;
            4'hc: cathode <= 8'b0110_0011;
            4'hd: cathode <= 8'b1000_0101;
            4'he: cathode <= 8'b0110_0001;
            4'hf: cathode <= 8'b0111_0001;
        endcase

        // display next tube
        counter <= counter + 1;

        displayData[3: 0] <= radix ? (data % 10) : data[3: 0];
        displayData[7: 4] <= radix ?  (data / 10 % 10) : data[7: 4];
        displayData[11: 8] <= radix ? (data / 100 % 10) : data[11: 8];
        displayData[15: 12] <= radix ? (data / 1000 % 10) : data[15: 12];
        displayData[19: 16] <= radix ? (data / 10000 % 10) : data[19: 16];
        displayData[23: 20] <= radix ? (data / 100000 % 10) : data[23: 20];
        displayData[27: 24] <= radix ? (data / 1000000 % 10) : data[27: 24];
        displayData[31: 28] <= radix ? (data / 10000000 % 10) : data[31: 28];


        if(reset) begin
            displayData <= 0;
            counter <= 0;
            cathode <= 0;
            anode <= 0;
        end
    end
endmodule
