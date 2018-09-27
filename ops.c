//
// Created by bertrand on 27/09/18.
//

#include <stdint.h>
#include "core.h"
#include "main.h"

/**
 * unique
[ 'NOP',
  'LD',
  'INC',
  'DEC',
  'RLCA',
  'ADD',
  'RRCA',
  'STOP',
  'RLA',
  'JR',
  'RRA',
  'DAA',
  'CPL',
  'SCF',
  'CCF',
  'HALT',
  'ADC',
  'SUB',
  'SBC',
  'AND',
  'XOR',
  'OR',
  'CP',
  'RET',
  'POP',
  'JP',
  'CALL',
  'PUSH',
  'RST',
  'PREFIX',
  'RETI',
  'LDH',
  'DI',
  'EI' ]
 */

void noop(void *core, uint8_t *args) {
    (void) core;
    (void) args;
}

void load(uint8_t *from, uint8_t *to) {
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