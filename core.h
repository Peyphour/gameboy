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

uint8_t core_ram_read(core *c, uint16_t address);
void core_ram_write(core *c, uint16_t address, uint8_t value);
void core_register_op(core *c, uint8_t code, operation op);
void core_register_ext_op(core *c, uint8_t code, operation op);
void core_init(core *c);
void core_loop(core *c);
void core_setup_operations(core *c);

#endif //GAMEBOY_CORE_H
