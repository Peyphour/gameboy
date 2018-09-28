//
// Created by bertrand on 27/09/18.
//

#ifndef GAMEBOY_OPS_H
#define GAMEBOY_OPS_H

#include "main.h"
typedef struct core core;

void noop(core *core, uint8_t *args);
void ld_bc_nn (core *c, uint8_t *args);
void ld_bc_a (core *c, uint8_t *args);
void ld_b_n (core *c, uint8_t *args);
void ld_nn_sp (core *c, uint8_t *args);
void ld_a_bc (core *c, uint8_t *args);
void ld_c_n (core *c, uint8_t *args);
void ld_de_nn (core *c, uint8_t *args);
void ld_de_a (core *c, uint8_t *args);
void ld_d_n (core *c, uint8_t *args);
void ld_a_de (core *c, uint8_t *args);
void ld_e_n (core *c, uint8_t *args);
void ld_hl_nn (core *c, uint8_t *args);
void ld_h_n (core *c, uint8_t *args);
void ld_l_n (core *c, uint8_t *args);
void ld_sp_nn (core *c, uint8_t *args);
void ld_hl_n (core *c, uint8_t *args);
void ld_a_n (core *c, uint8_t *args);
void ld_b_b (core *c, uint8_t *args);
void ld_b_c (core *c, uint8_t *args);
void ld_b_d (core *c, uint8_t *args);
void ld_b_e (core *c, uint8_t *args);
void ld_b_h (core *c, uint8_t *args);
void ld_b_l (core *c, uint8_t *args);
void ld_b_hl (core *c, uint8_t *args);
void ld_b_a (core *c, uint8_t *args);
void ld_c_b (core *c, uint8_t *args);
void ld_c_c (core *c, uint8_t *args);
void ld_c_d (core *c, uint8_t *args);
void ld_c_e (core *c, uint8_t *args);
void ld_c_h (core *c, uint8_t *args);
void ld_c_l (core *c, uint8_t *args);
void ld_c_hl (core *c, uint8_t *args);
void ld_c_a (core *c, uint8_t *args);
void ld_d_b (core *c, uint8_t *args);
void ld_d_c (core *c, uint8_t *args);
void ld_d_d (core *c, uint8_t *args);
void ld_d_e (core *c, uint8_t *args);
void ld_d_h (core *c, uint8_t *args);
void ld_d_l (core *c, uint8_t *args);
void ld_d_hl (core *c, uint8_t *args);
void ld_d_a (core *c, uint8_t *args);
void ld_e_b (core *c, uint8_t *args);
void ld_e_c (core *c, uint8_t *args);
void ld_e_d (core *c, uint8_t *args);
void ld_e_e (core *c, uint8_t *args);
void ld_e_h (core *c, uint8_t *args);
void ld_e_l (core *c, uint8_t *args);
void ld_e_hl (core *c, uint8_t *args);
void ld_e_a (core *c, uint8_t *args);
void ld_h_b (core *c, uint8_t *args);
void ld_h_c (core *c, uint8_t *args);
void ld_h_d (core *c, uint8_t *args);
void ld_h_e (core *c, uint8_t *args);
void ld_h_h (core *c, uint8_t *args);
void ld_h_l (core *c, uint8_t *args);
void ld_h_hl (core *c, uint8_t *args);
void ld_h_a (core *c, uint8_t *args);
void ld_l_b (core *c, uint8_t *args);
void ld_l_c (core *c, uint8_t *args);
void ld_l_d (core *c, uint8_t *args);
void ld_l_e (core *c, uint8_t *args);
void ld_l_h (core *c, uint8_t *args);
void ld_l_l (core *c, uint8_t *args);
void ld_l_hl (core *c, uint8_t *args);
void ld_l_a (core *c, uint8_t *args);
void ld_hl_b (core *c, uint8_t *args);
void ld_hl_c (core *c, uint8_t *args);
void ld_hl_d (core *c, uint8_t *args);
void ld_hl_e (core *c, uint8_t *args);
void ld_hl_h (core *c, uint8_t *args);
void ld_hl_l (core *c, uint8_t *args);
void ld_hl_a (core *c, uint8_t *args);
void ld_a_b (core *c, uint8_t *args);
void ld_a_c (core *c, uint8_t *args);
void ld_a_d (core *c, uint8_t *args);
void ld_a_e (core *c, uint8_t *args);
void ld_a_h (core *c, uint8_t *args);
void ld_a_l (core *c, uint8_t *args);
void ld_a_hl (core *c, uint8_t *args);
void ld_a_a (core *c, uint8_t *args);
void ld_nn_a (core *c, uint8_t *args);
void ld_sp_hl (core *c, uint8_t *args);
void ld_a_nn (core *c, uint8_t *args);
void xor_a(core *c, uint8_t *args);
void xor_b(core *c, uint8_t *args);
void xor_c(core *c, uint8_t *args);
void xor_d(core *c, uint8_t *args);
void xor_e(core *c, uint8_t *args);
void xor_h(core *c, uint8_t *args);
void xor_l(core *c, uint8_t *args);
void xor_hl(core *c, uint8_t *args);
void xor_n(core *c, uint8_t *args);
void inc_a(core *c, uint8_t *args);
void inc_b(core *c, uint8_t *args);
void inc_c(core *c, uint8_t *args);
void inc_d(core *c, uint8_t *args);
void inc_e(core *c, uint8_t *args);
void inc_h(core *c, uint8_t *args);
void inc_l(core *c, uint8_t *args);
void inc_hl(core *c, uint8_t *args);
void inc_bc(core *c, uint8_t *args);
void inc_de(core *c, uint8_t *args);
void inc_hl(core *c, uint8_t *args);
void inc_sp(core *c, uint8_t *args);
void dec_a(core *c, uint8_t *args);
void dec_b(core *c, uint8_t *args);
void dec_c(core *c, uint8_t *args);
void dec_d(core *c, uint8_t *args);
void dec_e(core *c, uint8_t *args);
void dec_h(core *c, uint8_t *args);
void dec_l(core *c, uint8_t *args);
void dec_hl(core *c, uint8_t *args);
void dec_bc(core *c, uint8_t *args);
void dec_de(core *c, uint8_t *args);
void dec_hl(core *c, uint8_t *args);
void dec_sp(core *c, uint8_t *args);
void rlc_a(core *c, uint8_t *args);
void rrc_a(core *c, uint8_t *args);
void rl_a(core *c, uint8_t *args);
void rr_a(core *c, uint8_t *args);
void add_a_a(core *c, uint8_t *args);
void add_a_b(core *c, uint8_t *args);
void add_a_c(core *c, uint8_t *args);
void add_a_d(core *c, uint8_t *args);
void add_a_e(core *c, uint8_t *args);
void add_a_h(core *c, uint8_t *args);
void add_a_l(core *c, uint8_t *args);
void add_a_hl(core *c, uint8_t *args);
void add_a_n(core *c, uint8_t *args);
void add_hl_bc(core *c, uint8_t *args);
void add_hl_de(core *c, uint8_t *args);
void add_hl_hl(core *c, uint8_t *args);
void add_hl_sp(core *c, uint8_t *args);
void sub_a_a(core *c, uint8_t *args);
void sub_a_b(core *c, uint8_t *args);
void sub_a_c(core *c, uint8_t *args);
void sub_a_d(core *c, uint8_t *args);
void sub_a_e(core *c, uint8_t *args);
void sub_a_h(core *c, uint8_t *args);
void sub_a_l(core *c, uint8_t *args);
void sub_a_hl(core *c, uint8_t *args);
void sub_a_n(core *c, uint8_t *args);
void and_a(core *c, uint8_t *args);
void and_b(core *c, uint8_t *args);
void and_c(core *c, uint8_t *args);
void and_d(core *c, uint8_t *args);
void and_e(core *c, uint8_t *args);
void and_h(core *c, uint8_t *args);
void and_l(core *c, uint8_t *args);
void and_hl(core *c, uint8_t *args);
void and_n(core *c, uint8_t *args);
void or_a(core *c, uint8_t *args);
void or_b(core *c, uint8_t *args);
void or_c(core *c, uint8_t *args);
void or_d(core *c, uint8_t *args);
void or_e(core *c, uint8_t *args);
void or_h(core *c, uint8_t *args);
void or_l(core *c, uint8_t *args);
void or_hl(core *c, uint8_t *args);
void or_n(core *c, uint8_t *args);
void ldi_hl_a(core *c, uint8_t *args);
void ldi_a_hl(core *c, uint8_t *args);
void ldd_hl_a(core *c, uint8_t *args);
void ldd_a_hl(core *c, uint8_t *args);
void adc_a_a(core *c, uint8_t *args);
void adc_a_b(core *c, uint8_t *args);
void adc_a_c(core *c, uint8_t *args);
void adc_a_d(core *c, uint8_t *args);
void adc_a_e(core *c, uint8_t *args);
void adc_a_h(core *c, uint8_t *args);
void adc_a_l(core *c, uint8_t *args);
void adc_a_hl(core *c, uint8_t *args);
void adc_a_n(core *c, uint8_t *args);
void sbc_a_a(core *c, uint8_t *args);
void sbc_a_b(core *c, uint8_t *args);
void sbc_a_c(core *c, uint8_t *args);
void sbc_a_d(core *c, uint8_t *args);
void sbc_a_e(core *c, uint8_t *args);
void sbc_a_h(core *c, uint8_t *args);
void sbc_a_l(core *c, uint8_t *args);
void sbc_a_n(core *c, uint8_t *args);
void sbc_a_hl(core *c, uint8_t *args);
void cp_a(core *c, uint8_t *args);
void cp_b(core *c, uint8_t *args);
void cp_c(core *c, uint8_t *args);
void cp_d(core *c, uint8_t *args);
void cp_e(core *c, uint8_t *args);
void cp_h(core *c, uint8_t *args);
void cp_l(core *c, uint8_t *args);
void cp_hl(core *c, uint8_t *args);
void cp_n(core *c, uint8_t *args);
void cpl(core *c, uint8_t *args);

