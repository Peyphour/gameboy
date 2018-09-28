//
// Created by bertrand on 27/09/18.
//

#include <stdint.h>
#include "core.h"
#include "main.h"

void noop(core *core, uint8_t *args) {
    (void) core;
    (void) args;
}

void jump(core *core, uint16_t location) {
    core->registers.pc += location;
}

void load(uint8_t *from, uint8_t *to) {
    *to = *from;
}

void load16(uint16_t *from, uint16_t *to) {
    *to = *from;
}

void inc(core *c, uint8_t *target) {
    if(*target & 0x0F)
        core_set_flag(c, HALF);
    (*target)++;
    if(!*target) { // result is zero
        core_set_flag(c, ZERO);
    }
    core_unset_flag(c, SUB);
}

void dec(core *c, uint8_t *target) {
    if(*target & 0x0F)
        core_set_flag(c, HALF);
    (*target)--;
    if(!*target)
        core_set_flag(c, ZERO);
    core_set_flag(c, SUB);
}

void inc16(uint16_t *target) {
    (*target)++;
}

void dec16(uint16_t *target) {
    (*target)--;
}

void and(core *c, uint8_t *target) {
    c->registers.a &= *target;
    if(!c->registers.a)
        core_set_flag(c, ZERO);
    core_unset_flag(c, CARRY | SUB);
    core_set_flag(c, HALF);
}

void or(core *c, uint8_t *target) {
    c->registers.a |= *target;
    if(!c->registers.a)
        core_set_flag(c, ZERO);
    core_unset_flag(c, CARRY | SUB | HALF);
}

void rlc(core *c, uint8_t *target) {
    core_unset_flag(c, SUB | HALF);
    uint8_t result = (*target) << 1;
    if(*target & (1 << 7)) {
        result |= 1;
        core_set_flag(c, CARRY);
    } else {
        core_unset_flag(c, CARRY);
    }
    if(!result)
        core_set_flag(c, ZERO);
    *target = result;
}

void rrc(core *c, uint8_t *target) {
    core_unset_flag(c, SUB | HALF);
    uint8_t result = (*target) >> 1;
    if(*target & 1) {
        result |=  (1 << 7);
        core_set_flag(c, CARRY);
    } else {
        core_unset_flag(c, CARRY);
    }
    if(!result)
        core_set_flag(c, ZERO);
    *target = result;
}

void rl(core *c, uint8_t *target) {
    uint8_t result = *target << 1;
    result |= (core_get_flag(c, CARRY) ? 1 : 0);
    if(*target & (1 << 7))
        core_set_flag(c, CARRY);
    if(!result)
        core_set_flag(c, ZERO);
    core_unset_flag(c, SUB | HALF);
    *target = result;
}

void rr(core *c, uint8_t *target) {
    uint8_t result = *target >> 1;
    result |= (core_get_flag(c, CARRY) ? (1 << 7) : 0);
    if(*target & 1)
        core_set_flag(c, CARRY);
    if(!result)
        core_set_flag(c, ZERO);
    core_unset_flag(c, SUB | HALF);
    *target = result;
}

void sla(core *c, uint8_t *target) {
    uint8_t result = *target << 1;
    if(*target & (1 << 7))
        core_set_flag(c, CARRY);
    if(!result)
        core_set_flag(c, ZERO);
    core_unset_flag(c, HALF | SUB);
    *target = result;
}

void sra(core *c, uint8_t *target) {
    uint8_t result = (uint8_t) ((*target >> 1) | (*target & (1 << 7)));
    if(*target & 1)
        core_set_flag(c, CARRY);
    if(!result)
        core_set_flag(c, ZERO);
    core_unset_flag(c, HALF | SUB);
    *target = result;
}

void swap(core *c, uint8_t *target) {
    uint8_t low = (uint8_t) (*target & 0x0f);
    uint8_t high = (uint8_t) (*target & 0xf0);
    uint8_t result = (low << 4) | (high >> 4);
    if(!result)
        core_set_flag(c, ZERO);
    core_unset_flag(c, CARRY | SUB | HALF);
    *target = result;
}

