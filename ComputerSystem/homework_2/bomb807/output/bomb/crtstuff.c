// address: 0x8048a1d
int main(int argc, char **argv, char **envp) {
    int local0; 		// infile{205}
    int local11; 		// infile{27}
    int local12; 		// %pc{25}
    char local14[0]; 		// infile{15}
    int local15; 		// infile{196}
    int local16; 		// local0{205}
    int local17; 		// %pc{207}
    int local18; 		// r8
    int local19; 		// r10
    int local20; 		// r24
    int local23; 		// r27
    int local24; 		// r28
    int local25; 		// r29
    int local26; 		// r30
    int local27; 		// r31
    int local6; 		// r28{64}

    local15 = infile;
    *(int*)(local24 - 4) = local25;
    *(int*)(local24 - 8) = local23;
    if (argc != 1) {
        if (argc != 2) {
            *(int*)(local24 - 16) = *argv;
            *(int*)(local24 - 20) = 0x804a207;
            *(int*)(local24 - 24) = 1;
            __printf_chk();
            *(int*)(local24 - 24) = 8;
            exit(8);
        } else {
            *(int*)(local24 - 20) = 0x804a1e8;
            local20 = *(int*)(argv + 4);
            *(int*)(local24 - 24) = local20;
            local20 = fopen(local20, "r");
            local17 = local12;
            local11 = local20;
            local15 = local11;
            local16 = local11;
            if (local20 != 0) {
L29:
                local0 = local16;
                local6 = initialize_bomb();
                local15 = local0;
                *(int*)local6 = 0x804a26c;
                puts("Welcome to my fiendish little bomb. You have 6 phases with");
                *(int*)local6 = 0x804a2a8;
                puts("which to blow yourself up. Have a nice day!");
                local20 = read_line(local18, local19, argv, local26, local27, infile[0], num_input_strings[0]); /* Warning: also results in local26, local19, local23, local27 */
                *(int*)local24 = local20;
                local23 = phase_1(local23, local27, local20); /* Warning: also results in local27 */
                local18 = phase_defused(local23, local27); /* Warning: also results in local23, local27 */
                *(int*)local24 = 0x804a2d4;
                puts("Phase 1 defused. How about the next one?");
                local20 = read_line(local18, local19, local23, local26, local27, infile[0], num_input_strings[0]); /* Warning: also results in local26, local18, local19, local23, local27 */
                *(int*)local24 = local20;
                local26 = phase_2(local18, local23, local26, local27, local20); /* Warning: also results in local23, local27 */
                local18 = phase_defused(local23, local27); /* Warning: also results in local23, local27 */
                *(int*)local24 = 0x804a221;
                puts("That's number 2.  Keep going!");
                local20 = read_line(local18, local19, local23, local26, local27, infile[0], num_input_strings[0]); /* Warning: also results in local26, local19, local23, local27 */
                *(int*)local24 = local20;
                local23 = phase_3(local23, local27, local20); /* Warning: also results in local27 */
                local18 = phase_defused(local23, local27); /* Warning: also results in local23, local27 */
                *(int*)local24 = 0x804a23f;
                puts("Halfway there!");
                local20 = read_line(local18, local19, local23, local26, local27, infile[0], num_input_strings[0]); /* Warning: also results in local26, local18, local19, local23, local27 */
                *(int*)local24 = local20;
                local23 = phase_4(local23, local27, local20); /* Warning: also results in local27 */
                local18 = phase_defused(local23, local27); /* Warning: also results in local23, local27 */
                *(int*)local24 = 0x804a300;
                puts("So you got that one.  Try this one.");
                local20 = read_line(local18, local19, local23, local26, local27, infile[0], num_input_strings[0]); /* Warning: also results in local26, local19, local23, local27 */
                *(int*)local24 = local20;
                local23 = phase_5(local23, local27, local20); /* Warning: also results in local27 */
                local18 = phase_defused(local23, local27); /* Warning: also results in local23, local27 */
                *(int*)local24 = 0x804a24e;
                puts("Good work!  On to the next...");
                local20 = read_line(local18, local19, local23, local26, local27, infile[0], num_input_strings[0]); /* Warning: also results in local26, local18, local23, local27 */
                *(int*)local24 = local20;
                local23 = phase_6(local18, local23, local26, local27, local20); /* Warning: also results in local27 */
                phase_defused(local23, local27);
                local20 = 0;
            } else {
                *(int*)(local24 - 12) = *(int*)(argv + 4);
                *(int*)(local24 - 16) = *argv;
                *(int*)(local24 - 20) = 0x804a1ea;
                *(int*)(local24 - 24) = 1;
                __printf_chk();
                *(int*)(local24 - 24) = 8;
                exit(8);
            }
        }
    } else {
        local14 = stdin;
        local16 = local14;
        goto L29;
    }
    infile = local15;
    return local20;
}

