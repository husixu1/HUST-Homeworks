`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: CodeDragon
//
// Create Date: 2018/03/08 11:36:41
// Design Name:
// Module Name: controller_test
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


module controller_test();
    reg [5: 0] operator;
    reg [5: 0] special;

    //wire[3: 0] aluOperator;
    //wire[1: 0] aluX;
    //wire[2: 0] aluY;
    //wire regWriteEnable;
    //wire[1: 0] regWriteDestinationControl;
    //wire regWriteSourceControl;
    //wire ramWrite;
    //wire[1: 0] pcWrite;
    //wire jump;
    //wire syscall;
    wire [31: 0] control;

    Controller controller(.operator(operator), .special(special), .result(control));
    initial begin
        // add
        operator = 6'b000000;
        special = 6'b100000;

        // addi
        #10 operator = 6'b001000;

        // addiu
        #10 operator = 6'b001001;

        //addu
        #10 operator = 6'b000000;
        special = 6'b100001;

        // and
        #10 operator = 6'b000000;
        special = 6'b100100;

        // andi
        #10 operator = 6'b001100;

        // sll
        #10 operator = 6'b000000;
        special = 6'b000000;

        // sra
        #10 operator = 6'b000000;
        special = 6'b000011;

        // srl
        #10 operator = 6'b000000;
        special = 6'b000010;

        // sub
        #10 operator = 6'b000000;
        special = 6'b100010;

        // or
        #10 operator = 6'b000000;
        special = 6'b100101;

        // ori
        #10 operator = 6'b001101;

        // nor
        #10 operator = 6'b000000;
        special = 6'b100111;

        // lw
        #10 operator = 6'b100011;

        // sw
        #10 operator = 6'b101011;

        // beq
        #10 operator = 6'b000100;

        // bne
        #10 operator = 6'b000101;

        // slt
        #10 operator = 6'b000000;
        special = 6'b101010;

        // slti
        #10 operator = 6'b001010;

        // sltu
        #10 operator = 6'b000000;
        special = 6'b101011;

        // j
        #10 operator = 6'b000010;

        // jal
        #10 operator = 6'b000011;

        // jr
        #10 operator = 6'b000000;
        special = 6'b001000;

        // sys
        #10 operator = 6'b000000;
        special = 6'b001100;
    end
endmodule
