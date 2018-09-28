//
// Created by bertrand on 27/09/18.
//

#include "core.h"
#include "main.h"

uint8_t core_ram_read(core *c, uint16_t address) {
    assert(address < RAM_SIZE - 1);
#ifdef DEBUG
    printf("RAM READ ram[%d]=%d\n", address, c->ram[address]);
#endif
    return c->ram[address];
}

void core_ram_write(core *c, uint16_t address, uint8_t value) {
    assert(address < RAM_SIZE - 1);
#ifdef DEBUG
    printf("RAM WRITE ram[%d]=%d\n", address, value);
#endif
    c->ram[address] = value;
}

void core_init(core *c) {
    memset(c, 0, sizeof(core)); // zero out memory and registers

    // Init boot rom
    uint8_t boot_buffer[BOOTSTRAP_ROM_SIZE];
    FILE *boot = fopen("resources/boot.bin", "r");
    fread(boot_buffer, sizeof(uint8_t), BOOTSTRAP_ROM_SIZE, boot);
    memcpy(c->ram, boot_buffer, BOOTSTRAP_ROM_SIZE);
    c->registers.pc = 0;
    core_setup_operations(c);
}

void core_loop(core *c) {
    while(1) {
        uint8_t code = core_ram_read(c, c->registers.pc++);
        if(code == 0xCB) { // extended operation
            code = core_ram_read(c, c->registers.pc++);
            printf("EXTENDED OP : ");
        }
        operation op = c->ops[code];

        if(op.handler == NULL) {
            printf("UNDEFINED OPERATION %02x\n", code);
            exit(EXIT_FAILURE);
        }

        printf("%s\n", op.disassembly);

        uint8_t *args = malloc(op.operand_number * sizeof(uint8_t));
        for(int i = 0; i < op.operand_number; i++)
            args[i] = core_ram_read(c, c->registers.pc++);
        op.handler(c, args);
        usleep(10000);
    }
}

void core_register_op(core *c, uint8_t code, operation op) {
    assert(c->ops[code].handler == NULL);
    c->ops[code] = op;
}

void core_register_ext_op(core *c, uint8_t code, operation op) {
    assert(c->ext_ops[code].handler == NULL);
    c->ext_ops[code] = op;
}

void core_stack_push8(core *c, uint8_t val) {
    core_ram_write(c, --c->registers.sp, val);
}

uint8_t core_stack_pop8(core *c) {
    return core_ram_read(c, c->registers.sp++);
}

void core_stack_push16(core *c, uint16_t val) {
    uint8_t lsb = (uint8_t) (val & 0xFF);
    uint8_t msb = (uint8_t) ((val >> 8) & 0xFF);
    core_stack_push8(c, lsb);
    core_stack_push8(c, msb);
}

uint16_t core_stack_pop16(core *c) {
    uint8_t msb = core_stack_pop8(c);
    uint8_t lsb = core_stack_pop8(c);
    return (msb << 8 | lsb);
}

void core_set_flag(core *c, flags f) {
    c->registers.f |= f;
}

void core_unset_flag(core *c, flags f) {
    c->registers.f &= ~f;
}

void core_toggle_flag(core *c, flags f) {
    c->registers.f ^= f;
}

uint8_t core_get_flag(core *c, flags f) {
    return c->registers.f & f;
}