// address: 0x80490b6
void initialize_bomb() {
    int local3; 		// m[r28 - 16]{5}
    int local4; 		// r24
    int local7; 		// r28

    local3 = *(int*)20;
    *(int*)(local7 - 0x2028) = 0x8048fa0;
    *(int*)(local7 - 0x202c) = 2;
    signal();
    *(int*)(local7 - 0x202c) = local7 - 0x2010;
    local4 = init_driver(local7 - 0x2010);
    if (local4 >= 0) {
        if ((*(int*)(local7 - 16) ^ *(int*)20) != 0) {
            __stack_chk_fail();
        }
    } else {
        *(int*)(local7 - 0x2024) = local7 - 0x2010;
        *(int*)(local7 - 0x2028) = 0x804a54e;
        *(int*)(local7 - 0x202c) = 1;
        __printf_chk();
        *(int*)(local7 - 0x202c) = 8;
        exit(8);
    }
    return;
}

// address: 0x8049354
read_line(int param4, int param5, int param3, int param2, int param1, int ??, int ??) {
    int local0; 		// num_input_strings{101}
    int local11; 		// %pc{171}
    int local12; 		// r28{195}
    int local13; 		// num_input_strings{198}
    int local14; 		// r24
    int local15; 		// r25
    int local16; 		// r26
    int local17; 		// r28
    int local2; 		// r28{168}
    int local3; 		// r25{179}
    int local5; 		// %pc{52}
    int local9; 		// %pc{12}

    local13 = num_input_strings;
    *(int*)(local17 - 4) = param1;
    *(int*)(local17 - 8) = param2;
    *(int*)(local17 - 12) = param3;
    local14 = skip(param4, param5); /* Warning: also results in param5 */
    local11 = local9;
    if (local14 != 0) {
L10:
        local2 = local17 - 28;
        local16 = num_input_strings;
        param3 = num_input_strings * 80 + 0x804c800;
        param1 = param3;
        local15 = -1;
        local12 = local2;
        do {
            if (local15 == 0) {
                goto L7;
            }
            param1++;
            local15 = local15 - 1;
        } while (CF);
L7:
        local3 = local15;
        local15 = local3 - 1;
        if (local3 - 1 > 78) {
            *(int*)(local17 - 28) = 0x804a5e0;
            puts("Error: Input line too long");
            local0 = num_input_strings + 1;
            local14 = num_input_strings * 80 + 0x804c800;
            param2 = 0x804a5fb;
            local15 = 4;
            param1 = local14;
            local13 = local0;
            while (local15 != 0) {
                *(int*)param1 = *(int*)param2;
                param2 += 4;
                param1 += 4;
                local15 = local15 - 1;
            }
            local15 = explode_bomb(param3, param1); /* Warning: also results in local16, param3 */
            local12 = local17;
        }
        local17 = local12;
        num_input_strings = local13;
        *(char*)(local15 + local16 * 80 + 0x804c7ff) = 0;
        num_input_strings = local16 + 1;
        local14 = param3;
        param4 = (unsigned char) param3;
        param3 = *(int*)(local17 + 16);
        param2 = *(int*)(local17 + 20);
        param1 = *(int*)(local17 + 24);
    } else {
        param4 = (unsigned char) stdin;
        if (infile != stdin) {
            *(int*)(local17 - 28) = 0x804a5d5;
            local14 = getenv("GRADE_BOMB");
            if (local14 == 0) {
                infile = stdin;
                local14 = skip((unsigned char) stdin, param5); /* Warning: also results in param4, param5 */
                local11 = local5;
                if (local14 != 0) {
                    goto L10;
                } else {
                    *(int*)(local17 - 28) = 0x804a5b7;
                    puts("Error: Premature EOF on stdin");
                    *(int*)(local17 - 28) = 0;
                    exit(0);
                }
            } else {
                *(int*)(local17 - 28) = 0;
                exit(0);
            }
        } else {
            *(int*)(local17 - 28) = 0x804a5b7;
            puts("Error: Premature EOF on stdin");
            *(int*)(local17 - 28) = 8;
            exit(8);
        }
    }
    return local14; /* WARNING: Also returning: param2, param4, param5, param3, param1 */
}

