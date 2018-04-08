`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/13/2018 11:33:09 AM
// Design Name: 
// Module Name: test_display_hardware
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


module test_display_hardware(
        output wire[7: 0] cathode,
        output wire[7: 0] anode,
        input wire clock
    );
    Display test_display(.data(32'h12345678), .clock(clock), .reset(0), .cathode(cathode), .anode(anode));
endmodule