void srl(core *c, uint8_t *target) {
    uint8_t result = *target >> 1;
    if(*target & 1)
        core_set_flag(c, CARRY);
    if(!result)
        core_set_flag(c, ZERO);
    core_unset_flag(c, HALF | SUB);
    *target = result;
}

void bit(core *c, uint8_t *target, uint8_t n) { // test bit n of register
    core_unset_flag(c, SUB);
    core_set_flag(c, HALF);
    if (!(*target & (1 << n)))
        core_set_flag(c, ZERO);
}

void res(uint8_t *target, uint8_t n) { // reset bit n of register
    *target &= ~(1 << n);
}

void set(uint8_t *target, uint8_t n) { // set bit n of register
    *target |= (1 << n);
}

void cp(core *c, uint8_t *target) {
    if(!c->registers.a - *target)
        core_set_flag(c, ZERO);
    core_set_flag(c, SUB);
    if(*target > c->registers.a)
        core_set_flag(c, CARRY);
    if((*target & 0x0f) > (c->registers.a & 0x0f))
        core_set_flag(c, HALF);
}

void add(core *c, uint8_t *from) {
    core_unset_flag(c, SUB);
    if(c->registers.a + *from > 0xff)
        core_set_flag(c, CARRY);
    if((c->registers.a & 0x0f) + (*from + 0x0f) > 0x0f)
        core_set_flag(c, HALF);
    c->registers.a += *from;
    if(!c->registers.a)
        core_set_flag(c, ZERO);
}

void add16(core *c, uint16_t *from, uint16_t *to) {
    core_unset_flag(c, SUB);
    if(*from + *to > 0xffff)
        core_set_flag(c, CARRY);
    if((*from & 0x0fff) + (*to & 0x0fff) > 0x0fff)
        core_set_flag(c, HALF);
    *to += *from;
}

void adc(core *c, uint8_t *from) {
    uint8_t carry = (uint8_t) (core_get_flag(c, CARRY) ? 1 : 0);
    if(c->registers.a + *from + carry == 0)
        core_set_flag(c, ZERO);
    core_unset_flag(c, SUB);
    if((int) c->registers.a + *from + carry > 0xff)
        core_set_flag(c, CARRY);
    if(((c->registers.a & 0x0f) + (*from & 0x0f) + carry) > 0x0f)
        core_set_flag(c, HALF);
}

void sbc(core *c, uint8_t *from) {
    uint8_t carry = (uint8_t) (core_get_flag(c, CARRY) ? 1 : 0);
    uint8_t result = c->registers.a - *from - carry;
    if(!result)
        core_set_flag(c, ZERO);
    core_set_flag(c, SUB);
    if((int) c->registers.a + *from + carry > 0xff)
        core_set_flag(c, CARRY);
    if((c->registers.a ^ *from ^ result) & (1 << 4))
        core_set_flag(c, HALF);
}

void sub(core *c, uint8_t *target) {
    if(!(c->registers.a - *target))
        core_set_flag(c, ZERO);
    core_set_flag(c, SUB);
    if(c->registers.a < *target)
        core_set_flag(c, CARRY);
    if((c->registers.a & 0x0f) < (*target & 0x0f))
        core_set_flag(c, HALF);
    c->registers.a -= *target;
}

void xor(core *c, uint8_t *target) {
    c->registers.a ^= *target;
    if(!c->registers.a)
        core_set_flag(c, ZERO);
    core_unset_flag(c, CARRY | HALF | SUB);
}

void inc_a(core *c, uint8_t *args) {
    inc(c, &c->registers.a);
}
void inc_b(core *c, uint8_t *args) {
    inc(c, &c->registers.b);
}
void inc_c(core *c, uint8_t *args) {
    inc(c, &c->registers.c);
}
void inc_d(core *c, uint8_t *args) {
    inc(c, &c->registers.d);
}
void inc_e(core *c, uint8_t *args) {
    inc(c, &c->registers.e);
}
void inc_h(core *c, uint8_t *args) {
    inc(c, &c->registers.h);
}
void inc_l(core *c, uint8_t *args) {
    inc(c, &c->registers.l);
}
void inc_hl(core *c, uint8_t *args) {
    inc(c, &c->ram[c->registers.hl]);
}