// address: 0x8048b90
phase_1(int param1, int param2, int param3) {
    int local2; 		// r24
    int local5; 		// r28

    *(int*)(local5 - 24) = 0x804a324;
    *(int*)(local5 - 28) = param1;
    local2 = strings_not_equal(param1, 0x804a324);
    if (local2 != 0) {
        param2 = explode_bomb(param2, param3); /* Warning: also results in param3 */
    }
    return param2; /* WARNING: Also returning: param3 */
}

// address: 0x804944e
phase_defused(int param1, int param2) {
    int local10; 		// r28{84}
    int local11; 		// %pc{88}
    int local12; 		// %pc{100}
    int local13; 		// r8
    int local14; 		// r10
    int local15; 		// r24
    int local18; 		// r28
    int local19; 		// r30
    int local5; 		// r28{37}
    int local6; 		// %pc{36}
    int local7; 		// r28{27}
    int local8; 		// %pc{26}
    int local9; 		// %pc{11}

    *(int*)(local18 - 16) = *(int*)20;
    *(int*)(local18 - 140) = 1;
    local13 = send_msg(param1, param2); /* Warning: also results in param1, param2 */
    local12 = local9;
    if (num_input_strings == 6) {
        *(int*)(local18 + 16) = local18 + 44;
        *(int*)(local18 + 12) = local18 + 40;
        *(int*)(local18 + 8) = local18 + 36;
        *(int*)(local18 + 4) = 0x804a60b;
        *(int*)local18 = 0x804c8f0;
        __isoc99_sscanf();
        local10 = local7;
        local11 = local8;
        if (local15 == 3) {
            *(int*)(local18 + 4) = 0x804a614;
            *(int*)local18 = local18 + 44;
            local15 = strings_not_equal(0x804a614, 0x804c8f0); /* Warning: also results in local13, local5 */
            local10 = local5;
            local11 = local6;
            if (local15 == 0) {
                *(int*)local18 = 0x804a460;
                puts("Curses, you've found the secret phase!");
                *(int*)local18 = 0x804a488;
                puts("But finding it and solving it are quite different...");
                local13 = secret_phase(local13, local14, param1, local19, param2);
                local10 = local18;
            }
        }
        local18 = local10;
        *(int*)local18 = 0x804a4c0;
        puts("Congratulations! You've defused the bomb!");
        *(int*)local18 = 0x804a4ec;
        puts("Your instructor has been notified and will verify your solution.");
    }
    return local13; /* WARNING: Also returning: param1, param2 */
}

// address: 0x8048bb4
phase_2(int param3, int param4, int param2, int param1, int param5) {
    int local0; 		// r27{70}
    int local1; 		// r28{79}
    int local2; 		// %pc{14}
    int local3; 		// r28{60}
    int local4; 		// %pc{81}
    int local8; 		// r28

    *(int*)(local8 - 4) = param1;
    *(int*)(local8 - 8) = param2;
    *(int*)(local8 - 56) = local8 - 36;
    *(int*)(local8 - 60) = param3;
    param4 = read_six_numbers(param3, local8 - 36, param4, param2, param5); /* Warning: also results in param2, param5 */
    local4 = local2;
    if (*(int*)(local8 + 24) != 1) {
        param4 = explode_bomb(param2, param5); /* Warning: also results in param5 */
    }
    local1 = local8;
    param2 = local1 + 28;
    local3 = local1;
    do {
        local8 = local3;
        if (*(int*)param2 != *(int*)(param2 - 4) + *(int*)(param2 - 4)) {
            param4 = explode_bomb(param2, param5); /* Warning: also results in param2, param5 */
        }
        local0 = param2;
        param2 = local0 + 4;
        local3 = local8;
    } while (local0 + 4 != local1 + 48);
    return *(int*)(local8 + 56); /* WARNING: Also returning: *(int*)(local8 + 52), param5 */
}

