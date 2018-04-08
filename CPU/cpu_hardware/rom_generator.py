#!/bin/env python3
import sys

romHeader = """
`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// This is a python-generated file defining rom
//////////////////////////////////////////////////////////////////////////////////

/// @brief the instruction rom (16 kbytes, address must be 4-bytes aligned)
// Usage:
//      Rom <name> (.address(<address>), .result(<result>))
module Rom #(
        ADDRESS_WIDTH = 32,
        DATA_WIDTH = 32
    )(
        input wire[ADDRESS_WIDTH - 1: 0] address,
        output reg[DATA_WIDTH - 1: 0] result
    );

    always @(address) begin
        case (address)
"""
romFooter = """
        default: result = 32'h00000000;
        endcase
    end
endmodule
"""

def main():
    if len(sys.argv) < 2:
        print("Usage: {} <raw-hex-file>".format(sys.argv[0]))
    # open file for processing
    with open(sys.argv[1], "r") as rawfile:
        lines = rawfile.readlines()

        # if this is a file for logisim
        if lines[0] == "v2.0 raw\n":
            lines.pop(0)

        # open file for output
        with open("rom.v", "w") as outFile:
            outFile.write(romHeader)
            counter = 0;
            for line in lines:
                outFile.write("        32'h{:08x}: result = 32'h{};\n".format(counter, line[:-1]))
                counter = counter + 4
            outFile.write(romFooter)

if __name__ == "__main__":
    main()