void dec_a(core *c, uint8_t *args) {
    dec(c, &c->registers.a);
}
void dec_b(core *c, uint8_t *args) {
    dec(c, &c->registers.b);
}
void dec_c(core *c, uint8_t *args) {
    dec(c, &c->registers.c);
}
void dec_d(core *c, uint8_t *args) {
    dec(c, &c->registers.d);
}
void dec_e(core *c, uint8_t *args) {
    dec(c, &c->registers.e);
}
void dec_h(core *c, uint8_t *args) {
    dec(c, &c->registers.h);
}
void dec_l(core *c, uint8_t *args) {
    dec(c, &c->registers.l);
}
void dec_hl(core *c, uint8_t *args) {
    dec(c, &c->ram[c->registers.hl]);
}

void ld_bc_nn (core *c, uint8_t *args) {
    c->registers.bc = args[0] << 8 | args[1];
}
void ld_bc_a (core *c, uint8_t *args) {
    load(&c->registers.a, &c->ram[c->registers.bc]);
}
void ld_b_n (core *c, uint8_t *args) {
    c->registers.b = args[0];
}
void ld_nn_sp (core *c, uint8_t *args) {
    c->registers.sp = args[0] << 8 | args[1];
}
void ld_a_bc (core *c, uint8_t *args) {
    load(&c->ram[c->registers.bc], &c->registers.a);
}
void ld_c_n (core *c, uint8_t *args) {
    c->registers.c = args[0];
}
void ld_de_nn (core *c, uint8_t *args) {
    c->registers.de = args[0] << 8 | args[1];
}
void ld_de_a (core *c, uint8_t *args) {
    load(&c->registers.a, &c->ram[c->registers.de]);
}
void ld_d_n (core *c, uint8_t *args) {
    c->registers.d = args[0];
}
void ld_a_de (core *c, uint8_t *args) {
    load(&c->ram[c->registers.de], &c->registers.a);
}
void ld_e_n (core *c, uint8_t *args) {
    c->registers.e = args[0];
}
void ld_hl_nn (core *c, uint8_t *args) {
    c->registers.hl = args[0] << 8 | args[1];
}
void ld_h_n (core *c, uint8_t *args) {
    c->registers.h = args[0];
}
void ld_l_n (core *c, uint8_t *args) {
    c->registers.l = args[0];
}
void ld_sp_nn (core *c, uint8_t *args) {
    c->registers.sp = args[0] << 8 | args[1];
}
void ld_hl_n (core *c, uint8_t *args) {
    c->ram[c->registers.hl] = args[0];
}
void ld_a_n (core *c, uint8_t *args) {
    c->registers.a = args[0];
}
void ld_b_b (core *c, uint8_t *args) {
    load(&c->registers.b, &c->registers.b);
}
void ld_b_c (core *c, uint8_t *args) {
    load(&c->registers.c, &c->registers.b);
}
void ld_b_d (core *c, uint8_t *args) {
    load(&c->registers.d, &c->registers.b);
}
void ld_b_e (core *c, uint8_t *args) {
    load(&c->registers.e, &c->registers.b);
}
void ld_b_h (core *c, uint8_t *args) {
    load(&c->registers.h, &c->registers.b);
}
void ld_b_l (core *c, uint8_t *args) {
    load(&c->registers.l, &c->registers.b);
}
void ld_b_hl (core *c, uint8_t *args) {
    load(&c->ram[c->registers.hl], &c->registers.b);
}
void ld_b_a (core *c, uint8_t *args) {
    load(&c->registers.a, &c->registers.b);
}
void ld_c_b (core *c, uint8_t *args) {
    load(&c->registers.b, &c->registers.c);
}
void ld_c_c (core *c, uint8_t *args) {
    load(&c->registers.c, &c->registers.c);
}
void ld_c_d (core *c, uint8_t *args) {
    load(&c->registers.d, &c->registers.c);
}
void ld_c_e (core *c, uint8_t *args) {
    load(&c->registers.e, &c->registers.c);
}
void ld_c_h (core *c, uint8_t *args) {
    load(&c->registers.h, &c->registers.c);
}
void ld_c_l (core *c, uint8_t *args) {
    load(&c->registers.l, &c->registers.c);
}
void ld_c_hl (core *c, uint8_t *args) {
    load(&c->ram[c->registers.hl], &c->registers.c);
}
void ld_c_a (core *c, uint8_t *args) {
    load(&c->registers.a, &c->registers.c);
}
void ld_d_b (core *c, uint8_t *args) {
    load(&c->registers.b, &c->registers.d);
}
void ld_d_c (core *c, uint8_t *args) {
    load(&c->registers.c, &c->registers.d);
}
void ld_d_d (core *c, uint8_t *args) {
    load(&c->registers.d, &c->registers.d);
}
void ld_d_e (core *c, uint8_t *args) {
    load(&c->registers.e, &c->registers.d);
}
void ld_d_h (core *c, uint8_t *args) {
    load(&c->registers.h, &c->registers.d);
}
void ld_d_l (core *c, uint8_t *args) {
    load(&c->registers.l, &c->registers.d);
}
void ld_d_hl (core *c, uint8_t *args) {
    load(&c->ram[c->registers.hl], &c->registers.d);
}
void ld_d_a (core *c, uint8_t *args) {
    load(&c->registers.a, &c->registers.d);
}
void ld_e_b (core *c, uint8_t *args) {
    load(&c->registers.b, &c->registers.e);
}
void ld_e_c (core *c, uint8_t *args) {
    load(&c->registers.c, &c->registers.e);
}
void ld_e_d (core *c, uint8_t *args) {
    load(&c->registers.d, &c->registers.e);
}
void ld_e_e (core *c, uint8_t *args) {
    load(&c->registers.e, &c->registers.e);
}
void ld_e_h (core *c, uint8_t *args) {
    load(&c->registers.h, &c->registers.e);
}
void ld_e_l (core *c, uint8_t *args) {
    load(&c->registers.l, &c->registers.e);
}
void ld_e_hl (core *c, uint8_t *args) {
    load(&c->ram[c->registers.hl], &c->registers.e);
}
void ld_e_a (core *c, uint8_t *args) {
    load(&c->registers.a, &c->registers.e);
}
void ld_h_b (core *c, uint8_t *args) {
    load(&c->registers.b, &c->registers.h);
}
void ld_h_c (core *c, uint8_t *args) {
    load(&c->registers.c, &c->registers.h);
}
void ld_h_d (core *c, uint8_t *args) {
    load(&c->registers.d, &c->registers.h);
}
void ld_h_e (core *c, uint8_t *args) {
    load(&c->registers.e, &c->registers.h);
}
void ld_h_h (core *c, uint8_t *args) {
    load(&c->registers.h, &c->registers.h);
}
void ld_h_l (core *c, uint8_t *args) {
    load(&c->registers.l, &c->registers.h);
}
void ld_h_hl (core *c, uint8_t *args) {
    load(&c->ram[c->registers.hl], &c->registers.h);
}
void ld_h_a (core *c, uint8_t *args) {
    load(&c->registers.a, &c->registers.h);
}
void ld_l_b (core *c, uint8_t *args) {
    load(&c->registers.b, &c->registers.l);
}
void ld_l_c (core *c, uint8_t *args) {
    load(&c->registers.c, &c->registers.l);
}
void ld_l_d (core *c, uint8_t *args) {
    load(&c->registers.d, &c->registers.l);
}
void ld_l_e (core *c, uint8_t *args) {
    load(&c->registers.e, &c->registers.l);
}
void ld_l_h (core *c, uint8_t *args) {
    load(&c->registers.h, &c->registers.l);
}
void ld_l_l (core *c, uint8_t *args) {
    load(&c->registers.l, &c->registers.l);
}
void ld_l_hl (core *c, uint8_t *args) {
    load(&c->ram[c->registers.hl], &c->registers.l);
}
void ld_l_a (core *c, uint8_t *args) {
    load(&c->registers.a, &c->registers.l);
}
void ld_hl_b (core *c, uint8_t *args) {
    load(&c->registers.b, &c->ram[c->registers.hl]);
}
void ld_hl_c (core *c, uint8_t *args) {
    load(&c->registers.c, &c->ram[c->registers.hl]);
}
void ld_hl_d (core *c, uint8_t *args) {
    load(&c->registers.d, &c->ram[c->registers.hl]);
}
void ld_hl_e (core *c, uint8_t *args) {
    load(&c->registers.e, &c->ram[c->registers.hl]);
}
void ld_hl_h (core *c, uint8_t *args) {
    load(&c->registers.h, &c->ram[c->registers.hl]);
}
void ld_hl_l (core *c, uint8_t *args) {
    load(&c->registers.l, &c->ram[c->registers.hl]);
}
void ld_hl_a (core *c, uint8_t *args) {
    load(&c->registers.a, &c->ram[c->registers.hl]);
}
void ld_a_b (core *c, uint8_t *args) {
    load(&c->registers.b, &c->registers.a);
}
void ld_a_c (core *c, uint8_t *args) {
    load(&c->registers.c, &c->registers.a);
}
void ld_a_d (core *c, uint8_t *args) {
    load(&c->registers.d, &c->registers.a);
}
void ld_a_e (core *c, uint8_t *args) {
    load(&c->registers.e, &c->registers.a);
}
void ld_a_h (core *c, uint8_t *args) {
    load(&c->registers.h, &c->registers.a);
}
void ld_a_l (core *c, uint8_t *args) {
    load(&c->registers.l, &c->registers.a);
}
void ld_a_hl (core *c, uint8_t *args) {
    load(&c->ram[c->registers.hl], &c->registers.a);
}
void ld_a_a (core *c, uint8_t *args) {
    load(&c->registers.a, &c->registers.a);
}
void ld_nn_a (core *c, uint8_t *args) {
    load(&c->registers.a, &c->ram[args[0] << 8 | args[1]]);
}
void ld_sp_hl (core *c, uint8_t *args) {
    load16(&c->registers.hl, &c->registers.sp);
}
void ld_a_nn (core *c, uint8_t *args) {
    load(&c->ram[args[0] << 8 | args[1]], &c->registers.a);
}