// address: 0x8048c02
phase_3(int param1, int param2, int param3) {
    int local0; 		// r28{14}
    int local1; 		// r28{53}
    int local10; 		// r24
    int local13; 		// r28
    int local5; 		// m[r28 - 48]{19}
    int local6; 		// %pc{13}
    int local7; 		// r28{110}
    int local8; 		// %pc{113}

    *(int*)(local13 - 32) = local13 - 16;
    *(int*)(local13 - 36) = local13 - 20;
    *(int*)(local13 - 40) = 0x804a5b1;
    *(int*)(local13 - 44) = param1;
    __isoc99_sscanf();
    local7 = local0;
    local8 = local6;
    if (local10 <= 1) {
        explode_bomb(param2, param3);
    }
    if ((unsigned)*(int*)(local13 - 20) > 7) {
        param2 = explode_bomb(param2, param3); /* Warning: also results in param3 */
        local7 = local13;
        local10 = 0;
    } else {
        switch(*(int*)(local1 + 24)) {
        case 0:
L12:
L11:
            local10 = *(int*)(local13 - 20) + 420;
L10:
            local10 = local10 - 321;
L9:
            local10 += 321;
L8:
            local10 = local10 - 321;
L7:
            local10 += 321;
            break;
        case 1:
            goto L12;
            break;
        case 2:
            goto L11;
            break;
        case 3:
            local10 = 0;
            goto L10;
            break;
        case 4:
            local10 = 0;
            goto L9;
            break;
        case 5:
            local10 = 0;
            goto L8;
            break;
        case 6:
            local10 = 0;
            goto L7;
            break;
        case 7:
            local10 = 0;
            break;
        }
        local10 = local10 - 321;
    }
    local13 = local7;
    if (*(int*)(local13 + 24) > 5) {
L1:
        param2 = explode_bomb(param2, param3); /* Warning: also results in param3 */
    } else {
        if (local10 != *(int*)(local13 + 28)) {
            goto L1;
        }
    }
    return param2; /* WARNING: Also returning: param3 */
}

// address: 0x8048d22
phase_4(int param1, int param2, int param3) {
    int local1; 		// m[r28]{28}
    int local12; 		// r28
    int local2; 		// m[r28 + 4]{26}
    int local3; 		// m[r28 + 8]{25}
    int local4; 		// r28{14}
    int local5; 		// %pc{13}
    int local6; 		// r28{57}
    int local7; 		// %pc{60}
    int local9; 		// r24

    *(int*)(local12 - 32) = local12 - 16;
    *(int*)(local12 - 36) = local12 - 20;
    *(int*)(local12 - 40) = 0x804a5b1;
    *(int*)(local12 - 44) = param1;
    __isoc99_sscanf();
    local6 = local4;
    local7 = local5;
    if (local9 != 2) {
L5:
        param2 = explode_bomb(param2, param3); /* Warning: also results in param3 */
        local6 = local12;
    } else {
        if ((unsigned)*(int*)(local12 - 20) > 14) {
            goto L5;
        }
    }
    local12 = local6;
    local3 = 14;
    local2 = 0;
    local1 = *(int*)(local12 + 24);
    local9 = func4(param1, *(int*)(local12 + 8), *(int*)local12);
    if (local9 != 6) {
L1:
        param2 = explode_bomb(param2, param3); /* Warning: also results in param3 */
    } else {
        if (*(int*)(local12 + 28) != 6) {
            goto L1;
        }
    }
    return param2; /* WARNING: Also returning: param3 */
}