void jr_nz_n(core *c, uint8_t *args);
void jr_z_n(core *c, uint8_t *args);
void jr_nc_n(core *c, uint8_t *args);
void jr_c_n(core *c, uint8_t *args);
void jr_n(core *c, uint8_t *args);

void jp_nz_nn(core *c, uint8_t *args);
void jp_z_nn(core *c, uint8_t *args);
void jp_nc_nn(core *c, uint8_t *args);
void jp_c_nn(core *c, uint8_t *args);
void jp_nn(core *c, uint8_t *args);
void jp_hl(core *c, uint8_t *args);

void call_nz_nn(core *c, uint8_t *args);
void call_z_nn(core *c, uint8_t *args);
void call_nc_nn(core *c, uint8_t *args);
void call_c_nn(core *c, uint8_t *args);
void call_nn(core *c, uint8_t *args);

void ret_nz(core *c, uint8_t *args);
void ret_z(core *c, uint8_t *args);
void ret(core *c, uint8_t *args);
void ret_nc(core *c, uint8_t *args);
void ret_c(core *c, uint8_t *args);

void pop_bc(core *c, uint8_t *args);
void pop_de(core *c, uint8_t *args);
void pop_af(core *c, uint8_t *args);
void pop_hl(core *c, uint8_t *args);

void push_bc(core *c, uint8_t *args);
void push_de(core *c, uint8_t *args);
void push_af(core *c, uint8_t *args);
void push_hl(core *c, uint8_t *args);

void scf(core *c, uint8_t *args);
void ccf(core *c, uint8_t *args);

void daa(core *c, uint8_t *args);
void halt(core *c, uint8_t *args);
void stop(core *c, uint8_t *args);
void xx(core *c, uint8_t *args);
#endif //GAMEBOY_OPS_H