void xor_a(core *c, uint8_t *args) {
    xor(c, &c->registers.a);
}

void xor_b(core *c, uint8_t *args) {
    xor(c, &c->registers.b);
}
void xor_c(core *c, uint8_t *args) {
    xor(c, &c->registers.c);
}
void xor_d(core *c, uint8_t *args) {
    xor(c, &c->registers.d);
}
void xor_e(core *c, uint8_t *args) {
    xor(c, &c->registers.e);
}

void xor_h(core *c, uint8_t *args) {
    xor(c, &c->registers.h);
}

void xor_l(core *c, uint8_t *args) {
    xor(c, &c->registers.l);
}

void xor_hl(core *c, uint8_t *args) {
    xor(c, &c->ram[c->registers.hl]);
}

void xor_n(core *c, uint8_t *args) {
    xor(c, &args[0]);
}

void rlc_a(core *c, uint8_t *args) {
    rlc(c, &c->registers.a);
}

void rrc_a(core *c, uint8_t *args) {
    rrc(c, &c->registers.a);
}
void rl_a(core *c, uint8_t *args) {
    rl(c, &c->registers.a);
}
void rr_a(core *c, uint8_t *args) {
    rr(c, &c->registers.a);
}

void add_a_a(core *c, uint8_t *args) {
    add(c, &c->registers.a);
}
void add_a_b(core *c, uint8_t *args) {
    add(c, &c->registers.b);
}
void add_a_c(core *c, uint8_t *args) {
    add(c, &c->registers.c);
}
void add_a_d(core *c, uint8_t *args) {
    add(c, &c->registers.d);
}
void add_a_e(core *c, uint8_t *args) {
    add(c, &c->registers.e);
}
void add_a_h(core *c, uint8_t *args) {
    add(c, &c->registers.h);
}
void add_a_l(core *c, uint8_t *args) {
    add(c, &c->registers.l);
}
void add_a_hl(core *c, uint8_t *args) {
    add(c, &c->ram[c->registers.b]);
}