// address: 0x8048d8b
phase_5(int param1, int param2, int param3) {
    int local0; 		// r25{36}
    int local1; 		// r26{32}
    int local10; 		// r24
    int local11; 		// r25
    int local12; 		// r26
    int local13; 		// r28
    int local2; 		// m[r28 + 24]{25}
    int local3; 		// r28{14}
    int local4; 		// %pc{13}
    int local5; 		// r28{63}
    int local6; 		// %pc{65}
    int local7; 		// r25{67}
    int local8; 		// r26{68}

    *(int*)(local13 - 32) = local13 - 16;
    *(int*)(local13 - 36) = local13 - 20;
    *(int*)(local13 - 40) = 0x804a5b1;
    *(int*)(local13 - 44) = param1;
    __isoc99_sscanf();
    local5 = local3;
    local6 = local4;
    if (local10 <= 1) {
        param2 = explode_bomb(param2, param3); /* Warning: also results in param3 */
        local5 = local13;
    }
    local13 = local5;
    local10 = *(int*)(local13 + 24) & 0xf;
    local2 = *(int*)(local13 + 24) & 0xf;
    if ((*(int*)(local13 + 24) & 0xf) == 15) {
L1:
        param2 = explode_bomb(param2, param3); /* Warning: also results in param3 */
    } else {
        local11 = 0;
        local12 = 0;
        local7 = local11;
        local8 = local12;
        do {
            local11 = local7;
            local12 = local8;
            local1 = local12 + 1;
            local10 = array.3144[local10];
            local0 = local11 + local10;
            local7 = local0;
            local8 = local1;
        } while (local10 != 15);
        *(int*)(local13 + 24) = local10;
        if (local12 + 1 != 15) {
            goto L1;
        } else {
            if (local11 + local10 != *(int*)(local13 + 28)) {
                goto L1;
            }
        }
    }
    return param2; /* WARNING: Also returning: param3 */
}

// address: 0x8048dff
phase_6(int param3, int param4, int param2, int param1, int param5) {
    int local0; 		// r30{204}
    int local1; 		// r24{192}
    int local10; 		// r24
    int local11; 		// r25
    int local12; 		// r26
    int local13; 		// r28
    int local2; 		// r24{97}
    int local3; 		// r27{187}
    int local4; 		// r24{174}
    int local5; 		// r24{169}
    int local6; 		// r27{165}
    int local7; 		// r30{136}
    int local8; 		// m[r24]{57}
    int local9; 		// local1{192}

    *(int*)(local13 - 4) = param1;
    *(int*)(local13 - 8) = param2;
    *(int*)(local13 - 72) = local13 - 60;
    *(int*)(local13 - 76) = param3;
    param4 = read_six_numbers(param3, local13 - 60, param4, param2, param5); /* Warning: also results in local11, local12, param2, param5 */
    param1 = 0;
    for(;;) {
        local7 = param1;
        local10 = *(int*)(local13 + local7 * 4 + 16);
        if ((unsigned)(local10 - 1) > 5) {
            param4 = explode_bomb(param2, param5); /* Warning: also results in local11, local12, param5 */
        }
        param1 = local7 + 1;
        if (local7 + 1 == 6) {
            break;
        }
        param2 = local7 + 1;
        do {
            local10 = *(int*)(local13 + param2 * 4 + 16);
            if (*(int*)(local13 + (local7 + 1) * 4 + 12) == local10) {
                param4 = explode_bomb(param2, param5); /* Warning: also results in local11, local12, param2, param5 */
            }
            local6 = param2;
            param2 = local6 + 1;
        } while (local6 + 1 <= 5);
    }
    local10 = local13 + 16;
    do {
        local5 = local10;
        local8 = 7 - *(int*)local5;
        local10 = local5 + 4;
    } while (local5 + 4 != local13 + 40);
    param2 = 0;
    do {
        local3 = param2;
        local11 = *(int*)(local13 + local3 * 4 + 16);
        if (local11 <= 1) {
            local12 = 0x804c154;
        } else {
            local10 = 1;
            local12 = 0x804c154;
            do {
                local4 = local10;
                local12 = *(int*)(local12 + 8);
                local10 = local4 + 1;
            } while (local4 + 1 != local11);
        }
        *(int*)(local13 + local3 * 4 + 40) = local12;
        param2 = local3 + 1;
    } while (local3 + 1 == 6);
    param2 = *(int*)(local13 + 40);
    local10 = local13 + 44;
    local11 = *(int*)(local13 + 40);
    local9 = local10;
    local1 = local9;
    local12 = *(int*)local1;
    *(int*)(local11 + 8) = *(int*)local1;
    local2 = local1 + 4;
    local9 = local2;
    while (local1 + 4 != local13 + 64) {
        local11 = *(int*)local1;
        local1 = local9;
        local12 = *(int*)local1;
        *(int*)(local11 + 8) = *(int*)local1;
        local2 = local1 + 4;
        local9 = local2;
    }
    *(int*)(*(int*)local1 + 8) = 0;
    param1 = 5;
    do {
        local0 = param1;
        local10 = *(int*)*(int*)(param2 + 8);
        if (*(int*)param2 < local10) {
            param4 = explode_bomb(param2, param5); /* Warning: also results in local11, local12, param2, param5 */
        }
        param2 = *(int*)(param2 + 8);
        param1 = local0 - 1;
    } while (local0 != 1);
    return *(int*)(local13 + 68); /* WARNING: Also returning: param5 */
}

