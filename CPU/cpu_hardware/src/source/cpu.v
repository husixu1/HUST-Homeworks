`timescale 10ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: HUST
// Engineer: HuSixu
//
// Create Date: 03/09/2018 05:37:42 PM
// Design Name:
// Module Name: cpu
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


///@brief the cpu
module Cpu(
        input wire rawClock,
        input wire resetButton,
        input wire frequency,
        input wire radix,
        input wire continue,
        input wire[2: 0] interrupt,
        input wire[2: 0] functionNumber,
        input wire[4: 0] checkRamAddress,   // by 4-bytes word, address = {4'b0000, checkRamAddress, 2'b00}
        output wire[7: 0] anode,
        output wire[7: 0] cathode,
        output wire[2: 0] interruptOut,
        output wire[2: 0] buttonOut
    );
    wire reset;
    assign reset = ~resetButton;
    assign buttonOut = interrupt;

    // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // %%%%%%%%%%%%%% CABLES %%%%%%%%%%%%%%%
    // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    // Note: wires are prefixed with the segment that their source-end located

    // global segment %%
    // %%%%%%%%%%%%%%%%%
    wire gl_ramAddressControl;
    wire clock;

    // me segment %%%%%%
    // %%%%%%%%%%%%%%%%%
    wire[31: 0] me_control;
    wire[31: 0] me_instruction;
    reg[4: 0] me_regWrite;
    wire[31: 0] me_aluResult;
    wire[31: 0] me_regSValue;
    wire[31: 0] me_regTValue;
    wire[31: 0] me_ramAddress;
    wire[31: 0] me_rawRamResult;
    reg[31: 0] me_ramResult;
    wire[31: 0] me_epc;

    always @(me_control[21: 20], me_instruction[15: 11], me_instruction[20: 16]) begin
        case (me_control[21: 20])
            2'b00: me_regWrite = me_instruction[15: 11];
            2'b01: me_regWrite = 5'h0;
            2'b10: me_regWrite = me_instruction[20: 16];
            2'b11: me_regWrite = 5'h1f;
        endcase
    end

    assign me_ramAddress = gl_ramAddressControl ? {4'b0000, checkRamAddress, 2'b00} :
        $signed({{16{me_instruction[15]}}, me_instruction[15: 0]}) + $signed(me_regSValue);

    always @(me_rawRamResult, me_ramAddress[1: 0], me_control[12]) begin
        case (me_control[13])
            1'b0: me_ramResult = me_rawRamResult;
            1'b1: begin
                case(me_ramAddress[1: 0])
                    2'b00: me_ramResult = {24'h0, me_rawRamResult[7: 0]};
                    2'b01: me_ramResult = {24'h0, me_rawRamResult[15: 8]};
                    2'b10: me_ramResult = {24'h0, me_rawRamResult[23: 16]};
                    2'b11: me_ramResult = {24'h0, me_rawRamResult[31: 24]};
                endcase
            end
        endcase
    end


    // wb segment %%%%%%
    // %%%%%%%%%%%%%%%%%
    wire[31: 0] wb_instruction;
    wire[31: 0] wb_control;
    wire[31: 0] wb_aluResult;
    wire[31: 0] wb_ramResult;
    reg[31: 0] wb_regWriteData;
    wire[31: 0] wb_regTValue;
    reg[4: 0] wb_regWrite;
    wire[31: 0] wb_epc;

    // assign wb_regWriteData = wb_control[19] ? wb_aluResult : wb_ramResult;
    always @(wb_control[19], wb_control[8], wb_aluResult, wb_ramResult, wb_epc) begin
        if(wb_control[8]) begin
            wb_regWriteData = wb_epc;
        end else begin
            wb_regWriteData = wb_control[19] ? wb_aluResult : wb_ramResult;
        end
    end

    always @(wb_control[21: 20], wb_instruction[15: 11], wb_instruction[20: 16]) begin
        case (wb_control[21: 20])
            2'b00: wb_regWrite = wb_instruction[15: 11];
            2'b01: wb_regWrite = 5'h0;
            2'b10: wb_regWrite = wb_instruction[20: 16];
            2'b11: wb_regWrite = 5'h1f;
        endcase
    end

    // if segment %%%%%%
    // %%%%%%%%%%%%%%%%%
    wire if_jumped;
    wire[31: 0] if_pc;
    wire[31: 0] if_totalCycle;
    wire[31: 0] if_unconditionalJump;
    wire[31: 0] if_conditionalJump;
    wire[31: 0] if_conditionalSuccessfulJump;
    wire[31: 0] if_instruction;
    wire[1: 0] if_cycleAfterJump;
    wire[31: 0] if_predict;
    wire if_predicted;

    // id segment %%%%%%
    // %%%%%%%%%%%%%%%%%
    wire id_bubble;
    wire[31: 0] id_instruction;
    wire[31: 0] id_pc;
    wire[31: 0] id_control;
    wire[4: 0] id_regA;
    wire[4: 0] id_regB;
    wire[31: 0]id_regSValue;
    wire[31: 0]id_regTValue;

    assign id_regA = id_control[14] ? 5'h02 : id_instruction[25: 21];
    assign id_regB = id_control[14] ? 5'h04 : id_instruction[20: 16];

    // ex segment %%%%%%
    // %%%%%%%%%%%%%%%%%
    wire[31: 0] ex_control;
    wire[31: 0] ex_instruction;
    wire[31: 0] ex_pc;
    wire ex_aluEqual;
    wire[31: 0] ex_regSValue;
    wire[31: 0] ex_regTValue;
    reg[31: 0] ex_redirectedRegSValue;
    reg[31: 0] ex_redirectedRegTValue;
    reg[4: 0] ex_regWrite;
    wire ex_sysEnable;
    wire[31: 0] ex_aluX;
    wire[31: 0] ex_aluY;
    wire[31: 0] ex_aluResult;
    wire[31: 0] ex_syscallOutput;
    wire[4: 0] ex_regA;
    wire[4: 0] ex_regB;
    wire[3:0] ex_redirect;
    wire[31: 0] ex_epc;
    wire[1:0] ex_redirectEpc;
    reg[31: 0] ex_redirectedEpc;

    always @(ex_redirect, ex_regSValue, wb_regWriteData, me_aluResult) begin
        case({ex_redirect[3], ex_redirect[1]})
            2'b00: ex_redirectedRegSValue = ex_regSValue;
            2'b01: ex_redirectedRegSValue = wb_regWriteData;
            2'b10: ex_redirectedRegSValue = me_aluResult;
            2'b11: ex_redirectedRegSValue = me_aluResult;
        endcase
    end
    always @(ex_redirect, ex_regTValue, wb_regWriteData, me_aluResult) begin
        case({ex_redirect[2], ex_redirect[0]})
            2'b00: ex_redirectedRegTValue = ex_regTValue;
            2'b01: ex_redirectedRegTValue = wb_regWriteData;
            2'b10: ex_redirectedRegTValue = me_aluResult;
            2'b11: ex_redirectedRegTValue = me_aluResult;
        endcase
    end
    always @(ex_redirectEpc, ex_epc, wb_epc, me_epc) begin
        case(ex_redirectEpc)
            2'b00: ex_redirectedEpc = ex_epc;
            2'b01: ex_redirectedEpc = wb_epc;
            2'b10: ex_redirectedEpc = me_epc;
            2'b11: ex_redirectedEpc = me_epc;
        endcase
    end

    always @(ex_control[21: 20], ex_instruction[15: 11], ex_instruction[20: 16]) begin
        case (ex_control[21: 20])
            2'b00: ex_regWrite = ex_instruction[15: 11];
            2'b01: ex_regWrite = 5'h0;
            2'b10: ex_regWrite = ex_instruction[20: 16];
            2'b11: ex_regWrite = 5'h1f;
        endcase
    end

    assign ex_regA = ex_control[14] ? 5'h02 : ex_instruction[25: 21];
    assign ex_regB = ex_control[14] ? 5'h04 : ex_instruction[20: 16];

    // global segment %%
    // %%%%%%%%%%%%%%%%%
    wire gl_enable;
    wire gl_enable_IFID;
    wire gl_clear_IFID;
    wire gl_clear_IDEX;
    wire gl_clear_EXME;
    wire gl_miscEnable;
    wire gl_interrupted;
    wire[31: 0] gl_displayData;

    assign gl_enable = (continue || ex_sysEnable) && gl_miscEnable;
    assign gl_enable_IFID = gl_enable && (!id_bubble);
    assign gl_clear_IFID = if_jumped || gl_interrupted || ex_control[7];
    assign gl_clear_IDEX = if_jumped || id_bubble || gl_interrupted || ex_control[7];
    assign gl_clear_EXME = gl_interrupted;

    // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // %%%%%%%%%%%%%% MODULES %%%%%%%%%%%%%%
    // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    // if segment %%%%%%
    // %%%%%%%%%%%%%%%%%
    Pc pc(
        .originalPc(ex_pc),
        .regSValue(ex_redirectedRegSValue),
        .instruction(ex_instruction[25: 0]),
        .pcWrite(ex_control[17: 16]),
        .aluEqual(ex_aluEqual),
        .reset(reset),
        .enable(gl_enable),
        .clock(clock),
        .jump(ex_control[15]),

        .interrupted(gl_interrupted),
        .interruptReturned(ex_control[7]),
        .interrupt(interruptOut),
        .epc(ex_redirectedEpc),

        .bltz(ex_control[12]),
        .bubble(id_bubble),
        .predicted(if_predicted),
        //--------//
        .jumped(if_jumped),
        .pc(if_pc),
        .totalCycle(if_totalCycle),
        .unconditionalJump(if_unconditionalJump),
        .conditionalJump(if_conditionalJump),
        .conditionalSuccessfulJump(if_conditionalSuccessfulJump),
        .cycleAfterJump(if_cycleAfterJump)
    );

    Bht bht(
        .clock(clock),
        .if_pc(if_pc),
        .ex_pc(ex_pc),
        .isJump(if_jumped),
        //--------//
        .predict(if_predict),
        .predicted(if_predicted)
    );

    Rom rom(
        .address(if_pc),
        //--------//
        .result(if_instruction)
    );

    PipeIFID pipeIFID(
        .clock(clock),
        .enable(gl_enable_IFID),
        .clear(gl_clear_IFID),
        .instruction(if_instruction),
        .pc(if_pc),
        //--------//
        .instructionOut(id_instruction),
        .pcOut(id_pc)
    );

    // id segment %%%%%%
    // %%%%%%%%%%%%%%%%%
    Controller controller(
    //    .operator(id_instruction[31: 26]),
    //    .special(id_instruction[5: 0]),
        .instruction(id_instruction),
        //--------//
        .result(id_control)
    );

    Reg regFile(
        .clock(clock),
        .writeData(wb_regWriteData),
        .writeEnable(wb_control[22]),
        .regWrite(wb_regWrite),
        .regA(id_regA),
        .regB(id_regB),
        //--------//
        .resultA(id_regSValue),
        .resultB(id_regTValue)
    );

    // epc tiny module ---------------
    // epc will be wrong if a recursive interrupt is triggered, because
    // there's a half cycle latch between resetting ex_pc and writting epc,
    // therefore we need a *_preEpc to perserve that value
    reg[31: 0]if_preEpc = 0;
    reg[31: 0]id_preEpc = 0;
    reg[31: 0]ex_preEpc = 0;
    reg[31: 0]id_epc = 0;
    always @(posedge clock) begin
        if_preEpc <= if_pc;
        id_preEpc <= id_pc;
        ex_preEpc <= ex_pc;
    end
    // write pc value to epc while interrupt
    always @(negedge clock) begin
        if (gl_interrupted) begin
            case(if_cycleAfterJump)
                2'b00: id_epc <= if_preEpc;
                2'b01: id_epc <= id_preEpc;
                2'b10: id_epc <= ex_preEpc;
                2'b11: id_epc <= 0;
            endcase
            id_epc <= id_preEpc;
        end else if(wb_control[9]) begin
            // write to epc if instruction is mtc
            id_epc <= wb_regTValue;
        end
    end
    // --------------- epc tiny module

    Bubble bubble(
        .regARead(id_control[11]),
        .regBRead(id_control[10]),
        .regA(id_regA),
        .regB(id_regB),
        .regWriteEnable_EX(ex_control[22]),
        .regWrite_EX(ex_regWrite),
        .instruction_EX(ex_instruction),
        //--------//
        .bubble(id_bubble)
    );

    PipeIDEX pipeIDEX(
        .clock(clock),
        .enable(gl_enable),
        .clear(gl_clear_IDEX),
        .control(id_control),
        .instruction(id_instruction),
        .pc(id_pc),
        .regSValue(id_regSValue),
        .regTValue(id_regTValue),
        .epc(id_epc),
        //--------//
        .controlOut(ex_control),
        .instructionOut(ex_instruction),
        .pcOut(ex_pc),
        .regSValueOut(ex_regSValue),
        .regTValueOut(ex_regTValue),
        .epcOut(ex_epc)
    );

    // ex segment %%%%%%
    // %%%%%%%%%%%%%%%%%
    AluInput aluInput(
        .pc(ex_pc),
        .regTValue(ex_redirectedRegTValue),
        .regSValue(ex_redirectedRegSValue),
        .instruction(ex_instruction[15: 0]),
        .aluX(ex_control[27: 26]),
        .aluY(ex_control[25: 23]),
        //--------//
        .resultX(ex_aluX),
        .resultY(ex_aluY)
    );

    Alu alu(
        .x(ex_aluX),
        .y(ex_aluY),
        .operator(ex_control[31: 28]),
        //--------//
        .result(ex_aluResult),
        .result2(),
        .equal(ex_aluEqual)
    );

    Syscall syscall(
        .regSValue(ex_redirectedRegSValue),
        .regTValue(ex_redirectedRegTValue),
        .syscall(ex_control[14]),
        .clock(clock),
        .reset(reset),
        .continue(continue),
        //--------//
        .enable(ex_sysEnable),
        .syscallOutput(ex_syscallOutput)
    );

    Redirect redirect(
        .regARead(ex_control[11]),
        .regBRead(ex_control[10]),
        .regA(ex_regA),
        .regB(ex_regB),
        .regWriteEable_ME(me_control[22]),
        .regWrite_ME(me_regWrite),
        .regWriteEable_WB(wb_control[22]),
        .regWrite_WB(wb_regWrite),
        .epcRead(ex_instruction[8]),//TODO
        .epcWrite_ME(ex_instruction[9]),
        .epcWrite_WB(ex_instruction[9]),
        //--------//
        .redirect(ex_redirect),
        .redirectEpc(ex_redirectEpc)
    );

    PipeEXME pipeEXME(
        .clock(clock),
        .enable(gl_enable),
        .clear(gl_clear_EXME),
        .control(ex_control),
        .instruction(ex_instruction),
        .aluR(ex_aluResult),
        .regSValue(ex_redirectedRegSValue),
        .regTValue(ex_redirectedRegTValue),
        .epc(ex_redirectedEpc),
        //--------//
        .controlOut(me_control),
        .instructionOut(me_instruction),
        .aluROut(me_aluResult),
        .regSValueOut(me_regSValue),
        .regTValueOut(me_regTValue),
        .epcOut(me_epc)
    );
    // me segment %%%%%%
    // %%%%%%%%%%%%%%%%%

    Ram ram(
        .clock(clock),
        .reset(reset),
        .store(me_control[18]),
        .address(me_ramAddress[9:0]),
        .data(me_regTValue),
        //--------//
        .result(me_rawRamResult)
    );

    PipeMEWB pipeMEWB(
        .clock(clock),
        .enable(gl_enable),
        .clear(0),
        .control(me_control),
        .instruction(me_instruction),
        .aluR(me_aluResult),
        .ramR(me_ramResult),
        .regTValue(me_regTValue),
        .epc(me_epc),
        //--------//
        .controlOut(wb_control),
        .instructionOut(wb_instruction),
        .aluROut(wb_aluResult),
        .ramROut(wb_ramResult),
        .regTValueOut(wb_regTValue),
        .epcOut(wb_epc)
    );

    // global segment %%
    // %%%%%%%%%%%%%%%%%
    Interrupt interruptManager(
        .clock(clock),
        .interrupt(interrupt),         // 3 bits, 3 interrupts
        .interruptEnd(ex_control[7]),  // TODO: validate this
        .reset(reset),
        //--------//
        .interruptOut(interruptOut),
        .interrupted(gl_interrupted)
    );

    MiscController miscController(
        .clock(rawClock),
        .frequency(frequency),
        .functionNumber(functionNumber),
        .syscallOutput(ex_syscallOutput),
        .memory(me_ramResult),
        .pc(if_pc),
        .totalCycle(if_totalCycle),
        .unconditionalJump(if_unconditionalJump),
        .conditionalJump(if_conditionalJump),
        .conditionalSuccessfulJump(if_conditionalSuccessfulJump),
        .bubble(id_bubble),
        //--------//
        .enable(gl_miscEnable),                       // if to print memory address, pc should be halted
        .memoryAddressControl(gl_ramAddressControl),  // define the source of memory address
        .data(gl_displayData),
        .clockOut(clock)
    );

    // Display, not part of cpu but still here, use default clock rather than
    // controlledClock
    Display display(
        .data(gl_displayData),
        .radix(radix),
        .clock(rawClock),
        .reset(reset),
        //--------//
        .cathode(cathode),    // from 7:CA to 0:DP
        .anode(anode)         // form 7:AN7 to 0:AN0
    );

endmodule