void add_a_n(core *c, uint8_t *args) {
    add(c, &args[0]);
}

void add_hl_bc(core *c, uint8_t *args) {
    add16(c, &c->registers.bc, &c->registers.hl);
}
void add_hl_de(core *c, uint8_t *args) {
    add16(c, &c->registers.de, &c->registers.hl);
}
void add_hl_hl(core *c, uint8_t *args) {
    add16(c, &c->registers.hl, &c->registers.hl);
}
void add_hl_sp(core *c, uint8_t *args) {
    add16(c, &c->registers.sp, &c->registers.hl);
}
void sub_a_a(core *c, uint8_t *args) {
    sub(c, &c->registers.a);
}
void sub_a_b(core *c, uint8_t *args) {
    sub(c, &c->registers.b);
}
void sub_a_c(core *c, uint8_t *args) {
    sub(c, &c->registers.c);
}
void sub_a_d(core *c, uint8_t *args) {
    sub(c, &c->registers.d);
}
void sub_a_e(core *c, uint8_t *args) {
    sub(c, &c->registers.e);
}
void sub_a_h(core *c, uint8_t *args) {
    sub(c, &c->registers.h);
}
void sub_a_l(core *c, uint8_t *args) {
    sub(c, &c->registers.l);
}
void sub_a_hl(core *c, uint8_t *args) {
    sub(c, &c->ram[c->registers.b]);
}
void sub_a_n(core *c, uint8_t *args) {
    sub(c, &args[0]);
}
void and_a(core *c, uint8_t *args) {
    and(c, &c->registers.a);
}
void and_b(core *c, uint8_t *args) {
    and(c, &c->registers.b);
}
void and_c(core *c, uint8_t *args) {
    and(c, &c->registers.c);
}
void and_d(core *c, uint8_t *args) {
    and(c, &c->registers.d);
}
void and_e(core *c, uint8_t *args) {
    and(c, &c->registers.e);
}
void and_h(core *c, uint8_t *args) {
    and(c, &c->registers.h);
}
void and_l(core *c, uint8_t *args) {
    and(c, &c->registers.l);
}
void and_hl(core *c, uint8_t *args) {
    and(c, &c->ram[c->registers.hl]);
}
void and_n(core *c, uint8_t *args) {
    and(c, &args[0]);
}
void or_a(core *c, uint8_t *args) {
    or(c, &c->registers.a);
}
void or_b(core *c, uint8_t *args) {
    or(c, &c->registers.b);
}
void or_c(core *c, uint8_t *args) {
    or(c, &c->registers.c);
}
void or_d(core *c, uint8_t *args) {
    or(c, &c->registers.d);
}
void or_e(core *c, uint8_t *args) {
    or(c, &c->registers.e);
}
void or_h(core *c, uint8_t *args) {
    or(c, &c->registers.h);
}
void or_l(core *c, uint8_t *args) {
    or(c, &c->registers.l);
}
void or_hl(core *c, uint8_t *args) {
    or(c, &c->ram[c->registers.hl]);
}
void or_n(core *c, uint8_t *args) {
    or(c, &args[0]);
}
void ldi_hl_a(core *c, uint8_t *args) {
    load(&c->registers.a, &c->ram[c->registers.hl]);
    inc16(&c->registers.hl);
}