// address: 0x804904a
strings_not_equal(int param1, int param2) {
    int local10; 		// r27{78}
    int local11; 		// local10{78}
    int local13; 		// r24
    int local14; 		// r26
    int local15; 		// r27
    int local16; 		// r28
    int local17; 		// r30
    int local5; 		// r27{45}
    int local6; 		// r30{79}
    int local7; 		// r24{16}

    local15 = param1;
    local17 = param2;
    local7 = string_length();
    local11 = local15;
    local13 = string_length();
    local14 = 1;
    if (local7 == local13) {
        local13 = *(unsigned char*)param1;
        if ((unsigned char) local13 == 0) {
            local14 = 0;
        } else {
            if ((unsigned char) local13 == *(int*)param2) {
                do {
                    local10 = local11;
                    local6 = local17;
                    local5 = local10 + 1;
                    local17 = local6 + 1;
                    local13 = *(unsigned char*)(local10 + 1);
                    local11 = local5;
                    if ((unsigned char) local13 == 0) {
                        local14 = 0;
                        goto L0;
                    }
                } while ((unsigned char) local13 != *(int*)(local6 + 1));
                local14 = 1;
            } else {
                local14 = 1;
            }
        }
    }
L0:
    return local14; /* WARNING: Also returning: (unsigned char) local14 */
}

// address: 0x80492c5
explode_bomb(int param1, int param2) {
    int local3; 		// r8
    int local4; 		// r24
    int local5; 		// r25
    int local6; 		// r26
    int local7; 		// r28

    *(int*)(local7 - 28) = 0x804a585;
    puts("\nBOOM!!!");
    *(int*)(local7 - 28) = 0x804a58e;
    puts("The bomb has blown up.");
    *(int*)(local7 - 28) = 0;
    local3 = send_msg(param1, param2); /* Warning: also results in param1, param2 */
    *(int*)local7 = 0x804a43c;
    puts("Your instructor has been notified.");
    *(int*)local7 = 8;
    exit(8);
    return local4; /* WARNING: Also returning: local3, local5, local6, param1, param2 */
}

// address: 0x8049304
read_six_numbers(int param2, int param1, int param5, int param3, int param4) {
    int local1; 		// r24
    int local2; 		// r25
    int local3; 		// r26
    int local4; 		// r28

    *(int*)(local4 - 16) = param1 + 20;
    *(int*)(local4 - 20) = param1 + 16;
    *(int*)(local4 - 24) = param1 + 12;
    *(int*)(local4 - 28) = param1 + 8;
    *(int*)(local4 - 32) = param1 + 4;
    *(int*)(local4 - 36) = param1;
    *(int*)(local4 - 40) = 0x804a5a5;
    *(int*)(local4 - 44) = param2;
    __isoc99_sscanf();
    if (local1 <= 5) {
        param5 = explode_bomb(param3, param4); /* Warning: also results in local2, local3, param3, param4 */
    }
    return param5; /* WARNING: Also returning: local2, local3, param3, param4 */
}

