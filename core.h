//
// Created by bertrand on 27/09/18.
//

#ifndef GAMEBOY_CORE_H
#define GAMEBOY_CORE_H

#define BOOTSTRAP_ROM_SIZE 256
#define RAM_SIZE 65536
#define DEBUG

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "ops.h"

typedef struct {
    char *disassembly;
    char operand_number;
    void (*handler)(void *core, uint8_t* args);
} operation;

struct core {
    uint8_t ram[RAM_SIZE];
    struct {
        union {
            struct {
                uint8_t a,f;
            };
            uint16_t af;
        };
        union {
            struct {
                uint8_t b, c;
            };
            uint16_t bc;
        };
        union {
            struct {
                uint8_t d, e;
            };
            uint16_t de;
        };
        union {
            struct {
                uint8_t h, l;
            };
            uint16_t hl;
        };
        uint16_t sp, pc;
    } registers;
    operation ops[256];
    operation ext_ops[256];
};

typedef struct core core;

typedef enum {
    ZERO = 1 << 7,
    SUB = 1 << 6,
    HALF = 1 << 5,
    CARRY = 1 << 4
} flags;

uint8_t core_ram_read(core *c, uint16_t address);
void core_ram_write(core *c, uint16_t address, uint8_t value);
void core_register_op(core *c, uint8_t code, operation op);
void core_register_ext_op(core *c, uint8_t code, operation op);
void core_init(core *c);
void core_loop(core *c);
void core_stack_push8(core *c, uint8_t val);
uint8_t core_stack_pop8(core *c);
void core_stack_push16(core *c, uint16_t val);
uint16_t core_stack_pop16(core *c);
void core_setup_operations(core *c);
void core_toggle_flag(core *c, flags f);
void core_set_flag(core *c, flags f);
void core_unset_flag(core *c, flags f);

#endif //GAMEBOY_CORE_H