void ldi_a_hl(core *c, uint8_t *args) {
    load(&c->ram[c->registers.hl], &c->registers.a);
    inc16(&c->registers.hl);
}

void ldd_hl_a(core *c, uint8_t *args) {
    load(&c->registers.a, &c->ram[c->registers.hl]);
    dec16(&c->registers.hl);
}

void ldd_a_hl(core *c, uint8_t *args) {
    load(&c->ram[c->registers.hl], &c->registers.a);
    dec16(&c->registers.hl);
}

void adc_a_a(core *c, uint8_t *args) {
    adc(c, &c->registers.a);
}
void adc_a_b(core *c, uint8_t *args) {
    adc(c, &c->registers.b);
}
void adc_a_c(core *c, uint8_t *args) {
    adc(c, &c->registers.c);
}
void adc_a_d(core *c, uint8_t *args) {
    adc(c, &c->registers.d);
}
void adc_a_e(core *c, uint8_t *args) {
    adc(c, &c->registers.e);
}
void adc_a_h(core *c, uint8_t *args) {
    adc(c, &c->registers.h);
}
void adc_a_l(core *c, uint8_t *args) {
    adc(c, &c->registers.l);
}
void adc_a_hl(core *c, uint8_t *args) {
    adc(c, &c->ram[c->registers.hl]);
}