// address: 0x8048cc1
func4(int param1, int param3, int param2) {
    int local10; 		// r25
    int local11; 		// r26
    int local12; 		// r28
    int local6; 		// r25{26}
    int local7; 		// r25{20}
    int local8; 		// r25{71}
    int local9; 		// r24

    local11 = param1;
    local7 = param2 + (param2 - param3) / 0x80000000 - param3;
    local6 = (local7 >> 1) + param3;
    local8 = local6;
    if ((local7 >> 1) + param3 <= param1) {
        local9 = 0;
        if ((local7 >> 1) + param3 < param1) {
            local10 = (local7 >> 1) + param3 + 1;
            local9 = func4(param1, local10, param2); /* Warning: also results in local10, local11 */
            local8 = local10;
            local9 = local9 + local9 + 1;
        }
    } else {
        local10 = (local7 >> 1) + param3 - 1;
        local9 = func4(param1, param3, local10); /* Warning: also results in local10, local11 */
        local8 = local10;
        local9 += local9;
    }
    local10 = local8;
    return local9; /* WARNING: Also returning: local10, local11 */
}

// address: 0x804902b
string_length() {
    int local0; 		// r24{16}
    int local1; 		// r24
    int local2; 		// r28

    if (*(int*)param1 != 0) {
        local1 = 0;
        do {
            local0 = local1;
            local1 = local0 + 1;
        } while (*(int*)(param1 + local0 + 1) != 0);
    }
    return 0;
}

// address: 0x8049e9e
init_driver(int param1) {
    int local22; 		// r24
    int local25; 		// r28

    signal();
    signal();
    signal();
    socket();
    if (local22 >= 0) {
        gethostbyname();
        if (local22 != 0) {
            __memmove_chk();
            *(short*)(local25 - 30) = 0x6e3b;
            connect();
            if (local22 >= 0) {
                close();
                *(int*)param1 = 0x4b4f;
                *(char*)(param1 + 2) = 0;
                local22 = 0;
            } else {
                __sprintf_chk();
                close();
                local22 = -1;
            }
        } else {
            *(int*)param1 = 0x6f727245;
            *(int*)(param1 + 4) = 0x44203a72;
            *(int*)(param1 + 8) = 0x6920534e;
            *(int*)(param1 + 12) = 0x6e752073;
            *(int*)(param1 + 16) = 0x656c6261;
            *(int*)(param1 + 20) = 0x206f7420;
            *(int*)(param1 + 24) = 0x6f736572;
            *(int*)(param1 + 28) = 0x2065766c;
            *(int*)(param1 + 32) = 0x76726573;
            *(int*)(param1 + 36) = 0x61207265;
            *(int*)(param1 + 40) = 0x65726464;
            *(short*)(param1 + 44) = 0x7373;
            *(char*)(param1 + 46) = 0;
            close();
            local22 = -1;
        }
    } else {
        *(int*)param1 = 0x6f727245;
        *(int*)(param1 + 4) = 0x43203a72;
        *(int*)(param1 + 8) = 0x6e65696c;
        *(int*)(param1 + 12) = 0x6e752074;
        *(int*)(param1 + 16) = 0x656c6261;
        *(int*)(param1 + 20) = 0x206f7420;
        *(int*)(param1 + 24) = 0x61657263;
        *(int*)(param1 + 28) = 0x73206574;
        *(int*)(param1 + 32) = 0x656b636f;
        *(short*)(param1 + 36) = 116;
        local22 = -1;
    }
    return local22;
}

// address: 0x80491b7
send_msg(int param2, int param1) {
    int local3; 		// r25
    int local5; 		// r28

    *(int*)(local5 - 4) = param1;
    *(int*)(local5 - 8) = param2;
    *(int*)(local5 - 16) = *(int*)20;
    param2 = (num_input_strings - 1) * 5;
    param1 = param2 * 16 + 0x804c800;
    local3 = -1;
    do {
        if (local3 == 0) {
            goto L4;
        }
        param1++;
        local3 = local3 - 1;
    } while (CF);
L4:
    if ((unsigned)(local3 + 99) <= 0x2000) {
    }
    *(int*)(local5 - 0x4038) = 0x804a418;
    *(int*)(local5 - 0x403c) = 1;
    __printf_chk();
    *(int*)(local5 - 0x403c) = 8;
    exit(8);
    return 0; /* WARNING: Also returning: param2 * 16 + 0x804c800, param1 */
}