void core_setup_operations(core *c) {
    core_register_op(c, 0x0, (operation) {
            "NOP", 0, noop
    });
    core_register_op(c, 0x1, (operation) {
            "LD BC,nn", 2, ld_bc_nn
    });
    core_register_op(c, 0x2, (operation) {
            "LD (BC),A", 0, ld_bc_a
    });
    core_register_op(c, 0x3, (operation) {
            "INC BC", 0, inc_bc
    });
    core_register_op(c, 0x4, (operation) {
            "INC B", 0, inc_b
    });
    core_register_op(c, 0x5, (operation) {
            "DEC B", 0, dec_b
    });
    core_register_op(c, 0x6, (operation) {
            "LD B,n", 1, ld_b_n
    });
    core_register_op(c, 0x7, (operation) {
            "RLC A", 0, rlc_a
    });
    core_register_op(c, 0x8, (operation) {
            "LD (nn),SP", 2, ld_nn_sp
    });
    core_register_op(c, 0x9, (operation) {
            "ADD HL,BC", 0, add_hl_bc
    });
    core_register_op(c, 0xa, (operation) {
            "LD A,(BC)", 0, ld_a_bc
    });
    core_register_op(c, 0xb, (operation) {
            "DEC BC", 0, dec_bc
    });
    core_register_op(c, 0xc, (operation) {
            "INC C", 0, inc_c
    });
    core_register_op(c, 0xd, (operation) {
            "DEC C", 0, dec_c
    });
    core_register_op(c, 0xe, (operation) {
            "LD C,n", 1, ld_c_n
    });
    core_register_op(c, 0xf, (operation) {
            "RRC A", 0, rrc_a
    });
    core_register_op(c, 0x10, (operation) {
            "STOP", 0, stop
    });
    core_register_op(c, 0x11, (operation) {
            "LD DE,nn", 2, ld_de_nn
    });
    core_register_op(c, 0x12, (operation) {
            "LD (DE),A", 0, ld_de_a
    });
    core_register_op(c, 0x13, (operation) {
            "INC DE", 0, inc_de
    });
    core_register_op(c, 0x14, (operation) {
            "INC D", 0, inc_d
    });
    core_register_op(c, 0x15, (operation) {
            "DEC D", 0, dec_d
    });
    core_register_op(c, 0x16, (operation) {
            "LD D,n", 1, ld_d_n
    });
    core_register_op(c, 0x17, (operation) {
            "RL A", 0, rl_a
    });
    core_register_op(c, 0x18, (operation) {
            "JR n", 1, jr_n
    });
    core_register_op(c, 0x19, (operation) {
            "ADD HL,DE", 0, add_hl_de
    });
    core_register_op(c, 0x1a, (operation) {
            "LD A,(DE)", 0, ld_a_de
    });
    core_register_op(c, 0x1b, (operation) {
            "DEC DE", 0, dec_de
    });
    core_register_op(c, 0x1c, (operation) {
            "INC E", 0, inc_e
    });
    core_register_op(c, 0x1d, (operation) {
            "DEC E", 0, dec_e
    });
    core_register_op(c, 0x1e, (operation) {
            "LD E,n", 1, ld_e_n
    });
    core_register_op(c, 0x1f, (operation) {
            "RR A", 0, rr_a
    });
    core_register_op(c, 0x20, (operation) {
            "JR NZ,n", 1, jr_nz_n
    });
    core_register_op(c, 0x21, (operation) {
            "LD HL,nn", 2, ld_hl_nn
    });
    core_register_op(c, 0x22, (operation) {
            "LDI (HL),A", 0, ldi_hl_a
    });
    core_register_op(c, 0x23, (operation) {
            "INC HL", 0, inc_hl
    });
    core_register_op(c, 0x24, (operation) {
            "INC H", 0, inc_h
    });
    core_register_op(c, 0x25, (operation) {
            "DEC H", 0, dec_h
    });
    core_register_op(c, 0x26, (operation) {
            "LD H,n", 1, ld_h_n
    });
    core_register_op(c, 0x27, (operation) {
            "DAA", 0, daa
    });
    core_register_op(c, 0x28, (operation) {
            "JR Z,n", 1, jr_z_n
    });
    core_register_op(c, 0x29, (operation) {
            "ADD HL,HL", 0, add_hl_hl
    });
    core_register_op(c, 0x2a, (operation) {
            "LDI A,(HL)", 0, ldi_a_hl
    });
    core_register_op(c, 0x2b, (operation) {
            "DEC HL", 0, dec_hl
    });
    core_register_op(c, 0x2c, (operation) {
            "INC L", 0, inc_l
    });
    core_register_op(c, 0x2d, (operation) {
            "DEC L", 0, dec_l
    });
    core_register_op(c, 0x2e, (operation) {
            "LD L,n", 1, ld_l_n
    });
    core_register_op(c, 0x2f, (operation) {
            "CPL", 0, cpl
    });
    core_register_op(c, 0x30, (operation) {
            "JR NC,n", 1, jr_nc_n
    });
    core_register_op(c, 0x31, (operation) {
            "LD SP,nn", 2, ld_sp_nn
    });
    core_register_op(c, 0x32, (operation) {
            "LDD (HL),A", 0, ldd_hl_a
    });
    core_register_op(c, 0x33, (operation) {
            "INC SP", 0, inc_sp
    });
    core_register_op(c, 0x34, (operation) {
            "INC (HL)", 0, inc_hl
    });
    core_register_op(c, 0x35, (operation) {
            "DEC (HL)", 0, dec_hl
    });
    core_register_op(c, 0x36, (operation) {
            "LD (HL),n", 1, ld_hl_n
    });
    core_register_op(c, 0x37, (operation) {
            "SCF", 0, scf
    });
    core_register_op(c, 0x38, (operation) {
            "JR C,n", 1, jr_c_n
    });
    core_register_op(c, 0x39, (operation) {
            "ADD HL,SP", 0, add_hl_sp
    });
    core_register_op(c, 0x3a, (operation) {
            "LDD A,(HL)", 0, ldd_a_hl
    });
    core_register_op(c, 0x3b, (operation) {
            "DEC SP", 0, dec_sp
    });
    core_register_op(c, 0x3c, (operation) {
            "INC A", 0, inc_a
    });
    core_register_op(c, 0x3d, (operation) {
            "DEC A", 0, dec_a
    });
    core_register_op(c, 0x3e, (operation) {
            "LD A,n", 1, ld_a_n
    });
    core_register_op(c, 0x3f, (operation) {
            "CCF", 0, ccf
    });
    core_register_op(c, 0x40, (operation) {
            "LD B,B", 0, ld_b_b
    });
    core_register_op(c, 0x41, (operation) {
            "LD B,C", 0, ld_b_c
    });
    core_register_op(c, 0x42, (operation) {
            "LD B,D", 0, ld_b_d
    });
    core_register_op(c, 0x43, (operation) {
            "LD B,E", 0, ld_b_e
    });
    core_register_op(c, 0x44, (operation) {
            "LD B,H", 0, ld_b_h
    });
    core_register_op(c, 0x45, (operation) {
            "LD B,L", 0, ld_b_l
    });
    core_register_op(c, 0x46, (operation) {
            "LD B,(HL)", 0, ld_b_hl
    });
    core_register_op(c, 0x47, (operation) {
            "LD B,A", 0, ld_b_a
    });
    core_register_op(c, 0x48, (operation) {
            "LD C,B", 0, ld_c_b
    });
    core_register_op(c, 0x49, (operation) {
            "LD C,C", 0, ld_c_c
    });
    core_register_op(c, 0x4a, (operation) {
            "LD C,D", 0, ld_c_d
    });
    core_register_op(c, 0x4b, (operation) {
            "LD C,E", 0, ld_c_e
    });
    core_register_op(c, 0x4c, (operation) {
            "LD C,H", 0, ld_c_h
    });
    core_register_op(c, 0x4d, (operation) {
            "LD C,L", 0, ld_c_l
    });
    core_register_op(c, 0x4e, (operation) {
            "LD C,(HL)", 0, ld_c_hl
    });
    core_register_op(c, 0x4f, (operation) {
            "LD C,A", 0, ld_c_a
    });
    core_register_op(c, 0x50, (operation) {
            "LD D,B", 0, ld_d_b
    });
    core_register_op(c, 0x51, (operation) {
            "LD D,C", 0, ld_d_c
    });
    core_register_op(c, 0x52, (operation) {
            "LD D,D", 0, ld_d_d
    });
    core_register_op(c, 0x53, (operation) {
            "LD D,E", 0, ld_d_e
    });
    core_register_op(c, 0x54, (operation) {
            "LD D,H", 0, ld_d_h
    });
    core_register_op(c, 0x55, (operation) {
            "LD D,L", 0, ld_d_l
    });
    core_register_op(c, 0x56, (operation) {
            "LD D,(HL)", 0, ld_d_hl
    });
    core_register_op(c, 0x57, (operation) {
            "LD D,A", 0, ld_d_a
    });
    core_register_op(c, 0x58, (operation) {
            "LD E,B", 0, ld_e_b
    });
    core_register_op(c, 0x59, (operation) {
            "LD E,C", 0, ld_e_c
    });
    core_register_op(c, 0x5a, (operation) {
            "LD E,D", 0, ld_e_d
    });
    core_register_op(c, 0x5b, (operation) {
            "LD E,E", 0, ld_e_e
    });
    core_register_op(c, 0x5c, (operation) {
            "LD E,H", 0, ld_e_h
    });
    core_register_op(c, 0x5d, (operation) {
            "LD E,L", 0, ld_e_l
    });
    core_register_op(c, 0x5e, (operation) {
            "LD E,(HL)", 0, ld_e_hl
    });
    core_register_op(c, 0x5f, (operation) {
            "LD E,A", 0, ld_e_a
    });
    core_register_op(c, 0x60, (operation) {
            "LD H,B", 0, ld_h_b
    });
    core_register_op(c, 0x61, (operation) {
            "LD H,C", 0, ld_h_c
    });
    core_register_op(c, 0x62, (operation) {
            "LD H,D", 0, ld_h_d
    });
    core_register_op(c, 0x63, (operation) {
            "LD H,E", 0, ld_h_e
    });
    core_register_op(c, 0x64, (operation) {
            "LD H,H", 0, ld_h_h
    });
    core_register_op(c, 0x65, (operation) {
            "LD H,L", 0, ld_h_l
    });
    core_register_op(c, 0x66, (operation) {
            "LD H,(HL)", 0, ld_h_hl
    });
    core_register_op(c, 0x67, (operation) {
            "LD H,A", 0, ld_h_a
    });
    core_register_op(c, 0x68, (operation) {
            "LD L,B", 0, ld_l_b
    });
    core_register_op(c, 0x69, (operation) {
            "LD L,C", 0, ld_l_c
    });
    core_register_op(c, 0x6a, (operation) {
            "LD L,D", 0, ld_l_d
    });
    core_register_op(c, 0x6b, (operation) {
            "LD L,E", 0, ld_l_e
    });
    core_register_op(c, 0x6c, (operation) {
            "LD L,H", 0, ld_l_h
    });
    core_register_op(c, 0x6d, (operation) {
            "LD L,L", 0, ld_l_l
    });
    core_register_op(c, 0x6e, (operation) {
            "LD L,(HL)", 0, ld_l_hl
    });
    core_register_op(c, 0x6f, (operation) {
            "LD L,A", 0, ld_l_a
    });
    core_register_op(c, 0x70, (operation) {
            "LD (HL),B", 0, ld_hl_b
    });
    core_register_op(c, 0x71, (operation) {
            "LD (HL),C", 0, ld_hl_c
    });
    core_register_op(c, 0x72, (operation) {
            "LD (HL),D", 0, ld_hl_d
    });
    core_register_op(c, 0x73, (operation) {
            "LD (HL),E", 0, ld_hl_e
    });
    core_register_op(c, 0x74, (operation) {
            "LD (HL),H", 0, ld_hl_h
    });
    core_register_op(c, 0x75, (operation) {
            "LD (HL),L", 0, ld_hl_l
    });
    core_register_op(c, 0x76, (operation) {
            "HALT", 0, halt
    });
    core_register_op(c, 0x77, (operation) {
            "LD (HL),A", 0, ld_hl_a
    });
    core_register_op(c, 0x78, (operation) {
            "LD A,B", 0, ld_a_b
    });
    core_register_op(c, 0x79, (operation) {
            "LD A,C", 0, ld_a_c
    });
    core_register_op(c, 0x7a, (operation) {
            "LD A,D", 0, ld_a_d
    });
    core_register_op(c, 0x7b, (operation) {
            "LD A,E", 0, ld_a_e
    });
    core_register_op(c, 0x7c, (operation) {
            "LD A,H", 0, ld_a_h
    });
    core_register_op(c, 0x7d, (operation) {
            "LD A,L", 0, ld_a_l
    });
    core_register_op(c, 0x7e, (operation) {
            "LD A,(HL)", 0, ld_a_hl
    });
    core_register_op(c, 0x7f, (operation) {
            "LD A,A", 0, ld_a_a
    });
    core_register_op(c, 0x80, (operation) {
            "ADD A,B", 0, add_a_b
    });
    core_register_op(c, 0x81, (operation) {
            "ADD A,C", 0, add_a_c
    });
    core_register_op(c, 0x82, (operation) {
            "ADD A,D", 0, add_a_d
    });
    core_register_op(c, 0x83, (operation) {
            "ADD A,E", 0, add_a_e
    });
    core_register_op(c, 0x84, (operation) {
            "ADD A,H", 0, add_a_h
    });
    core_register_op(c, 0x85, (operation) {
            "ADD A,L", 0, add_a_l
    });
    core_register_op(c, 0x86, (operation) {
            "ADD A,(HL)", 0, add_a_hl
    });
    core_register_op(c, 0x87, (operation) {
            "ADD A,A", 0, add_a_a
    });
    core_register_op(c, 0x88, (operation) {
            "ADC A,B", 0, adc_a_b
    });
    core_register_op(c, 0x89, (operation) {
            "ADC A,C", 0, adc_a_c
    });
    core_register_op(c, 0x8a, (operation) {
            "ADC A,D", 0, adc_a_d
    });
    core_register_op(c, 0x8b, (operation) {
            "ADC A,E", 0, adc_a_e
    });
    core_register_op(c, 0x8c, (operation) {
            "ADC A,H", 0, adc_a_h
    });
    core_register_op(c, 0x8d, (operation) {
            "ADC A,L", 0, adc_a_l
    });
    core_register_op(c, 0x8e, (operation) {
            "ADC A,(HL)", 0, adc_a_hl
    });
    core_register_op(c, 0x8f, (operation) {
            "ADC A,A", 0, adc_a_a
    });
    core_register_op(c, 0x90, (operation) {
            "SUB A,B", 0, sub_a_b
    });
    core_register_op(c, 0x91, (operation) {
            "SUB A,C", 0, sub_a_c
    });
    core_register_op(c, 0x92, (operation) {
            "SUB A,D", 0, sub_a_d
    });
    core_register_op(c, 0x93, (operation) {
            "SUB A,E", 0, sub_a_e
    });
    core_register_op(c, 0x94, (operation) {
            "SUB A,H", 0, sub_a_h
    });
    core_register_op(c, 0x95, (operation) {
            "SUB A,L", 0, sub_a_l
    });
    core_register_op(c, 0x96, (operation) {
            "SUB A,(HL)", 0, sub_a_hl
    });
    core_register_op(c, 0x97, (operation) {
            "SUB A,A", 0, sub_a_a
    });
    core_register_op(c, 0x98, (operation) {
            "SBC A,B", 0, sbc_a_b
    });
    core_register_op(c, 0x99, (operation) {
            "SBC A,C", 0, sbc_a_c
    });
    core_register_op(c, 0x9a, (operation) {
            "SBC A,D", 0, sbc_a_d
    });
    core_register_op(c, 0x9b, (operation) {
            "SBC A,E", 0, sbc_a_e
    });
    core_register_op(c, 0x9c, (operation) {
            "SBC A,H", 0, sbc_a_h
    });
    core_register_op(c, 0x9d, (operation) {
            "SBC A,L", 0, sbc_a_l
    });
    core_register_op(c, 0x9e, (operation) {
            "SBC A,(HL)", 0, sbc_a_hl
    });
    core_register_op(c, 0x9f, (operation) {
            "SBC A,A", 0, sbc_a_a
    });
    core_register_op(c, 0xa0, (operation) {
            "AND B", 0, and_b
    });
    core_register_op(c, 0xa1, (operation) {
            "AND C", 0, and_c
    });
    core_register_op(c, 0xa2, (operation) {
            "AND D", 0, and_d
    });
    core_register_op(c, 0xa3, (operation) {
            "AND E", 0, and_e
    });
    core_register_op(c, 0xa4, (operation) {
            "AND H", 0, and_h
    });
    core_register_op(c, 0xa5, (operation) {
            "AND L", 0, and_l
    });
    core_register_op(c, 0xa6, (operation) {
            "AND (HL)", 0, and_hl
    });
    core_register_op(c, 0xa7, (operation) {
            "AND A", 0, and_a
    });
    core_register_op(c, 0xa8, (operation) {
            "XOR B", 0, xor_b
    });
    core_register_op(c, 0xa9, (operation) {
            "XOR C", 0, xor_c
    });
    core_register_op(c, 0xaa, (operation) {
            "XOR D", 0, xor_d
    });
    core_register_op(c, 0xab, (operation) {
            "XOR E", 0, xor_e
    });
    core_register_op(c, 0xac, (operation) {
            "XOR H", 0, xor_h
    });
    core_register_op(c, 0xad, (operation) {
            "XOR L", 0, xor_l
    });
    core_register_op(c, 0xae, (operation) {
            "XOR (HL)", 0, xor_hl
    });
    core_register_op(c, 0xaf, (operation) {
            "XOR A", 0, xor_a
    });
    core_register_op(c, 0xb0, (operation) {
            "OR B", 0, or_b
    });
    core_register_op(c, 0xb1, (operation) {
            "OR C", 0, or_c
    });
    core_register_op(c, 0xb2, (operation) {
            "OR D", 0, or_d
    });
    core_register_op(c, 0xb3, (operation) {
            "OR E", 0, or_e
    });
    core_register_op(c, 0xb4, (operation) {
            "OR H", 0, or_h
    });
    core_register_op(c, 0xb5, (operation) {
            "OR L", 0, or_l
    });
    core_register_op(c, 0xb6, (operation) {
            "OR (HL)", 0, or_hl
    });
    core_register_op(c, 0xb7, (operation) {
            "OR A", 0, or_a
    });
    core_register_op(c, 0xb8, (operation) {
            "CP B", 0, cp_b
    });
    core_register_op(c, 0xb9, (operation) {
            "CP C", 0, cp_c
    });
    core_register_op(c, 0xba, (operation) {
            "CP D", 0, cp_d
    });
    core_register_op(c, 0xbb, (operation) {
            "CP E", 0, cp_e
    });
    core_register_op(c, 0xbc, (operation) {
            "CP H", 0, cp_h
    });
    core_register_op(c, 0xbd, (operation) {
            "CP L", 0, cp_l
    });
    core_register_op(c, 0xbe, (operation) {
            "CP (HL)", 0, cp_hl
    });
    core_register_op(c, 0xbf, (operation) {
            "CP A", 0, cp_a
    });
    core_register_op(c, 0xc0, (operation) {
            "RET NZ", 0, ret_nz
    });
    core_register_op(c, 0xc1, (operation) {
            "POP BC", 0, pop_bc
    });
    core_register_op(c, 0xc2, (operation) {
            "JP NZ,nn", 2, jp_nz_nn
    });
    core_register_op(c, 0xc3, (operation) {
            "JP nn", 2, jp_nn
    });
    core_register_op(c, 0xc4, (operation) {
            "CALL NZ,nn", 2, call_nz_nn
    });
    core_register_op(c, 0xc5, (operation) {
            "PUSH BC", 0, push_bc
    });
    core_register_op(c, 0xc6, (operation) {
            "ADD A,n", 1, add_a_n
    });
    core_register_op(c, 0xc7, (operation) {
            "RST 0", 0, rst_0
    });
    core_register_op(c, 0xc8, (operation) {
            "RET Z", 0, ret_z
    });
    core_register_op(c, 0xc9, (operation) {
            "RET", 0, ret
    });
    core_register_op(c, 0xca, (operation) {
            "JP Z,nn", 2, jp_z_nn
    });
    core_register_op(c, 0xcc, (operation) {
            "CALL Z,nn", 2, call_z_nn
    });
    core_register_op(c, 0xcd, (operation) {
            "CALL nn", 2, call_nn
    });
    core_register_op(c, 0xce, (operation) {
            "ADC A,n", 1, adc_a_n
    });
    core_register_op(c, 0xcf, (operation) {
            "RST 8", 0, rst_8
    });
    core_register_op(c, 0xd0, (operation) {
            "RET NC", 0, ret_nc
    });
    core_register_op(c, 0xd1, (operation) {
            "POP DE", 0, pop_de
    });
    core_register_op(c, 0xd2, (operation) {
            "JP NC,nn", 2, jp_nc_nn
    });
    core_register_op(c, 0xd3, (operation) {
            "XX", 0, xx
    });
    core_register_op(c, 0xd4, (operation) {
            "CALL NC,nn", 2, call_nc_nn
    });
    core_register_op(c, 0xd5, (operation) {
            "PUSH DE", 0, push_de
    });
    core_register_op(c, 0xd6, (operation) {
            "SUB A,n", 1, sub_a_n
    });
    core_register_op(c, 0xd7, (operation) {
            "RST 10", 0, rst_10
    });
    core_register_op(c, 0xd8, (operation) {
            "RET C", 0, ret_c
    });
    core_register_op(c, 0xd9, (operation) {
            "RETI", 0, reti
    });
    core_register_op(c, 0xda, (operation) {
            "JP C,nn", 2, jp_c_nn
    });
    core_register_op(c, 0xdb, (operation) {
            "XX", 0, xx
    });
    core_register_op(c, 0xdc, (operation) {
            "CALL C,nn", 2, call_c_nn
    });
    core_register_op(c, 0xdd, (operation) {
            "XX", 0, xx
    });
    core_register_op(c, 0xde, (operation) {
            "SBC A,n", 1, sbc_a_n
    });
    core_register_op(c, 0xdf, (operation) {
            "RST 18", 0, rst_18
    });
    core_register_op(c, 0xe0, (operation) {
            "LDH (n),A", 1, ldh_n_a
    });
    core_register_op(c, 0xe1, (operation) {
            "POP HL", 0, pop_hl
    });
    core_register_op(c, 0xe2, (operation) {
            "LDH (C),A", 0, ldh_c_a
    });
    core_register_op(c, 0xe3, (operation) {
            "XX", 0, xx
    });
    core_register_op(c, 0xe4, (operation) {
            "XX", 0, xx
    });
    core_register_op(c, 0xe5, (operation) {
            "PUSH HL", 0, push_hl
    });
    core_register_op(c, 0xe6, (operation) {
            "AND n", 1, and_n
    });
    core_register_op(c, 0xe7, (operation) {
            "RST 20", 0, rst_20
    });
    core_register_op(c, 0xe8, (operation) {
            "ADD SP,d", 0, add_sp_d
    });
    core_register_op(c, 0xe9, (operation) {
            "JP (HL)", 0, jp_hl
    });
    core_register_op(c, 0xea, (operation) {
            "LD (nn),A", 2, ld_nn_a
    });
    core_register_op(c, 0xeb, (operation) {
            "XX", 0, xx
    });
    core_register_op(c, 0xec, (operation) {
            "XX", 0, xx
    });
    core_register_op(c, 0xed, (operation) {
            "XX", 0, xx
    });
    core_register_op(c, 0xee, (operation) {
            "XOR n", 1, xor_n
    });
    core_register_op(c, 0xef, (operation) {
            "RST 28", 0, rst_28
    });
    core_register_op(c, 0xf0, (operation) {
            "LDH A,(n)", 1, ldh_a_n
    });
    core_register_op(c, 0xf1, (operation) {
            "POP AF", 0, pop_af
    });
    core_register_op(c, 0xf2, (operation) {
            "XX", 0, xx
    });
    core_register_op(c, 0xf3, (operation) {
            "DI", 0, di
    });
    core_register_op(c, 0xf4, (operation) {
            "XX", 0, xx
    });
    core_register_op(c, 0xf5, (operation) {
            "PUSH AF", 0, push_af
    });
    core_register_op(c, 0xf6, (operation) {
            "OR n", 1, or_n
    });
    core_register_op(c, 0xf7, (operation) {
            "RST 30", 0, rst_30
    });
    /*core_register_op(c, 0xf8, (operation) {
            "LDHL SP,d", 0, ldhl_sp_d
    });*/
    core_register_op(c, 0xf9, (operation) {
            "LD SP,HL", 0, ld_sp_hl
    });
    core_register_op(c, 0xfa, (operation) {
            "LD A,(nn)", 2, ld_a_nn
    });
    core_register_op(c, 0xfb, (operation) {
            "EI", 0, ei
    });
    core_register_op(c, 0xfc, (operation) {
            "XX", 0, xx
    });
    core_register_op(c, 0xfd, (operation) {
            "XX", 0, xx
    });
    core_register_op(c, 0xfe, (operation) {
            "CP n", 1, cp_n
    });
    core_register_op(c, 0xff, (operation) {
            "RST 38", 0, rst_38
    });

    core_register_ext_op(c, 0x0, (operation) {
        "RLC B", 0, rlc_b
    });
    core_register_ext_op(c, 0x1, (operation) {
        "RLC C", 0, rlc_c
    });
    core_register_ext_op(c, 0x2, (operation) {
        "RLC D", 0, rlc_d
    });
    core_register_ext_op(c, 0x3, (operation) {
        "RLC E", 0, rlc_e
    });
    core_register_ext_op(c, 0x4, (operation) {
        "RLC H", 0, rlc_h
    });
    core_register_ext_op(c, 0x5, (operation) {
        "RLC L", 0, rlc_l
    });
    core_register_ext_op(c, 0x6, (operation) {
        "RLC (HL)", 0, rlc_hl
    });
    core_register_ext_op(c, 0x7, (operation) {
        "RLC A", 0, rlc_a
    });
    core_register_ext_op(c, 0x8, (operation) {
        "RRC B", 0, rrc_b
    });
    core_register_ext_op(c, 0x9, (operation) {
        "RRC C", 0, rrc_c
    });
    core_register_ext_op(c, 0xa, (operation) {
        "RRC D", 0, rrc_d
    });
    core_register_ext_op(c, 0xb, (operation) {
        "RRC E", 0, rrc_e
    });
    core_register_ext_op(c, 0xc, (operation) {
        "RRC H", 0, rrc_h
    });
    core_register_ext_op(c, 0xd, (operation) {
        "RRC L", 0, rrc_l
    });
    core_register_ext_op(c, 0xe, (operation) {
        "RRC (HL)", 0, rrc_hl
    });
    core_register_ext_op(c, 0xf, (operation) {
        "RRC A", 0, rrc_a
    });
    core_register_ext_op(c, 0x10, (operation) {
        "RL B", 0, rl_b
    });
    core_register_ext_op(c, 0x11, (operation) {
        "RL C", 0, rl_c
    });
    core_register_ext_op(c, 0x12, (operation) {
        "RL D", 0, rl_d
    });
    core_register_ext_op(c, 0x13, (operation) {
        "RL E", 0, rl_e
    });
    core_register_ext_op(c, 0x14, (operation) {
        "RL H", 0, rl_h
    });
    core_register_ext_op(c, 0x15, (operation) {
        "RL L", 0, rl_l
    });
    core_register_ext_op(c, 0x16, (operation) {
        "RL (HL)", 0, rl_hl
    });
    core_register_ext_op(c, 0x17, (operation) {
        "RL A", 0, rl_a
    });
    core_register_ext_op(c, 0x18, (operation) {
        "RR B", 0, rr_b
    });
    core_register_ext_op(c, 0x19, (operation) {
        "RR C", 0, rr_c
    });
    core_register_ext_op(c, 0x1a, (operation) {
        "RR D", 0, rr_d
    });
    core_register_ext_op(c, 0x1b, (operation) {
        "RR E", 0, rr_e
    });
    core_register_ext_op(c, 0x1c, (operation) {
        "RR H", 0, rr_h
    });
    core_register_ext_op(c, 0x1d, (operation) {
        "RR L", 0, rr_l
    });
    core_register_ext_op(c, 0x1e, (operation) {
        "RR (HL)", 0, rr_hl
    });
    core_register_ext_op(c, 0x1f, (operation) {
        "RR A", 0, rr_a
    });
    core_register_ext_op(c, 0x20, (operation) {
        "SLA B", 0, sla_b
    });
    core_register_ext_op(c, 0x21, (operation) {
        "SLA C", 0, sla_c
    });
    core_register_ext_op(c, 0x22, (operation) {
        "SLA D", 0, sla_d
    });
    core_register_ext_op(c, 0x23, (operation) {
        "SLA E", 0, sla_e
    });
    core_register_ext_op(c, 0x24, (operation) {
        "SLA H", 0, sla_h
    });
    core_register_ext_op(c, 0x25, (operation) {
        "SLA L", 0, sla_l
    });
    core_register_ext_op(c, 0x26, (operation) {
        "SLA (HL)", 0, sla_hl
    });
    core_register_ext_op(c, 0x27, (operation) {
        "SLA A", 0, sla_a
    });
    core_register_ext_op(c, 0x28, (operation) {
        "SRA B", 0, sra_b
    });
    core_register_ext_op(c, 0x29, (operation) {
        "SRA C", 0, sra_c
    });
    core_register_ext_op(c, 0x2a, (operation) {
        "SRA D", 0, sra_d
    });
    core_register_ext_op(c, 0x2b, (operation) {
        "SRA E", 0, sra_e
    });
    core_register_ext_op(c, 0x2c, (operation) {
        "SRA H", 0, sra_h
    });
    core_register_ext_op(c, 0x2d, (operation) {
        "SRA L", 0, sra_l
    });
    core_register_ext_op(c, 0x2e, (operation) {
        "SRA (HL)", 0, sra_hl
    });
    core_register_ext_op(c, 0x2f, (operation) {
        "SRA A", 0, sra_a
    });
    core_register_ext_op(c, 0x30, (operation) {
        "SWAP B", 0, swap_b
    });
    core_register_ext_op(c, 0x31, (operation) {
        "SWAP C", 0, swap_c
    });
    core_register_ext_op(c, 0x32, (operation) {
        "SWAP D", 0, swap_d
    });
    core_register_ext_op(c, 0x33, (operation) {
        "SWAP E", 0, swap_e
    });
    core_register_ext_op(c, 0x34, (operation) {
        "SWAP H", 0, swap_h
    });
    core_register_ext_op(c, 0x35, (operation) {
        "SWAP L", 0, swap_l
    });
    core_register_ext_op(c, 0x36, (operation) {
        "SWAP (HL)", 0, swap_hl
    });
    core_register_ext_op(c, 0x37, (operation) {
        "SWAP A", 0, swap_a
    });
    core_register_ext_op(c, 0x38, (operation) {
        "SRL B", 0, srl_b
    });
    core_register_ext_op(c, 0x39, (operation) {
        "SRL C", 0, srl_c
    });
    core_register_ext_op(c, 0x3a, (operation) {
        "SRL D", 0, srl_d
    });
    core_register_ext_op(c, 0x3b, (operation) {
        "SRL E", 0, srl_e
    });
    core_register_ext_op(c, 0x3c, (operation) {
        "SRL H", 0, srl_h
    });
    core_register_ext_op(c, 0x3d, (operation) {
        "SRL L", 0, srl_l
    });
    core_register_ext_op(c, 0x3e, (operation) {
        "SRL (HL)", 0, srl_hl
    });
    core_register_ext_op(c, 0x3f, (operation) {
        "SRL A", 0, srl_a
    });
    core_register_ext_op(c, 0x40, (operation) {
        "BIT 0,B", 0, bit_0_b
    });
    core_register_ext_op(c, 0x41, (operation) {
        "BIT 0,C", 0, bit_0_c
    });
    core_register_ext_op(c, 0x42, (operation) {
        "BIT 0,D", 0, bit_0_d
    });
    core_register_ext_op(c, 0x43, (operation) {
        "BIT 0,E", 0, bit_0_e
    });
    core_register_ext_op(c, 0x44, (operation) {
        "BIT 0,H", 0, bit_0_h
    });
    core_register_ext_op(c, 0x45, (operation) {
        "BIT 0,L", 0, bit_0_l
    });
    core_register_ext_op(c, 0x46, (operation) {
        "BIT 0,(HL)", 0, bit_0_hl
    });
    core_register_ext_op(c, 0x47, (operation) {
        "BIT 0,A", 0, bit_0_a
    });
    core_register_ext_op(c, 0x48, (operation) {
        "BIT 1,B", 0, bit_1_b
    });
    core_register_ext_op(c, 0x49, (operation) {
        "BIT 1,C", 0, bit_1_c
    });
    core_register_ext_op(c, 0x4a, (operation) {
        "BIT 1,D", 0, bit_1_d
    });
    core_register_ext_op(c, 0x4b, (operation) {
        "BIT 1,E", 0, bit_1_e
    });
    core_register_ext_op(c, 0x4c, (operation) {
        "BIT 1,H", 0, bit_1_h
    });
    core_register_ext_op(c, 0x4d, (operation) {
        "BIT 1,L", 0, bit_1_l
    });
    core_register_ext_op(c, 0x4e, (operation) {
        "BIT 1,(HL)", 0, bit_1_hl
    });
    core_register_ext_op(c, 0x4f, (operation) {
        "BIT 1,A", 0, bit_1_a
    });
    core_register_ext_op(c, 0x50, (operation) {
        "BIT 2,B", 0, bit_2_b
    });
    core_register_ext_op(c, 0x51, (operation) {
        "BIT 2,C", 0, bit_2_c
    });
    core_register_ext_op(c, 0x52, (operation) {
        "BIT 2,D", 0, bit_2_d
    });
    core_register_ext_op(c, 0x53, (operation) {
        "BIT 2,E", 0, bit_2_e
    });
    core_register_ext_op(c, 0x54, (operation) {
        "BIT 2,H", 0, bit_2_h
    });
    core_register_ext_op(c, 0x55, (operation) {
        "BIT 2,L", 0, bit_2_l
    });
    core_register_ext_op(c, 0x56, (operation) {
        "BIT 2,(HL)", 0, bit_2_hl
    });
    core_register_ext_op(c, 0x57, (operation) {
        "BIT 2,A", 0, bit_2_a
    });
    core_register_ext_op(c, 0x58, (operation) {
        "BIT 3,B", 0, bit_3_b
    });
    core_register_ext_op(c, 0x59, (operation) {
        "BIT 3,C", 0, bit_3_c
    });
    core_register_ext_op(c, 0x5a, (operation) {
        "BIT 3,D", 0, bit_3_d
    });
    core_register_ext_op(c, 0x5b, (operation) {
        "BIT 3,E", 0, bit_3_e
    });
    core_register_ext_op(c, 0x5c, (operation) {
        "BIT 3,H", 0, bit_3_h
    });
    core_register_ext_op(c, 0x5d, (operation) {
        "BIT 3,L", 0, bit_3_l
    });
    core_register_ext_op(c, 0x5e, (operation) {
        "BIT 3,(HL)", 0, bit_3_hl
    });
    core_register_ext_op(c, 0x5f, (operation) {
        "BIT 3,A", 0, bit_3_a
    });
    core_register_ext_op(c, 0x60, (operation) {
        "BIT 4,B", 0, bit_4_b
    });
    core_register_ext_op(c, 0x61, (operation) {
        "BIT 4,C", 0, bit_4_c
    });
    core_register_ext_op(c, 0x62, (operation) {
        "BIT 4,D", 0, bit_4_d
    });
    core_register_ext_op(c, 0x63, (operation) {
        "BIT 4,E", 0, bit_4_e
    });
    core_register_ext_op(c, 0x64, (operation) {
        "BIT 4,H", 0, bit_4_h
    });
    core_register_ext_op(c, 0x65, (operation) {
        "BIT 4,L", 0, bit_4_l
    });
    core_register_ext_op(c, 0x66, (operation) {
        "BIT 4,(HL)", 0, bit_4_hl
    });
    core_register_ext_op(c, 0x67, (operation) {
        "BIT 4,A", 0, bit_4_a
    });
    core_register_ext_op(c, 0x68, (operation) {
        "BIT 5,B", 0, bit_5_b
    });
    core_register_ext_op(c, 0x69, (operation) {
        "BIT 5,C", 0, bit_5_c
    });
    core_register_ext_op(c, 0x6a, (operation) {
        "BIT 5,D", 0, bit_5_d
    });
    core_register_ext_op(c, 0x6b, (operation) {
        "BIT 5,E", 0, bit_5_e
    });
    core_register_ext_op(c, 0x6c, (operation) {
        "BIT 5,H", 0, bit_5_h
    });
    core_register_ext_op(c, 0x6d, (operation) {
        "BIT 5,L", 0, bit_5_l
    });
    core_register_ext_op(c, 0x6e, (operation) {
        "BIT 5,(HL)", 0, bit_5_hl
    });
    core_register_ext_op(c, 0x6f, (operation) {
        "BIT 5,A", 0, bit_5_a
    });
    core_register_ext_op(c, 0x70, (operation) {
        "BIT 6,B", 0, bit_6_b
    });
    core_register_ext_op(c, 0x71, (operation) {
        "BIT 6,C", 0, bit_6_c
    });
    core_register_ext_op(c, 0x72, (operation) {
        "BIT 6,D", 0, bit_6_d
    });
    core_register_ext_op(c, 0x73, (operation) {
        "BIT 6,E", 0, bit_6_e
    });
    core_register_ext_op(c, 0x74, (operation) {
        "BIT 6,H", 0, bit_6_h
    });
    core_register_ext_op(c, 0x75, (operation) {
        "BIT 6,L", 0, bit_6_l
    });
    core_register_ext_op(c, 0x76, (operation) {
        "BIT 6,(HL)", 0, bit_6_hl
    });
    core_register_ext_op(c, 0x77, (operation) {
        "BIT 6,A", 0, bit_6_a
    });
    core_register_ext_op(c, 0x78, (operation) {
        "BIT 7,B", 0, bit_7_b
    });
    core_register_ext_op(c, 0x79, (operation) {
        "BIT 7,C", 0, bit_7_c
    });
    core_register_ext_op(c, 0x7a, (operation) {
        "BIT 7,D", 0, bit_7_d
    });
    core_register_ext_op(c, 0x7b, (operation) {
        "BIT 7,E", 0, bit_7_e
    });
    core_register_ext_op(c, 0x7c, (operation) {
        "BIT 7,H", 0, bit_7_h
    });
    core_register_ext_op(c, 0x7d, (operation) {
        "BIT 7,L", 0, bit_7_l
    });
    core_register_ext_op(c, 0x7e, (operation) {
        "BIT 7,(HL)", 0, bit_7_hl
    });
    core_register_ext_op(c, 0x7f, (operation) {
        "BIT 7,A", 0, bit_7_a
    });
    core_register_ext_op(c, 0x80, (operation) {
        "RES 0,B", 0, res_0_b
    });
    core_register_ext_op(c, 0x81, (operation) {
        "RES 0,C", 0, res_0_c
    });
    core_register_ext_op(c, 0x82, (operation) {
        "RES 0,D", 0, res_0_d
    });
    core_register_ext_op(c, 0x83, (operation) {
        "RES 0,E", 0, res_0_e
    });
    core_register_ext_op(c, 0x84, (operation) {
        "RES 0,H", 0, res_0_h
    });
    core_register_ext_op(c, 0x85, (operation) {
        "RES 0,L", 0, res_0_l
    });
    core_register_ext_op(c, 0x86, (operation) {
        "RES 0,(HL)", 0, res_0_hl
    });
    core_register_ext_op(c, 0x87, (operation) {
        "RES 0,A", 0, res_0_a
    });
    core_register_ext_op(c, 0x88, (operation) {
        "RES 1,B", 0, res_1_b
    });
    core_register_ext_op(c, 0x89, (operation) {
        "RES 1,C", 0, res_1_c
    });
    core_register_ext_op(c, 0x8a, (operation) {
        "RES 1,D", 0, res_1_d
    });
    core_register_ext_op(c, 0x8b, (operation) {
        "RES 1,E", 0, res_1_e
    });
    core_register_ext_op(c, 0x8c, (operation) {
        "RES 1,H", 0, res_1_h
    });
    core_register_ext_op(c, 0x8d, (operation) {
        "RES 1,L", 0, res_1_l
    });
    core_register_ext_op(c, 0x8e, (operation) {
        "RES 1,(HL)", 0, res_1_hl
    });
    core_register_ext_op(c, 0x8f, (operation) {
        "RES 1,A", 0, res_1_a
    });
    core_register_ext_op(c, 0x90, (operation) {
        "RES 2,B", 0, res_2_b
    });
    core_register_ext_op(c, 0x91, (operation) {
        "RES 2,C", 0, res_2_c
    });
    core_register_ext_op(c, 0x92, (operation) {
        "RES 2,D", 0, res_2_d
    });
    core_register_ext_op(c, 0x93, (operation) {
        "RES 2,E", 0, res_2_e
    });
    core_register_ext_op(c, 0x94, (operation) {
        "RES 2,H", 0, res_2_h
    });
    core_register_ext_op(c, 0x95, (operation) {
        "RES 2,L", 0, res_2_l
    });
    core_register_ext_op(c, 0x96, (operation) {
        "RES 2,(HL)", 0, res_2_hl
    });
    core_register_ext_op(c, 0x97, (operation) {
        "RES 2,A", 0, res_2_a
    });
    core_register_ext_op(c, 0x98, (operation) {
        "RES 3,B", 0, res_3_b
    });
    core_register_ext_op(c, 0x99, (operation) {
        "RES 3,C", 0, res_3_c
    });
    core_register_ext_op(c, 0x9a, (operation) {
        "RES 3,D", 0, res_3_d
    });
    core_register_ext_op(c, 0x9b, (operation) {
        "RES 3,E", 0, res_3_e
    });
    core_register_ext_op(c, 0x9c, (operation) {
        "RES 3,H", 0, res_3_h
    });
    core_register_ext_op(c, 0x9d, (operation) {
        "RES 3,L", 0, res_3_l
    });
    core_register_ext_op(c, 0x9e, (operation) {
        "RES 3,(HL)", 0, res_3_hl
    });
    core_register_ext_op(c, 0x9f, (operation) {
        "RES 3,A", 0, res_3_a
    });
    core_register_ext_op(c, 0xa0, (operation) {
        "RES 4,B", 0, res_4_b
    });
    core_register_ext_op(c, 0xa1, (operation) {
        "RES 4,C", 0, res_4_c
    });
    core_register_ext_op(c, 0xa2, (operation) {
        "RES 4,D", 0, res_4_d
    });
    core_register_ext_op(c, 0xa3, (operation) {
        "RES 4,E", 0, res_4_e
    });
    core_register_ext_op(c, 0xa4, (operation) {
        "RES 4,H", 0, res_4_h
    });
    core_register_ext_op(c, 0xa5, (operation) {
        "RES 4,L", 0, res_4_l
    });
    core_register_ext_op(c, 0xa6, (operation) {
        "RES 4,(HL)", 0, res_4_hl
    });
    core_register_ext_op(c, 0xa7, (operation) {
        "RES 4,A", 0, res_4_a
    });
    core_register_ext_op(c, 0xa8, (operation) {
        "RES 5,B", 0, res_5_b
    });
    core_register_ext_op(c, 0xa9, (operation) {
        "RES 5,C", 0, res_5_c
    });
    core_register_ext_op(c, 0xaa, (operation) {
        "RES 5,D", 0, res_5_d
    });
    core_register_ext_op(c, 0xab, (operation) {
        "RES 5,E", 0, res_5_e
    });
    core_register_ext_op(c, 0xac, (operation) {
        "RES 5,H", 0, res_5_h
    });
    core_register_ext_op(c, 0xad, (operation) {
        "RES 5,L", 0, res_5_l
    });
    core_register_ext_op(c, 0xae, (operation) {
        "RES 5,(HL)", 0, res_5_hl
    });
    core_register_ext_op(c, 0xaf, (operation) {
        "RES 5,A", 0, res_5_a
    });
    core_register_ext_op(c, 0xb0, (operation) {
        "RES 6,B", 0, res_6_b
    });
    core_register_ext_op(c, 0xb1, (operation) {
        "RES 6,C", 0, res_6_c
    });
    core_register_ext_op(c, 0xb2, (operation) {
        "RES 6,D", 0, res_6_d
    });
    core_register_ext_op(c, 0xb3, (operation) {
        "RES 6,E", 0, res_6_e
    });
    core_register_ext_op(c, 0xb4, (operation) {
        "RES 6,H", 0, res_6_h
    });
    core_register_ext_op(c, 0xb5, (operation) {
        "RES 6,L", 0, res_6_l
    });
    core_register_ext_op(c, 0xb6, (operation) {
        "RES 6,(HL)", 0, res_6_hl
    });
    core_register_ext_op(c, 0xb7, (operation) {
        "RES 6,A", 0, res_6_a
    });
    core_register_ext_op(c, 0xb8, (operation) {
        "RES 7,B", 0, res_7_b
    });
    core_register_ext_op(c, 0xb9, (operation) {
        "RES 7,C", 0, res_7_c
    });
    core_register_ext_op(c, 0xba, (operation) {
        "RES 7,D", 0, res_7_d
    });
    core_register_ext_op(c, 0xbb, (operation) {
        "RES 7,E", 0, res_7_e
    });
    core_register_ext_op(c, 0xbc, (operation) {
        "RES 7,H", 0, res_7_h
    });
    core_register_ext_op(c, 0xbd, (operation) {
        "RES 7,L", 0, res_7_l
    });
    core_register_ext_op(c, 0xbe, (operation) {
        "RES 7,(HL)", 0, res_7_hl
    });
    core_register_ext_op(c, 0xbf, (operation) {
        "RES 7,A", 0, res_7_a
    });
    core_register_ext_op(c, 0xc0, (operation) {
        "SET 0,B", 0, set_0_b
    });
    core_register_ext_op(c, 0xc1, (operation) {
        "SET 0,C", 0, set_0_c
    });
    core_register_ext_op(c, 0xc2, (operation) {
        "SET 0,D", 0, set_0_d
    });
    core_register_ext_op(c, 0xc3, (operation) {
        "SET 0,E", 0, set_0_e
    });
    core_register_ext_op(c, 0xc4, (operation) {
        "SET 0,H", 0, set_0_h
    });
    core_register_ext_op(c, 0xc5, (operation) {
        "SET 0,L", 0, set_0_l
    });
    core_register_ext_op(c, 0xc6, (operation) {
        "SET 0,(HL)", 0, set_0_hl
    });
    core_register_ext_op(c, 0xc7, (operation) {
        "SET 0,A", 0, set_0_a
    });
    core_register_ext_op(c, 0xc8, (operation) {
        "SET 1,B", 0, set_1_b
    });
    core_register_ext_op(c, 0xc9, (operation) {
        "SET 1,C", 0, set_1_c
    });
    core_register_ext_op(c, 0xca, (operation) {
        "SET 1,D", 0, set_1_d
    });
    core_register_ext_op(c, 0xcb, (operation) {
        "SET 1,E", 0, set_1_e
    });
    core_register_ext_op(c, 0xcc, (operation) {
        "SET 1,H", 0, set_1_h
    });
    core_register_ext_op(c, 0xcd, (operation) {
        "SET 1,L", 0, set_1_l
    });
    core_register_ext_op(c, 0xce, (operation) {
        "SET 1,(HL)", 0, set_1_hl
    });
    core_register_ext_op(c, 0xcf, (operation) {
        "SET 1,A", 0, set_1_a
    });
    core_register_ext_op(c, 0xd0, (operation) {
        "SET 2,B", 0, set_2_b
    });
    core_register_ext_op(c, 0xd1, (operation) {
        "SET 2,C", 0, set_2_c
    });
    core_register_ext_op(c, 0xd2, (operation) {
        "SET 2,D", 0, set_2_d
    });
    core_register_ext_op(c, 0xd3, (operation) {
        "SET 2,E", 0, set_2_e
    });
    core_register_ext_op(c, 0xd4, (operation) {
        "SET 2,H", 0, set_2_h
    });
    core_register_ext_op(c, 0xd5, (operation) {
        "SET 2,L", 0, set_2_l
    });
    core_register_ext_op(c, 0xd6, (operation) {
        "SET 2,(HL)", 0, set_2_hl
    });
    core_register_ext_op(c, 0xd7, (operation) {
        "SET 2,A", 0, set_2_a
    });
    core_register_ext_op(c, 0xd8, (operation) {
        "SET 3,B", 0, set_3_b
    });
    core_register_ext_op(c, 0xd9, (operation) {
        "SET 3,C", 0, set_3_c
    });
    core_register_ext_op(c, 0xda, (operation) {
        "SET 3,D", 0, set_3_d
    });
    core_register_ext_op(c, 0xdb, (operation) {
        "SET 3,E", 0, set_3_e
    });
    core_register_ext_op(c, 0xdc, (operation) {
        "SET 3,H", 0, set_3_h
    });
    core_register_ext_op(c, 0xdd, (operation) {
        "SET 3,L", 0, set_3_l
    });
    core_register_ext_op(c, 0xde, (operation) {
        "SET 3,(HL)", 0, set_3_hl
    });
    core_register_ext_op(c, 0xdf, (operation) {
        "SET 3,A", 0, set_3_a
    });
    core_register_ext_op(c, 0xe0, (operation) {
        "SET 4,B", 0, set_4_b
    });
    core_register_ext_op(c, 0xe1, (operation) {
        "SET 4,C", 0, set_4_c
    });
    core_register_ext_op(c, 0xe2, (operation) {
        "SET 4,D", 0, set_4_d
    });
    core_register_ext_op(c, 0xe3, (operation) {
        "SET 4,E", 0, set_4_e
    });
    core_register_ext_op(c, 0xe4, (operation) {
        "SET 4,H", 0, set_4_h
    });
    core_register_ext_op(c, 0xe5, (operation) {
        "SET 4,L", 0, set_4_l
    });
    core_register_ext_op(c, 0xe6, (operation) {
        "SET 4,(HL)", 0, set_4_hl
    });
    core_register_ext_op(c, 0xe7, (operation) {
        "SET 4,A", 0, set_4_a
    });
    core_register_ext_op(c, 0xe8, (operation) {
        "SET 5,B", 0, set_5_b
    });
    core_register_ext_op(c, 0xe9, (operation) {
        "SET 5,C", 0, set_5_c
    });
    core_register_ext_op(c, 0xea, (operation) {
        "SET 5,D", 0, set_5_d
    });
    core_register_ext_op(c, 0xeb, (operation) {
        "SET 5,E", 0, set_5_e
    });
    core_register_ext_op(c, 0xec, (operation) {
        "SET 5,H", 0, set_5_h
    });
    core_register_ext_op(c, 0xed, (operation) {
        "SET 5,L", 0, set_5_l
    });
    core_register_ext_op(c, 0xee, (operation) {
        "SET 5,(HL)", 0, set_5_hl
    });
    core_register_ext_op(c, 0xef, (operation) {
        "SET 5,A", 0, set_5_a
    });
    core_register_ext_op(c, 0xf0, (operation) {
        "SET 6,B", 0, set_6_b
    });
    core_register_ext_op(c, 0xf1, (operation) {
        "SET 6,C", 0, set_6_c
    });
    core_register_ext_op(c, 0xf2, (operation) {
        "SET 6,D", 0, set_6_d
    });
    core_register_ext_op(c, 0xf3, (operation) {
        "SET 6,E", 0, set_6_e
    });
    core_register_ext_op(c, 0xf4, (operation) {
        "SET 6,H", 0, set_6_h
    });
    core_register_ext_op(c, 0xf5, (operation) {
        "SET 6,L", 0, set_6_l
    });
    core_register_ext_op(c, 0xf6, (operation) {
        "SET 6,(HL)", 0, set_6_hl
    });
    core_register_ext_op(c, 0xf7, (operation) {
        "SET 6,A", 0, set_6_a
    });
    core_register_ext_op(c, 0xf8, (operation) {
        "SET 7,B", 0, set_7_b
    });
    core_register_ext_op(c, 0xf9, (operation) {
        "SET 7,C", 0, set_7_c
    });
    core_register_ext_op(c, 0xfa, (operation) {
        "SET 7,D", 0, set_7_d
    });
    core_register_ext_op(c, 0xfb, (operation) {
        "SET 7,E", 0, set_7_e
    });
    core_register_ext_op(c, 0xfc, (operation) {
        "SET 7,H", 0, set_7_h
    });
    core_register_ext_op(c, 0xfd, (operation) {
        "SET 7,L", 0, set_7_l
    });
    core_register_ext_op(c, 0xfe, (operation) {
        "SET 7,(HL)", 0, set_7_hl
    });
    core_register_ext_op(c, 0xff, (operation) {
        "SET 7,A", 0, set_7_a
    });
}