void adc_a_n(core *c, uint8_t *args) {
    adc(c, &args[0]);
}

void sbc_a_a(core *c, uint8_t *args) {
    sbc(c, &c->registers.a);
}
void sbc_a_b(core *c, uint8_t *args) {
    sbc(c, &c->registers.b);
}
void sbc_a_c(core *c, uint8_t *args) {
    sbc(c, &c->registers.c);
}
void sbc_a_d(core *c, uint8_t *args) {
    sbc(c, &c->registers.d);
}
void sbc_a_e(core *c, uint8_t *args) {
    sbc(c, &c->registers.e);
}
void sbc_a_h(core *c, uint8_t *args) {
    sbc(c, &c->registers.h);
}
void sbc_a_l(core *c, uint8_t *args) {
    sbc(c, &c->registers.l);
}

void sbc_a_n(core *c, uint8_t *args) {
    sbc(c, &args[0]);
}

void sbc_a_hl(core *c, uint8_t *args) {
    sbc(c, &c->ram[c->registers.hl]);
}

void jr_nz_n(core *c, uint8_t *args) {
    if(!core_get_flag(c, ZERO))
        jump(c, (int8_t) args[0]);
}

void jr_z_n(core *c, uint8_t *args) {
    if(core_get_flag(c, ZERO))
        jump(c, (int8_t) args[0]);
}

void jr_nc_n(core *c, uint8_t *args) {
    if(!core_get_flag(c, CARRY))
        jump(c, (int8_t) args[0]);
}

void jr_c_n(core *c, uint8_t *args) {
    if(core_get_flag(c, CARRY))
        jump(c, (int8_t) args[0]);
}

void jr_n(core *c, uint8_t *args) {
    jump(c, (int8_t) args[0]);
}

void jp_nz_nn(core *c, uint8_t *args) {
    if(!core_get_flag(c, ZERO))
        c->registers.pc = (args[0] << 8 | args[1]);
}

void jp_z_nn(core *c, uint8_t *args) {
    if(core_get_flag(c, ZERO))
        c->registers.pc = (args[0] << 8 | args[1]);
}

void jp_nc_nn(core *c, uint8_t *args) {
    if(!core_get_flag(c, CARRY))
        c->registers.pc = (args[0] << 8 | args[1]);
}

void jp_c_nn(core *c, uint8_t *args) {
    if(core_get_flag(c, CARRY))
        c->registers.pc = (args[0] << 8 | args[1]);
}

void jp_nn(core *c, uint8_t *args) {
    c->registers.pc = (args[0] << 8 | args[1]);
}

void jp_hl(core *c, uint8_t *args) {
    c->registers.pc = c->ram[c->registers.hl];
}

void call_nz_nn(core *c, uint8_t *args) {
    if(!core_get_flag(c, ZERO)) {
        core_stack_push16(c, c->registers.pc);
        c->registers.pc = (args[0] << 8 | args[1]);
    }
}

void call_z_nn(core *c, uint8_t *args) {
    if(core_get_flag(c, ZERO)) {
        core_stack_push16(c, c->registers.pc);
        c->registers.pc = (args[0] << 8 | args[1]);
    }
}

void call_nc_nn(core *c, uint8_t *args) {
    if(!core_get_flag(c, CARRY)) {
        core_stack_push16(c, c->registers.pc);
        c->registers.pc = (args[0] << 8 | args[1]);
    }
}

void call_c_nn(core *c, uint8_t *args) {
    if(core_get_flag(c, CARRY)) {
        core_stack_push16(c, c->registers.pc);
        c->registers.pc = (args[0] << 8 | args[1]);
    }
}