// address: 0x8049171
skip(int param1, int param2) {
    int local11; 		// r28
    int local3; 		// m[r28 - 28]
    int local5; 		// r24{30}
    int local8; 		// r24

    do {
        local8 = num_input_strings * 80 + 0x804c800;
        local8 = fgets(local8, 80, infile);
        if (local8 == 0) {
            break;
        }
        local5 = blank_line(local3, param2); /* Warning: also results in param1, param2 */
    } while (local5 != 0);
    return local8; /* WARNING: Also returning: param1, param2 */
}

// address: 0x8048f3e
secret_phase(int param2, int param3, int param1, int param4, int param5) {
    int local10; 		// r31{25}
    int local11; 		// r31{9}
    int local12; 		// r27{17}
    int local13; 		// r28{16}
    int local14; 		// %pc{15}
    int local16; 		// r27{68}
    int local17; 		// r31{70}
    int local18; 		// local3{60}
    int local19; 		// r28{61}
    int local20; 		// local4{62}
    int local21; 		// %pc{63}
    int local22; 		// r28{69}
    int local23; 		// %pc{71}
    int local24; 		// r24
    int local27; 		// r28
    int local3; 		// r27{60}
    int local4; 		// r31{62}
    int local5; 		// r28{31}
    int local6; 		// %pc{30}
    int local7; 		// m[r28]{27}
    int local8; 		// m[r28 + 4]{26}
    int local9; 		// r27{25}

    *(int*)(local27 - 4) = param1;
    local24 = read_line(param2, param3, param1, param4, param5, infile[0], num_input_strings[0]); /* Warning: also results in local11 */
    local20 = local11;
    *(int*)(local27 + 8) = 10;
    *(int*)(local27 + 4) = 0;
    *(int*)local27 = local24;
    strtol();
    local19 = local13;
    local21 = local14;
    local12 = local24;
    local18 = local12;
    if ((unsigned)(local24 - 1) > 1000) {
        local9 = explode_bomb(local24, local11); /* Warning: also results in local10 */
        local18 = local9;
        local19 = local27;
        local20 = local10;
    }
    local3 = local18;
    local27 = local19;
    local4 = local20;
    local8 = local3;
    local7 = 0x804c0a0;
    local24 = fun7(*(int*)(local27 + 4), *(int*)local27); /* Warning: also results in local5 */
    local22 = local5;
    local23 = local6;
    if (local24 != 0) {
        explode_bomb(local3, local4);
        local22 = local27;
    }
    local27 = local22;
    *(int*)local27 = 0x804a358;
    puts("Wow! You've defused the secret stage!");
    param2 = phase_defused(local16, local17);
    return param2;
}

// address: 0x8048eed
fun7(int param1, int param2) {
    int local4; 		// r24
    int local5; 		// r25
    int local6; 		// r26
    int local7; 		// r27
    int local8; 		// r28

    local6 = param1;
    local5 = param2;
    if (param1 == 0) {
        local4 = -1;
    } else {
        local7 = *(int*)param1;
        if (local7 <= param2) {
            local4 = 0;
            if (local7 != param2) {
                local4 = *(int*)(param1 + 8);
                local4 = fun7(local4, param2); /* Warning: also results in local5, local6 */
                local4 = local4 + local4 + 1;
            }
        } else {
            local4 = *(int*)(param1 + 4);
            local4 = fun7(local4, param2); /* Warning: also results in local5, local6 */
            local4 += local4;
        }
    }
    return local4; /* WARNING: Also returning: local5, local6 */
}

// address: 0x8049135
blank_line(int param1, int param2) {
    char local10; 		// tmpb
    int local3; 		// r27{51}
    char local4; 		// r8
    char local5; 		// r24
    char local8; 		// r27
    char local9; 		// r28

    local8 = param1;
    do {
        local3 = local8;
        if ((unsigned char) *(unsigned char*)local3 == 0) {
            local5 = 1;
            local4 = 1;
            goto L0;
        }
        __ctype_b_loc();
        local8 = local3 + 1;
        param2 = (unsigned char) *(unsigned char*)local3;
        local10 = *(int*)(*(char*)local5 + (int) (unsigned char) *(unsigned char*)local3 * 2 + 1) & 0x20;
    } while (local10 == 0);
    local5 = 0;
    local4 = 0;
L0:
    return local5; /* WARNING: Also returning: local4, param2 */
}

