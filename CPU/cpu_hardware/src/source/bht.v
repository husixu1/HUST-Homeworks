`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date: 03/23/2018 08:40:12 AM
// Design Name:
// Module Name: bht
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


module Bht(
        input wire clock,
        input wire[31: 0] if_pc,
        input wire[31: 0] ex_pc,
        input wire isJump,
        output reg[31: 0] predict = 0,
        output wire predicted
    );

    reg[31: 0] lastEx_pc;
    reg lastIsJump;
    reg[7: 0] branchPredict;

    always @(posedge clock) begin
        lastEx_pc <= ex_pc;
        lastIsJump <= isJump;
    end

    // format:
    //   valid instructionAddr dstAddr predict LRU
    //  |    |               |       |       |   |
    // 70   69              37       5       3   0
    reg[69: 0] tab[7: 0];

    integer k;
    initial begin
        for(k = 0; k < 8; k = k + 1) begin
            tab[k] = 70'h0;
        end
    end

    wire[7: 0] compareResult;
    assign compareResult = {
            {lastEx_pc == tab[7][68: 37]},
            {lastEx_pc == tab[6][68: 37]},
            {lastEx_pc == tab[5][68: 37]},
            {lastEx_pc == tab[4][68: 37]},
            {lastEx_pc == tab[3][68: 37]},
            {lastEx_pc == tab[2][68: 37]},
            {lastEx_pc == tab[1][68: 37]},
            {lastEx_pc == tab[0][68: 37]}
        };

    wire[7: 0] validBits = {
            tab[7][69], tab[6][69], tab[5][69], tab[4][69],
            tab[3][69], tab[2][69], tab[1][69], tab[0][69]
        };

    always @(if_pc) begin
        if(compareResult) begin
            case (compareResult)
                8'b0000_0001: predict = tab[0][36: 5];
                8'b0000_0010: predict = tab[1][36: 5];
                8'b0000_0100: predict = tab[2][36: 5];
                8'b0000_1000: predict = tab[3][36: 5];
                8'b0001_0000: predict = tab[4][36: 5];
                8'b0010_0000: predict = tab[5][36: 5];
                8'b0100_0000: predict = tab[6][36: 5];
                8'b1000_0000: predict = tab[7][36: 5];
                default: predict = 32'h0;
            endcase
        end else begin
            predict = 32'h0;
        end
    end

    assign predicted = (branchPredict != 8'b0);

    integer i;
    always @(posedge clock) begin
        // update bht
        if (validBits == 8'hff && lastIsJump && !(compareResult)) begin
            // full but cannot find one, eliminate one
            for(i=0; i < 8; i = i+1) begin
                branchPredict[i] <= 0;
                if(tab[i][2: 0] == 3'h7) begin
                    tab[i][69] <= 1;
                    tab[i][68: 37] <= lastEx_pc;
                    tab[i][36: 5] <= if_pc;
                    tab[i][4:3] <= 10;
                    tab[i][2:0] <= 0;
                end else begin
                    tab[i][2: 0] <= tab[i][2: 0] + 1;
                end
            end
        end else if (lastIsJump && (compareResult & validBits)) begin
            // update prediction
            for(i=0; i < 8; i = i+1) begin
                if(tab[i][68: 37] == lastEx_pc) begin
                    if(tab[i][36: 5] == if_pc) begin
                        // correct predict
                        branchPredict[i] <= 1;
                        if(tab[i][4: 3] != 2'b11) begin
                            tab[i][4: 3] <= tab[i][4: 3] + 1;
                        end
                    end else begin
                        branchPredict[i] <= 0;
                        if(tab[i][4: 3] != 2'b00) begin
                            tab[i][4: 3] <= tab[i][4: 3] - 1;
                        end
                    end

                    if(tab[i][4:3] > 1) begin
                        tab[i][36: 5] <= if_pc;
                    end else if(tab[i][4:3] < 2) begin
                        tab[i][36: 5] <= 32'h0000_0000;
                    end
                end
            end
        end else if(lastIsJump & validBits != 8'hff) begin
            branchPredict = 8'h00;
            case (validBits)
                8'b0000_0000: begin
                    tab[0][69] <= 1;
                    tab[0][68: 37] <= lastEx_pc;
                    tab[0][36: 5] <= if_pc;
                    tab[0][4:3] <= 10;
                    tab[0][2:0] <= 0;
                end
                8'b0000_0001: begin
                    tab[1][69] <= 1;
                    tab[1][68: 37] <= lastEx_pc;
                    tab[1][36: 5] <= if_pc;
                    tab[1][4:3] <= 10;
                    tab[1][2:0] <= 1;
                end
                8'b0000_0011: begin
                    tab[2][69] <= 1;
                    tab[2][68: 37] <= lastEx_pc;
                    tab[2][36: 5] <= if_pc;
                    tab[2][4:3] <= 10;
                    tab[2][2:0] <= 2;
                end
                8'b0000_0111: begin
                    tab[3][69] <= 1;
                    tab[3][68: 37] <= lastEx_pc;
                    tab[3][36: 5] <= if_pc;
                    tab[3][4:3] <= 10;
                    tab[3][2:0] <= 3;
                end
                8'b0000_1111: begin
                    tab[4][69] <= 1;
                    tab[4][68: 37] <= lastEx_pc;
                    tab[4][36: 5] <= if_pc;
                    tab[4][4:3] <= 10;
                    tab[4][2:0] <= 4;
                end
                8'b0001_1111: begin
                    tab[5][69] <= 1;
                    tab[5][68: 37] <= lastEx_pc;
                    tab[5][36: 5] <= if_pc;
                    tab[5][4:3] <= 10;
                    tab[5][2:0] <= 5;
                end
                8'b0011_1111: begin
                    tab[6][69] <= 1;
                    tab[6][68: 37] <= lastEx_pc;
                    tab[6][36: 5] <= if_pc;
                    tab[6][4:3] <= 10;
                    tab[6][2:0] <= 6;
                end
                8'b0111_1111: begin
                    tab[7][69] <= 1;
                    tab[7][68: 37] <= lastEx_pc;
                    tab[7][36: 5] <= if_pc;
                    tab[7][4:3] <= 10;
                    tab[7][2:0] <= 7;
                end
            endcase
        end else begin
            branchPredict = 8'h00;
        end

//        if (lastIsJump && (compareResult & validBits)) begin
//            // update prediction
//            for(i=0; i < 8; i = i+1) begin
//                if(tab[i][68: 37] == lastEx_pc && tab[i][36: 5] == if_pc) begin
//                    // correct predict
//                    branchPredict[i] <= 1;
//                end else begin
//                    branchPredict[i] <= 0;
//                end
//            end
//        end
    end
endmodule