void call_nn(core *c, uint8_t *args) {
    core_stack_push16(c, c->registers.pc);
    c->registers.pc = (args[0] << 8 | args[1]);
}

void ret_nz(core *c, uint8_t *args) {
    if(!core_get_flag(c, ZERO))
        c->registers.pc = core_stack_pop16(c);
}

void ret_z(core *c, uint8_t *args) {
    if(core_get_flag(c, ZERO))
        c->registers.pc = core_stack_pop16(c);
}

void ret_nc(core *c, uint8_t *args) {
    if(!core_get_flag(c, CARRY))
        c->registers.pc = core_stack_pop16(c);
}

void ret_c(core *c, uint8_t *args) {
    if(core_get_flag(c, CARRY))
        c->registers.pc = core_stack_pop16(c);
}

void ret(core *c, uint8_t *args) {
    c->registers.pc = core_stack_pop16(c);
}

void pop_bc(core *c, uint8_t *args) {
    c->registers.bc = core_stack_pop16(c);
}

void pop_de(core *c, uint8_t *args) {
    c->registers.de = core_stack_pop16(c);
}

void pop_af(core *c, uint8_t *args) {
    c->registers.af = core_stack_pop16(c);
}

void pop_hl(core *c, uint8_t *args) {
    c->registers.hl = core_stack_pop16(c);
}

void push_bc(core *c, uint8_t *args) {
    core_stack_push16(c, c->registers.bc);
}

void push_de(core *c, uint8_t *args) {
    core_stack_push16(c, c->registers.de);
}

void push_af(core *c, uint8_t *args) {
    core_stack_push16(c, c->registers.af);
}

void push_hl(core *c, uint8_t *args) {
    core_stack_push16(c, c->registers.hl);
}

void cpl(core *c, uint8_t *args) {
    c->registers.a = ~c->registers.a;
    core_set_flag(c, SUB | HALF);
}

void scf(core *c, uint8_t *args) {
    core_set_flag(c, CARRY);
}

void ccf(core *c, uint8_t *args) {
    core_unset_flag(c, CARRY);
}

void cp_a(core *c, uint8_t *args) {
    cp(c, &c->registers.a);
}
void cp_b(core *c, uint8_t *args) {
    cp(c, &c->registers.b);
}
void cp_c(core *c, uint8_t *args) {
    cp(c, &c->registers.c);
}
void cp_d(core *c, uint8_t *args) {
    cp(c, &c->registers.d);
}
void cp_e(core *c, uint8_t *args) {
    cp(c, &c->registers.e);
}
void cp_h(core *c, uint8_t *args) {
    cp(c, &c->registers.h);
}
void cp_l(core *c, uint8_t *args) {
    cp(c, &c->registers.l);
}
void cp_hl(core *c, uint8_t *args) {
    cp(c, &c->ram[c->registers.hl]);
}

void cp_n(core *c, uint8_t *args) {
    cp(c, &args[0]);
}
void daa(core *c, uint8_t *args) {
    int result = c->registers.a;
    if(core_get_flag(c, SUB)) {
        if(core_get_flag(c, HALF)) {
            result = ((result - 6) & 0xff);
        }
        if(core_get_flag(c, CARRY)) {
            result = (result - 0x60) & 0xff;
        }
    } else {
        if(core_get_flag(c, HALF) || (result & 0xf) > 9) {
            result += 0x06;
        }
        if(core_get_flag(c, CARRY) || result > 0x9f) {
            result += 0x60;
        }
    }
    core_unset_flag(c, HALF);
    if(result > 0xff) {
        core_set_flag(c, CARRY);
    }
    c->registers.a = (uint8_t) (result & 0xff);
    if(!result)
        core_set_flag(c, ZERO);
}

void halt(core *c, uint8_t *args) {
    exit(1);
}

void stop(core *c, uint8_t *args) {
    exit(EXIT_SUCCESS);
}

void xx(core *c, uint8_t *args) {
    printf("XX CALLED, ABORT");
    exit(EXIT_FAILURE);
}